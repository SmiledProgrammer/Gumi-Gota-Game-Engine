
/* INCLUDES */
// Self Include
#include "PhysicsScene3D.h"

#include <iostream> /// remove l8r



namespace gg
{

	/* Constructors */
	PhysicsScene3D::PhysicsScene3D()
	{}

	/* Functions */
	void PhysicsScene3D::update(const float deltaTime)
	{
		// Updating Rigid Bodies
		for (RigidBody3D* rigidBody : m_bodys)
		{
			rigidBody->update(deltaTime);
		}

		// Updating Cameras
		for (MoveableCamera3D* camera : m_cameras)
		{
			camera->update(deltaTime);
		}
	}


	void PhysicsScene3D::explosion(const vector3& center, const float explosionStrength)
	{
		// Going Through Rigid Bodies
		for (RigidBody3D* rigidBody : m_bodys)
		{
			vector3 toRigidBodyVector = rigidBody->getPosition() - center;
			float rigidBodyDistance = toRigidBodyVector.length();
			float explosionAttenuationFactor = explosionStrength / rigidBodyDistance;

			toRigidBodyVector.normalize();
			rigidBody->addVelocity(toRigidBodyVector * explosionAttenuationFactor);
		}

		// Going Through Cameras
		for (MoveableCamera3D* camera : m_cameras)
		{
			vector3 toCameraVector = camera->getPosition() - center;
			float cameraDistance = toCameraVector.length();
			float explosionAttenuationFactor = explosionStrength / cameraDistance;

			toCameraVector.normalize();
			camera->addVelocity(toCameraVector * explosionAttenuationFactor);
		}
	}

	void PhysicsScene3D::explosion(const vector3& center, const vector2& explosionAttenuation)
	{
		// Going Through Rigid Bodies
		for (RigidBody3D* rigidBody : m_bodys)
		{
			vector3 toRigidBodyVector = rigidBody->getPosition() - center;
			float rigidBodyDistance = toRigidBodyVector.length();
			float explosionAttenuationFactor = explosionAttenuation.x / rigidBodyDistance / rigidBodyDistance + explosionAttenuation.y / rigidBodyDistance;

			toRigidBodyVector.normalize();
			rigidBody->addVelocity(toRigidBodyVector * explosionAttenuationFactor);
		}
		
		// Going Through Cameras
		for (MoveableCamera3D* camera : m_cameras)
		{
			vector3 toCameraVector = camera->getPosition() - center;
			float cameraDistance = toCameraVector.length();
			float explosionAttenuationFactor = explosionAttenuation.x / cameraDistance / cameraDistance + explosionAttenuation.y / cameraDistance;

			toCameraVector.normalize();
			camera->addVelocity(toCameraVector * explosionAttenuationFactor);
		}
	}


	unsigned int PhysicsScene3D::addRigidBody(RigidBody3D* body)
	{
		m_bodys.push_back(body);

		return m_bodys.size() - 1;
	}

	void PhysicsScene3D::popBackRigidBodys()
	{
		m_bodys.pop_back();
	}

	void PhysicsScene3D::removeRigidBody(const unsigned int index)
	{
		m_bodys.erase(m_bodys.begin() + index);
	}

	void PhysicsScene3D::reserveRigidBodys(const unsigned int amount)
	{
		m_bodys.reserve(amount);
	}


	unsigned int PhysicsScene3D::addCamera(MoveableCamera3D* cam)
	{
		m_cameras.push_back(cam);

		return m_cameras.size() - 1;
	}

	void PhysicsScene3D::popBackCameras()
	{
		m_cameras.pop_back();
	}

	void PhysicsScene3D::removeCamera(const unsigned int index)
	{
		m_cameras.erase(m_cameras.begin() + index);
	}

	void PhysicsScene3D::reserveCameras(const unsigned int amount)
	{
		m_cameras.reserve(amount);
	}

}