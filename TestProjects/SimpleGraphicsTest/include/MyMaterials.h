#pragma once

#include <graphics\Shader.h>

struct SharedShaderValues : public core::Object
{
	float totalTime;
	slmath::mat4 matModel;
	slmath::mat4 matView;
	slmath::mat4 matProj;
	slmath::mat4 matModelView;
	slmath::mat4 matNormal;
	slmath::mat4 matModelViewProj;
	slmath::vec3 lightPos;
	slmath::vec3 camPos;
};

class GlobalShaderUniforms : public graphics::ShaderUniforms
{
public:
	GlobalShaderUniforms(graphics::Shader* _shader, const SharedShaderValues* _sharedShaderValues = 0);
	virtual ~GlobalShaderUniforms();

	virtual void getUniformLocations(graphics::Shader* _shader);
	virtual void bind(graphics::Shader* _shader);

private:
	const SharedShaderValues* m_sharedShaderValues;
	GLint m_id[8];
	GLint m_idtime;
};