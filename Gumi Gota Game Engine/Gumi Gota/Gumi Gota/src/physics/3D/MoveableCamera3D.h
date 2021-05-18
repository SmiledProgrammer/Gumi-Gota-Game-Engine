#ifndef GUMIGOTA_MOVEABLE_CAMERA_3D_H
#define GUMIGOTA_MOVEABLE_CAMERA_3D_H



/* INCLUDES */
// Header Includes
#include "../../graphics/3D/Camera3D.h"



namespace gg
{

	class MoveableCamera3D : public Camera3D
	{
	protected:
		/* Variables */
		vector3 m_velocity;

		Window* m_window; //used for right getting sensitivity and checking which buttons are clicked

	public:
		/* Constructors */
		MoveableCamera3D(const vector3& pos, const float aspectRatio, Window* window);

		/* Functions */
		virtual void update(const float deltaTime) = 0;

		// Setters
		void addVelocity(const vector3& vel);
		void setVelocity(const vector3& vel);

		void setWindow(Window* window);

		// Getters
		inline const vector3& getVelocity() const { return m_velocity; }

	};

}



#endif