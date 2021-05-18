
/* INCLUDES */
// Self Include
#include "FlyingCamera3D.h"

// Header Includes
#include "../../maths/general/BasicOperations.h"
#include "../../maths/geometry/AngleTransformations.h"
#include "../../maths/trigonometry/Trigonometry.h"
#include "../../maths/geometry/VectorTransformations.h"
#include "../../maths/general/Constants.h"



/* MACROS */
#define FLYING_CAMERA_3D_DEFAULT_FLYING_SPEED 50.0f
#define FLYING_CAMERA_3D_DEFAULT_SENSITIVTY 0.5f, 0.5f

#define FLYING_CAMERA_3D_DEFAULT_FORWARD_KEY	GLFW_KEY_W
#define FLYING_CAMERA_3D_DEFAULT_BACKWARD_KEY	GLFW_KEY_S
#define FLYING_CAMERA_3D_DEFAULT_LEFT_KEY		GLFW_KEY_A
#define FLYING_CAMERA_3D_DEFAULT_RIGHT_KEY		GLFW_KEY_D



namespace gg
{

	/* Constructors */
	FlyingCamera3D::FlyingCamera3D(const vector3& pos, const float aspectRatio, Window* window)
		: MoveableCamera3D(pos, aspectRatio, window),
		  m_flyingSpeed(FLYING_CAMERA_3D_DEFAULT_FLYING_SPEED), m_sensitivity(FLYING_CAMERA_3D_DEFAULT_SENSITIVTY),
		  m_forwardKey(FLYING_CAMERA_3D_DEFAULT_FORWARD_KEY), m_backwardKey(FLYING_CAMERA_3D_DEFAULT_BACKWARD_KEY), m_leftKey(FLYING_CAMERA_3D_DEFAULT_LEFT_KEY), m_rightKey(FLYING_CAMERA_3D_DEFAULT_RIGHT_KEY)
	{}

	/* Functions */
	void FlyingCamera3D::update(const float deltaTime)
	{
		// Resetting Velocity
		m_velocity = vector3(0.0f);

		// Position Movement
		if (m_window->isKeyPressed(m_forwardKey))
		{
			m_velocity.y = sine(m_pitch);
			m_velocity.x = -sine(m_yaw) * cosine(m_pitch);
			m_velocity.z = -cosine(m_yaw) * cosine(m_pitch);
		}

		if (m_window->isKeyPressed(m_backwardKey))
		{
			m_velocity.y = -sine(m_pitch);
			m_velocity.x = sine(m_yaw) * cosine(m_pitch);
			m_velocity.z = cosine(m_yaw) * cosine(m_pitch);
		}

		if (m_window->isKeyPressed(m_leftKey))
		{
			float perpendicularAngle = m_yaw - GUMIGOTA_PI_DIVIDED_BY_2;

			m_velocity.x = sine(perpendicularAngle);
			m_velocity.z = cosine(perpendicularAngle);
		}

		if (m_window->isKeyPressed(m_rightKey))
		{
			float perpendicularAngle = m_yaw - GUMIGOTA_PI_DIVIDED_BY_2;

			m_velocity.x = -sine(perpendicularAngle);
			m_velocity.z = -cosine(perpendicularAngle);
		}

		// Normalizing Velocity
		m_velocity.normalize();

		// Moving by Velocity
		m_velocity *= m_flyingSpeed * deltaTime;
		move(m_velocity);

		// Angle Movement
		vector2 currentMousePosition = m_window->getMousePosition();
		vector2 mouseDifference = m_lastMousePosition - currentMousePosition;

		addYaw(mouseDifference.x * m_sensitivity.x * deltaTime);
		addPitch(mouseDifference.y * m_sensitivity.y * deltaTime);

		m_pitch = clamp(m_pitch, -GUMIGOTA_PI_DIVIDED_BY_2, GUMIGOTA_PI_DIVIDED_BY_2);

		// Centering Cursor
		m_window->setMousePosition(vector2((float)(m_window->getWidth() / 2.0f), (float)(m_window->getHeight() / 2.0f)));
		m_lastMousePosition = vector2((float)(m_window->getWidth() / 2.0f), (float)(m_window->getHeight() / 2.0f));
	}

	// Setters
	void FlyingCamera3D::setSpeed(const float speed)
	{
		m_flyingSpeed = speed;
	}

	void FlyingCamera3D::setSensitivity(const vector2& sens)
	{
		m_sensitivity = sens;
	}


	void FlyingCamera3D::setForwardKey(const int key)
	{
		m_forwardKey = key;
	}

	void FlyingCamera3D::setBackwardKey(const int key)
	{
		m_backwardKey = key;
	}

	void FlyingCamera3D::setLeftKey(const int key)
	{
		m_leftKey = key;
	}

	void FlyingCamera3D::setRightKey(const int key)
	{
		m_rightKey = key;
	}

	void FlyingCamera3D::setKeyControls(const int forwardKey, const int backwardKey, const int leftKey, const int rightKey)
	{
		m_forwardKey = forwardKey;
		m_backwardKey = backwardKey;
		m_leftKey = leftKey;
		m_rightKey = rightKey;
	}


}