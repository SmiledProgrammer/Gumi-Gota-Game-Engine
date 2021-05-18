#ifndef GUMIGOTA_RIGID_BODY_3D_H
#define GUMIGOTA_RIGID_BODY_3D_H



/* INCLUDES */
// Normal Includes
#include <vector>

// Header Includes
#include "../../graphics/3D/Renderable3D.h"
#include "AxisAlignedBoundingBox3D.h"
#include "OrientedBoundingBox3D.h"
#include "SphericalBoundingBox3D.h"



namespace gg
{
	
	/* ADDITIONAL STUFF */
	enum RigidBody3DBoundingBoxType
	{
		AXIS_ALIGNED = 0,
		ORIENTED,
		SPHERICAL
	};



	class RigidBody3D : public Renderable3D
	{
	protected:
		/* Variables */
		vector3 m_velocity;
		vector3 m_angularVelocity;

		std::vector<AxisAlignedBoundingBox3D> m_AABBs;
		std::vector<OrientedBoundingBox3D> m_OBBs;
		std::vector<SphericalBoundingBox3D> m_SBBs;

	public:
		/* Constructors */
		RigidBody3D(Model* model, const vector3& pos);

		/* Functions */
		virtual void update(const float deltaTime);

		unsigned int addBoundingBox(const AxisAlignedBoundingBox3D& box);
		unsigned int addBoundingBox(const OrientedBoundingBox3D& box);
		unsigned int addBoundingBox(const SphericalBoundingBox3D& box);
		void reserveBoundingBoxes(const RigidBody3DBoundingBoxType boxType, const unsigned int amount);
		void removeBoundingBox(const RigidBody3DBoundingBoxType boxType, const unsigned int index);
		void popBackBoundingBoxes(const RigidBody3DBoundingBoxType boxType);

		// Setters
		void addVelocity(const vector3& vel);
		void addAngularVelocity(const vector3& vel);
		void setVelocity(const vector3& vel);
		void setAngularVelocity(const vector3& vel);

		// Getters
		inline const vector3& getVelocity()        const { return m_velocity; }
		inline const vector3& getAngularVelocity() const { return m_angularVelocity; }

		inline const std::vector<AxisAlignedBoundingBox3D>& getAxisAlignedBoundingBoxes() const { return m_AABBs; }
		inline const std::vector<OrientedBoundingBox3D>&    getOrientedBoundingBoxes()    const { return m_OBBs;  }
		inline const std::vector<SphericalBoundingBox3D>&   getSphericalBoundingBoxes()   const { return m_SBBs;  }

	};

}



#endif