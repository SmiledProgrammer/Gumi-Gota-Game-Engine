
/* INCLUDES */
// Self Include
#include "SphericalBoundingBox3D.h"



namespace gg
{

	/* Constructors */
	SphericalBoundingBox3D::SphericalBoundingBox3D(const vector3& pos, const float radius)
		: m_position(pos), m_radius(radius)
	{}

	/* Functions */
	// Setters
	void SphericalBoundingBox3D::setPosition(const vector3& pos)
	{
		m_position = pos;
	}

	void SphericalBoundingBox3D::setRadius(const float radius)
	{
		m_radius = radius;
	}

}