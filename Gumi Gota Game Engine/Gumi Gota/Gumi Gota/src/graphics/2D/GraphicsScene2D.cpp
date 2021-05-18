
/* INCLUDES */
// Self Include
#include "GraphicsScene2D.h"

// Header Includes
#include "../../utils/Sorting.h"



namespace gg
{

	/* Constructors */
	GraphicsScene2D::GraphicsScene2D()
	{}

	/* Functions */
	void GraphicsScene2D::addSprite(Sprite2D* sprite)
	{
		m_sprites.push_back(sprite);
	}

	void GraphicsScene2D::popBackSprites()
	{
		m_sprites.pop_back();
	}

	void GraphicsScene2D::reserveSprites(const unsigned int size)
	{
		m_sprites.reserve(size);
	}


	void GraphicsScene2D::addLabel(Label2D* label)
	{
		m_labels.push_back(label);
	}

	void GraphicsScene2D::popBackLabels()
	{
		m_labels.pop_back();
	}

	void GraphicsScene2D::reserveLabels(const unsigned int size)
	{
		m_labels.reserve(size);
	}


	void GraphicsScene2D::addLight(Light2D* light)
	{
		m_lights.push_back(light);
		m_isImportantLight.push_back(false);
	}

	void GraphicsScene2D::addLight(Light2D* light, const bool isImportant)
	{
		m_lights.push_back(light);
		m_isImportantLight.push_back(isImportant);
	}

	void GraphicsScene2D::popBackLights()
	{
		m_lights.pop_back();
	}

	void GraphicsScene2D::reserveLights(const unsigned int size)
	{
		m_lights.reserve(size);
	}



	/* ADDITIONAL FUNCTIONS */
	std::vector<unsigned int> getClosestLightsIndices(const std::vector<Light2D*>& lights, const Camera2D& cam, const unsigned int amount)
	{
		// Init
		std::vector<float> distances;
		distances.reserve(amount);

		vector2 transformedCamPos = cam.getPosition() + cam.getHalfOfDisplaySize();

		// Calculating Distances from Lights
		for (unsigned int i = 0; i < lights.size(); i++)
		{
			// Distance Calculating
			distances.push_back((transformedCamPos - lights[i]->getPosition()).length());
		}

		// Getting Indices of Sorted Values
		std::vector<unsigned int> indices = quickSortIndices(distances);


		return indices;
	}


}