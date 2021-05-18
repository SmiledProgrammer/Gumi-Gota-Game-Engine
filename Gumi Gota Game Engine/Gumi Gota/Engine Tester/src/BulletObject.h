#ifndef REXER_BULLET_OBJECT_H
#define REXER_BULLET_OBJECT_H



/* INCLUDES */
// Gumi Gota Include
#include "../../Gumi Gota/src/GumiGota.h"



namespace rx
{

	class BulletObject : public gg::RigidBody3D
	{
	protected:
		/* Variables */
		gg::Model m_model = gg::loadModelFromFile("Gumi Gota/Engine Tester/res/models/rexer_bullet_model.obj", new gg::Texture(gg::Color(0.0f, 0.0f, 0.0f, 1.0f)));

		float m_lifetime; //stored in seconds
		gg::Timer m_lifeTimer;

	public:
		/* Constructors */
		BulletObject(const gg::vector3& pos, const gg::vector3& dir, const float speed); //dir stands for direction
		BulletObject(const gg::vector3& pos, const float lifetime, const gg::vector3& dir, const float speed); //dir stands for direction, lifetime is given in seconds
		BulletObject(const gg::vector3& pos, const gg::vector3& vel); //vel stands for velocity
		BulletObject(const gg::vector3& pos, const float lifetime, const gg::vector3& vel); //vel stands for velocity, lifetime is given in seconds
		BulletObject(const gg::vector3& pos, const gg::vector3& dir, const float speed, const gg::vector3& rot); //dir stands for direction; rot stands for rotation
		BulletObject(const gg::vector3& pos, const float lifetime, const gg::vector3& dir, const float speed, const gg::vector3& rot); //dir stands for direction, lifetime is given in seconds; rot stands for rotation
		BulletObject(const gg::vector3& pos, const gg::vector3& vel, const gg::vector3& rot); //vel stands for velocity; rot stands for rotation
		BulletObject(const gg::vector3& pos, const float lifetime, const gg::vector3& vel, const gg::vector3& rot); //vel stands for velocity, lifetime is given in seconds; rot stands for rotation

		/* Functions */
		// Overrides
		void update(const float deltaTime) override;

		// Getters
		bool isAlive();

	};

}



#endif