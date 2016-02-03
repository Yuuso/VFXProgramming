#include "scene.h"
#include "MyMaterials.h"

#include <graphics/Shader.h>
#include <core/log.h>


class FullscreenQuadScene : public Scene
{
public:
	FullscreenQuadScene()
	{
		LOG("FullscreenQuadScene construct");
		checkOpenGL();

		m_count = 0.1f;
		m_timer = 0.0f;
		

		FRM_SHADER_ATTRIBUTE attributes[2] = 
		{
			{ "g_vVertex", 0 },
			{ "g_vColor", 1 }
		};

		int numAttributes = sizeof(attributes) / sizeof(FRM_SHADER_ATTRIBUTE);
		m_shader = new graphics::Shader("assets/FullScreenQuad.vs", "assets/PlasmaEffect.fs", attributes, numAttributes);

		m_sharedValues = new SharedShaderValues();
		//m_sharedValues->matModelViewProj = 0.0f;
		m_material = new GlobalShaderUniforms(m_shader, m_sharedValues);
		
		checkOpenGL();
	}


	virtual ~FullscreenQuadScene()
	{
		LOG("FullscreenQuadScene destruct");
	}


	virtual void update(graphics::ESContext* esContext, float deltaTime)
	{
		static float DIR = 1.0f;
		m_count += deltaTime / 10 * DIR;
		m_timer += deltaTime*3.5f;

		//m_sharedValues->totalTime += deltaTime;

		if (m_count > 0.2f)
			DIR = -1.0f;
		if (m_count < 0.1f)
			DIR = 1.0f;
	}


	virtual void render(graphics::ESContext* esContext)
	{
		checkOpenGL();

		// Set the viewport
		glViewport(0, 0, esContext->width, esContext->height);
		checkOpenGL();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		float aspectRatio = float(esContext->width) / float(esContext->height);
		float fSize = 0.5f;

		float quadX = 1.0f;
		float quadY = 1.0f;

		float VertexPositions [] =
		{
			-quadX, -quadY, 0.0f, 1.0f,
			-quadX, quadY, 0.0f, 1.0f,
			quadX, quadY, 0.0f, 1.0f,
			quadX, -quadY, 0.0f, 1.0f,
		};

		float VertexColors [] =
		{
			0.0f, 0.2f, 0.6f, 1.0f,
			0.2f, 0.5f, 0.0f, 1.0f,
			0.8f, 0.3f, 0.6f, 0.8f,
			0.4f, 0.1f, 0.0f, 0.7f,
		};

		//float texCoord[]=
		//{
		//	1.0f, 0.0f,
		//	0.0f, 0.0f,
		//	0.0f, 1.0f,
		//	1.0f, 1.0f,
		//};

		char indices [] =
		{
			0, 1, 2, 0, 2, 3,
		};

		// Clear the backbuffer and depth-buffer
		glClearColor(0.0f, m_count / 2.0f, m_count, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		checkOpenGL();

		// Set the shader program and the texture
		m_material->bind();
		checkOpenGL();

		// Draw the colored triangle
		glVertexAttribPointer(0, 4, GL_FLOAT, 0, 0, VertexPositions);
		checkOpenGL();

		glEnableVertexAttribArray(0);
		checkOpenGL();

		glVertexAttribPointer(1, 4, GL_FLOAT, 0, 0, VertexColors);
		checkOpenGL();

		glEnableVertexAttribArray(1);
		checkOpenGL();

		//glVertexAttribPointer(2, 2, GL_FLOAT, 0, 0, texCoord);
		//checkOpenGL();

		//glEnableVertexAttribArray(2);
		//checkOpenGL();

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);
		checkOpenGL();

		glDisableVertexAttribArray(0);
		checkOpenGL();
		glDisableVertexAttribArray(1);
		checkOpenGL();
	}
private:
	float m_count;
	float m_timer;
	core::Ref<graphics::Shader> m_shader;
	SharedShaderValues* m_sharedValues;
	core::Ref<graphics::ShaderUniforms> m_material;
};


