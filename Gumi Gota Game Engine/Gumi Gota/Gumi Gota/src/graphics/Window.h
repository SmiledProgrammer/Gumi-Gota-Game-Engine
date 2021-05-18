#ifndef GUMIGOTA_WINDOW_H
#define GUMIGOTA_WINDOW_H



/* INCLUDES */
// OpenGL Includes
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Header Includes
#include "../maths/vectors/vector2.h"
#include "../maths/vectors/vector4.h"

#include "Color.h"



/* CONSTANTS */
#define WINDOW_MAX_KEYS 1024
#define WINDOW_MAX_BUTTONS 32



namespace gg
{

	class Window
	{
	private:
		/* Variables */
		const char* m_title;
		int m_width, m_height;
		bool m_closed;
		GLFWwindow* m_window;

		bool m_keysPressed[WINDOW_MAX_KEYS]; //used for checking if keyboard keys are pressed (continuously)
		bool m_buttonsPressed[WINDOW_MAX_BUTTONS]; //used for checking if mouse buttons are pressed (continuously)
		bool m_keysClicked[WINDOW_MAX_KEYS]; //used for checking if keyboard keys are pressed (once)
		bool m_buttonsClicked[WINDOW_MAX_BUTTONS]; //used for checking if mouse buttons are pressed (once)
		vector2 m_mousePos; //used for storing information about mouse position

	public:
		/* Constructors */
		Window(const char* title, const unsigned int& width, const unsigned int& height);

		/* Destructor */
		~Window();

		/* Functions */
		void update(); //should be called after all key/button input update
		void clear() const;

		void setClearColor(const Color& color) const;
		void setMousePosition(const vector2& pos);
		void setMouseToCenter();

		void hideCursor();
		void showCursor();

		void close();

		bool closed() const;

		// Getters
		float getAspectRatio() const;

		inline int     getWidth()  const { return m_width;  }
		inline int     getHeight() const { return m_height; }
		inline vector2 getSize()   const { return vector2((float)m_width, (float)m_height); }

		bool isKeyPressed(const unsigned int key) const;
		bool isButtonPressed(const unsigned int button) const;
		bool isKeyClicked(const unsigned int key);
		bool isButtonClicked(const unsigned int button);
		vector2 getMousePosition() const;

		bool isOnTop() const;

	private:
		// Private Functions
		bool m_init();

		/* Additional Functions */
		friend static void windowResize(GLFWwindow* window, const int width, const int height);

		friend static void keyCallback(GLFWwindow* window, const int key, const int scancode, const int action, const int mods);
		friend static void buttonCallback(GLFWwindow* window, const int button, const int action, const int mods);
		friend static void mousePositionCallback(GLFWwindow* window, const double xPos, const double yPos);

	};

}



#endif