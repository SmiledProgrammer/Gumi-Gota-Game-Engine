
/* INCLUDES */
// Self Include
#include "GraphicsScene3D.h"

// Header Includes
#include "../../utils/Sorting.h"

#include <iostream> /// remove l8r



namespace gg
{

	/* ADDITIONAL FUNCTIONS */
	std::vector<unsigned int> getClosestLightsIndices(const vector3& pos, const std::vector<Light3D*>& lights, const unsigned int amount)
	{
		// Init
		std::vector<float> distances;
		distances.reserve(amount);

		// Calculating Distances from Lights
		for (unsigned int i = 0; i < lights.size(); i++)
		{
			// Distance Calculating
			distances.push_back((pos - lights[ i ]->getPosition()).length());
		}

		if (lights.size() > 0)
		{
			// Getting Indices of Sorted Values
			std::vector<unsigned int> indices = quickSortIndices(distances);

			return indices;
		}


		std::vector<unsigned int> emptyVector;
		return emptyVector;
	}



	/* Constructors */
	GraphicsScene3D::GraphicsScene3D()
	{}

	/* Functions */
	void GraphicsScene3D::update(const Camera3D& cam, const float deltaTime)
	{
		// Updating Particles
		m_updateParticleGroups(cam, deltaTime);
	}

	unsigned int GraphicsScene3D::addRenderable(Renderable3D* renderable)
	{
		// Checking if Renderable's Model has already been added to the Vector
		for (unsigned int i = 0; i < m_renderables.size(); i++)
		{
			std::vector<Renderable3D*>& modelVector = m_renderables[i]; // Getting Vector of Renderables

			if (modelVector[0]->getModel() == renderable->getModel())
			{
				// Attaching Renderable to Exisiting Model
				modelVector.push_back(renderable);

				return i;
			}
		}

		// Attaching Renderable to New Model
		std::vector<Renderable3D*> newModelVector;
		newModelVector.push_back(renderable);
		m_renderables.push_back(newModelVector);

		return m_renderables.size() - 1; //returning index of new model in the vector
	}

	void GraphicsScene3D::popBackRenderables()
	{
		m_renderables.pop_back();
	}

	void GraphicsScene3D::removeRenderable(const unsigned int index)
	{
		m_renderables.erase(m_renderables.begin() + index);
	}

	void GraphicsScene3D::reserveRenderables(const unsigned int amount)
	{
		m_renderables.reserve(amount);
	}


	unsigned int GraphicsScene3D::addLight(Light3D* light)
	{
		m_lights.push_back(light);
		m_isImportantLight.push_back(false);

		return m_lights.size() - 1;
	}

	unsigned int GraphicsScene3D::addLight(Light3D* light, const bool isImportant)
	{
		m_lights.push_back(light);
		m_isImportantLight.push_back(isImportant);
		
		return m_lights.size() - 1;
	}

	void GraphicsScene3D::popBackLights()
	{
		m_lights.pop_back();
	}

	void GraphicsScene3D::removeLight(const unsigned int index)
	{
		m_lights.erase(m_lights.begin() + index);
	}

	void GraphicsScene3D::reserveLights(const unsigned int amount)
	{
		m_lights.reserve(amount);
	}


	unsigned int GraphicsScene3D::addParticleGroup(const ParticleGroup& pg)
	{
		m_particleGroups.push_back(pg);

		return m_particleGroups.size() - 1;
	}

	void GraphicsScene3D::popBackParticleGroups()
	{
		m_particleGroups.pop_back();
	}

	void GraphicsScene3D::removeParticleGroup(const unsigned int index)
	{
		m_particleGroups.erase(m_particleGroups.begin() + index);
	}

	void GraphicsScene3D::reserveParticleGroups(const unsigned int amount)
	{
		m_particleGroups.reserve(amount);
	}


	unsigned int GraphicsScene3D::addWaterTile(WaterTile* tile)
	{
		m_waterTiles.push_back(tile);

		return m_waterTiles.size() - 1;
	}

	void GraphicsScene3D::popBackWaterTiles()
	{
		m_waterTiles.pop_back();
	}

	void GraphicsScene3D::removeWaterTile(const unsigned int index)
	{
		m_waterTiles.erase(m_waterTiles.begin() + index);
	}

	void GraphicsScene3D::reserveWaterTiles(const unsigned int amount)
	{
		m_waterTiles.reserve(amount);
	}


	void GraphicsScene3D::setSkybox(CubeMap* skybox)
	{
		m_skybox = skybox;
	}


	void GraphicsScene3D::m_updateParticleGroups(const Camera3D& cam, const float deltaTime)
	{
		for (unsigned int i = 0; i < m_particleGroups.size(); i++)
		{
			// Updating Particles in Group
			m_particleGroups[i].update(cam, deltaTime);

			// Checking if All Particles from Group are Deleted
			if (m_particleGroups[i].getParticles().size() == 0)
			{
				// Deleting Particle Group
				m_particleGroups.erase(m_particleGroups.begin() + i);
			}
		}
	}

}