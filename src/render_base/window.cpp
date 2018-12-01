#include <iostream>

#include "render_base/window.hpp"
#include "render_base/context3d.hpp"

using namespace Render3D;

std::atomic<GLuint> Window::clusterCount(0);

Window::Window(int w, int h, const char* title, Window* parent) {
	SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	if (!window) {
		SDL_Quit();
		// throw exception
        std::cout << "Exception occurred with initializing window: " << SDL_GetError() << std::endl;
		return;
	}
    if (parent != NULL && !parent->destroyed && parent->context != NULL) {
        SDL_GL_MakeCurrent(parent->window, parent->glContext);
		SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
        parentWindow = parent;
        clusterID = parent->clusterID;
	} else {
		SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 0);
        parentWindow = NULL;
        clusterID = ++clusterCount;
	}
	glContext = SDL_GL_CreateContext(window);
    if (glContext == NULL) {
        SDL_Quit();
		// throw exception
        std::cout << "Exception occurred with initializing context: " << SDL_GetError() << std::endl;
		return;
    }

	SDL_GL_MakeCurrent(window, glContext);

	if (glewInit() != GLEW_OK) {
		SDL_Quit();
		// throw exception
        std::cout << "Exception occurred with initializing glew" << std::endl;
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
	fullscreenEnabled = false;
    mouseLockEnabled = false;
	mouseDownCallback = mouseUpCallback = NULL;
	mouseMoveCallback = NULL;
	windowResizeCallback = NULL;
    mouseX = mouseY = 0;

    SDL_AddEventWatch(eventWatcher, this);

    std::cout << "Window created and binds all set" << std::endl;

    context = new Context3D(this);
}

Window::~Window() {
	close();
}

void Window::activate() {
    //glfwMakeContextCurrent(window);
    //glViewport(0, 0, width, height);
    //glfwSwapInterval(vsyncEnabled);
    //curWindow = this;
}

void Window::makeCurrent(bool isCurrent) {
    if (isCurrent) {
        if (SDL_GL_GetCurrentContext() != glContext) {
            SDL_GL_MakeCurrent(window, glContext);
			SDL_GL_SetSwapInterval(vsyncEnabled);
        }
    } else {
        SDL_GL_MakeCurrent(window, NULL);
    }
}

void Window::updateViewport() {
    glViewport(0, 0, width, height);
}

void Window::close() {
	if (!destroyed) {
		destroyed = true;
		SDL_GL_DeleteContext(glContext);
		SDL_DestroyWindow(window);
        delete context;
        context = NULL;
	}
}

bool Window::isActive() const {
	return active;
}

void Window::pollEvents() {
    SDL_PumpEvents();
}

void Window::waitEvents() {
	SDL_Event event;
	SDL_WaitEvent(&event);
    SDL_PumpEvents();
}

void Window::clear() {
	//makeCurrent();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::drawTriangle(GLfloat vertices[]) {
	
}

int Window::getWidth() const {
	return width;
}

int Window::getHeight() const {
	return height;
}

void Window::getSize(int& w, int& h) const {
    w = width;
    h = height;
}

void Window::setWidth(int w) {
	width = w;
    SDL_SetWindowSize(window, width, height);
}

void Window::setHeight(int h) {
	height = h;
    SDL_SetWindowSize(window, width, height);
}

void Window::setSize(int w, int h) {
    width = w;
    height = h;
    SDL_SetWindowSize(window, width, height);
}

void Window::applyResize(int w, int h) {
    width = w;
    height = h;
}

void Window::update() {
	//makeCurrent();
	SDL_GL_SwapWindow(window);
}

void Window::setVSyncEnabled(bool enabled) {
	vsyncEnabled = enabled;
	SDL_GL_SetSwapInterval(enabled);
}

void Window::setFullscreenEnabled(bool enabled) {
    if (enabled && !fullscreenEnabled) {
        windowedWidth = width;
        windowedHeight = height;

        SDL_DisplayMode mode;
        SDL_GetCurrentDisplayMode(SDL_GetWindowDisplayIndex(window), &mode);
        setSize(mode.w, mode.h);
    }
    SDL_SetWindowFullscreen(window, (enabled ? SDL_WINDOW_FULLSCREEN : 0));
    if (!enabled && fullscreenEnabled) {
        setSize(windowedWidth, windowedHeight);
    }
    fullscreenEnabled = enabled;
}

void Window::setMouseLockEnabled(bool enabled) {
	mouseLockEnabled = enabled;
	SDL_SetRelativeMouseMode((enabled ? SDL_TRUE : SDL_FALSE));
    if (!enabled) {
        SDL_WarpMouseInWindow(window, mouseX, mouseY);
    }
}

bool Window::isVSyncEnabled() const {
	return vsyncEnabled;
}

bool Window::isFullscreenEnabled() const {
	return fullscreenEnabled;
}

bool Window::isMouseLockEnabled() const {
	return mouseLockEnabled;
}

void Window::toggleFullscreen() {
    setFullscreenEnabled(!fullscreenEnabled);
}

int Window::eventWatcher(void* data, SDL_Event* event) {
    Window* window = static_cast<Window*>(data);
    if (window == NULL) {
        return 0;
    }
    if (event->type == SDL_QUIT) {
        window->active = false;
    } else if (event->type == SDL_MOUSEBUTTONDOWN) {
        if (event->button.windowID == SDL_GetWindowID(window->window)) {
            window->mouseState[event->button.button] = true;
            if (!window->mouseLockEnabled) {
                window->mouseX = event->button.x;
                window->mouseY = event->button.y;
            }
            if (window->mouseDownCallback != NULL) {
                window->mouseDownCallback(static_cast<MOUSE_BUTTON>(event->button.button), event->button.x, event->button.y);
            }
        }
    } else if (event->type == SDL_MOUSEBUTTONUP) {
        if (event->button.windowID == SDL_GetWindowID(window->window)) {
            window->mouseState[event->button.button] = false;
            if (!window->mouseLockEnabled) {
                window->mouseX = event->button.x;
                window->mouseY = event->button.y;
            }
            if (window->mouseUpCallback != NULL) {
                window->mouseUpCallback(static_cast<MOUSE_BUTTON>(event->button.button), event->button.x, event->button.y);
            }
        }
    } else if (event->type == SDL_MOUSEMOTION) {
        if (event->button.windowID == SDL_GetWindowID(window->window)) {
            int tempX = window->mouseX;
            int tempY = window->mouseY;
            if (!window->mouseLockEnabled) {
                window->mouseX = event->motion.x;
                window->mouseY = event->motion.y;
            }
            if ((window->mouseMoveCallback != NULL) &&
                (window->mouseLockEnabled || event->motion.x != tempX || event->motion.y != tempY)) {
                window->mouseMoveCallback(tempX, tempY, event->motion.xrel, event->motion.yrel);
            }
        }
    } else if (event->type == SDL_KEYDOWN) {
        if (event->key.windowID == SDL_GetWindowID(window->window)) {
            window->keyboardState[event->key.keysym.scancode] = true;
            if (window->keyDownCallback != NULL) {
                window->keyDownCallback(static_cast<KEYCODE>(event->key.keysym.scancode));
            }
        }
    } else if (event->type == SDL_KEYUP) {
        if (event->key.windowID == SDL_GetWindowID(window->window)) {
            window->keyboardState[event->key.keysym.scancode] = false;
            if (window->keyUpCallback != NULL) {
                window->keyUpCallback(static_cast<KEYCODE>(event->key.keysym.scancode));
            }
        }
    } else if (event->type == SDL_WINDOWEVENT) {
        if (event->window.windowID == SDL_GetWindowID(window->window)) {
            switch (event->window.event) {
                case SDL_WINDOWEVENT_SIZE_CHANGED:
                    if (window->windowResizeCallback != NULL) {
                        window->windowResizeCallback(event->window.data1, event->window.data2);
                    } else {
						window->applyResize(event->window.data1, event->window.data2);
					}
                    break;
                case SDL_WINDOWEVENT_CLOSE:
                    window->active = false;
                default:
                    break;
            }
        }
    }
    return 0;
}

KEYCODE Window::getKeycodeFromName(const std::string& name) {
    return static_cast<KEYCODE>(SDL_GetScancodeFromName(name.c_str()));
}

std::string Window::getKeyNameFromCode(KEYCODE key) {
    return std::string(SDL_GetScancodeName(SDL_Scancode(static_cast<int>(key))));
}

bool Window::isKeyPressed(KEYCODE key) {
    std::unordered_map<int, bool>::iterator it = keyboardState.find(static_cast<int>(key));
    if (it != keyboardState.end()) {
        return it->second;
    } else {
        return false;
    }
}

bool Window::isMouseDown(MOUSE_BUTTON button) {
	std::unordered_map<int, bool>::iterator it = mouseState.find(static_cast<int>(button));
    if (it != mouseState.end()) {
        return it->second;
    } else {
        return false;
    }
}

void Window::getMousePosition(double& x, double& y) {
	//glfwGetCursorPos(window, &x, &y);
}

void Window::setMouseDownCallback(const MouseButtonCallback& callback) {
	mouseDownCallback = callback;
}

void Window::setMouseUpCallback(const MouseButtonCallback& callback) {
	mouseUpCallback = callback;
}

void Window::setKeyDownCallback(const KeyCallback& callback) {
    keyDownCallback = callback;
}

void Window::setKeyUpCallback(const KeyCallback& callback) {
    keyUpCallback = callback;
}

void Window::setMouseMoveCallback(const MouseMoveCallback& callback) {
	mouseMoveCallback = callback;
}

void Window::setResizeCallback(const WindowResizeCallback& callback) {
	windowResizeCallback = callback;
}

bool Window::isChild() const {
    return parentWindow != NULL;
}

GLuint Window::getClusterID() const {
    return clusterID;
}

Window* Window::getParent() const {
    return parentWindow;
}

Context3D* Window::getContext() const {
    return context;
}
