// Renderer
/*void render(const Scene &scene, const Shader &shader, const Light &light) const;
void render(const U32 vbo, const Shader &shader, const int dim, U32 size) const;

void render(Entity &entity, const Texture &texture, const Shader &shader) const;

void render(Entity &entity, const Shader &shader) const;

void renderLINES(Entity &entity, const Shader &shader) const;
void renderUI(Entity &entity, const Shader &shader) const;

void renderRenderSkin(Entity &entity, const Shader &shader) const;
void renderAABB(const collision::AABB3D &aabb, const Shader &shader) const;
void renderAABBs(const collision::AABB3D *aabb, const U32 size, const Shader &shader) const;
void renderTerrain(const U32 vbo, const Shader &shader, const int dim, U32 size) const;
void renderReflection(Entity &entity) const;

//void renderPBR(Entity &entity) const;*/



/*
void Renderer::render(const Scene &scene, const Shader &shader, const Light &light) const {

for each (Entity *entity in scene.getContainer())
{
if (entity->model == nullptr) continue;
if (entity->mesh == nullptr) continue;
if (entity->material == nullptr) continue;
if (entity->material->shader == nullptr) continue;
Shader &rShader = *entity->material->shader;
rShader.enable();
GLCall(glBindVertexArray(entity->model->vao));
//glBindTexture(GL_TEXTURE_2D, entity->material->texture->getID());
GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_Skybox->getTextureID()));

// Entity
rShader.setUniformMat4("trans_matrix", entity->getTransformation());
// Camera
rShader.setUniformMat4("proj_matrix", m_Camera->getProjectionMatrix());
rShader.setUniformMat4("view_matrix", m_Camera->getViewMatrix());
rShader.setUniform3f("camera_position", m_Camera->getPosition());

// Light
//rShader.setUniform3f("lightPosition", light.getPosition());
//shader.setUniform3f("light.color", light.getColor());

GLCall(glDrawArrays(GL_TRIANGLES, 0, entity->mesh->getVertices().size() * sizeof(Vec3)));
rShader.disable();
}
GLCall(glBindVertexArray(0));
}

void Renderer::renderLINES(Entity &entity, const Shader &shader) const {
shader.enable();
GLCall(glBindVertexArray(entity.model->vao));
shader.setUniformMat4("pr_matrix", m_Camera->getProjectionMatrix());
shader.setUniformMat4("view_matrix", m_Camera->getViewMatrix());
shader.setUniformMat4("transformation_matrix", entity.getTransformation());
GLCall(glDrawArrays(GL_LINES, 0, entity.mesh->getVertices().size() * sizeof(Vec3)));
GLCall(glBindVertexArray(0));
shader.disable();

}



float counter = 0;

void Renderer::renderRenderSkin(Entity &entity, const Shader &shader) const {
shader.enable();
if (entity.animatable == nullptr) LOG_ERROR("IN RENDERER: IST NULLPTR");
if (entity.animatable->m_pSkin == nullptr) LOG_ERROR("IN RENDERER: SKIN IST NULLPTR");
GLCall(glBindVertexArray(entity.animatable->m_pSkin->vao));

counter += 0.01f;
if (counter >= 2.0) counter = 0;
Vec3 color(counter, 1.0, 0.0);

shader.setUniformMat4("pr_matrix", m_Camera->getProjectionMatrix());
shader.setUniformMat4("view_matrix", m_Camera->getViewMatrix());
shader.setUniformMat4("trans_matrix", entity.getTransformation());
shader.setUniformMat4Array("joint_transforms", entity.animatable->m_pBones->elements);
shader.setUniform3f("color", color);

GLCall(glDrawArrays(GL_TRIANGLES, 0, entity.animatable->m_pSkin->numVerts));
GLCall(glBindVertexArray(0));
shader.disable();
}


void Renderer::renderUI(Entity &entity, const Shader &shader) const {
shader.enable();
GLCall(glBindVertexArray(entity.model->vao));
//shader.setUniformMat4("pr_matrix", m_Camera->getProjectionMatrix());
shader.setUniformMat4("view_matrix", m_Camera->getViewMatrix());
shader.setUniformMat4Array("transformation_matrix", entity.getTransformation().elements);
GLCall(glDrawArrays(GL_LINES, 0, entity.mesh->getVertices().size() * sizeof(Vec3)));
GLCall(glBindVertexArray(0));
}

void Renderer::render(const U32 vbo, const Shader &shader, const int dim, U32 size) const {
shader.enable();
GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
GLCall(glVertexAttribPointer(0, dim, GL_FLOAT, GL_FALSE, 0, 0));
shader.setUniformMat4("view_matrix", m_Camera->getRotationMatrix());
//shader.setUniformMat4("pr_matrix", m_Camera->getProjectionMatrix());
GLCall(glDrawArrays(GL_LINES, 0, size * sizeof(float)));
}

void Renderer::render(Entity & entity, const Texture &texture, const Shader &shader) const
{
shader.enable();

GLCall(glBindVertexArray(entity.model->vao));
GLCall(glDrawArrays(GL_TRIANGLES, 0, entity.mesh->numVertices * sizeof(Vec3)));

shader.setUniform3f("lightPosition", m_Lights[0].getPosition());
shader.setUniformMat4("view_matrix", m_Camera->getViewMatrix());
shader.setUniformMat4("pr_matrix", m_Camera->getProjectionMatrix());
shader.setUniformMat4("transformation_matrix", entity.getTransformation());
shader.setUniform3f("viewPos", m_Camera->getPosition());

shader.setUniformTexture("modelTexture", 0, texture.getID());

shader.disable();
}

void Renderer::render(Entity & entity, const Shader & shader) const
{
shader.enable();
GLCall(glBindVertexArray(entity.model->vao));
shader.setUniformMat4("view_matrix", m_Camera->getViewMatrix());
shader.setUniformMat4("proj_matrix", m_Camera->getProjectionMatrix());
shader.setUniformMat4("model_matrix", entity.getTransformation());

GLCall(glDrawArrays(GL_TRIANGLES, 0, entity.mesh->numVertices * sizeof(Vec3)));
shader.disable();
}



void Renderer::renderTerrain(const U32 vao, const Shader &shader, const int dim, U32 indicesSize) const {
shader.enable();
GLCall(glBindVertexArray(vao));
//glVertexAttribPointer(0, dim, GL_FLOAT, GL_FALSE, 0, 0);
shader.setUniformMat4("view_matrix", m_Camera->getViewMatrix());
shader.setUniformMat4("pr_matrix", m_Camera->getProjectionMatrix());


GLCall(glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0));
//glDrawArrays(GL_TRIANGLES, 0, size * sizeof(float));
GLCall(glBindVertexArray(0));
}

void Renderer::renderReflection(Entity & entity) const
{
entity.material->shader->enable();
GLCall(glBindVertexArray(entity.model->vao));


entity.material->shader->setUniformMat4("view_matrix", m_Camera->getViewMatrix());
entity.material->shader->setUniformMat4("proj_matrix", m_Camera->getProjectionMatrix());
entity.material->shader->setUniformMat4("trans_matrix", entity.getTransformation());

entity.material->shader->setUniform3f("camera_position", m_Camera->getPosition());

GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_Skybox->getTextureID()));
GLCall(glDrawArrays(GL_TRIANGLES, 0, entity.mesh->numVertices * sizeof(Vec3)));
GLCall(glBindVertexArray(0));
entity.material->shader->disable();

}

void Renderer::renderAABB(const collision::AABB3D &aabb, const Shader &shader) const {
//if (change) {
//	change = false;
//	Vec3 verts[8];
//	verts[0] = aabb.min;
//	verts[1] = Vec3(aabb.max.x, aabb.min.y, aabb.min.z);
//	verts[2] = Vec3(aabb.max.x, aabb.min.y, aabb.max.z);
//	verts[3] = Vec3(aabb.min.x, aabb.min.y, aabb.max.z);
//
//	verts[4] = Vec3(aabb.min.x, aabb.max.y, aabb.min.z);
//	verts[5] = Vec3(aabb.max.x, aabb.max.y, aabb.min.z);
//	verts[6] = aabb.max;
//	verts[7] = Vec3(aabb.min.x, aabb.max.y, aabb.max.z);
//
//	glGenBuffers(1, &vbo_aabb);
//
//	glBindBuffer(GL_ARRAY_BUFFER, vbo_aabb);
//	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), verts, GL_STATIC_DRAW);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
//	glEnableVertexAttribArray(0);
//}


U16 indices[24] = {
0, 1, 1, 2, 2, 3, 3, 0,
4, 5, 5, 6, 6, 7, 7, 4,
4, 0, 5, 1, 6, 2, 7, 3
};

mat4 result(1.0f);
Vec3 scale = aabb.max - aabb.min;
result.elements[0] = scale.x;
result.elements[5] = scale.y;
result.elements[10] = scale.z;
result.elements[12] = aabb.min.x;
result.elements[13] = aabb.min.y;
result.elements[14] = aabb.min.z;

shader.enable();

//glVertexPointer(3, GL_FLOAT, 0, verts);
GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo_aabb));
GLCall(glVertexAttribPointer(3, GL_UNSIGNED_SHORT, false, 0, 0, 0));
shader.setUniformMat4("view_matrix", m_Camera->getViewMatrix());
shader.setUniformMat4("pr_matrix", m_Camera->getProjectionMatrix());
shader.setUniformMat4("transformation_matrix", result);

GLCall(glDrawElements(GL_LINES, 24, GL_UNSIGNED_SHORT, indices));
GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
shader.disable();
}

void Renderer::renderAABBs(const collision::AABB3D *colliders, const U32 size, const Shader &shader) const {
GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo_aabb));
GLCall(glVertexAttribPointer(3, GL_UNSIGNED_SHORT, false, 0, 0, 0));
shader.enable();
for (U32 i = 0; i < size; i++) {

mat4 result(1.0f);
Vec3 scale = colliders[i].max - colliders[i].min;
result.elements[0] = scale.x;
result.elements[5] = scale.y;
result.elements[10] = scale.z;
result.elements[12] = colliders[i].min.x;
result.elements[13] = colliders[i].min.y;
result.elements[14] = colliders[i].min.z;

unsigned short indices[24] = {
0, 1, 1, 2, 2, 3, 3, 0,
4, 5, 5, 6, 6, 7, 7, 4,
4, 0, 5, 1, 6, 2, 7, 3
};

shader.setUniformMat4("view_matrix", m_Camera->getViewMatrix());
shader.setUniformMat4("pr_matrix", m_Camera->getProjectionMatrix());
shader.setUniformMat4("transformation_matrix", result);

GLCall(glDrawElements(GL_LINES, 24, GL_UNSIGNED_SHORT, indices));
}
GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
shader.disable();
}





*/

//void Renderer::renderPBR(Entity &entity) const
//{
//	const PBRMaterial &material = *entity.pbrmaterial;
//	const Shader &shader = material.getShader();

//	shader.enable();

//	GLCall(glBindVertexArray(entity.model->vao));

//	shader.setUniformMat4("projection", m_Camera->getProjectionMatrix());
//	shader.setUniformMat4("view", m_Camera->getViewMatrix());
//	shader.setUniformMat4("model", entity.getTransformation());

//	//shader.setUniform1f("roughness", material.roughness);
//	//shader.setUniform1f("metallic", material.metallic);
//	//shader.setUniform3f("albedo", material.albedo);

//	shader.setUniform1f("ao", 1.0f);
//	shader.setUniform3f("light", m_Lights[0].getPosition());

//	shader.setUniformTexture("albedoMap", 0, material.getAlbedo());
//	shader.setUniformTexture("metallicMap", 1, material.getMetallic());
//	shader.setUniformTexture("roughnessMap", 2, material.getRoughness());

//	shader.setUniform3f("camPos", m_Camera->getPosition());

//	GLCall(glDrawArrays(GL_TRIANGLES, 0, entity.mesh->numVertices * sizeof(Vec3)));
//	
//	GLCall(glBindVertexArray(0));
//	shader.disable();
//}

//





// Pool Allocator


//#pragma once
//#include <memory>
//#include <common\types.h>
//
//namespace engine {
//	namespace memory {
//
//		template <typename T>
//		union PoolChunk {
//			T value;
//			PoolChunk<T> *nextPoolChunk;
//
//			PoolChunk() {};
//			~PoolChunk() {};
//		};
//
//		template <typename T, typename Allocator = std::allocator<PoolChunk<T>>>
//		class PoolAllocator
//		{
//		private:
//			static const size_t POOLALLOCATORDEFAULTSIZE = 1024;
//			size_t m_OpenAllocations = 0;
//
//			size_t m_Size = 0;
//
//			PoolChunk<T> *m_Data = nullptr;
//			PoolChunk<T> *m_Head = nullptr;
//
//			Allocator* m_ParentAllocator = nullptr;
//			bool m_needsToDeleteParentAllocator = false;
//
//		public:
//			explicit PoolAllocator(size_t size = POOLALLOCATORDEFAULTSIZE, Allocator *parentAllocator = nullptr);
//
//			PoolAllocator(const PoolAllocator &other) = delete; // Copy Cstr
//			PoolAllocator(const PoolAllocator &&other) = delete; // Move Cstr
//			PoolAllocator& operator= (const PoolAllocator &other) = delete; // Copy Assignment
//			PoolAllocator& operator= (const PoolAllocator &&other) = delete; // Move Assignment
//
//			~PoolAllocator();
//
//			template <typename... arguments>
//			T* allocate(arguments&&... args);
//
//			void deallocate(T* data);
//
//			U32 getSize() const;
//		};
//
//	}
//}
//
//// IMPLEMENTATION
//
//namespace engine {
//	namespace memory {
//
//		template <typename T, typename Allocator = std::allocator<PoolChunk<T>>>
//		PoolAllocator<T, Allocator>::PoolAllocator(size_t size = POOLALLOCATORDEFAULTSIZE, Allocator *parentAllocator = nullptr)
//			: m_Size(size), m_ParentAllocator(parentAllocator)
//		{
//			if (parentAllocator == nullptr) {
//				m_ParentAllocator = new Allocator();
//				m_needsToDeleteParentAllocator = true;
//			}
//
//			m_Data = m_ParentAllocator->allocate(m_Size);// new PoolChunk<T>[size];
//			m_Head = m_Data;
//
//			for (size_t i = 0; i < m_Size - 1; ++i)
//			{
//				m_Data[i].nextPoolChunk = std::addressof(m_Data[i + 1]);
//			}
//			m_Data[m_Size - 1].nextPoolChunk = nullptr;
//		}
//
//
//		template <typename T, typename Allocator = std::allocator<PoolChunk<T>>>
//		PoolAllocator<T, Allocator>::~PoolAllocator()
//		{
//			if (m_OpenAllocations != 0) {
//				// Error
//			}
//
//			m_ParentAllocator->deallocate(m_Data, m_Size);
//			if (m_needsToDeleteParentAllocator)
//			{
//				delete m_ParentAllocator;
//			}
//			m_Data = nullptr;
//			m_Head = nullptr;
//		}
//
//		template <typename T, typename Allocator = std::allocator<PoolChunk<T>>>
//		template <typename... arguments>
//		T* PoolAllocator<T, Allocator>::allocate(arguments&&... args)
//		{
//			if (m_Head == nullptr)
//				return nullptr;
//
//			m_OpenAllocations++;
//
//			PoolChunk<T> *poolChunk = m_Head;
//			m_Head = m_Head->nextPoolChunk;
//			T* retVal = new (std::addressof(poolChunk->value)) T(std::forward<arguments>(args)...);
//			return retVal;
//		}
//
//		template <typename T, typename Allocator = std::allocator<PoolChunk<T>>>
//		void PoolAllocator<T, Allocator>::deallocate(T* data)
//		{
//			if (data == nullptr)
//			{
//				return;
//			}
//			data->~T();
//			m_OpenAllocations--;
//			PoolChunk<T>* poolChunk = reinterpret_cast<PoolChunk<T>*>(data);
//			poolChunk->nextPoolChunk = m_Head;
//			m_Head = poolChunk;
//		}
//
//		template <typename T, typename Allocator = std::allocator<PoolChunk<T>>>
//		U32 PoolAllocator<T, Allocator>::getSize() const {
//			return m_Size;
//		}
//	}
//}
















//#include "tests\EngineTest.h"
//
//int main() {
//
//	EngineTest test;
//	test.init();
//	test.run();
//
//	return 0;
//}

//int xPosition = 0, yPosition = 0;
//
//void cursor_position_callback(GLFWwindow* commingWindow, double xpos, double ypos)
//{
//	TwEventMousePosGLFW3(commingWindow, xpos, ypos);
//	int dx = (int)xpos - xPosition;
//	int dy = (int)ypos - yPosition;
//
//	float sens = 0.01f;
//	float ry = -dy * sens;
//	float rx = -dx * sens;
//
//	xPosition = (int)xpos;
//	yPosition = (int)ypos;
//
//	camera.Rotate(Vec3(1, 0, 0), -ry);
//	camera.Rotate(Vec3(0, 1, 0), -rx);
//}
//
//float angle = 0;
//Transform fromTrans(Vec3(0, 0, 0), Quaternion(0, 0, 0, 1), 1.0f);
//Transform toTrans(Vec3(0, 0, 0), Quaternion(1, 0, 0, 1), 1.0f);
//float countering = 0;
//bool cameraState = false;
//void updateInput() {
//	// Translation
//	if (gInputManager.KEYS[GLFW_KEY_W]) camera.Translate(Vec3(0, 0, 1));
//	if (gInputManager.KEYS[GLFW_KEY_S]) camera.Translate(Vec3(0, 0, -1));
//	if (gInputManager.KEYS[GLFW_KEY_A]) camera.Translate(Vec3(1, 0, 0));
//	if (gInputManager.KEYS[GLFW_KEY_D]) camera.Translate(Vec3(-1, 0, 0));
//	if (gInputManager.KEYS[GLFW_KEY_SPACE]) camera.Translate(Vec3(0, -1, 0));
// 	if (gInputManager.KEYS[GLFW_KEY_C]) camera.Translate(Vec3(0, 1, 0));
//
//	// Inits
//	if (gInputManager.KEYS[GLFW_KEY_L]) sun.setPosition(Vec3(0, 5, 0));
//
//	// Camera: UP - DOWN - FRONT - BACK- RIGHT - LEFT
//	if (gInputManager.KEYS[GLFW_KEY_8]) { camera.setRotation(Vec3(1, 0, 0), (float)(PI / 2)); camera.setPosition(Vec3(0, -10, 0)); }
//	if (gInputManager.KEYS[GLFW_KEY_2]) { camera.setRotation(Vec3(-1, 0, 0), (float)(PI / 2)); camera.setPosition(Vec3(0, 10, 0)); }
//	if (gInputManager.KEYS[GLFW_KEY_5]) { camera.setRotation(Vec3(0, 0, 0), 1); camera.setPosition(Vec3(0, 0, -10)); }
//	if (gInputManager.KEYS[GLFW_KEY_0]) { camera.setRotation(Vec3(0, 0, 1), (float)(PI / 2)); camera.setPosition(Vec3(0, 0, -10)); }
//	if (gInputManager.KEYS[GLFW_KEY_6]) { camera.setRotation(Vec3(0, 0, 1), (float)(PI / 2)); camera.setPosition(Vec3(-10, 0, 0)); }
//	if (gInputManager.KEYS[GLFW_KEY_4]) { camera.setRotation(Vec3(0, 0, -1), (float)(PI / 2)); camera.setPosition(Vec3(10, 0, 0)); }
//
//	//Rotation
//	if (gInputManager.KEYS[GLFW_KEY_Q]) camera.Rotate(Vec3(0, 0, 1), 0.1f);
//
//	// Special
//	if (gInputManager.KEYS[GLFW_KEY_ENTER]) camera.printInfo();
//	if (gInputManager.KEYS[GLFW_KEY_U]) { camera.setRotation(Vec3(1, 0, 0), (float)(PI / 2));	camera.setPosition(Vec3(0, -10, 0)); }
//	if (gInputManager.KEYS[GLFW_KEY_V]) checkConsole();
//
//	if (gInputManager.KEYS[GLFW_KEY_X]) camera.m_Speed = 0.1f;
//	if (gInputManager.KEYS[GLFW_KEY_B]) camera.m_Speed = 0.01f;
//
//	if (gInputManager.KEYS[GLFW_KEY_P]) {
//		/*if (beta < 1)*/ beta += 0.01f;
//	}
//	if (gInputManager.KEYS[GLFW_KEY_O]) {
//		/*if (beta > 0)*/ beta -= 0.01f;
//	}
//
//	if (gInputManager.KEYS[GLFW_KEY_K]) {
//		//if(entity.animatable->m_pHandle->playbackRate < 3.0f)	entity.animatable->m_pHandle->playbackRate += 0.02f;
//		//LOG("PlaybackRate", entity.animatable->m_pHandle->playbackRate);
//
//		gAnimationManager.update(0.005f);
//		//selectAnimation = 0;
//	}
//	if (gInputManager.KEYS[GLFW_KEY_J]) {
//		//if(entity.animatable->m_pHandle->playbackRate > -1.02) entity.animatable->m_pHandle->playbackRate -= 0.02f;
//		//LOG("PlaybackRate", entity.animatable->m_pHandle->playbackRate);
//		gAnimationManager.update(-0.005f);
//		//selectAnimation = 1.0f;
//	}
//	if (gInputManager.KEYS[GLFW_KEY_UP]) {
//		eentity.transform.position.y += 0.1f;
//		eentity.collidable->max.y += 0.1f;
//		eentity.collidable->min.y += 0.1f;
//		//AnimationEvent e(0, true);
//		//gAnimationManager.onEvent(&e);
//	}
//	else {
//		//AnimationEvent e(0, false);
//		//gAnimationManager.onEvent(&e);
//	}
//
//	if (gInputManager.KEYS[GLFW_KEY_DOWN]) {
//		eentity.transform.position.y -= 0.1f;
//		eentity.collidable->max.y -= 0.1f;
//		eentity.collidable->min.y -= 0.1f;
//	}
//	if (gInputManager.KEYS[GLFW_KEY_RIGHT]) {
//		eentity.transform.position.x += 0.1f;
//		eentity.collidable->max.x += 0.1f;
//		eentity.collidable->min.x += 0.1f;
//	}
//	if (gInputManager.KEYS[GLFW_KEY_LEFT]) {
//		eentity.transform.position.x -= 0.1f;
//		eentity.collidable->max.x -= 0.1f;
//		eentity.collidable->min.x -= 0.1f;
//	}
//
//	if (gInputManager.KEYS[GLFW_KEY_H]) {
//		if(countering < 1)countering += 0.001f;
//		eentity.transform = ani_transform(fromTrans, toTrans, countering);
//	}
//	
//	if (gInputManager.KEYS[GLFW_KEY_M]) {			
//		renderer.setCamera(&followCam);
//	}
//	if (gInputManager.KEYS[GLFW_KEY_N]) {
//		renderer.setCamera(&camera);
//	}
//}
//
//
//
//void key_callback(GLFWwindow* commingWindow, int key, int scancode, int action, int mods){	
//	TwEventKeyGLFW3(commingWindow, key, scancode, action, mods);
//	
//	if (action == GLFW_PRESS) {
//		gInputManager.KEYS[key] = true;
//		KeyPressedEvent ke(key);
//		engine::Event *e = &ke;
//		gInputManager.onEvent(e);
//	}
//	else if (action == GLFW_RELEASE) {
//		gInputManager.KEYS[key] = false;
//		engine::Event e(EventType::EVENT_KEY_RELEASED);
//		gInputManager.onEvent(&e);
//	}
//}