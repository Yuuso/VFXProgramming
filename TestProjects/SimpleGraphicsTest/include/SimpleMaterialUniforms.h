#pragma once

#include "MyMaterials.h"
#include <graphics\Shader.h>
#include <graphics\Texture.h>

class SimpleMaterialUniforms : public graphics::ShaderUniforms
{
public:
	slmath::vec4 vAmbient;
	slmath::vec4 vDiffuse;
	slmath::vec4 vSpecular;
	core::Ref<graphics::Texture2D> diffuseMap;
	core::Ref<graphics::TextureCube> cubeMap;

public:
	SimpleMaterialUniforms(graphics::Shader* shader, SharedShaderValues* sharedValues = 0) : ShaderUniforms(shader), 
		m_globalShaderUniforms(new GlobalShaderUniforms(shader, sharedValues))
	{}
	virtual ~SimpleMaterialUniforms()
	{
	}

	virtual void getUniformLocations(graphics::Shader* shader)
	{
		m_globalShaderUniforms->getUniformLocations(shader);
		m_materialAmbientLoc = glGetUniformLocation(shader->getProgram(), "g_Material.vAmbient");
		m_materialDiffuseLoc = glGetUniformLocation(shader->getProgram(), "g_Material.vDiffuse");
		m_materialSpecularLoc = glGetUniformLocation(shader->getProgram(), "g_Material.vSpecular");
		m_diffuseMapLoc = glGetUniformLocation(shader->getProgram(), "s_diffuseMap");
		m_cubeMapLoc = glGetUniformLocation(shader->getProgram(), "s_cubeMap");
	}

	virtual void bind(graphics::Shader* shader)
	{
		shader->bind();
		m_globalShaderUniforms->bind(shader);

		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap->getTextureId());

		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap->getTextureId());

		glActiveTexture(GL_TEXTURE0 + 1);

		glUniform4fv(m_materialAmbientLoc, 1, &vAmbient[0]);
		glUniform4fv(m_materialDiffuseLoc, 1, &vDiffuse[0]);
		glUniform4fv(m_materialSpecularLoc, 1, &vSpecular[0]);

		glUniform1i(m_diffuseMapLoc, 0);
		glUniform1i(m_cubeMapLoc, 1);
	}

private:
	core::Ref<GlobalShaderUniforms> m_globalShaderUniforms;
	GLint m_materialAmbientLoc;
	GLint m_materialDiffuseLoc;
	GLint m_materialSpecularLoc;
	GLint m_diffuseMapLoc;
	GLint m_cubeMapLoc;
};