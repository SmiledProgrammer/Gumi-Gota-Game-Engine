#ifndef GUMIGOTA_GRAPHICS_SCENE_2D_H
#define GUMIGOTA_GRAPHICS_SCENE_2D_H



/* INCLUDES */
// Normal Includes
#include <vector>

// Header Includes
#include "Sprite2D.h"
#include "Label2D.h"
#include "Light2D.h"
#include "Camera2D.h"

#include "../../utils/Timer.h"



namespace gg
{

	class GraphicsScene2D
	{
	protected:
		/* Variables */
		std::vector<Sprite2D*> m_sprites;

		std::vector<Label2D*> m_labels;

		std::vector<Light2D*> m_lights;
		std::vector<bool> m_isImportantLight; //used for lights that are forced to be rendered
		Timer m_lightTimer; //used for updating rendered lights (finding closest lights to the camera)
		float m_lightUpdateInterval; //used for storing time used for updating rendered lights (finding closest lights to the camera)

	public:
		/* Constructors */
		GraphicsScene2D();

		/* Functions */
		virtual void addSprite(Sprite2D* sprite);
		void popBackSprites();
		void reserveSprites(const unsigned int size);

		virtual void addLabel(Label2D* label);
		void popBackLabels();
		void reserveLabels(const unsigned int size);

		virtual void addLight(Light2D* light);
		virtual void addLight(Light2D* light, const bool isImportant); //if is isImportant is true light will be forced to be rendered
		void popBackLights();
		void reserveLights(const unsigned int size);

	};



	/* ADDITIONAL FUNCTIONS */
	std::vector<unsigned int> getClosestLightsIndices(const std::vector<Light2D*>& lights, const Camera2D& cam, const unsigned int amount);

}



#endif