#include "scene.h"
#include <core/log.h>


class QuadScene : public Scene
{
public:
	QuadScene()
	{
		LOG("TriangleScene construct");
		checkOpenGL();

		m_count = 0.1f;
		m_timer = 0.0f;
		m_hShaderProgram = 0;

		const char* g_strVSProgram =
			"attribute vec4 g_vVertex;											 \n"
			"attribute vec4 g_vColor;											 \n"
			//"attribute vec2 g_vTexCoord;										 \n"
			"varying   vec4 g_vVSColor;                                          \n"
			"varying   vec2 coord;                                          \n"
			"																	 \n"
			"void main()														 \n"
			"{																	 \n"
			"    gl_Position  = vec4( g_vVertex.x, g_vVertex.y,                  \n"
			"                         g_vVertex.z, g_vVertex.w );                \n"
			"    g_vVSColor = g_vColor;                                          \n"
			"    coord = gl_Position.xy;                                         \n"
			"}																	 \n";


		const char* g_strFSProgram =
			"#ifdef GL_FRAGMENT_PRECISION_HIGH									 \n"
			"   precision highp float;											 \n"
			"#else																 \n"
			"   precision mediump float;										 \n"
			"#endif																 \n"
			"																	 \n"
			"varying   vec4 g_vVSColor;                                          \n"
			"varying   vec2 coord;                                          \n"
			"uniform float timer;                                          \n"
			"																	 \n"
			"void main()														 \n"
			"{																	 \n"
			"	float t= timer;																 \n"
			"	float x = coord.x * 250.0;																 \n"
			"	float y = coord.y * 250.0;																 \n"
			"																	 \n"
			"	float vv = 0.25 * (																 \n"
			"			(0.5 + (0.5 * sin(x/7.0+t))) +														 \n"
			"			(0.5 + (0.5 * sin(y/5.0-t))) +														 \n"
			"			(0.5 + (0.5 * sin((x+y)/6.0-t))) +														 \n"
			"			(0.5 + (0.5 * sin(sqrt(float(x*x + y*y))/3.0+sin(t)*5))) 														 \n"
			"			);														 \n"
			"																	 \n"
			"    gl_FragColor = vec4(vv*0.5, vv*0.6*t/6, vv, 1.0f) * g_vVSColor;                                      \n"
			"    if(gl_FragColor.x < 0.1)                                     \n"
			"    {                                    \n"
			"		discard;                                     \n"
			"    }                                    \n"
			"}																	 \n";

		FRM_SHADER_ATTRIBUTE attributes[2] = {
			{ "g_vVertex", 0 },
			{ "g_vColor", 1 }
		};

		FrmCompileShaderProgram(g_strVSProgram, g_strFSProgram, &m_hShaderProgram, attributes, sizeof(attributes) / sizeof(FRM_SHADER_ATTRIBUTE));

		checkOpenGL();
	}


	virtual ~QuadScene()
	{
		glDeleteProgram(m_hShaderProgram);
		LOG("QuadScene destruct");
	}


	virtual void update(graphics::ESContext* esContext, float deltaTime)
	{
		static float DIR = 1.0f;
		m_count += deltaTime/10*DIR;
		m_timer += deltaTime*3.5f;

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
		glClearColor(0.0f, m_count/2.0f, m_count, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		checkOpenGL();

		// Set the shader program and the texture
		glUseProgram(m_hShaderProgram);
		checkOpenGL();

		GLint timerLocation = glGetUniformLocation(m_hShaderProgram, "timer");
		glUniform1f(timerLocation, m_timer);

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
	GLuint m_hShaderProgram;
};


