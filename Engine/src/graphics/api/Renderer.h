//------------------------------------------------------------------------------
// UniformBuffer.h
//
// To render graphics
//
// Author: Sommerauer Christian
// Created: xx.xx.xx
// Changed: 30.07.18
//------------------------------------------------------------------------------
//

#pragma once


#include <entity/Entity.h>
#include <entity\Scene.h>
#include <graphics\api\Shader.h>
#include <graphics\api\Cubemap.h>

#include <entity\Light.h>

#include <graphics\pbr\PBRMaterial.h>
#include <graphics\api\VertexArray.h>

#include <Components\Renderable3D.h>

#include <common/types.h>

#define MAX_LIGHTS 4

using namespace engine::component;

namespace engine {
	// forward declaration
	struct collision::AABB3D;

	class RenderQueue
	{

	};
	
	namespace graphics {
		// forward declaration
		class Camera;

		class Renderer {
		private:
			Camera *m_Camera;
			Cubemap *m_Skybox;
			U32 vbo_aabb;
			Light m_Lights[MAX_LIGHTS];


			void loadCollisionShapes();
		public:
			Renderer();
			Renderer(Camera *camera);

			Renderer(const Renderer&) = delete;

			~Renderer();

			void initialize();
			void render(const Cubemap &cubemap, const VertexArray& vao, const Shader &shader) const;
			
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

			void renderPBR(Entity &entity) const;*/



			void setCamera(Camera *camera);
			void setSkybox(Cubemap *skybox) { m_Skybox = skybox; }

			void setLight(U32 slot, const Light &light);
			Light &getLight(U32 slot) ;

			void combineTextures(const U32 vao, const Texture &tex1, const Texture &tex2, const Shader &shader);
		
		
			// NEW

			void submit(const RenderItem& renderable);

		};
	}
}