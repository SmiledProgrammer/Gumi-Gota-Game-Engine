#ifndef GUMIGOTA_LAYER2D_H
#define GUMIGOTA_LAYER2D_H



/* INCLUDES */
// Normal Includes
#include <vector>

// Header Includes
#include "Sprite2D.h"
#include "Camera2D.h"

#include "../Shader.h"

#include "../buffers/VertexArray.h"
#include "../buffers/Indexbuffer.h"

#include "../../maths/matrices/matrix4.h"

#include "GraphicsScene2D.h"

#include "../Window.h"



namespace gg
{

	class Layer2D : public GraphicsScene2D
	{
	protected:
		/* Variables */
		VertexArray* m_colorVAO;
		VertexArray* m_textureVAO;
		matrix4 m_projectionMatrix;

		Shader m_colorShader;
		Shader m_textureShader;
		Shader m_colorLightShader;
		Shader m_textureLightShader;

		float m_lightUpdateInterval;

	public:
		/* Constructors */
		Layer2D(const Window& window);

		/* Functions */
		void render();
		void render(const bool renderLighting);
		void render(const Camera2D& cam);
		void render(const Camera2D& cam, const bool renderLighting);

		void updateLights(const Camera2D& cam);
		void setLightUpdateInterval(const float value);

		// Function Overrides
		void addLight(Light2D* light) override;
		void addLight(Light2D* light, const bool isImportant) override; //if isImportant is true, light will be forced to be rendered, even when other lights are closer

	private:
		// Private Functions
		void m_render(const bool renderLighting, const vector2& pos);
		void m_renderSprites(const bool renderLighting);
		void m_renderLabels(const bool renderLighting);

		void m_updateLightUniforms(Light2D* light, const unsigned int index);

	};

}



#endif