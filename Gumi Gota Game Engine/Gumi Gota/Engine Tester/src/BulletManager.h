#ifndef REXER_BULLET_MANAGER_H
#define REXER_BULLET_MANAGER_H



/* INCLUDES */
// Normal Includes
#include <vector>

// Header Includes
#include "BulletObject.h"



namespace rx
{

	class BulletManager
	{
	protected:
		/* Variables */
		std::vector<BulletObject*> m_bulletObjects;
		std::vector<unsigned int> m_bulletsRendererIDs;
		std::vector<unsigned int> m_bulletsPhysicsSceneIDs;

		gg::Renderer3D* m_renderer;
		gg::PhysicsScene3D* m_physicsScene;

		gg::Texture m_bulletParticlesTexture;
		gg::DirectionParticleSystem m_bulletParticleSystem;

	public:
		/* Constructors */
		BulletManager(gg::Renderer3D* renderer, gg::PhysicsScene3D* physicsScene);

		/* Functions */
		void update(const float deltaTime);

		void createBullet(const gg::Camera3D& cam);
		void reserveBullets(const unsigned int amount);

	private:
		// Private Functions
		void m_lowerIndices(const unsigned int rendererIndex, const unsigned int physicsSceneIndex);

	};

}



#endif