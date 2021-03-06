#include <entity\EntityManager.h>
#include <EngineCore.h>

namespace engine {

	U32 EntityManager::EntityIds = 0;
	
	EntityManager::EntityManager() : 
		m_Allocator(&Engine::gMemoryManager, 100)
	{

	}

	EntityManager::~EntityManager()
	{
	}

	Entity * EntityManager::newEntity()
	{
		Entity* entity = m_Allocator.allocate();
		m_Handle.insertEntry(EntityIds, entity);
		entity->m_UniqueId.id = EntityIds;

		EntityIds++;
		return entity;
	}

	void EntityManager::deleteEntity(U32 id) {
		m_Allocator.deallocate(m_Handle.getEntry(id));
		m_Handle.deleteEntry(id);
	}

	Entity* EntityManager::getEntity(U32 id) const {
		return m_Handle.getEntry(id);	
	}

	U32 EntityManager::getEntityCount() const {
		return m_Allocator.getAllocations();
	}
}