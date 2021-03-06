//------------------------------------------------------------------------------
// File name: SystemManager.cpp
//
// Implementation of the system manager.
//
// Author: Sommerauer Christian
// Created: xx.xx.xx
// Changed: 13.08.18
//------------------------------------------------------------------------------
//

#include <Systems/ISystem.h>
//#include "messageing\Event.h"

#include "SystemManager.h"
#include "../utils/Log.h"

#define MEMORY_SIZE 10*1024 // 10 KiB

namespace engine {
	using namespace engine::memory;
	
	//--------------------------------------------------------------------------
	SystemManager::SystemManager() :
		m_Allocator(1000*1024), isActivated(false)
	{
		LOG("SystemManager");
		m_Allocator.logStates();
		m_Time.start();
	}

	//--------------------------------------------------------------------------
	void SystemManager::startUp() 
	{
		if (isActivated) return;
		isActivated = true;
		LOG("SystemManager is starting up ...");
		for (std::vector<ISystem*>::iterator it = m_Systems.begin(); 
			it != m_Systems.end(); it++) 
		{
			(*it)->startUp();
		}
	}

	//--------------------------------------------------------------------------
	void SystemManager::shutDown() 
	{
		if (!isActivated) return;
		isActivated = false;
		LOG("SystemManager is shuting down ...");
		for (std::vector<ISystem*>::iterator it = m_Systems.begin(); it != m_Systems.end(); it++) 
		{
			(*it)->shutDown();
		}
	}

	//--------------------------------------------------------------------------
	SystemManager::~SystemManager() 
	{

	}

	//--------------------------------------------------------------------------
	void SystemManager::update() 
	{
		m_FPSCounter++;
		m_Time.measureTime();
		if (m_Time.getPastTime() >= 1000l) {
			m_Time.erasePastTime();
			m_FPS = m_FPSCounter;
			m_FPSCounter = 0;
		}
		for (std::vector<ISystem*>::iterator it = m_Systems.begin(); it != m_Systems.end(); it++) {
			(*it)->update(m_Time.getDeltaF32());
		}
	}

	//--------------------------------------------------------------------------
	void SystemManager::postEvent(Event *e) 
	{
		for (std::vector<ISystem*>::iterator it = m_Systems.begin(); it != m_Systems.end(); it++) {
			(*it)->onEvent(e);						
		}
	}

	//--------------------------------------------------------------------------
	void SystemManager::registerSystem(ISystem & system) 
	{
		LOG("Registered System");
		m_Systems.push_back(&system);
		system.setSystemManager(this);
	}

	//--------------------------------------------------------------------------
	void SystemManager::deregisterSystem(ISystem * system) 
	{

	}

	//--------------------------------------------------------------------------
	void* SystemManager::requestMemory(size_t size) 
	{
		blk temp = m_Allocator.allocate(size);
		return temp.ptr;
	}

	//--------------------------------------------------------------------------
	template <typename T>
	T* SystemManager::requestMemory(size_t N) 
	{
		union {
			void* as_void;
			T* as_T;
		};
		as_void = m_allocator.allocate(sizeof(T)*N);
		return as_T;
	}

	//--------------------------------------------------------------------------
	void SystemManager::testManager() 
	{
		// To test the SystemManager for bugs
		
		m_Allocator.logStates();
	}

	//--------------------------------------------------------------------------
	U64 SystemManager::getDeltaTime() const
	{
		return m_Time.getDelta();
	}

	//--------------------------------------------------------------------------
	F32 SystemManager::getDeltaTimef() const
	{
		return m_Time.getDeltaF32();
	}

	//--------------------------------------------------------------------------
	U32 SystemManager::getFPS() const
	{
		return m_FPS;
	}
}