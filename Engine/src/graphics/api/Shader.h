//------------------------------------------------------------------------------
// Shader.h
//
// Represents a shader program
//
// Author: Sommerauer Christian
// Created: xx.xx.xx
// Changed: 30.07.18
//------------------------------------------------------------------------------
//


#pragma once

#include <math/mat4.h>
#include <common\types.h>
#include <string>
#include <graphics\api\UniformBuffer.h>

#define SHADER_FOLDER "res/shader/"

namespace engine { namespace graphics{


	struct ShaderSource
	{
		std::string vertexShader;
		std::string fragmentShader;
	};

	class Shader
	{
	private:

		U32 m_ShaderID;

		//U32 m_UniformIndex;

		const char* m_VertPath;
		const char* m_FragPath;

	private:

		ShaderSource parseFile(const char* path);
		ShaderSource parseFile(const char* vertexPath, const char* fragmentPath);

		U32 load(ShaderSource source);

	public:
		// Ctr
		Shader(const char* vertexPath, const char* fragmentPath);
		Shader(const char* shaderPath);

		// Deleted Ctr
		Shader() = delete;
		Shader(const Shader&) = delete;

		~Shader();

		void enable() const;
		void disable() const;

		// Set Uniform variables
		void setUniformMat4(const char *name, const mat4 &matrix) const;
		void setUniform3f(const char *name, const Vec3 &value) const;
		void setUniformMat4Array(const char *name, const F32 *matrix) const;
		void setUniform1f(const char *name, F32 value) const;
		void setUniform1i(const char *name, I32 value) const;
		void setUniformTexture(const char *name, I32 slot, I32 texture) const;

		void setUniformBlockIndex(const char* name, U32 slot);
		//void setUniformBuffer(const UniformBuffer& uniforms, const char* name);
	};

}}