#ifndef GUMIGOTA_RENDERER3D_H
#define GUMIGOTA_RENDERER3D_H



/* INCLUDES */
// Header Includes
#include "GraphicsScene3D.h"
#include "Model.h"
#include "Camera3D.h"

#include "../Shader.h"
#include "../Color.h"
#include "../Window.h"

#include "../../utils/Timer.h"

#include "../buffers/FrameBuffer.h"



namespace gg
{

	class Renderer3D : public GraphicsScene3D
	{
	protected:
		/* Variables */
		Window* m_renderWindow;

		Shader m_basicLightShader;
		Shader m_skyboxShader;
		Shader m_particleShader;
		Shader m_aboveWaterShader;
		Shader m_belowWaterShader;

		float m_zNear, m_zFar;
		
		Timer m_lightTimer; //used for updating rendered lights (finding closest lights to the camera)
		float m_lightUpdateInterval; //used for storing time used for updating rendered lights (finding closest lights to the camera)

		VertexArray* m_particleVAO;
		VertexBuffer* m_instanceVBO;

		VertexArray* m_waterVAO;
		FrameBuffer m_waterReflectionFBO;
		FrameBuffer m_waterRefractionFBO;
		Texture m_waterDuDvMap;
		Texture m_waterNormalMap;
		unsigned int m_waterReflectionFboWidth, m_waterReflectionFboHeight;
		unsigned int m_waterRefractionFboWidth, m_waterRefractionFboHeight;

		FrameBuffer m_multisampledFBO; //used for antiasliasing

	public:
		/* Constructors */
		Renderer3D(Window* window);

		/* Functions */
		void render(const Camera3D& cam);

		void updateLights(const vector3& pos);
		void setLightUpdateInterval(const float value);

		// Function Overrides
		unsigned int addLight(Light3D* light) override;
		unsigned int addLight(Light3D* light, const bool isImportant) override; //if isImportant is true, light will be forced to be rendered, even when other lights are closer

		// Setters
		void setZNear(const float value);
		void setZFar(const float value);
		void setZLimits(const float zNear, const float zFar);

		void setFogDensity(const float value);
		void setFogGradient(const float value);
		void setSkyColor(const Color& color);

		void setReflectLightOnWater(const bool value);
		void setReflectSceneOnWater(const bool value);
		void setWaterReflectionFboSize(const unsigned int width, const unsigned int height);
		void setWaterRefractionFboSize(const unsigned int width, const unsigned int height);

	private:
		// Private Functions
		void m_initParticleVAO();
		void m_initWaterStuff();

		void m_renderRenderables(const matrix4& projection, const matrix4& view);
		void m_renderSkybox(const Camera3D& cam, const matrix4& projection);
		void m_renderParticles(const matrix4& projection, const matrix4& view);
		void m_renderWater(const Camera3D& cam, const matrix4& projection, const matrix4& view);

		void m_setClippingPlaneUniforms(const vector4& plane);
		void m_updateLightUniforms(Light3D* light, const unsigned int index);

	};

}



#endif