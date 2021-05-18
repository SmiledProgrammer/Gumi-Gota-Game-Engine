
/* INCLUDES */
// Self Include
#include "Window.h"

// Normal Includes
#include <stdlib.h>
#include <time.h>

// Header Includes
#include "ErrorHandling.h"

#include "../Log.h"



namespace gg
{

	/* Constructors */
	Window::Window(const char* title, const unsigned int& width, const unsigned int& height)
		: m_title(title), m_width(width), m_height(height)
	{
		if (!m_init())
			glfwTerminate();

		// Input Init
		for (int i = 0; i < WINDOW_MAX_KEYS; i++)
		{
			m_keysPressed[i] = false;
			m_keysClicked[i] = false;
		}

		for (int i = 0; i < WINDOW_MAX_BUTTONS; i++)
		{
			m_buttonsPressed[i] = false;
			m_buttonsClicked[i] = false;
		}
	}

	/* Destructor */
	Window::~Window()
	{
		glfwTerminate();
	}

	/* Functions */
	void Window::update()
	{
		// Swapping Buffers
		GLcall( glfwSwapBuffers(m_window) );

		// Event Handling
		GLcall( glfwPollEvents() );

		// Updating Clicked Keys and Buttons
		for (unsigned int i = 0; i < WINDOW_MAX_KEYS; i++)
		{
			///m_keysClicked[i] = false;
		}

		for (unsigned int i = 0; i < WINDOW_MAX_BUTTONS; i++)
		{
			m_buttonsClicked[i] = false;
		}
	}

	void Window::clear() const
	{
		GLcall( glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) );
	}


	void Window::setClearColor(const Color& color) const
	{
		GLcall( glClearColor(color.r, color.g, color.b, color.a) );
	}

	void Window::setMousePosition(const vector2& pos)
	{
		GLcall( glfwSetCursorPos(m_window, (double)pos.x, (double)pos.y) );
	}

	void Window::setMouseToCenter()
	{
		GLcall( glfwSetCursorPos(m_window, (double)(m_width / 2), (double)(m_height / 2)) );
	}


	void Window::hideCursor()
	{
		GLcall( glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN) );
	}

	void Window::showCursor()
	{
		GLcall( glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL) );
	}


	void Window::close()
	{
		GLcall( glfwSetWindowShouldClose(m_window, 1) );
	}


	bool Window::closed() const
	{
		return glfwWindowShouldClose(m_window) == 1;
	}

	// Getters
	float Window::getAspectRatio() const
	{
		return (float)m_width / (float)m_height;
	}


	bool Window::isKeyPressed(const unsigned int key) const
	{
		if (key >= WINDOW_MAX_KEYS)
			return false;

		return m_keysPressed[key];
	}

	bool Window::isButtonPressed(const unsigned int button) const
	{
		if (button >= WINDOW_MAX_BUTTONS)
			return false;

		return m_buttonsPressed[button];
	}

	bool Window::isKeyClicked(const unsigned int key)
	{
		if (key >= WINDOW_MAX_KEYS)
			return false;

		if (m_keysPressed[key])
		{
			if (!m_keysClicked[key])
			{
				m_keysClicked[key] = true;

				return m_keysClicked[key];
			}
		}
		else
		{
			m_keysClicked[key] = false;
		}

		return false;
	}

	bool Window::isButtonClicked(const unsigned int button)
	{
		if (button >= WINDOW_MAX_BUTTONS)
			return false;

		if (m_buttonsPressed[button])
		{
			if (!m_buttonsClicked[button])
			{
				m_buttonsClicked[button] = true;

				return m_buttonsClicked[button];
			}
		}
		else
		{
			m_buttonsClicked[button] = false;
		}

		return false;
	}

	vector2 Window::getMousePosition() const
	{
		return m_mousePos;
	}


	bool Window::isOnTop() const
	{
		if (m_window == glfwGetCurrentContext())
			return true;

		return false;
	}

	// Private Functions
	bool Window::m_init()
	{
		// C Stuff
		srand((unsigned int)time(NULL));

		// Initializing GLFW
		if (!glfwInit())
		{
			LOG("[Error] GLFW couldn't initialize!");

			return false;
		}

		// Creating Window
		m_window = glfwCreateWindow(m_width, m_height, m_title, NULL, NULL);

		// Checking for Window Opening Fail
		if (!m_window)
		{
			LOG("[Error] Failed to create GLFW window!");

			return false;
		}

		// Making Context Current
		glfwMakeContextCurrent(m_window);

		glfwSetWindowUserPointer(m_window, this);

		// Resize Handling
		glfwSetFramebufferSizeCallback(m_window, windowResize);

		// Input Handling
		glfwSetKeyCallback(m_window, keyCallback);
		glfwSetMouseButtonCallback(m_window, buttonCallback);
		glfwSetCursorPosCallback(m_window, mousePositionCallback);


		// Initializing GLEW
		if (glewInit() != GLEW_OK)
		{
			LOG("[Error] GLEW couldn't initialize!");

			return false;
		}

		// Version Setting
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// Optimization Stuff
		GLcall( glfwSwapInterval(0) ); //turning off v-sync

		// Enabling Antialiasing Stuff
		GLcall( glEnable(GL_MULTISAMPLE) );


		return true;
	}

	/* Additional Functions */
	void windowResize(GLFWwindow* window, const int width, const int height)
	{
		Window* win = (Window*)glfwGetWindowUserPointer(window); //getting the window class

		// Resizing Stuff
		GLcall( glViewport(0, 0, width, height) );

		// Updating Window Class Variables
		win->m_width = width;
		win->m_height = height;
	}


	void keyCallback(GLFWwindow* window, const int key, const int scancode, const int action, const int mods)
	{
		Window* win = (Window*)glfwGetWindowUserPointer(window); //getting the window class

		win->m_keysPressed[key] = (action != GLFW_RELEASE);
	}

	void buttonCallback(GLFWwindow* window, const int button, const int action, const int mods)
	{
		Window* win = (Window*)glfwGetWindowUserPointer(window); //getting the window class

		win->m_buttonsPressed[button] = (action != GLFW_RELEASE);
	}

	void mousePositionCallback(GLFWwindow* window, const double xPos, const double yPos)
	{
		Window* win = (Window*)glfwGetWindowUserPointer(window); //getting the window class

		win->m_mousePos = vector2((float)xPos, (float)yPos);
	}

}