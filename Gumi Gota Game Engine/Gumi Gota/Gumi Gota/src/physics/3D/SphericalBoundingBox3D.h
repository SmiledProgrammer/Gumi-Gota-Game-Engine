#ifndef GUMIGOTA_SPHERICAL_BOUNDING_BOX_3D_H
#define GUMIGOTA_SPHERICAL_BOUNDING_BOX_3D_H



/* INCLUDES */
// Header Includes
#include "../../maths/vectors/vector3.h"



namespace gg
{

	class SphericalBoundingBox3D
	{
	protected:
		/* Variables */
		vector3 m_position;
		float m_radius;

	public:
		/* Constructors */
		SphericalBoundingBox3D(const vector3& pos, const float radius);

		/* Functions */
		// Setters
		void setPosition(const vector3& pos);
		void setRadius(const float radius);

		// Getters
		inline const vector3& getPosition() const { return m_position; }
		inline float getRadius() const { return m_radius; }

	};

}



#endif