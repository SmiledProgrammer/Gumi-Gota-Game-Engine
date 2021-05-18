
/* INCLUDES */
// Self Include
#include "OrientedBoundingBox3D.h"



namespace gg
{

	/* Constructors */
	OrientedBoundingBox3D::OrientedBoundingBox3D(const vector3& pos, const vector3& size)
		: m_position(pos), m_size(size), m_rotation(vector3())
	{}

	/* Functions */
	// Setters
	void OrientedBoundingBox3D::setPosition(const vector3& pos)
	{
		m_position = pos;
	}

	void OrientedBoundingBox3D::setSize(const vector3& size)
	{
		m_size = size;
	}

	void OrientedBoundingBox3D::setRotation(const vector3& rot)
	{
		m_rotation = rot;
	}

}