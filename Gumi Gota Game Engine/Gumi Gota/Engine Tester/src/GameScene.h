#ifndef REXER_GAME_H
#define REXER_GAME_H



/* INCLUDES */
// Gumi Gota Include
#include "../../Gumi Gota/src/GumiGota.h"

// Normal Includes
#include <vector>



namespace rx
{

	class GameScene
	{
	protected:
		/* Variables */
		std::vector<gg::Texture> m_textures;

		std::vector<gg::Model> m_models;
		std::vector<gg::Renderable3D> m_renderables3D;
		std::vector<gg::Light3D> m_lights3D;
		std::vector<gg::CubeMap> m_cubeMaps;
		std::vector<gg::ParticleSystem> m_particleSystems;
		std::vector<gg::WaterTile> m_waterTiles;

		std::vector<gg::Sprite2D> m_sprites2D;
		std::vector<gg::Light2D> m_lights2D;
		std::vector<gg::Font> m_fonts;
		std::vector<gg::Label2D> m_labels;

	public:
		/* Constructors */
		GameScene();

		/* Functions */
		void update();

		void loadScene(gg::Renderer3D& renderer3D, gg::Layer2D& renderer2D, gg::PhysicsScene3D& physicsScene3D);

	private:
		// Private Functions


	};

}



#endif