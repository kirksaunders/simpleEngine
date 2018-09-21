#ifndef WINDOW__HPP
#define WINDOW__HPP

#define GLEW_STATIC

#include <functional>

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include "render_base/color.hpp"

namespace Render3D {
	class Window {
	 public:
		typedef std::function<void(int)> MouseButtonCallback;
		typedef std::function<void(double, double)> MouseMoveCallback;
		Window(int width, int height, const char* title, Window* parent = NULL);

		~Window();

		void activate();

		void close();

		bool isActive() const;

		void pollEvents();

		void clear();

		int getWidth() const;

		int getHeight() const;

		void drawTriangle(GLfloat[]);

		void update();

		void destroy();

		void setVSyncEnabled(bool enabled);

		void setMouseLockEnabled(bool enabled);

		bool isVSyncEnabled() const;

		bool isMouseLockEnabled() const;

		static Window* getCurrent();

		bool isKeyPressed(int key);
		bool isMouseDown(int button);

		void getMousePosition(double& x, double& y);

		void setMouseDownCallback(MouseButtonCallback callback);
		void setMouseUpCallback(MouseButtonCallback callback);
		void setMouseMoveCallback(MouseMoveCallback callback);

	 private:
		GLFWwindow *window;
		bool active;
		bool destroyed;
		int width;
		int height;
		bool vsyncEnabled;
		bool mouseLockEnabled;
		MouseButtonCallback mouseDownCallback;
		MouseButtonCallback mouseUpCallback;
		MouseMoveCallback mouseMoveCallback;

		static Window* curWindow;
		static Window* firstWindow;

		static void closeCallback(GLFWwindow* window);
		//static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void resizeCallback(GLFWwindow* glfwWin, int width, int height);
		static void mouseButtonCallback(GLFWwindow* glfwWin, int button, int action, int mods);
		static void cursorPositionCallback(GLFWwindow* glfwWin, double x, double y);
	};
}

#endif