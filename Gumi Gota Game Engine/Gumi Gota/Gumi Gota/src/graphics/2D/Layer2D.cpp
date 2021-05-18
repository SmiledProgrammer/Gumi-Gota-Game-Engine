
/* INCLUDES */
// Self Include
#include "Layer2D.h"

// OpenGL Includes
#include <GL/glew.h>

// Normal Includes
#include <sstream>

// Header Includes
#include "../buffers/VertexBuffer.h"

#include "../ErrorHandling.h"

#include <iostream> ///remove l8r



/* MACROS */
#define RENDERER2D_DEFAULT_LIGHT_UPDATE_INTERVAL 0.1f
#define RENDERER2D_MAX_RENDERED_LIGHTS 512



namespace gg
{

	/* ADDITIONAL STUFF */
	static float s_vboData[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
	};



	/* Constructors */
	Layer2D::Layer2D(const Window& window)
		: m_colorShader("Gumi Gota/Gumi Gota/res/shaders/Color2D.shader"),
		  m_textureShader("Gumi Gota/Gumi Gota/res/shaders/Texture2D.shader"),
		  m_colorLightShader("Gumi Gota/Gumi Gota/res/shaders/ColorLight2D.shader"),
		  m_textureLightShader("Gumi Gota/Gumi Gota/res/shaders/TextureLight2D.shader"),
		  m_projectionMatrix(matrix4::orthographic(0.0f, (float)window.getWidth(), 0.0f, (float)window.getHeight(), -1.0f, 1.0f)),
		  m_lightUpdateInterval(RENDERER2D_DEFAULT_LIGHT_UPDATE_INTERVAL)
	{
		// Shader Setup
		m_textureShader.start();
		m_textureShader.setUniform1i("u_textureSampler", 0);
		
		m_textureLightShader.start();
		m_textureLightShader.setUniform1i("u_textureSampler", 0);
		Shader::stop();

		// Color VAO Setup
		m_colorVAO = new VertexArray();
		VertexBuffer* dataVBO = new VertexBuffer(s_vboData, 8, 2);
		m_colorVAO->addAttribute(dataVBO, 0);

		// Texture VAO Setup
		m_textureVAO = new VertexArray();
		VertexBuffer* positionsVBO = new VertexBuffer(s_vboData, 8, 2);
		VertexBuffer* textureCoordinatesVBO = new VertexBuffer(s_vboData, 8, 2);
		m_textureVAO->addAttribute(positionsVBO, 0);
		m_textureVAO->addAttribute(textureCoordinatesVBO, 1);

		VertexArray::unbind();
	}

	/* Functions */
	void Layer2D::render()
	{
		m_render(false, vector2());
	}

	void Layer2D::render(const bool renderLighting)
	{
		m_render(renderLighting, vector2());
	}

	void Layer2D::render(const Camera2D& cam)
	{
		m_render(false, cam.getPosition());
	}

	void Layer2D::render(const Camera2D& cam, const bool renderLighting)
	{
		m_render(renderLighting, cam.getPosition());
	}


	void Layer2D::updateLights(const Camera2D& cam)
	{
		// Checking if There is Enough Lights to Sort Them
		if (m_lights.size() >= RENDERER2D_MAX_RENDERED_LIGHTS)
		{
			// Checking If Lights Should Get Updated
			if (m_lightTimer.elapsed() >= m_lightUpdateInterval && m_lightUpdateInterval != 0.0f)
			{
				m_lightTimer.reset(); // Resetting Timer

									  // Init
				unsigned int loadedLights = 0;

				// Getting Closest Lights Indices
				std::vector<unsigned int> closestLightsIndices = getClosestLightsIndices(m_lights, cam, RENDERER2D_MAX_RENDERED_LIGHTS);

				// Important Lights
				for (unsigned int i = 0; i < m_isImportantLight.size() && loadedLights < RENDERER2D_MAX_RENDERED_LIGHTS; i++)
				{
					if (m_isImportantLight[i])
					{
						// Getting Light Pointer
						Light2D* light = m_lights[i];

						// Preparing String
						std::stringstream ss;
						ss << loadedLights;

						// Uniforms Setting
						m_colorLightShader.start();
						m_colorLightShader.setUniform2f("u_lightPosition[" + ss.str() + "]", light->getPosition());
						m_colorLightShader.setUniform3f("u_lightColor[" + ss.str() + "]", light->getColor());
						m_colorLightShader.setUniform3f("u_lightAttenuation[" + ss.str() + "]", light->getAttenuation());

						m_textureLightShader.start();
						m_textureLightShader.setUniform2f("u_lightPosition[" + ss.str() + "]", light->getPosition());
						m_textureLightShader.setUniform3f("u_lightColor[" + ss.str() + "]", light->getColor());
						m_textureLightShader.setUniform3f("u_lightAttenuation[" + ss.str() + "]", light->getAttenuation());

						loadedLights++; // Updating Loaded Lights Amount
					}
				}

				// Closest Lights
				for (unsigned int i = 0; i < closestLightsIndices.size() && loadedLights < RENDERER2D_MAX_RENDERED_LIGHTS; i++)
				{
					if (!m_isImportantLight[closestLightsIndices[i]])
					{
						// Getting Light Pointer
						Light2D* light = m_lights[closestLightsIndices[i]];

						// Preparing String
						std::stringstream ss;
						ss << loadedLights;

						// Uniforms Setting
						m_colorLightShader.start();
						m_colorLightShader.setUniform2f("u_lightPosition[" + ss.str() + "]", light->getPosition());
						m_colorLightShader.setUniform3f("u_lightColor[" + ss.str() + "]", light->getColor());
						m_colorLightShader.setUniform3f("u_lightAttenuation[" + ss.str() + "]", light->getAttenuation());

						m_textureLightShader.start();
						m_textureLightShader.setUniform2f("u_lightPosition[" + ss.str() + "]", light->getPosition());
						m_textureLightShader.setUniform3f("u_lightColor[" + ss.str() + "]", light->getColor());
						m_textureLightShader.setUniform3f("u_lightAttenuation[" + ss.str() + "]", light->getAttenuation());

						loadedLights++; // Updating Loaded Lights Amount
					}

				}

				Shader::stop();
			}
		}
	}

	void Layer2D::setLightUpdateInterval(const float value)
	{
		m_lightUpdateInterval = value;
	}

	// Function Overrides
	void Layer2D::addLight(Light2D* light)
	{
		if (m_lights.size() < RENDERER2D_MAX_RENDERED_LIGHTS) // Checking if Lights won't Get Sorted Anyway
		{
			// Updating Uniforms in Shaders
			m_updateLightUniforms(light, m_lights.size());
		}

		// Doing Usual Stuff
		m_lights.push_back(light);
		m_isImportantLight.push_back(false);
	}

	void Layer2D::addLight(Light2D* light, const bool isImportant)
	{
		if (m_lights.size() < RENDERER2D_MAX_RENDERED_LIGHTS) // Checking if Lights won't Get Sorted Anyway
		{
			// Updating Uniforms in Shaders
			m_updateLightUniforms(light, m_lights.size());
		}

		// Doing Usual Stuff
		m_lights.push_back(light);
		m_isImportantLight.push_back(isImportant);
	}

	// Private Functions
	void Layer2D::m_render(const bool renderLighting, const vector2& pos)
	{
		// Matrix Calculations
		matrix4 viewMatrix = matrix4::fpsView(vector3(pos, -0.5f), 0.0f, 0.0f);

		// Shader Setup
		m_colorLightShader.start();
		m_colorLightShader.setUniformMatrix4f("u_projectionMatrix", m_projectionMatrix);
		m_colorLightShader.setUniformMatrix4f("u_viewMatrix", viewMatrix);

		m_colorShader.start();
		m_colorShader.setUniformMatrix4f("u_projectionMatrix", m_projectionMatrix);
		m_colorShader.setUniformMatrix4f("u_viewMatrix", viewMatrix);

		m_textureLightShader.start();
		m_textureLightShader.setUniformMatrix4f("u_projectionMatrix", m_projectionMatrix);
		m_textureLightShader.setUniformMatrix4f("u_viewMatrix", viewMatrix);

		m_textureShader.start();
		m_textureShader.setUniformMatrix4f("u_projectionMatrix", m_projectionMatrix);
		m_textureShader.setUniformMatrix4f("u_viewMatrix", viewMatrix);

		// Blending Setup
		GLcall( glEnable(GL_BLEND) );
		GLcall( glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) );

		// Rendering
		m_renderSprites(renderLighting);
		m_renderLabels(renderLighting);

		// Finishing
		GLcall( glDisable(GL_BLEND) );

		VertexArray::unbind();

		Shader::stop();
	}

	void Layer2D::m_renderSprites(const bool renderLighting)
	{
		// Rendering Renderables from std::vector
		for (Sprite2D* sprite : m_sprites)
		{
			// Shader Setup
			if (sprite->getTexture() == nullptr)
			{
				// Binding
				m_colorVAO->bind();

				// Texture Setup
				Texture::unbind();

				// Uniforms Setup
				if (renderLighting)
				{
					// Uniforms Setup
					m_colorLightShader.start();

					matrix4 model = matrix4::translation(vector3(sprite->getOrigin(), 0.0f))
						* matrix4::rotation(sprite->getRotation(), vector3(0.0f, 0.0f, 1.0f))
						* matrix4::translation(vector3(sprite->getPosition() - sprite->getOrigin(), 0.0f))
						* matrix4::scale(vector3(sprite->getSize().x, sprite->getSize().y, 1.0f));

					m_colorLightShader.setUniformMatrix4f("u_modelMatrix", model);


					const Color& renderableColor = sprite->getColor();
					m_colorLightShader.setUniform4f("u_color", renderableColor.r, renderableColor.g, renderableColor.b, renderableColor.a);
				}
				else
				{
					// Uniforms Setup
					m_colorShader.start();

					matrix4 model = matrix4::translation(vector3(sprite->getOrigin(), 0.0f))
						* matrix4::rotation(sprite->getRotation(), vector3(0.0f, 0.0f, 1.0f))
						* matrix4::translation(vector3(sprite->getPosition() - sprite->getOrigin(), 0.0f))
						* matrix4::scale(vector3(sprite->getSize().x, sprite->getSize().y, 1.0f));

					m_colorShader.setUniformMatrix4f("u_modelMatrix", model);


					const Color& renderableColor = sprite->getColor();
					m_colorShader.setUniform4f("u_color", renderableColor.r, renderableColor.g, renderableColor.b, renderableColor.a);
				}
			}
			else
			{
				// Binding
				m_textureVAO->bind();

				// Texture Setup
				sprite->getTexture()->bind();

				// Uniforms Setup
				if (renderLighting)
				{
					// Uniforms Setup
					m_textureLightShader.start();

					matrix4 model = matrix4::translation(vector3(sprite->getOrigin(), 0.0f))
						* matrix4::rotation(sprite->getRotation(), vector3(0.0f, 0.0f, 1.0f))
						* matrix4::translation(vector3(sprite->getPosition() - sprite->getOrigin(), 0.0f))
						* matrix4::scale(vector3(sprite->getSize().x, sprite->getSize().y, 1.0f));

					m_textureLightShader.setUniformMatrix4f("u_modelMatrix", model);

					std::cout << model.elements[0] << " - " << model.elements[4] << " - " << model.elements[8] << " - " << model.elements[12] << " - " << std::endl;
					std::cout << model.elements[1] << " - " << model.elements[5] << " - " << model.elements[9] << " - " << model.elements[13] << " - " << std::endl;
					std::cout << model.elements[2] << " - " << model.elements[6] << " - " << model.elements[10] << " - " << model.elements[14] << " - " << std::endl;
					std::cout << model.elements[3] << " - " << model.elements[7] << " - " << model.elements[11] << " - " << model.elements[15] << " - " << std::endl;
				}
				else
				{
					// Uniforms Setup
					m_textureShader.start();

					matrix4 model = matrix4::translation(vector3(sprite->getOrigin(), 0.0f))
						* matrix4::rotation(sprite->getRotation(), vector3(0.0f, 0.0f, 1.0f))
						* matrix4::translation(vector3(sprite->getPosition() - sprite->getOrigin(), 0.0f))
						* matrix4::scale(vector3(sprite->getSize().x, sprite->getSize().y, 1.0f));

					m_textureShader.setUniformMatrix4f("u_modelMatrix", model);
				}
			}

			system("pause");
			std::cout << std::endl;

			// Rendering
			GLcall( glDrawArrays(GL_TRIANGLE_STRIP, 0, 8) ); /// check l8r if 8 shouldn be 4
		}
	}

	void Layer2D::m_renderLabels(const bool renderLighting)
	{
		// Uniforms Setup
		if (renderLighting)
		{
			// Uniforms Setup
			m_textureLightShader.start();

			m_textureLightShader.setUniformMatrix4f("u_modelMatrix", matrix4(1.0f));
		}
		else
		{
			// Uniforms Setup
			m_textureShader.start();

			m_textureShader.setUniformMatrix4f("u_modelMatrix", matrix4(1.0f));
		}

		// Rendering Labels from std::vector
		for (Label2D* label : m_labels)
		{
			// Binding
			m_textureVAO->bind();

			label->bindFontTexture();

			// Starting Shaders
			if (renderLighting)
				m_textureLightShader.start();
			else
				m_textureShader.start();

			// Preparation Stuff
			std::vector<float> positions;
			std::vector<float> textureCoordinates;

			///const vector2 screenSize(1280.0f, 720.0f);

			float offsetX = label->getPosition().x;

			// Loading Characters
			for (unsigned int i = 0; i < label->getText().length(); i++)
			{
				// Loading Glyph
				ftgl::texture_glyph_t* glyph = ftgl::texture_font_get_glyph(label->getFTGLFont(), &label->getText().at(i));

				if (glyph != NULL)
				{
					if (i > 0)
					{
						float kerning = texture_glyph_get_kerning(glyph, &label->getText().at(i - 1));
						offsetX += kerning;
					}

					// Glyph Position/Offset/Size Calculations
					float x0 = offsetX + glyph->offset_x;
					float y0 = label->getPosition().y + glyph->offset_y;
					float x1 = x0 + glyph->width;
					float y1 = y0 - glyph->height;
					float u0 = glyph->s0;
					float v0 = glyph->t0;
					float u1 = glyph->s1;
					float v1 = glyph->t1;

					positions.push_back(x0);
					positions.push_back(y0);
					positions.push_back(x0);
					positions.push_back(y1);
					positions.push_back(x1);
					positions.push_back(y0);
					positions.push_back(x1);
					positions.push_back(y1);
					

					textureCoordinates.push_back(u0);
					textureCoordinates.push_back(v0);
					textureCoordinates.push_back(u0);
					textureCoordinates.push_back(v1);
					textureCoordinates.push_back(u1);
					textureCoordinates.push_back(v0);
					textureCoordinates.push_back(u1);
					textureCoordinates.push_back(v1);
					

					offsetX += glyph->advance_x;

					///std::cout << "x0, y0: " << x0 << ", " << y0 << "; x1, y1: " << x0 + xSize << ", " << y0 + ySize << "; u0, v0: " << u0 << ", " << v0 << "; u1, v1: " << u0 + uSize << ", " << v0 + vSize << std::endl;
					///std::cout << "u0, v0: " << u0 << ", " << v0 << ";  u1, v1: " << glyph->s1 << ", " << glyph->t1 << ";  uSize, vSize: " << uSize << ", " << vSize << std::endl;
				}
			}

			///system("pause");
			///std::cout << std::endl;

			VertexArray textVAO;
			VertexBuffer* textPosVBO = new VertexBuffer(&positions[0], positions.size(), 2);
			VertexBuffer* textTexCoordsVBO = new VertexBuffer(&textureCoordinates[0], textureCoordinates.size(), 2);
			textVAO.addAttribute(textPosVBO, 0);
			textVAO.addAttribute(textTexCoordsVBO, 1);
			textVAO.bind();

			// Rendering
			GLcall( glDrawArrays(GL_TRIANGLE_STRIP, 0, 4 * label->getText().length()) );

		}
	}


	void Layer2D::m_updateLightUniforms(Light2D* light, const unsigned int index)
	{
		// Preparing String
		std::stringstream ss;
		ss << index;
		std::string indexString = ss.str();

		// Uniforms Setting
		m_colorLightShader.start();
		m_colorLightShader.setUniform2f("u_lightPosition[" + indexString + "]", light->getPosition());
		m_colorLightShader.setUniform3f("u_lightColor[" + indexString + "]", light->getColor());
		m_colorLightShader.setUniform3f("u_lightAttenuation[" + indexString + "]", light->getAttenuation());

		m_textureLightShader.start();
		m_textureLightShader.setUniform2f("u_lightPosition[" + indexString + "]", light->getPosition());
		m_textureLightShader.setUniform3f("u_lightColor[" + indexString + "]", light->getColor());
		m_textureLightShader.setUniform3f("u_lightAttenuation[" + indexString + "]", light->getAttenuation());

		Shader::stop();
	}

}