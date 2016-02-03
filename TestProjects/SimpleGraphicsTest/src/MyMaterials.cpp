#include "MyMaterials.h"


GlobalShaderUniforms::GlobalShaderUniforms(graphics::Shader* _shader, const SharedShaderValues* _sharedShaderValues) : graphics::ShaderUniforms(_shader), m_sharedShaderValues(_sharedShaderValues)
{
}
GlobalShaderUniforms::~GlobalShaderUniforms()
{
}


void GlobalShaderUniforms::getUniformLocations(graphics::Shader* _shader)
{
	m_id = glGetUniformLocation(_shader->getProgram(), "g_matModelViewProj");
	m_idtime = glGetUniformLocation(_shader->getProgram(), "totalTime");
}

void GlobalShaderUniforms::bind(graphics::Shader* _shader)
{
	_shader->bind();
	if (m_sharedShaderValues)
	{
		if (m_id >= 0)
		{
			glUniformMatrix4fv(m_id, 1, GL_FALSE, &m_sharedShaderValues->matModelViewProj[0][0]);
		}
		if (m_idtime >= 0)
		{
			glUniform1f(m_idtime, m_sharedShaderValues->totalTime);
		}
	}
}