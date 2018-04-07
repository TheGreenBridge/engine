#pragma once
#include <memory>
#include <common\types.h>

namespace engine {
	namespace memory {

		template <typename T>
		union PoolChunk {
			T value;
			PoolChunk<T> *nextPoolChunk;

			PoolChunk() {};
			~PoolChunk() {};
		};

		template <typename T, typename Allocator = std::allocator<PoolChunk<T>>>
		class PoolAllocator
		{
		private:
			static const size_t POOLALLOCATORDEFAULTSIZE = 1024;
			size_t m_OpenAllocations = 0;

			size_t m_Size = 0;

			PoolChunk<T> *m_Data = nullptr;
			PoolChunk<T> *m_Head = nullptr;

			Allocator* m_ParentAllocator = nullptr;
			bool m_needsToDeleteParentAllocator = false;

		public:
			explicit PoolAllocator(size_t size = POOLALLOCATORDEFAULTSIZE, Allocator *parentAllocator = nullptr);

			PoolAllocator(const PoolAllocator &other) = delete; // Copy Cstr
			PoolAllocator(const PoolAllocator &&other) = delete; // Move Cstr
			PoolAllocator& operator= (const PoolAllocator &other) = delete; // Copy Assignment
			PoolAllocator& operator= (const PoolAllocator &&other) = delete; // Move Assignment

			~PoolAllocator();

			template <typename... arguments>
			T* allocate(arguments&&... args);

			void deallocate(T* data);

			U32 getSize() const;
		};

	}
}

// IMPLEMENTATION

namespace engine {
	namespace memory {

		template <typename T, typename Allocator = std::allocator<PoolChunk<T>>>
		PoolAllocator<T, Allocator>::PoolAllocator(size_t size = POOLALLOCATORDEFAULTSIZE, Allocator *parentAllocator = nullptr)
			: m_Size(size), m_ParentAllocator(parentAllocator)
		{
			if (parentAllocator == nullptr) {
				m_ParentAllocator = new Allocator();
				m_needsToDeleteParentAllocator = true;
			}

			m_Data = m_ParentAllocator->allocate(m_Size);// new PoolChunk<T>[size];
			m_Head = m_Data;

			for (size_t i = 0; i < m_Size - 1; ++i)
			{
				m_Data[i].nextPoolChunk = std::addressof(m_Data[i + 1]);
			}
			m_Data[m_Size - 1].nextPoolChunk = nullptr;
		}


		template <typename T, typename Allocator = std::allocator<PoolChunk<T>>>
		PoolAllocator<T, Allocator>::~PoolAllocator()
		{
			if (m_OpenAllocations != 0) {
				// Error
			}

			m_ParentAllocator->deallocate(m_Data, m_Size);
			if (m_needsToDeleteParentAllocator)
			{
				delete m_ParentAllocator;
			}
			m_Data = nullptr;
			m_Head = nullptr;
		}

		template <typename T, typename Allocator = std::allocator<PoolChunk<T>>>
		template <typename... arguments>
		T* PoolAllocator<T, Allocator>::allocate(arguments&&... args)
		{
			if (m_Head == nullptr)
				return nullptr;

			m_OpenAllocations++;

			PoolChunk<T> *poolChunk = m_Head;
			m_Head = m_Head->nextPoolChunk;
			T* retVal = new (std::addressof(poolChunk->value)) T(std::forward<arguments>(args)...);
			return retVal;
		}

		template <typename T, typename Allocator = std::allocator<PoolChunk<T>>>
		void PoolAllocator<T, Allocator>::deallocate(T* data)
		{
			if (data == nullptr)
			{
				return;
			}
			data->~T();
			m_OpenAllocations--;
			PoolChunk<T>* poolChunk = reinterpret_cast<PoolChunk<T>*>(data);
			poolChunk->nextPoolChunk = m_Head;
			m_Head = poolChunk;
		}

		template <typename T, typename Allocator = std::allocator<PoolChunk<T>>>
		U32 PoolAllocator<T, Allocator>::getSize() const {
			return m_Size;
		}
	}
}
