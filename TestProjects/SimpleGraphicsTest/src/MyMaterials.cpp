#include "MyMaterials.h"


GlobalShaderUniforms::GlobalShaderUniforms(graphics::Shader* _shader, const SharedShaderValues* _sharedShaderValues) : ShaderUniforms(_shader), m_sharedShaderValues(_sharedShaderValues)
{
}
GlobalShaderUniforms::~GlobalShaderUniforms()
{
}


void GlobalShaderUniforms::getUniformLocations(graphics::Shader* _shader)
{
	m_id[0] = glGetUniformLocation(_shader->getProgram(), "g_matModel");
	m_id[1] = glGetUniformLocation(_shader->getProgram(), "g_matView");
	m_id[2] = glGetUniformLocation(_shader->getProgram(), "g_matProj");
	m_id[3] = glGetUniformLocation(_shader->getProgram(), "g_matModelView");
	m_id[4] = glGetUniformLocation(_shader->getProgram(), "g_matNormal");
	m_id[5] = glGetUniformLocation(_shader->getProgram(), "g_matModelViewProj");
	m_id[6] = glGetUniformLocation(_shader->getProgram(), "g_lightPos");
	m_id[7] = glGetUniformLocation(_shader->getProgram(), "g_camPos");
	m_idtime = glGetUniformLocation(_shader->getProgram(), "totalTime");
}

void GlobalShaderUniforms::bind(graphics::Shader* _shader)
{
	_shader->bind();
	if (m_sharedShaderValues)
	{
		glUniformMatrix4fv(m_id[0], 1, GL_FALSE, &m_sharedShaderValues->matModel[0][0]);
		glUniformMatrix4fv(m_id[1], 1, GL_FALSE, &m_sharedShaderValues->matView[0][0]);
		glUniformMatrix4fv(m_id[2], 1, GL_FALSE, &m_sharedShaderValues->matProj[0][0]);
		glUniformMatrix4fv(m_id[3], 1, GL_FALSE, &m_sharedShaderValues->matModelView[0][0]);
		glUniformMatrix4fv(m_id[4], 1, GL_FALSE, &m_sharedShaderValues->matNormal[0][0]);
		glUniformMatrix4fv(m_id[5], 1, GL_FALSE, &m_sharedShaderValues->matModelViewProj[0][0]);

		glUniform3fv(m_id[6], 1, &m_sharedShaderValues->lightPos[0]);
		glUniform3fv(m_id[7], 1, &m_sharedShaderValues->camPos[0]);

		glUniform1f(m_idtime, m_sharedShaderValues->totalTime);
	}
}