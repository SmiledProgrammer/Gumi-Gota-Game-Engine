
/* INCLUDES */
// Self Include
#include "BulletObject.h"



/* MACROS */
#define BULLETOBJECT_DEFAULT_LIFETIME 8.0f



namespace rx
{

	/* Constructors */
	BulletObject::BulletObject(const gg::vector3& pos, const gg::vector3& dir, const float speed)
		: RigidBody3D(&m_model, pos), m_lifetime(BULLETOBJECT_DEFAULT_LIFETIME)
	{
		m_velocity = dir.normalized();
		m_velocity *= speed;
	}

	BulletObject::BulletObject(const gg::vector3& pos, const float lifetime, const gg::vector3& dir, const float speed)
		: RigidBody3D(&m_model, pos), m_lifetime(lifetime)
	{
		m_velocity = dir.normalized();
		m_velocity *= speed;
	}

	BulletObject::BulletObject(const gg::vector3& pos, const gg::vector3& vel)
		: RigidBody3D(&m_model, pos), m_lifetime(BULLETOBJECT_DEFAULT_LIFETIME)
	{
		m_velocity = vel;
	}

	BulletObject::BulletObject(const gg::vector3& pos, const float lifetime, const gg::vector3& vel)
		: RigidBody3D(&m_model, pos), m_lifetime(lifetime)
	{
		m_velocity = vel;
	}

	BulletObject::BulletObject(const gg::vector3& pos, const gg::vector3& dir, const float speed, const gg::vector3& rot)
		: RigidBody3D(&m_model, pos), m_lifetime(BULLETOBJECT_DEFAULT_LIFETIME)
	{
		m_velocity = dir.normalized();
		m_velocity *= speed;

		m_rotation = rot;
	}

	BulletObject::BulletObject(const gg::vector3& pos, const float lifetime, const gg::vector3& dir, const float speed, const gg::vector3& rot)
		: RigidBody3D(&m_model, pos), m_lifetime(lifetime)
	{
		m_velocity = dir.normalized();
		m_velocity *= speed;

		m_rotation = rot;
	}

	BulletObject::BulletObject(const gg::vector3& pos, const gg::vector3& vel, const gg::vector3& rot)
		: RigidBody3D(&m_model, pos), m_lifetime(BULLETOBJECT_DEFAULT_LIFETIME)
	{
		m_velocity = vel;

		m_rotation = rot;
	}

	BulletObject::BulletObject(const gg::vector3& pos, const float lifetime, const gg::vector3& vel, const gg::vector3& rot)
		: RigidBody3D(&m_model, pos), m_lifetime(lifetime)
	{
		m_velocity = vel;

		m_rotation = rot;
	}

	/* Functions */
	// Overrides
	void BulletObject::update(const float deltaTime)
	{
		RigidBody3D::m_position += m_velocity * deltaTime;
		RigidBody3D::m_rotation += m_angularVelocity * deltaTime;

		Renderable3D::m_position += m_velocity * deltaTime;
		Renderable3D::m_rotation += m_angularVelocity * deltaTime;
	}

	// Getters
	bool BulletObject::isAlive()
	{
		return (m_lifetime > m_lifeTimer.elapsed());
	}

}