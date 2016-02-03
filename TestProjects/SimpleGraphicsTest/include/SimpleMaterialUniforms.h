#pragma once

#include "MyMaterials.h"
#include <graphics\Shader.h>

class SimpleMaterialUniforms : public graphics::ShaderUniforms
{
public:
	slmath::vec4 vAmbient;
	slmath::vec4 vDiffuse;
	slmath::vec4 vSpecular;

public:
	SimpleMaterialUniforms(graphics::Shader* shader, SharedShaderValues* sharedValues = 0) : ShaderUniforms(shader), 
		m_globalShaderUniforms(new GlobalShaderUniforms(shader, sharedValues))
	{}
	virtual ~SimpleMaterialUniforms()
	{}

	virtual void getUniformLocations(graphics::Shader* shader)
	{
		m_globalShaderUniforms->getUniformLocations(shader);
		m_materialAmbientLoc = glGetUniformLocation(shader->getProgram(), "g_Material.vAmbient");
		m_materialDiffuseLoc = glGetUniformLocation(shader->getProgram(), "g_Material.vDiffuse");
		m_materialSpecularLoc = glGetUniformLocation(shader->getProgram(), "g_Material.vSpecular");
	}

	virtual void bind(graphics::Shader* shader)
	{
		shader->bind();
		m_globalShaderUniforms->bind(shader);
		glUniform4fv(m_materialAmbientLoc, 1, &vAmbient[0]);
		glUniform4fv(m_materialDiffuseLoc, 1, &vDiffuse[0]);
		glUniform4fv(m_materialSpecularLoc, 1, &vSpecular[0]);
	}

private:
	core::Ref<GlobalShaderUniforms> m_globalShaderUniforms;
	GLint m_materialAmbientLoc;
	GLint m_materialDiffuseLoc;
	GLint m_materialSpecularLoc;
};