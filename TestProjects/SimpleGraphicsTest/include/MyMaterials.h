#pragma once

#include <graphics\Shader.h>

struct SharedShaderValues : public core::Object
{
	float totalTime;
	slmath::mat4 matModelViewProj;
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
	GLint m_id;
	GLint m_idtime;
};