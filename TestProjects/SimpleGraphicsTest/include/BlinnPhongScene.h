#pragma once

#define PI 3.141592653589793238462f

#include "teapot.h"
#include "SimpleMaterialUniforms.h"
#include <graphics/Mesh.h>
#include <slmath/slmath.h>
#include <core/Input.h>


class BlinnPhongScene : public Scene
{
public:
	BlinnPhongScene()
	{
		LOG("BlinnPhongScene construct");

		FRM_SHADER_ATTRIBUTE attributes[3] =
		{
			{ "g_vPositionOS", graphics::ATTRIB_POSITION },
			{ "g_vNormalOS", graphics::ATTRIB_NORMAL },
			{ "g_vUVCoordOS", graphics::ATTRIB_UV }
		};

		core::Ref<graphics::Shader> shader = new graphics::Shader("assets/Blinn-Phong.vs", "assets/Blinn-Phong.fs", attributes, sizeof(attributes) / sizeof(FRM_SHADER_ATTRIBUTE));

		m_mesh = createTeapotMesh();
		mesh = graphics::Mesh::loadFromOBJ("assets/Sphere.obj");

		m_sharedValues = new SharedShaderValues();
		m_sharedValues->totalTime = 0.0f;
		SimpleMaterialUniforms* simpleMaterialUniforms = new SimpleMaterialUniforms(shader, m_sharedValues);

		simpleMaterialUniforms->vAmbient = slmath::vec4(0.5f, 0.2f, 1.0f, 1.0f);
		simpleMaterialUniforms->vDiffuse = slmath::vec4(0.5f, 0.2f, 1.0f, 1.0f);
		simpleMaterialUniforms->vSpecular = slmath::vec4(1.0f, 1.0f, 1.0f, 5.0f);
		m_material = simpleMaterialUniforms;

		checkOpenGL();
	}


	virtual ~BlinnPhongScene()
	{
		LOG("BlinnPhongScenedestruct");
		delete m_sharedValues;
		delete mesh;
	}


	virtual void update(graphics::ESContext* esContext, float deltaTime)
	{
		m_totalTime += deltaTime;
		m_sharedValues->totalTime = m_totalTime;

		float fAspect = (float) esContext->width / (float) esContext->height;

		m_matProjection = slmath::perspectiveFovRH(slmath::radians(45.0f), fAspect, 5.0f, 1000.0f);

		m_matView = slmath::lookAtRH(slmath::vec3(0.0f, 70.0f, 70.0f), slmath::vec3(0.0f, 15.0f, 0.0f), slmath::vec3(0.0f, 1.0f, 0.0f));

		m_matModel = slmath::rotationX(-PI*0.5f);
		m_matModel = slmath::rotationY(m_totalTime/3.0f) * m_matModel;
		m_matModel = slmath::translation(slmath::vec3(cos(m_totalTime), sin(m_totalTime), sin(m_totalTime))) * m_matModel;

		m_sharedValues->matModel = m_matModel;
		m_sharedValues->matView = m_matView;
		m_sharedValues->matProj = m_matProjection;

		slmath::mat4 matModelView = m_matView * m_matModel;
		slmath::mat4 matModelViewProj = m_matProjection * matModelView;
		slmath::mat4 matNormal = slmath::transpose(slmath::inverse(matModelView));
		
		m_sharedValues->matModelView = matModelView;
		m_sharedValues->matNormal = matNormal;
		m_sharedValues->matModelViewProj = matModelViewProj;

		m_sharedValues->lightPos = slmath::vec3((float) core::getMouseAxisX() - esContext->width/2.0f, (float) core::getMouseAxisY() + esContext->height/2.0f, -20.0f);
		m_sharedValues->camPos = slmath::vec3(0.0f, 70.0f, 70.0f);
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
		mesh->render();
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
	graphics::Mesh* mesh;
	float m_totalTime;
	SharedShaderValues* m_sharedValues;
	core::Ref<graphics::ShaderUniforms> m_material;
};