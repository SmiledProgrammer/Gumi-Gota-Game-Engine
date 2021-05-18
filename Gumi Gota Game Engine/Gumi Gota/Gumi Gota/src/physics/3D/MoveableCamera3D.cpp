
/* INCLUDES */
// Self Include
#include "MoveableCamera3D.h"



namespace gg
{

	/* Constructors */
	MoveableCamera3D::MoveableCamera3D(const vector3& pos, const float aspectRatio, Window* window)
		: Camera3D(pos, aspectRatio),
		  m_velocity(vector3()),
		  m_window(window)
	{}

	/* Functions */
	// Setters
	void MoveableCamera3D::addVelocity(const vector3& vel)
	{
		m_velocity += vel;
	}

	void MoveableCamera3D::setVelocity(const vector3& vel)
	{
		m_velocity = vel;
	}

	void MoveableCamera3D::setWindow(Window* window)
	{
		m_window = window;
	}

}