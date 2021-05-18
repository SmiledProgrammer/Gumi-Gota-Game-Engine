#ifndef GUMIGOTA_ORIENTED_BOUNDING_BOX_3D_H
#define GUMIGOTA_ORIENTED_BOUNDING_BOX_3D_H



/* INCLUDES */
// Header Includes
#include "../../maths/vectors/vector3.h"



namespace gg
{

	class OrientedBoundingBox3D
	{
	protected:
		/* Variables */
		vector3 m_position;
		vector3 m_size;
		vector3 m_rotation;

	public:
		/* Constructors */
		OrientedBoundingBox3D(const vector3& pos, const vector3& size);

		/* Functions */
		// Setters
		void setPosition(const vector3& pos);
		void setSize(const vector3& size);
		void setRotation(const vector3& rot);

		// Getters
		inline const vector3& getPosition() const { return m_position; }
		inline const vector3& getSize()     const { return m_size;     }
		inline const vector3& getRotation() const { return m_rotation; }

	};

}



#endif