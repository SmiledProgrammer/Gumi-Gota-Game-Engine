
/* INCLUDES */
// Self Include
#include "BulletManager.h"



namespace rx
{

	/* Constructors */
	BulletManager::BulletManager(gg::Renderer3D* renderer, gg::PhysicsScene3D* physicsScene)
		: m_renderer(renderer), m_physicsScene(physicsScene),
		  m_bulletParticlesTexture("Gumi Gota/Engine Tester/res/textures/particle texture atlas 2.png", 4), m_bulletParticleSystem(&m_bulletParticlesTexture)
	{
		// Bullet Particle System Setup
		m_bulletParticleSystem.setUsingAdditiveBlending(true);
		m_bulletParticleSystem.setGravity(0.0f);
		m_bulletParticleSystem.setLifetime(0.25f);
		m_bulletParticleSystem.setDirectionError(gg::vector3(2.0f));
	}

	/* Functions */
	void BulletManager::update(const float deltaTime) 
	{
		for (unsigned int i = 0; i < m_bulletObjects.size(); i++)
		{
			// Generating Particles
			m_bulletParticleSystem.setDirection(m_bulletObjects[i]->getVelocity() * -1.0f);
			m_renderer->addParticleGroup(m_bulletParticleSystem.generateParticles(m_bulletObjects[i]->getPosition(), 1));

			// Checking If Bullet Should Die
			if (!m_bulletObjects[i]->isAlive())
			{
				m_renderer->removeRenderable(m_bulletsRendererIDs[i]);
				m_physicsScene->removeRigidBody(m_bulletsPhysicsSceneIDs[i]);

				m_lowerIndices(m_bulletsRendererIDs[i], m_bulletsPhysicsSceneIDs[i]);

				m_bulletsRendererIDs.erase(m_bulletsRendererIDs.begin() + i);
				m_bulletsPhysicsSceneIDs.erase(m_bulletsPhysicsSceneIDs.begin() + i);

				m_bulletObjects.erase(m_bulletObjects.begin() + i);
			}
		}
	}

	void BulletManager::createBullet(const gg::Camera3D& cam)
	{
		float pitch = cam.getPitch();
		float yaw = cam.getYaw();

		gg::vector3 direction(-gg::sine(yaw) * gg::cosine(pitch), gg::sine(pitch), -gg::cosine(yaw) * gg::cosine(pitch));

		m_bulletObjects.push_back(new rx::BulletObject(cam.getPosition(), direction, 10.0f, gg::vector3(pitch, yaw, 0.0f)));
		unsigned int rendererIndex = m_renderer->addRenderable(m_bulletObjects.back());
		unsigned int physicsSceneIndex = m_physicsScene->addRigidBody(m_bulletObjects.back());

		m_bulletsRendererIDs.push_back(rendererIndex);
		m_bulletsPhysicsSceneIDs.push_back(physicsSceneIndex);

		///std::cout << "Pitch: " << gg::toDegrees(pitch) << ", Yaw: " << gg::toDegrees(yaw) << std::endl;
		///std::cout << "X: " << gg::toDegrees(bullet.getRotation().x) << " Y: " << gg::toDegrees(bullet.getRotation().y) << ", Z: " << gg::toDegrees(bullet.getRotation().z) << std::endl;
	}

	void BulletManager::reserveBullets(const unsigned int amount)
	{
		m_bulletObjects.reserve(amount);
		m_bulletsRendererIDs.reserve(amount);
		m_bulletsPhysicsSceneIDs.reserve(amount);
	}

	// Private Functions
	void BulletManager::m_lowerIndices(const unsigned int rendererIndex, const unsigned int physicsSceneIndex)
	{
		for (unsigned int i = 0; i < m_bulletObjects.size(); i++)
		{
			if (m_bulletsRendererIDs[i] > rendererIndex)
				m_bulletsRendererIDs[i]--;

			if (m_bulletsPhysicsSceneIDs[i] > physicsSceneIndex)
				m_bulletsPhysicsSceneIDs[i]--;
		}
	}

}