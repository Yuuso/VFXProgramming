#pragma once

#define PI 3.141592653589793238462f

#include "teapot.h"
#include <graphics/Mesh.h>


class SimpleMeshRenderingScene : public Scene
{
public:
	SimpleMeshRenderingScene()
	{
		LOG("SimpleMeshRenderingScene construct");

		FRM_SHADER_ATTRIBUTE attributes[3] =			
		{
			{ "g_vPositionOS", graphics::ATTRIB_POSITION },
			{ "g_vNormalOS", graphics::ATTRIB_NORMAL },
			{ "g_vUVCoordOS", graphics::ATTRIB_UV }
		};

		core::Ref<graphics::Shader> shader = new graphics::Shader("assets/Simple3D.vs", "assets/Simple3D.fs", attributes, sizeof(attributes) / sizeof(FRM_SHADER_ATTRIBUTE));
		
		m_mesh = createTeapotMesh();
		
		m_sharedValues = new SharedShaderValues();
		m_sharedValues->totalTime = 0.0f;
		m_material = new GlobalShaderUniforms(shader, m_sharedValues);

		checkOpenGL();
	}


	virtual ~SimpleMeshRenderingScene()
	{
		LOG("SimpleMeshRenderingScene destruct");
		delete m_sharedValues;
	}


	virtual void update(graphics::ESContext* esContext, float deltaTime)
	{
		m_totalTime += deltaTime;
		m_sharedValues->totalTime = m_totalTime;

		float fAspect = (float) esContext->width / (float) esContext->height;

		m_matProjection = slmath::perspectiveFovRH(slmath::radians(45.0f), fAspect, 5.0f, 1000.0f);

		m_matView = slmath::lookAtRH(slmath::vec3(0.0f, 70.0f, 70.0f), slmath::vec3(0.0f, 15.0f, 0.0f), slmath::vec3(0.0f, 1.0f, 0.0f));

		m_matModel = slmath::rotationX(-PI*0.5f);
		m_matModel = slmath::rotationY(m_totalTime) * m_matModel;
		m_matModel = slmath::translation(slmath::vec3(0.0f, 0.0f, 0.0f)) * m_matModel;
	}


	virtual void render(graphics::ESContext* esContext)
	{
		glViewport(0, 0, esContext->width, esContext->height);

		glClearColor(0.0f, 0.8f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		checkOpenGL();

		glDisable(GL_BLEND);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		checkOpenGL();

		slmath::mat4 matModelView = m_matView * m_matModel;
		slmath::mat4 matModelViewProj = m_matProjection * matModelView;
		 
		m_sharedValues->matModelViewProj = matModelViewProj;

		m_material->bind();
		checkOpenGL();

		m_mesh->render();
		checkOpenGL();

	}
private:
	graphics::Mesh* createTeapotMesh()
	{
		graphics::IndexBuffer* ib = new graphics::IndexBuffer(TeapotData::indices, TeapotData::numIndices);
		graphics::VertexArray* va [] =
		{
			new graphics::VertexArrayImpl<slmath::vec3>
			(graphics::ATTRIB_POSITION, (slmath::vec3*)TeapotData::positions, TeapotData::numVertices),
			new graphics::VertexArrayImpl<slmath::vec3>
			(graphics::ATTRIB_NORMAL, (slmath::vec3*)TeapotData::normals, TeapotData::numVertices),
			new graphics::VertexArrayImpl<slmath::vec3>
			(graphics::ATTRIB_UV, (slmath::vec3*)TeapotData::texCoords, TeapotData::numVertices),
		};
		graphics::VertexBuffer* vb = new graphics::VertexBuffer(&va[0], sizeof(va) / sizeof(va[0]));
		return new graphics::Mesh(ib, vb);
	}

	slmath::mat4 m_matProjection;
	slmath::mat4 m_matView;
	slmath::mat4 m_matModel;

	core::Ref<graphics::Mesh> m_mesh;
	float m_totalTime;
	SharedShaderValues* m_sharedValues;
	core::Ref<graphics::ShaderUniforms> m_material;
};