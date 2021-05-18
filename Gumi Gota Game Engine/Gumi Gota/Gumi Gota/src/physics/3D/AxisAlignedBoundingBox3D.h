#ifndef GUMIGOTA_AXIS_ALIGNED_BOUNDING_BOX_3D_H
#define GUMIGOTA_AXIS_ALIGNED_BOUNDING_BOX_3D_H



/* INCLUDES */
// Header Includes
#include "../../maths/vectors/vector3.h"



namespace gg
{

	class AxisAlignedBoundingBox3D
	{
	protected:
		/* Variables */
		vector3 m_position;
		vector3 m_size;

	public:
		/* Constructors */
		AxisAlignedBoundingBox3D(const vector3& pos, const vector3& size);

		/* Functions */
		// Setters
		void setPosition(const vector3& pos);
		void setSize(const vector3& size);

		// Getters
		inline const vector3& getPosition() const { return m_position; }
		inline const vector3& getSize()     const { return m_size;     }

	};

}



#endif