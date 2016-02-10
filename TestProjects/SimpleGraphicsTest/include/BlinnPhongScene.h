#pragma once

#define PI 3.141592653589793238462f

#include "teapot.h"
#include "SimpleMaterialUniforms.h"
#include <graphics/Mesh.h>
#include <slmath/slmath.h>
#include <core/Input.h>
#include <graphics/Texture.h>
#include <graphics/Image.h>


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

		graphics::Image* image = new graphics::Image(256, 256, 8);
		image = graphics::Image::loadFromTGA("assets/CheckerBoard.tga");
		simpleMaterialUniforms->diffuseMap->setData(image);

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
		static slmath::vec3 transl = slmath::vec3(1.0f);
		static float cameraSpeed = 3.0f;
		static float rotation = 0.0f;
		static slmath::vec2 lastMouseCoord = slmath::vec2(core::getMouseAxisX(), core::getMouseAxisY());
		static float viewY = 15.0f;


		m_totalTime += deltaTime;
		m_sharedValues->totalTime = m_totalTime;

		float fAspect = (float) esContext->width / (float) esContext->height;

		m_matProjection = slmath::perspectiveFovRH(slmath::radians(90.0f), fAspect, 5.0f, 1000.0f);

		m_matView = slmath::lookAtRH(slmath::vec3(0.0f, 70.0f, 70.0f), slmath::vec3(0.0f, viewY, 0.0f), slmath::vec3(0.0f, 1.0f, 0.0f));

		if (core::getKeyState(core::KEY_W))
		{
			transl.z += cameraSpeed;
		}
		if (core::getKeyState(core::KEY_S))
		{
			transl.z -= cameraSpeed;
		}
		if (core::getKeyState(core::KEY_A))
		{
			transl.x += cameraSpeed;
		}
		if (core::getKeyState(core::KEY_D))
		{
			transl.x -= cameraSpeed;
		}
		if (core::getMouseButtonState(core::MOUSE_LEFT))
		{
			transl.x += (core::getMouseAxisX() - lastMouseCoord.x) * 0.3f;
			transl.y += (core::getMouseAxisY() - lastMouseCoord.y) * 0.3f;
		}
		if (core::getMouseButtonState(core::MOUSE_RIGHT))
		{
			rotation += (core::getMouseAxisX() - lastMouseCoord.x) * 0.003f;
			transl.x -= (core::getMouseAxisX() - lastMouseCoord.x) * 0.3f;
			viewY -= (core::getMouseAxisY() - lastMouseCoord.y) * 0.3f;
		}

		m_matModel = slmath::rotationX(-PI*0.5f);
		m_matModel = slmath::rotationY(rotation) * m_matModel;
		m_matModel = slmath::translation(transl) * m_matModel;

		m_sharedValues->matModel = m_matModel;
		m_sharedValues->matView = m_matView;
		m_sharedValues->matProj = m_matProjection;

		slmath::mat4 matModelView = m_matView * m_matModel;
		slmath::mat4 matModelViewProj = m_matProjection * matModelView;
		slmath::mat4 matNormal = slmath::transpose(slmath::inverse(matModelView));
		
		m_sharedValues->matModelView = matModelView;
		m_sharedValues->matNormal = matNormal;
		m_sharedValues->matModelViewProj = matModelViewProj;

		m_sharedValues->lightPos = slmath::vec3((float) core::getMouseAxisX() - esContext->width / 2.0f, (float) core::getMouseAxisY() + esContext->height / 2.0f, -20.0f);
		m_sharedValues->camPos = slmath::vec3(0.0f, (float) core::getMouseAxisY(), (float) core::getMouseAxisX());
		m_sharedValues->camPos = transl;

		lastMouseCoord = slmath::vec2(core::getMouseAxisX(), core::getMouseAxisY());
	}


	virtual void render(graphics::ESContext* esContext)
	{
		glViewport(0, 0, esContext->width, esContext->height);

		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
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