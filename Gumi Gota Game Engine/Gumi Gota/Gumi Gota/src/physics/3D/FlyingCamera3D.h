#ifndef GUMIGOTA_FLYING_CAMERA_3D_H
#define GUMIGOTA_FLYING_CAMERA_3D_H



/* INCLUDES */
// Header Includes
#include "MoveableCamera3D.h"



namespace gg
{

	class FlyingCamera3D : public MoveableCamera3D
	{
	protected:
		/* Variables */
		float m_flyingSpeed;
		vector2 m_sensitivity;

		vector2 m_lastMousePosition;

		int m_forwardKey, m_backwardKey, m_leftKey, m_rightKey;

	public:
		/* Constructors */
		FlyingCamera3D(const vector3& pos, const float aspectRatio, Window* window);

		/* Functions */
		void update(const float deltaTime);

		// Setters
		void setSpeed(const float speed);
		void setSensitivity(const vector2& sens);

		void setForwardKey(const int key);
		void setBackwardKey(const int key);
		void setLeftKey(const int key);
		void setRightKey(const int key);
		void setKeyControls(const int forwardKey, const int backwardKey, const int leftKey, const int rightKey);

	};

}



#endif