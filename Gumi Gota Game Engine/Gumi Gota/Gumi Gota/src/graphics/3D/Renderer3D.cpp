
/* INCLUDES */
// Self Include
#include "Renderer3D.h"

// OpenGL Includes
#include <GL/glew.h>

// Normal Includes
#include <sstream>
#include <iostream>

// Header Includes
#include "../ErrorHandling.h"



/* MACROS */
#define RENDERER3D_DEFAULT_ZNEAR		0.1f
#define RENDERER3D_DEFAULT_ZFAR			100.0f
#define RENDERER3D_DEFAULT_FOG_DENSITY	0.016f
#define RENDERER3D_DEFAULT_FOG_GRADIENT 4.0f
#define RENDERER3D_DEFAULT_SKY_COLOR	0.0f, 0.0f, 0.0f

#define RENDERER3D_MAX_RENDERED_LIGHTS	 16
#define RENDERER3D_LIGHT_UPDATE_INTERVAL 0.1f

#define RENDERER3D_MAX_PARTICLE_INSTANCES				 10000
#define RENDERER3D_PARTICLE_INSTANCE_DATA_LENGTH		 18

#define RENDERER3D_DEFAULT_REFLECT_LIGHT_ON_WATER  true
#define RENDERER3D_DEFAULT_REFLECT_SCENE_ON_WATER  true
#define RENDERER3D_DEFAULT_WATER_REFLECTION_WIDTH  720
#define RENDERER3D_DEFAULT_WATER_REFLECTION_HEIGHT 1280
#define RENDERER3D_DEFAULT_WATER_REFRACTION_WIDTH  720
#define RENDERER3D_DEFAULT_WATER_REFRACTION_HEIGHT 1280

#define RENDERER3D_DEFAULT_ANTIALIASING_SAMPLE_AMOUNT 4



namespace gg
{

	/* ADDITIONAL STUFF */
	const static float s_particleData[] = {
		-0.5f,  0.5f,
		-0.5f, -0.5f,
		 0.5f,  0.5f,
		 0.5f, -0.5f
	};

	const static float s_waterData[] = {
		-1.0f, 0.0f,  1.0f,
		-1.0f, 0.0f, -1.0f,
		 1.0f, 0.0f,  1.0f,
		 1.0f, 0.0f, -1.0f
	};



	/* Constructors */
	Renderer3D::Renderer3D(Window* window)
		: m_renderWindow(window),
		  m_basicLightShader("Gumi Gota/Gumi Gota/res/shaders/BasicLight3D.shader"),
		  m_skyboxShader("Gumi Gota/Gumi Gota/res/shaders/Skybox3D.shader"),
		  m_particleShader("Gumi Gota/Gumi Gota/res/shaders/Particle3D.shader"),
		  m_aboveWaterShader("Gumi Gota/Gumi Gota/res/shaders/AboveWater3D.shader"),
		  m_belowWaterShader("Gumi Gota/Gumi Gota/res/shaders/BelowWater3D.shader"),
		  m_zNear(RENDERER3D_DEFAULT_ZNEAR), m_zFar(RENDERER3D_DEFAULT_ZFAR),
		  m_lightUpdateInterval(RENDERER3D_LIGHT_UPDATE_INTERVAL),
		  m_particleVAO(),
		  m_waterDuDvMap("Gumi Gota/Gumi Gota/res/maps/WaterDuDvMap.png"), m_waterNormalMap("Gumi Gota/Gumi Gota/res/maps/WaterNormalMap.png"),
		  m_waterReflectionFboWidth(RENDERER3D_DEFAULT_WATER_REFLECTION_WIDTH), m_waterReflectionFboHeight(RENDERER3D_DEFAULT_WATER_REFLECTION_HEIGHT),
		  m_waterRefractionFboWidth(RENDERER3D_DEFAULT_WATER_REFRACTION_WIDTH), m_waterRefractionFboHeight(RENDERER3D_DEFAULT_WATER_REFRACTION_HEIGHT)
	{
		// Init
		m_initParticleVAO();
		m_initWaterStuff();

		// Shader Setup
		m_basicLightShader.start();
		m_basicLightShader.setUniform1i("u_textureSampler", 0);
		m_basicLightShader.setUniform1f("u_fogDensity", RENDERER3D_DEFAULT_FOG_DENSITY);
		m_basicLightShader.setUniform1f("u_fogGradient", RENDERER3D_DEFAULT_FOG_GRADIENT);
		m_basicLightShader.setUniform3f("u_skyColor", RENDERER3D_DEFAULT_SKY_COLOR);

		m_particleShader.start();
		m_particleShader.setUniform1i("u_textureSampler", 0);

		m_aboveWaterShader.start();
		m_aboveWaterShader.setUniform1i("u_reflectionTexture", 0);
		m_aboveWaterShader.setUniform1i("u_refractionTexture", 1);
		m_aboveWaterShader.setUniform1i("u_dudvMap", 2);
		m_aboveWaterShader.setUniform1i("u_normalMap", 3);
		m_aboveWaterShader.setUniform1i("u_depthMap", 4);
		m_aboveWaterShader.setUniform2f("u_zLimitsData", m_zNear, m_zFar);
		m_aboveWaterShader.setUniform1i("u_reflectLightVertex", RENDERER3D_DEFAULT_REFLECT_LIGHT_ON_WATER);
		m_aboveWaterShader.setUniform1i("u_reflectLightFragment", RENDERER3D_DEFAULT_REFLECT_LIGHT_ON_WATER);
		m_aboveWaterShader.setUniform1i("u_reflectSceneVertex", RENDERER3D_DEFAULT_REFLECT_SCENE_ON_WATER);
		m_aboveWaterShader.setUniform1i("u_reflectSceneFragment", RENDERER3D_DEFAULT_REFLECT_SCENE_ON_WATER);

		m_belowWaterShader.start();
		m_belowWaterShader.setUniform1i("u_refractionTexture", 1);
		m_belowWaterShader.setUniform1i("u_dudvMap", 2);
		m_belowWaterShader.setUniform1i("u_normalMap", 3);
		m_belowWaterShader.setUniform1i("u_depthMap", 4);
		m_belowWaterShader.setUniform2f("u_zLimitsData", m_zNear, m_zFar);

		// Multisampled Framebuffer Setup
		m_multisampledFBO.attachMultisampleTexture(m_renderWindow->getWidth(), m_renderWindow->getHeight(), RENDERER3D_DEFAULT_ANTIALIASING_SAMPLE_AMOUNT);
		m_multisampledFBO.attachMultisampleDepthBuffer(m_renderWindow->getWidth(), m_renderWindow->getHeight(), RENDERER3D_DEFAULT_ANTIALIASING_SAMPLE_AMOUNT);

		// Finishing
		VertexArray::unbind();

		Shader::stop();
	}

	/* Functions */
	void Renderer3D::render(const Camera3D& cam)
	{
		// Matrices Calculations
		matrix4 projectionMatrix = matrix4::projection(cam.getAspectRatio(), cam.getFOV(), m_zNear, m_zFar);
		matrix4 viewMatrix = matrix4::fpsView(cam.getPosition(), cam.getPitch(), cam.getYaw());

		// Antialiasing Setup
		///m_multisampledFBO.bind(m_renderWindow->getWidth(), m_renderWindow->getHeight());

		// Rendering Skybox
		m_renderSkybox(cam, projectionMatrix);

		// Rendering Renderables
		m_renderRenderables(projectionMatrix, viewMatrix);

		// Rendering Water
		m_renderWater(cam, projectionMatrix, viewMatrix);

		// Rendering Particles
		m_renderParticles(projectionMatrix, viewMatrix);

		// Antialiasing - Sampling to Actual Screen
		///m_multisampledFBO.sampleToScreen(m_renderWindow->getWidth(), m_renderWindow->getHeight());
		///m_multisampledFBO.unbind(m_renderWindow->getWidth(), m_renderWindow->getHeight());

		// Finishing
		VertexArray::unbind();

		Shader::stop();
	}


	void Renderer3D::updateLights(const vector3& pos)
	{
		// Checking if There is Enough Lights to Sort Them
		if (m_lights.size() >= RENDERER3D_MAX_RENDERED_LIGHTS)
		{
			// Checking if Lights Should Get Updated
			if (m_lightTimer.elapsed() >= m_lightUpdateInterval && m_lightUpdateInterval != 0.0f)
			{
				m_lightTimer.reset(); // Resetting Timer

				// Init
				unsigned int loadedLights = 0;

				// Getting Closest Lights Indices
				std::vector<unsigned int> closestLightsIndices = getClosestLightsIndices(pos, m_lights, RENDERER3D_MAX_RENDERED_LIGHTS);

				// Important Lights
				for (unsigned int i = 0; i < closestLightsIndices.size() && loadedLights < RENDERER3D_MAX_RENDERED_LIGHTS; i++)
				{
					if (m_isImportantLight[ closestLightsIndices[ i ] ])
					{
						m_updateLightUniforms(m_lights[ closestLightsIndices[ i ] ], loadedLights);

						loadedLights++; // Updating Loaded Lights Amount
					}
				}

				// Closest Lights
				for (unsigned int i = 0; i < closestLightsIndices.size() && loadedLights < RENDERER3D_MAX_RENDERED_LIGHTS; i++)
				{
					if (!m_isImportantLight[ closestLightsIndices[ i ] ])
					{
						m_updateLightUniforms(m_lights[ closestLightsIndices[ i ] ], loadedLights);

						loadedLights++; // Updating Loaded Lights Amount
					}

				}

				Shader::stop();
			}
		}
	}

	void Renderer3D::setLightUpdateInterval(const float value)
	{
		m_lightUpdateInterval = value;
	}

	// Function Overrides
	unsigned int Renderer3D::addLight(Light3D* light)
	{
		if (m_lights.size() < RENDERER3D_MAX_RENDERED_LIGHTS) // Checking if Lights won't Get Sorted Anyway
		{
			// Updating Uniforms in Shaders
			m_updateLightUniforms(light, m_lights.size());
		}

		// Doing Usual Stuff
		m_lights.push_back(light);
		m_isImportantLight.push_back(false);


		return m_lights.size() - 1;
	}

	unsigned int Renderer3D::addLight(Light3D* light, const bool isImportant)
	{
		if (m_lights.size() < RENDERER3D_MAX_RENDERED_LIGHTS) // Checking if Lights won't Get Sorted Anyway
		{
			// Updating Uniforms in Shaders
			m_updateLightUniforms(light, m_lights.size());
		}

		// Doing Usual Stuff
		m_lights.push_back(light);
		m_isImportantLight.push_back(isImportant);


		return m_lights.size() - 1;
	}

	// Setters
	void Renderer3D::setZNear(const float value)
	{
		m_zNear = value;

		m_aboveWaterShader.start();
		m_aboveWaterShader.setUniform2f("u_zNearFarData", m_zNear, m_zFar);
		Shader::stop();
	}

	void Renderer3D::setZFar(const float value)
	{
		m_zFar = value;

		m_aboveWaterShader.start();
		m_aboveWaterShader.setUniform2f("u_zNearFarData", m_zNear, m_zFar);
		Shader::stop();
	}

	void Renderer3D::setZLimits(const float zNear, const float zFar)
	{
		m_zNear = zNear;
		m_zFar = zFar;

		m_aboveWaterShader.start();
		m_aboveWaterShader.setUniform2f("u_zNearFarData", m_zNear, m_zFar);
		Shader::stop();
	}


	void Renderer3D::setFogDensity(const float value)
	{
		m_basicLightShader.start();
		m_basicLightShader.setUniform1f("u_fogDensity", value);
		Shader::stop();
	}

	void Renderer3D::setFogGradient(const float value)
	{
		m_basicLightShader.start();
		m_basicLightShader.setUniform1f("u_fogGradient", value);
		Shader::stop();
	}

	void Renderer3D::setSkyColor(const Color& color)
	{
		m_basicLightShader.start();
		m_basicLightShader.setUniform3f("u_skyColor", color.r, color.g, color.b);
		Shader::stop();

		m_renderWindow->setClearColor(color);
	}


	void Renderer3D::setReflectLightOnWater(const bool value)
	{
		m_aboveWaterShader.start();
		m_aboveWaterShader.setUniform1i("u_reflectLightVertex", value);
		m_aboveWaterShader.setUniform1i("u_reflectLightFragment", value);
		Shader::stop();
	}

	void Renderer3D::setReflectSceneOnWater(const bool value)
	{
		m_aboveWaterShader.start();
		m_aboveWaterShader.setUniform1i("u_reflectSceneVertex", value);
		m_aboveWaterShader.setUniform1i("u_reflectSceneFragment", value);
		Shader::stop();
	}

	void Renderer3D::setWaterReflectionFboSize(const unsigned int width, const unsigned int height)
	{
		m_waterReflectionFboWidth = width;
		m_waterReflectionFboHeight = height;

		m_waterReflectionFBO.attachTexture(m_waterReflectionFboWidth, m_waterReflectionFboHeight);
		m_waterReflectionFBO.attachDepthBuffer(m_waterReflectionFboWidth, m_waterReflectionFboHeight);
	}

	void Renderer3D::setWaterRefractionFboSize(const unsigned int width, const unsigned int height)
	{
		m_waterRefractionFboWidth = width;
		m_waterRefractionFboHeight = height;

		m_waterRefractionFBO.attachTexture(m_waterRefractionFboWidth, m_waterRefractionFboHeight);
		m_waterRefractionFBO.attachDepthTexture(m_waterRefractionFboWidth, m_waterRefractionFboHeight);
	}

	// Private Functions
	void Renderer3D::m_initParticleVAO()
	{
		m_particleVAO = new VertexArray();
		VertexBuffer* instancePositionsVBO = new VertexBuffer(s_particleData, 8, 2);
		m_instanceVBO = new VertexBuffer(nullptr, RENDERER3D_MAX_PARTICLE_INSTANCES * RENDERER3D_PARTICLE_INSTANCE_DATA_LENGTH, RENDERER3D_PARTICLE_INSTANCE_DATA_LENGTH, VERTEX_BUFFER_STREAM_DRAW);

		m_particleVAO->addAttribute(instancePositionsVBO, 0);
		m_particleVAO->addInstancedAttribute(m_instanceVBO, 1, 4, RENDERER3D_PARTICLE_INSTANCE_DATA_LENGTH, 0);
		m_particleVAO->addInstancedAttribute(m_instanceVBO, 2, 4, RENDERER3D_PARTICLE_INSTANCE_DATA_LENGTH, 4);
		m_particleVAO->addInstancedAttribute(m_instanceVBO, 3, 4, RENDERER3D_PARTICLE_INSTANCE_DATA_LENGTH, 8);
		m_particleVAO->addInstancedAttribute(m_instanceVBO, 4, 4, RENDERER3D_PARTICLE_INSTANCE_DATA_LENGTH, 12);
		m_particleVAO->addInstancedAttribute(m_instanceVBO, 5, 2, RENDERER3D_PARTICLE_INSTANCE_DATA_LENGTH, 16);
	}

	void Renderer3D::m_initWaterStuff()
	{
		// Water VAO Setup
		m_waterVAO = new VertexArray();

		// Creating VBO
		VertexBuffer* waterVBO = new VertexBuffer(s_waterData, sizeof(s_waterData), 3);

		// Attaching VBO to VAO
		m_waterVAO->addAttribute(waterVBO, 0);

		// Water FBO Setup
		m_waterReflectionFBO.attachTexture(m_waterReflectionFboWidth, m_waterReflectionFboHeight);
		m_waterReflectionFBO.attachDepthBuffer(m_waterReflectionFboWidth, m_waterReflectionFboHeight);

		m_waterRefractionFBO.attachTexture(m_waterRefractionFboWidth, m_waterRefractionFboHeight);
		m_waterRefractionFBO.attachDepthTexture(m_waterRefractionFboWidth, m_waterRefractionFboHeight);
	}


	void Renderer3D::m_renderRenderables(const matrix4& projection, const matrix4& view)
	{
		// OpenGL 3D Stuff Setup
		GLcall( glEnable(GL_DEPTH_TEST) );
		GLcall( glDepthFunc(GL_LESS) );

		// Main Shader Setup
		m_basicLightShader.start();
		m_basicLightShader.setUniformMatrix4f("u_projectionMatrix", projection);
		m_basicLightShader.setUniformMatrix4f("u_viewMatrix", view);
		
		
		// Rendering Renderables
		for (std::vector<Renderable3D*> modelVector : m_renderables)
		{
			// Model Setup
			modelVector[0]->getModel()->bind(); //binding model's vao and ibo

			m_basicLightShader.setUniform1f("u_shineDamper", modelVector[0]->getModel()->getShineDamper());
			m_basicLightShader.setUniform1f("u_reflectivity", modelVector[0]->getModel()->getReflectivity());

			m_basicLightShader.setUniform1f("u_hasFakeLighting", modelVector[0]->getModel()->hasFakeLighting());

			m_basicLightShader.setUniform1f("u_textureRows", (float)modelVector[0]->getModel()->getTextureRows());

			if (modelVector[0]->getModel()->hasTransparency())
			{
				GLcall( glDisable(GL_CULL_FACE) );
			}
			else
			{
				GLcall( glEnable(GL_CULL_FACE) );
				GLcall( glCullFace(GL_BACK) );
			}
			
			// Rendering Renderables Separately
			for (Renderable3D* renderable : modelVector)
			{
				matrix4 modelMatrix = matrix4::model(renderable->getPosition(), renderable->getRotation().x, renderable->getRotation().y, renderable->getRotation().z, renderable->getScale());
				m_basicLightShader.setUniformMatrix4f("u_modelMatrix", modelMatrix);

				m_basicLightShader.setUniform2f("u_textureCoordinatesOffset", renderable->getTextureOffset());

				// Rendering
				GLcall( glDrawElements(GL_TRIANGLES, renderable->getModel()->getVertexCount(), GL_UNSIGNED_INT, nullptr) );
			}
		}
		

		// Finishing
		GLcall( glDisable(GL_DEPTH_TEST) );
	}

	void Renderer3D::m_renderSkybox(const Camera3D& cam, const matrix4& projection)
	{
		if (m_skybox != nullptr) // Checking if There's Skybox to Render
		{
			// OpenGL Stuff Setup
			GLcall( glEnable(GL_DEPTH_TEST) );
			GLcall( glDepthFunc(GL_LEQUAL) );

			// Shader Setup
			matrix4 viewMatrix = matrix4::fpsView(vector3(0.0f), cam.getPitch(), cam.getYaw());

			m_skyboxShader.start();
			m_skyboxShader.setUniformMatrix4f("u_projectionMatrix", projection);
			m_skyboxShader.setUniformMatrix4f("u_viewMatrix", viewMatrix);

			// Binding
			m_skybox->bind();

			// Rendering
			GLcall( glDrawArrays(GL_TRIANGLES, 0, 36) );

			// Finishing
			GLcall( glDisable(GL_DEPTH_TEST) );
		}
	}

	void Renderer3D::m_renderParticles(const matrix4& projection, const matrix4& view)
	{
		// Checking if There Are Any Particles
		if (m_particleGroups.size() > 0)
		{
			// OpenGL Stuff Setup
			GLcall( glEnable(GL_BLEND) );

			GLcall( glEnable(GL_DEPTH_TEST) );
			GLcall( glDepthMask(false) );

			// Shader Setup
			m_particleShader.start();
			m_particleShader.setUniformMatrix4f("u_projectionMatrix", projection);


			// Going through Particle Groups One by One
			for (ParticleGroup& pg : m_particleGroups)
			{
				// Init
				unsigned int offsetPointer = 0;
				float* vboData = new float[pg.getParticles().size() * RENDERER3D_PARTICLE_INSTANCE_DATA_LENGTH];

				// Blending Setup
				if (pg.usingAdditiveBlending())
				{
					GLcall(glBlendFunc(GL_SRC_ALPHA, GL_ONE)); // Additive Blending
				}
				else
				{
					GLcall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); // Alpha Blending
				}

				// Binding
				m_particleVAO->bind();

				pg.bind();

				m_particleShader.start();

				// Shader Setup
				m_particleShader.setUniform1f("u_textureAtlasRows", (float)pg.getTextureAtlasRows());

				// Creating Data for Instance VBO
				for (const Particle& particle : pg.getParticles())
				{
					// Matrices Setup
					matrix4 modelMatrix = matrix4::translation(particle.getPosition());
					modelMatrix.elements[0 + 0 * 4] = view.elements[0 + 0 * 4];
					modelMatrix.elements[0 + 1 * 4] = view.elements[1 + 0 * 4];
					modelMatrix.elements[0 + 2 * 4] = view.elements[2 + 0 * 4];
					modelMatrix.elements[1 + 0 * 4] = view.elements[0 + 1 * 4];
					modelMatrix.elements[1 + 1 * 4] = view.elements[1 + 1 * 4];
					modelMatrix.elements[1 + 2 * 4] = view.elements[2 + 1 * 4];
					modelMatrix.elements[2 + 0 * 4] = view.elements[0 + 2 * 4];
					modelMatrix.elements[2 + 1 * 4] = view.elements[1 + 2 * 4];
					modelMatrix.elements[2 + 2 * 4] = view.elements[2 + 2 * 4];
					modelMatrix *= matrix4::rotation(particle.getRotation(), vector3(0.0f, 0.0f, 1.0f));
					modelMatrix *= matrix4::scale(particle.getScale());
					matrix4 modelViewMatrix = view * modelMatrix;

					// Loading Data to VBO
					vboData[offsetPointer++] = modelViewMatrix.elements[0 + 0 * 4];	// 0
					vboData[offsetPointer++] = modelViewMatrix.elements[1 + 0 * 4]; // 1
					vboData[offsetPointer++] = modelViewMatrix.elements[2 + 0 * 4];	// 2
					vboData[offsetPointer++] = modelViewMatrix.elements[3 + 0 * 4];	// 3
					vboData[offsetPointer++] = modelViewMatrix.elements[0 + 1 * 4];	// 4
					vboData[offsetPointer++] = modelViewMatrix.elements[1 + 1 * 4];	// 5
					vboData[offsetPointer++] = modelViewMatrix.elements[2 + 1 * 4];	// 6
					vboData[offsetPointer++] = modelViewMatrix.elements[3 + 1 * 4];	// 7
					vboData[offsetPointer++] = modelViewMatrix.elements[0 + 2 * 4];	// 8
					vboData[offsetPointer++] = modelViewMatrix.elements[1 + 2 * 4];	// 9
					vboData[offsetPointer++] = modelViewMatrix.elements[2 + 2 * 4];	// 10
					vboData[offsetPointer++] = modelViewMatrix.elements[3 + 2 * 4];	// 11
					vboData[offsetPointer++] = modelViewMatrix.elements[0 + 3 * 4];	// 12
					vboData[offsetPointer++] = modelViewMatrix.elements[1 + 3 * 4];	// 13
					vboData[offsetPointer++] = modelViewMatrix.elements[2 + 3 * 4];	// 14
					vboData[offsetPointer++] = modelViewMatrix.elements[3 + 3 * 4];	// 15

					vboData[offsetPointer++] = pg.getTextureOffsets()[particle.getTextureAtlasIndex()].x; // 16
					vboData[offsetPointer++] = pg.getTextureOffsets()[particle.getTextureAtlasIndex()].y; // 17
				}

				// Updating Data in VBO
				m_instanceVBO->bind();
				m_instanceVBO->bufferData(nullptr, RENDERER3D_MAX_PARTICLE_INSTANCES * RENDERER3D_PARTICLE_INSTANCE_DATA_LENGTH, VERTEX_BUFFER_STREAM_DRAW);
				m_instanceVBO->bufferSubData(vboData, offsetPointer);

				// Rendering
				GLcall(glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, pg.getParticles().size()));

				// Deleting VBO Data
				delete[] vboData;
			}
		}
		
		// Finishing
		GLcall( glDisable(GL_BLEND) );

		GLcall( glDisable(GL_DEPTH_TEST) );
		GLcall( glDepthMask(true) );

		ParticleGroup::unbind();
	}

	void Renderer3D::m_renderWater(const Camera3D& cam, const matrix4& projection, const matrix4& view)
	{
		if (m_waterTiles.size() > 0)
		{
			// OpenGL Setup Stuff
			GLcall( glEnable(GL_CLIP_DISTANCE0) );

			// Main Shader Setup
			m_aboveWaterShader.start();
			m_aboveWaterShader.setUniformMatrix4f("u_projectionMatrix", projection);
			m_aboveWaterShader.setUniformMatrix4f("u_viewMatrix", view);

			m_aboveWaterShader.setUniform3f("u_cameraPosition", cam.getPosition());

			m_belowWaterShader.start();
			m_belowWaterShader.setUniformMatrix4f("u_projectionMatrix", projection);
			m_belowWaterShader.setUniformMatrix4f("u_viewMatrix", view);

			m_belowWaterShader.setUniform3f("u_cameraPosition", cam.getPosition());

			// Texture Setup
			m_waterDuDvMap.bind(2);
			m_waterNormalMap.bind(3);

			// Getting Default Window Size
			int defaultViewport[4];
			GLcall( glGetIntegerv(GL_VIEWPORT, defaultViewport) );

			// Rendering Renderables
			for (WaterTile* tile : m_waterTiles)
			{
				if (cam.getPosition().y >= tile->getPosition().y) // Camera is Above Water
				{
					// Reflection FBO
					// Camera Setup
					Camera3D reflectionCamera(cam);
					float heightAboveWater = reflectionCamera.getPosition().y;
					reflectionCamera.move(vector3(0.0f, -heightAboveWater * 2, 0.0f));
					reflectionCamera.invertPitch();

					// Matrices Calculations
					matrix4 reflectionProjectionMatrix = matrix4::projection(reflectionCamera.getAspectRatio(), reflectionCamera.getFOV(), m_zNear, m_zFar);
					matrix4 reflectionViewMatrix = matrix4::fpsView(reflectionCamera.getPosition(), reflectionCamera.getPitch(), reflectionCamera.getYaw());

					// Other Shaders Setup
					m_setClippingPlaneUniforms(vector4(0.0f, 1.0f, 0.0f, -tile->getPosition().y));

					// Binding FBO
					m_waterReflectionFBO.bind(m_waterReflectionFboWidth, m_waterReflectionFboHeight);

					// Clearing Buffers
					GLcall( glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) );

					// Rendering
					m_renderRenderables(reflectionProjectionMatrix, reflectionViewMatrix);
					m_renderSkybox(reflectionCamera, reflectionProjectionMatrix);
					m_renderParticles(reflectionProjectionMatrix, reflectionViewMatrix);


					// Refraction FBO
					// Other Shaders Setup
					m_setClippingPlaneUniforms(vector4(0.0f, -1.0f, 0.0f, tile->getPosition().y + 1.0f));

					// Binding FBO
					m_waterRefractionFBO.bind(m_waterRefractionFboWidth, m_waterRefractionFboHeight);

					// Clearing Buffers
					GLcall( glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) );

					// Rendering
					m_renderRenderables(projection, view);
					m_renderSkybox(cam, projection);
					m_renderParticles(projection, view);


					// FBO Stuff Setup
					m_waterReflectionFBO.bindColorTexture(0);
					m_waterRefractionFBO.bindColorTexture(1);
					m_waterRefractionFBO.bindDepthTexture(4);

					FrameBuffer::unbind(defaultViewport[2], defaultViewport[3]);

					// OpenGL Setup Stuff
					GLcall( glEnable(GL_DEPTH_TEST) );
					GLcall( glDepthFunc(GL_LESS) );

					GLcall( glDisable(GL_CULL_FACE) );

					GLcall( glEnable(GL_BLEND) );
					GLcall( glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) );

					// Binding
					m_waterVAO->bind();

					// Shader Setup
					m_aboveWaterShader.start();

					matrix4 modelMatrix = matrix4::model(tile->getPosition(), 0.0f, tile->getRotation(), 0.0f, tile->getScale());
					m_aboveWaterShader.setUniformMatrix4f("u_modelMatrix", modelMatrix);

					m_aboveWaterShader.setUniform1f("u_movementFactor", tile->getMovementFactor());
					m_aboveWaterShader.setUniform1f("u_tilingFactor", tile->getTilingFactor());
					m_aboveWaterShader.setUniform1f("u_waveStrength", tile->getWaveStrength());
					m_aboveWaterShader.setUniform1f("u_lightShineDamper", tile->getLightShineDamper());
					m_aboveWaterShader.setUniform1f("u_lightReflectivity", tile->getLightReflectivity());
				}
				else // Camera is Below Water
				{
					// Refraction FBO
					// Other Shaders Setup
					m_setClippingPlaneUniforms(vector4(0.0f, 1.0f, 0.0f, -tile->getPosition().y - 1.0f));

					// Binding FBO
					m_waterRefractionFBO.bind(m_waterRefractionFboWidth, m_waterRefractionFboHeight);

					// Clearing Buffers
					GLcall( glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) );

					// Rendering
					m_renderRenderables(projection, view);
					m_renderSkybox(cam, projection);
					m_renderParticles(projection, view);


					// FBO Stuff Setup
					m_waterRefractionFBO.bindColorTexture(1);
					m_waterRefractionFBO.bindDepthTexture(4);

					FrameBuffer::unbind(defaultViewport[2], defaultViewport[3]);

					// OpenGL Setup Stuff
					GLcall( glEnable(GL_DEPTH_TEST) );
					GLcall( glDepthFunc(GL_LESS) );

					GLcall( glDisable(GL_CULL_FACE) );

					GLcall( glEnable(GL_BLEND) );
					GLcall( glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) );

					// Binding
					m_waterVAO->bind();

					// Shader Setup
					m_belowWaterShader.start();

					matrix4 modelMatrix = matrix4::model(tile->getPosition(), 0.0f, tile->getRotation(), 0.0f, tile->getScale());
					m_belowWaterShader.setUniformMatrix4f("u_modelMatrix", modelMatrix);

					m_belowWaterShader.setUniform1f("u_movementFactor", tile->getMovementFactor());
					m_belowWaterShader.setUniform1f("u_tilingFactor", tile->getTilingFactor());
					m_belowWaterShader.setUniform1f("u_waveStrength", tile->getWaveStrength());
				}

				// Rendering
				GLcall( glDrawArrays(GL_TRIANGLE_STRIP, 0, 4) );
				
			}

			// Removing Clipping Planes from Shaders
			m_setClippingPlaneUniforms(vector4(0.0f, -1.0f, 0.0f, 10000000000000.0f));

			// Finishing
			GLcall( glDisable(GL_DEPTH_TEST) );

			GLcall( glEnable(GL_CULL_FACE) );
			GLcall( glCullFace(GL_BACK) );

			GLcall( glDisable(GL_CLIP_DISTANCE0) );

			GLcall( glDisable(GL_BLEND) );
		}
	}


	void Renderer3D::m_setClippingPlaneUniforms(const vector4& plane)
	{
		m_basicLightShader.start();
		m_basicLightShader.setUniform4f("u_clippingPlane", plane);

		m_skyboxShader.start();
		m_skyboxShader.setUniform4f("u_clippingPlane", plane);

		m_particleShader.start();
		m_particleShader.setUniform4f("u_clippingPlane", plane);
	}

	void Renderer3D::m_updateLightUniforms(Light3D* light, const unsigned int index)
	{
		// Preparing String
		std::stringstream ss;
		ss << index;
		std::string indexString = ss.str();

		// Uniforms Setting
		m_basicLightShader.start();
		m_basicLightShader.setUniform3f("u_lightPosition[" + indexString + "]", light->getPosition());
		m_basicLightShader.setUniform3f("u_lightColor[" + indexString + "]", light->getColor());
		m_basicLightShader.setUniform3f("u_lightAttenuation[" + indexString + "]", light->getAttenuation());

		m_aboveWaterShader.start();
		m_aboveWaterShader.setUniform3f("u_lightPosition[" + indexString + "]", light->getPosition());
		m_aboveWaterShader.setUniform3f("u_lightColor[" + indexString + "]", light->getColor());
		m_aboveWaterShader.setUniform3f("u_lightAttenuation[" + indexString + "]", light->getAttenuation());

		Shader::stop();
	}

}