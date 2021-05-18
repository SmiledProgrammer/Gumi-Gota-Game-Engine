
/* INCLUDES */
// Self Include
#include "Game.h"



namespace rx
{

	/* Constructors */
	Game::Game()
		: m_window(gg::Window("Gumi Gota Engine v0.0.1 - Tester", 1920, 1080)), m_renderer2D(m_window), m_renderer3D(&m_window)
	{
		init();
	}

	/* Functions */
	void Game::update()
	{
		// Getting Delta Time
		float deltaTime = m_deltaTimeTimer.elapsed();
		m_deltaTimeTimer.reset();

		/* Updating */
		m_physicsScene.update(deltaTime);
		///m_renderer3D.update(deltaTime);
	}

	void Game::render()
	{
		/* Clearing */
		m_window.clear();

		/* Rendering */
		///m_renderer3D.render();
		m_renderer2D.render();

		/* Drawing */
		m_window.update();
	}


	bool Game::running()
	{
		return !m_window.closed();
	}

	// Private Functions
	void Game::init()
	{
		// Window Setup
		m_window.hideCursor();

		// Renderer Setup
		m_renderer3D.setSkyColor(gg::Color(0.098f, 0.149f, 0.271f));
		m_renderer3D.reserveParticleGroups(300); /// check l8r
	}

}