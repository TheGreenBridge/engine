#pragma once
#include <memory>
#include <common\types.h>
#include <memory\allocator\IAllocator.h>
#include <utils\Log.h>

namespace engine {
	namespace memory {

		template <typename T>
		union PoolChunk {
			T value;
			PoolChunk<T> *nextPoolChunk;

			PoolChunk() {};
			~PoolChunk() {};
		};

		template <typename T>
		class PoolAllocator
		{
		private:
			static const size_t POOLALLOCATORDEFAULTSIZE = 1024;
			size_t m_OpenAllocations = 0;

			size_t m_Size = 0;

			PoolChunk<T> *m_Data = nullptr;
			PoolChunk<T> *m_Head = nullptr;

			IAllocator* m_ParentAllocator = nullptr;
			bool m_needsToDeleteParentAllocator = false;

		public:
			explicit PoolAllocator(IAllocator *parentAllocator, size_t size = POOLALLOCATORDEFAULTSIZE);

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

		template <typename T>
		PoolAllocator<T>::PoolAllocator(IAllocator *parentAllocator, size_t size = POOLALLOCATORDEFAULTSIZE)
			: m_Size(size), m_ParentAllocator(parentAllocator)
		{
			if (parentAllocator == nullptr) {
				LOG_ERROR("parent allocator is nullptr");
				//m_ParentAllocator = new Allocator();
				//m_needsToDeleteParentAllocator = true;
			}
			blk allocation = m_ParentAllocator->allocate(m_Size * sizeof(T));// new PoolChunk<T>[size];
			m_Data = static_cast<PoolChunk<T>*>(allocation.ptr);
			m_Head = m_Data;

			for (size_t i = 0; i < m_Size - 1; ++i)
			{
				m_Data[i].nextPoolChunk = std::addressof(m_Data[i + 1]);
			}
			m_Data[m_Size - 1].nextPoolChunk = nullptr;
		}


		template <typename T>
		PoolAllocator<T>::~PoolAllocator()
		{
			if (m_OpenAllocations != 0) {
				LOG_ERROR("Open allocations");
			}

			//m_ParentAllocator->deallocate(m_Data, m_Size);
			//if (m_needsToDeleteParentAllocator)
			//{
			//	delete m_ParentAllocator;
			//}
			m_Data = nullptr;
			m_Head = nullptr;
		}

		template <typename T>
		template <typename... arguments>
		T* PoolAllocator<T>::allocate(arguments&&... args)
		{
			if (m_Head == nullptr)
				return nullptr;

			m_OpenAllocations++;

			PoolChunk<T> *poolChunk = m_Head;
			m_Head = m_Head->nextPoolChunk;
			T* retVal = new (std::addressof(poolChunk->value)) T(std::forward<arguments>(args)...);
			return retVal;
		}

		template <typename T>
		void PoolAllocator<T>::deallocate(T* data)
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

		template <typename T>
		U32 PoolAllocator<T>::getSize() const {
			return m_Size;
		}
	}
}
