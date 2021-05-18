#ifndef GUMIGOTA_PHYSICS_SCENE_3D_H
#define GUMIGOTA_PHYSICS_SCENE_3D_H



/* INCLUDES */
// Normal Includes
#include <vector>

// Header Includes
#include "MoveableCamera3D.h"
#include "RigidBody3D.h"



namespace gg
{

	class PhysicsScene3D
	{
	protected:
		/* Variables */
		std::vector<RigidBody3D*> m_bodys;
		std::vector<MoveableCamera3D*> m_cameras;

	public:
		/* Constructors */
		PhysicsScene3D();

		/* Functions */
		void update(const float deltaTime);

		void explosion(const vector3& center, const float explosionStrength);
		void explosion(const vector3& center, const vector2& explosionAttenuation); // explosionAttenuation are values indicating how much the explosion should affect objects in different distance ranges (so also how strong the explosion is)

		unsigned int addRigidBody(RigidBody3D* body);
		void popBackRigidBodys();
		void removeRigidBody(const unsigned int index);
		void reserveRigidBodys(const unsigned int amount);

		unsigned int addCamera(MoveableCamera3D* cam);
		void popBackCameras();
		void removeCamera(const unsigned int index);
		void reserveCameras(const unsigned int amount);

	};

}



#endif