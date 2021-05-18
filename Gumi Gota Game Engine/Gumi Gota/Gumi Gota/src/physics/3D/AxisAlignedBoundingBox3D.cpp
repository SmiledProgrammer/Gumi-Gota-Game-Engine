
/* INCLUDES */
// Self Include
#include "AxisAlignedBoundingBox3D.h"



namespace gg
{

	/* Constructors */
	AxisAlignedBoundingBox3D::AxisAlignedBoundingBox3D(const vector3& pos, const vector3& size)
		: m_position(pos), m_size(size)
	{}

	/* Functions */
	// Setters
	void AxisAlignedBoundingBox3D::setPosition(const vector3& pos)
	{
		m_position = pos;
	}

	void AxisAlignedBoundingBox3D::setSize(const vector3& size)
	{
		m_size = size;
	}

}