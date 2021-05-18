
/* INCLUDES */
// Self Include
#include "RigidBody3D.h"

#include <iostream> /// remove l8r



namespace gg
{

	/* Constructors */
	RigidBody3D::RigidBody3D(Model* model, const vector3& pos)
		: Renderable3D(model, pos),
		  m_velocity(vector3()), m_angularVelocity(vector3())
	{}

	/* Functions */
	void RigidBody3D::update(const float deltaTime)
	{
		m_position += m_velocity * deltaTime;
		m_rotation += m_angularVelocity * deltaTime;
	}


	unsigned int RigidBody3D::addBoundingBox(const AxisAlignedBoundingBox3D& box)
	{
		m_AABBs.push_back(box);

		return m_AABBs.size() - 1;
	}

	unsigned int RigidBody3D::addBoundingBox(const OrientedBoundingBox3D& box)
	{
		m_OBBs.push_back(box);

		return m_OBBs.size() - 1;
	}

	unsigned int RigidBody3D::addBoundingBox(const SphericalBoundingBox3D& box)
	{
		m_SBBs.push_back(box);

		return m_SBBs.size() - 1;
	}

	void RigidBody3D::reserveBoundingBoxes(const RigidBody3DBoundingBoxType boxType, const unsigned int amount)
	{
		switch (boxType)
		{
		case AXIS_ALIGNED :
			m_AABBs.reserve(amount);
			break;

		case ORIENTED:
			m_OBBs.reserve(amount);
			break;

		case SPHERICAL:
			m_SBBs.reserve(amount);
			break;
		}
	}

	void RigidBody3D::removeBoundingBox(const RigidBody3DBoundingBoxType boxType, const unsigned int index)
	{
		switch (boxType)
		{
		case AXIS_ALIGNED:
			m_AABBs.erase(m_AABBs.begin() + index);
			break;

		case ORIENTED:
			m_OBBs.erase(m_OBBs.begin() + index);
			break;

		case SPHERICAL:
			m_SBBs.erase(m_SBBs.begin() + index);
			break;
		}
	}

	void RigidBody3D::popBackBoundingBoxes(const RigidBody3DBoundingBoxType boxType)
	{
		switch (boxType)
		{
		case AXIS_ALIGNED:
			m_AABBs.pop_back();
			break;

		case ORIENTED:
			m_OBBs.pop_back();
			break;

		case SPHERICAL:
			m_SBBs.pop_back();
			break;
		}
	}


	// Setters
	void RigidBody3D::addVelocity(const vector3& vel)
	{
		m_velocity += vel;
	}

	void RigidBody3D::addAngularVelocity(const vector3& vel)
	{
		m_angularVelocity += vel;
	}

	void RigidBody3D::setVelocity(const vector3& vel)
	{
		m_velocity = vel;
	}

	void RigidBody3D::setAngularVelocity(const vector3& vel)
	{
		m_angularVelocity = vel;
	}

}