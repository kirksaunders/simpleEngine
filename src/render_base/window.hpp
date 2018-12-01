#ifndef WINDOW__HPP
#define WINDOW__HPP

#define GLEW_STATIC

#include <functional>
#include <atomic>
#include <unordered_map>

#include <GLEW/glew.h>
#include <SDL2/sdl.h>

#include "keycodes.hpp"

namespace Render3D {
    class Context3D; // forward declaration

    enum class MOUSE_BUTTON {
        LEFT = 1,
        MIDDLE,
        RIGHT,
        X1,
        X2
    };

	class Window {
        friend Context3D; // so that Context3D can set/get the value of the "context" member

	 public:
		typedef std::function<void(MOUSE_BUTTON, int, int)> MouseButtonCallback;
		typedef std::function<void(KEYCODE)> KeyCallback;
		typedef std::function<void(int, int, int, int)> MouseMoveCallback;
		typedef std::function<void(int, int)> WindowResizeCallback;
		Window(int width, int height, const char* title, Window* parent = NULL);

		~Window();

		void activate();
		void makeCurrent(bool isCurrent = true);
		void updateViewport();

		void close();

		bool isActive() const;

		static void pollEvents();
		static void waitEvents();

		void clear();

		int getWidth() const;
		int getHeight() const;
        void getSize(int& w, int& h) const;

		void setWidth(int w);
		void setHeight(int h);
        void setSize(int w, int h);
        void applyResize(int w, int h);

		void drawTriangle(GLfloat[]);

		void update();

		void destroy();

		void setVSyncEnabled(bool enabled);
        void setFullscreenEnabled(bool enabled);
		void setMouseLockEnabled(bool enabled);

		bool isVSyncEnabled() const;
		bool isFullscreenEnabled() const;
		bool isMouseLockEnabled() const;

        void toggleFullscreen();

        static KEYCODE getKeycodeFromName(const std::string& name);
        static std::string getKeyNameFromCode(KEYCODE key);
		bool isKeyPressed(KEYCODE key);
		bool isMouseDown(MOUSE_BUTTON button);

		void getMousePosition(double& x, double& y);

		void setMouseDownCallback(const MouseButtonCallback& callback);
		void setMouseUpCallback(const MouseButtonCallback& callback);
		void setMouseMoveCallback(const MouseMoveCallback& callback);
        void setKeyDownCallback(const KeyCallback& callback);
        void setKeyUpCallback(const KeyCallback& callback);
		void setResizeCallback(const WindowResizeCallback& callback);

        bool isChild() const;

        GLuint getClusterID() const;
        Window* getParent() const;
        Context3D* getContext() const;

	 private:
		SDL_Window* window;
		SDL_GLContext glContext;
        static std::atomic<GLuint> clusterCount;
        Window* parentWindow;
        Context3D* context;
        GLuint clusterID;
		bool active;
		bool destroyed;
		int width;
		int height;
        int windowedWidth;
        int windowedHeight;
		bool vsyncEnabled;
		bool fullscreenEnabled;
		bool mouseLockEnabled;
		MouseButtonCallback mouseDownCallback;
		MouseButtonCallback mouseUpCallback;
		MouseMoveCallback mouseMoveCallback;
        KeyCallback keyDownCallback;
        KeyCallback keyUpCallback;
        WindowResizeCallback windowResizeCallback;

        std::unordered_map<int, bool> mouseState;
        std::unordered_map<int, bool> keyboardState;

        int mouseX;
        int mouseY;

        static int eventWatcher(void* data, SDL_Event* event);

		/*static void closeCallback(GLFWwindow* window);
		//static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void resizeCallback(GLFWwindow* glfwWin, int width, int height);
		static void mouseButtonCallback(GLFWwindow* glfwWin, int button, int action, int mods);
		static void cursorPositionCallback(GLFWwindow* glfwWin, double x, double y);*/
	};
}

#endif