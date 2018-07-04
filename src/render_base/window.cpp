#include "render_base/window.hpp"

using namespace Render3D;

Window* Window::curWindow = NULL;
Window* Window::firstWindow = NULL;

Window::Window(int w, int h, const char* title, Window* parent) {
	if (!glfwInit()) {
		// throw exception
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_SAMPLES, 4);

	if (parent != NULL) {
		window = glfwCreateWindow(w, h, title, NULL, parent->window);
	} else if (firstWindow != NULL) {
		window = glfwCreateWindow(w, h, title, NULL, firstWindow->window);
	} else {
		window = glfwCreateWindow(w, h, title, NULL, NULL);
		firstWindow = this;
	}
	if (!window) {
		glfwTerminate();
		// throw exception
	}

	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) {
		glfwTerminate();
		// throw exception
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	width = w;
	height = h;
	active = true;
	destroyed = false;
	vsyncEnabled = false;
	mouseDownCallback = mouseUpCallback = NULL;

	glfwSetWindowUserPointer(window, this);
	glfwSetWindowCloseCallback(window, closeCallback);
	//glfwSetKeyCallback(window, keyCallback);
	glfwSetWindowSizeCallback(window, resizeCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetCursorPosCallback(window, cursorPositionCallback);

	if (curWindow != NULL) {
		curWindow->activate();
	} else {
		curWindow = this;
	}
}

Window::~Window() {
	close();
}

void Window::activate() {
	glfwMakeContextCurrent(window);
	glViewport(0, 0, width, height);
	glfwSwapInterval(vsyncEnabled);
	curWindow = this;
}

void Window::close() {
	if (!destroyed) {
		destroyed = true;
		glfwDestroyWindow(window);
	}
}

bool Window::isActive() const {
	return active;
}

void Window::pollEvents() {
	glfwPollEvents();
}

void Window::clear() {
	if (curWindow != this) {
		activate();
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::drawTriangle(GLfloat vertices[]) {
	
}

/*void Window::drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, const Color color) {
	glColor4f(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
	glBegin(GL_TRIANGLES);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glVertex2f(x3, y3);
	glEnd();
}*/

int Window::getWidth() const {
	return width;
}

int Window::getHeight() const {
	return height;
}

void Window::update() {
	if (curWindow != this) {
		activate();
	}
	glfwSwapBuffers(window);
}

void Window::setVSyncEnabled(bool enabled) {
	vsyncEnabled = enabled;
	glfwSwapInterval(enabled);
}

void Window::setMouseLockEnabled(bool enabled) {
	mouseLockEnabled = enabled;
	glfwSetInputMode(window, GLFW_CURSOR, (enabled ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL));
}

bool Window::isVSyncEnabled() const {
	return vsyncEnabled;
}

bool Window::isMouseLockEnabled() const {
	return mouseLockEnabled;
}

void Window::closeCallback(GLFWwindow* glfwWin) {
	Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWin));
	window->active = false;
}

/*void Window::keyCallback(GLFWwindow* glfwWin, int key, int scancode, int action, int mods) {
	Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWin));
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		window->active = false;
	}
}*/

void Window::mouseButtonCallback(GLFWwindow* glfwWin, int button, int action, int mods) {
	Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWin));
	if (action == GLFW_PRESS) {
		if (window->mouseDownCallback != NULL) {
			window->mouseDownCallback(button);
		}
	} else if (action == GLFW_RELEASE) {
		if (window->mouseUpCallback != NULL) {
			window->mouseUpCallback(button);
		}
	}
}

void Window::resizeCallback(GLFWwindow* glfwWin, int width, int height) {
	Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWin));
	window->width = width;
	window->height = height;
	if (curWindow == window) {
		glViewport(0, 0, width, height);
	}
}

void Window::cursorPositionCallback(GLFWwindow* glfwWin, double x, double y) {
	Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWin));
	if (window->mouseMoveCallback != NULL) {
		window->mouseMoveCallback(x, y);
	}
}

Window* Window::getCurrent() {
	return curWindow;
}

bool Window::isKeyPressed(int key) {
	int state = glfwGetKey(window, key);
	if (state == GLFW_PRESS) {
		return true;
	} else {
		return false;
	}
}

bool Window::isMouseDown(int button) {
	int state = glfwGetMouseButton(window, button);
	if (state == GLFW_PRESS) {
		return true;
	} else {
		return false;
	}
}

void Window::getMousePosition(double& x, double& y) {
	glfwGetCursorPos(window, &x, &y);
}

void Window::setMouseDownCallback(MouseButtonCallback callback) {
	mouseDownCallback = callback;
}

void Window::setMouseUpCallback(MouseButtonCallback callback) {
	mouseUpCallback = callback;
}

void Window::setMouseMoveCallback(MouseMoveCallback callback) {
	mouseMoveCallback = callback;
}
