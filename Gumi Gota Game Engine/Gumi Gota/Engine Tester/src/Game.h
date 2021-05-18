#ifndef REXER_GAME_H
#define REXER_GAME_H



/* INCLUDES */
// Gumi Gota Include
#include "../../Gumi Gota/src/GumiGota.h"



namespace rx
{

	class Game
	{
	private:
		/* Variables */
		gg::Window m_window;
		gg::Renderer3D m_renderer3D;
		gg::Layer2D m_renderer2D;

		gg::PhysicsScene3D m_physicsScene;

		gg::Timer m_deltaTimeTimer;

	public:
		/* Constructors */
		Game();

		/* Functions */
		void update();
		void render();

		bool running();

	private:
		// Private Functions
		void init();

	};

}



#endif