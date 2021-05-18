#ifndef GUMIGOTA_GRAPHICS_SCENE_3D_H
#define GUMIGOTA_GRAPHICS_SCENE_3D_H



/* INCLUDES */
// Header Includes
#include "Renderable3D.h"
#include "Light3D.h"
#include "particles/ParticleGroup.h"
#include "CubeMap.h"
#include "water/WaterTile.h"



namespace gg
{

	/* ADDITIONAL FUNCTIONS */
	std::vector<unsigned int> getClosestLightsIndices(const vector3& pos, const std::vector<Light3D*>& lights, const unsigned int amount);



	class GraphicsScene3D
	{
	protected:
		/* Variables */
		std::vector<std::vector<Renderable3D*>> m_renderables; //vector of Renderable3D* vectors is used for optimizing rendering (binding 1 Model and drawing many Rendrable3Ds from it

		std::vector<Light3D*> m_lights;
		std::vector<bool> m_isImportantLight; //used for lights that are forced to be rendered

		std::vector<ParticleGroup> m_particleGroups;

		std::vector<WaterTile*> m_waterTiles;

		CubeMap* m_skybox;

	public:
		/* Constructors */
		GraphicsScene3D();

		/* Functions */
		void update(const Camera3D& cam, const float deltaTime);

		virtual unsigned int addRenderable(Renderable3D* renderable); //returns index of renderable in the vector; attaches renderable to new model in vector or existing model if any renderable already has the same model
		void popBackRenderables();
		void removeRenderable(const unsigned int index);
		void reserveRenderables(const unsigned int amount);

		virtual unsigned int addLight(Light3D* light);
		virtual unsigned int addLight(Light3D* light, const bool isImportant); //if isImportant is true, light will be forced to be rendered, even when other lights are closer
		void popBackLights();
		void removeLight(const unsigned int index);
		void reserveLights(const unsigned int amount);

		virtual unsigned int addParticleGroup(const ParticleGroup& pg);
		void popBackParticleGroups();
		void removeParticleGroup(const unsigned int index);
		void reserveParticleGroups(const unsigned int amount);

		virtual unsigned int addWaterTile(WaterTile* tile);
		void popBackWaterTiles();
		void removeWaterTile(const unsigned int index);
		void reserveWaterTiles(const unsigned int amount);

		void setSkybox(CubeMap* skybox);

	private:
		// Private Functions
		void m_updateParticleGroups(const Camera3D& cam, const float deltaTime);

	};

}



#endif