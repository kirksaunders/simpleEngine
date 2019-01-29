#include "render_base/window.hpp"

#include <algorithm>
#include <iostream>

#include "render_base/context3d.hpp"
#include "render_base/exception.hpp"

using namespace Render3D;

// static initializations
std::atomic<GLuint> Window::clusterCount(0);
thread_local const Window* Window::currentWindow = nullptr;

Window::Window(int w, int h, const char* title, Window* parent) {
    SDL_Init(SDL_INIT_VIDEO);

    //SDL_SetHint(SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS, "0");

    // I've noticed that OGL 3.1 performs better and with less bugs than 3.2+ on my high-end rig, so 3.1 it is for now
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); // default is 16
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); // force core profile

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    if (!window) {
        SDL_Quit();
        throw Exception(std::string("Exception occurred with initializing window: ") + SDL_GetError());
    }
    if (parent != nullptr && !parent->destroyed && parent->context != nullptr) {
        SDL_GL_MakeCurrent(parent->window, parent->glContext);
        SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
        parentWindow = parent;
        clusterID = parent->clusterID;
    } else {
        SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 0);
        parentWindow = nullptr;
        clusterID = ++clusterCount;
    }
    glContext = SDL_GL_CreateContext(window);
    if (glContext == nullptr) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        throw Exception(std::string("Exception occurred with initializing opengl context: ") + SDL_GetError());
    }

    SDL_GL_MakeCurrent(window, glContext);

    GLenum glewError = glewInit();
    if (glewError != GLEW_OK) {
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
        throw Exception(std::string("Exception occurred with initializing glew: ") + reinterpret_cast<const char*>(glewGetErrorString(glewError)));
    }

    std::cout << "Using OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    width = w;
    height = h;
    active = true;
    destroyed = false;
    vsyncEnabled = false;
    fullscreenEnabled = false;
    mouseLockEnabled = false;
    depthTestEnabled = true;
    mouseDownCallback = mouseUpCallback = nullptr;
    mouseMoveCallback = nullptr;
    windowResizeCallback = nullptr;
    mouseX = mouseY = 0;
    currentThread = std::this_thread::get_id();
    currentWindow = this;

    SDL_AddEventWatch(eventWatcher, this);

    std::cout << "Window created and binds all set" << std::endl;

    context = new Context3D(this);
}

Window::~Window() {
    close();
}

void Window::makeCurrent(bool isCurrent) {
    if (isCurrent) {
        if (currentWindow != this) {
            if (currentThread == std::thread::id() || currentThread == std::this_thread::get_id()) {
                if (SDL_GL_MakeCurrent(window, glContext) != 0) {
                    throw Exception(std::string("Exception occurred with making window current: ") + SDL_GetError());
                }
                if (SDL_GL_SetSwapInterval(vsyncEnabled) != 0) {
                    throw Exception(std::string("Error setting window swap interval: ") + SDL_GetError());
                }
                currentWindow = this;
                currentThread = std::this_thread::get_id();
            } else {
                throw Exception("Attempted to activate window in thread while it was active in another");
            }
        }
    } else {
        if (currentWindow == this) {
            if (currentThread == std::this_thread::get_id()) {
                if (SDL_GL_MakeCurrent(window, nullptr) != 0) {
                    throw Exception(std::string("Exception occurred with making window not current: ") + SDL_GetError());
                }
                currentWindow = nullptr;
                currentThread = std::thread::id(); // set thread to default (basically null)
            } else {
                throw Exception("Attempted to make window inactive in thread that it wasn't active in");
            }
        } else {
            throw Exception("Attempted to make window inactive when it wasn't active");
        }
    }
}

void Window::updateViewport() {
    glViewport(0, 0, width, height);
}

void Window::close() {
    if (!destroyed) {
        destroyed = true;
        context->clearObjects();
        context->clearShaders();
        context->clearTextures();
        delete context;
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(window);
        context = nullptr;
        if (currentWindow == this) {
            currentWindow = nullptr;
        }
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

float Window::getAspectRatio() const {
    return static_cast<float>(width) / height;
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

void Window::applyResize() {
    width = pendingWidth;
    height = pendingHeight;
}

void Window::update() {
    SDL_GL_SwapWindow(window);
}

void Window::setVSyncEnabled(bool enabled) {
    vsyncEnabled = enabled;
    if (SDL_GL_SetSwapInterval(vsyncEnabled) != 0) {
        throw Exception(std::string("Error setting window swap interval: ") + SDL_GetError());
    }
}

void Window::setFullscreenEnabled(bool enabled) {
    if (enabled != fullscreenEnabled) {
        if (enabled) {
            windowedWidth = width;
            windowedHeight = height;

            SDL_DisplayMode mode;
            SDL_GetCurrentDisplayMode(SDL_GetWindowDisplayIndex(window), &mode);

            SDL_DisplayMode disp;
            SDL_GetWindowDisplayMode(window, &disp);
            disp.w = mode.w;
            disp.h = mode.h;
            SDL_SetWindowDisplayMode(window, &disp);

            SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
        } else {
            SDL_SetWindowFullscreen(window, 0);
            setSize(windowedWidth, windowedHeight);
        }
        fullscreenEnabled = enabled;
    }
}

void Window::setMouseLockEnabled(bool enabled) {
    if (enabled != mouseLockEnabled) {
        mouseLockEnabled = enabled;
        SDL_SetRelativeMouseMode((enabled ? SDL_TRUE : SDL_FALSE));
        if (!enabled) {
            SDL_WarpMouseInWindow(window, mouseX, mouseY);
        }
    }
}

void Window::setDepthTestEnabled(bool enabled) {
    if (enabled != depthTestEnabled) {
        depthTestEnabled = enabled;
        if (enabled) {
            glEnable(GL_DEPTH_TEST);
        } else {
            glDisable(GL_DEPTH_TEST);
        }
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

bool Window::isDepthTestEnabled() const {
    return depthTestEnabled;
}

void Window::toggleFullscreen() {
    setFullscreenEnabled(!fullscreenEnabled);
}

int Window::eventWatcher(void* data, SDL_Event* event) {
    Window* window = static_cast<Window*>(data);
    if (window == nullptr) {
        return 0;
    }
    if (event->type == SDL_QUIT) {
        window->active = false;
    } else if (event->type == SDL_MOUSEBUTTONDOWN) {
        if (event->button.windowID == SDL_GetWindowID(window->window)) {
            for (unsigned int i = 0; i < window->mouseState.size(); ++i) {
                if (window->mouseState[i] == event->button.button) {
                    return 0;
                }
            }
            window->mouseState.push_back(event->button.button);
            if (!window->mouseLockEnabled) {
                window->mouseX = event->button.x;
                window->mouseY = event->button.y;
            }
            if (window->mouseDownCallback != nullptr) {
                window->mouseDownCallback(static_cast<MOUSE_BUTTON>(event->button.button), event->button.x, event->button.y);
            }
        }
    } else if (event->type == SDL_MOUSEBUTTONUP) {
        if (event->button.windowID == SDL_GetWindowID(window->window)) {
            for (unsigned int i = 0; i < window->mouseState.size(); ++i) {
                if (window->mouseState[i] == event->button.button) {
                    std::swap(window->mouseState[i], window->mouseState.back());
                    window->mouseState.pop_back();
                    break;
                }
            }
            if (!window->mouseLockEnabled) {
                window->mouseX = event->button.x;
                window->mouseY = event->button.y;
            }
            if (window->mouseUpCallback != nullptr) {
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
            if ((window->mouseMoveCallback != nullptr) &&
                (window->mouseLockEnabled || event->motion.x != tempX || event->motion.y != tempY)) {
                window->mouseMoveCallback(tempX, tempY, event->motion.xrel, event->motion.yrel);
            }
        }
    } else if (event->type == SDL_KEYDOWN) {
        if (event->key.windowID == SDL_GetWindowID(window->window)) {
            for (unsigned int i = 0; i < window->keyboardState.size(); ++i) {
                if (window->keyboardState[i] == event->key.keysym.scancode) {
                    return 0;
                }
            }
            window->keyboardState.push_back(event->key.keysym.scancode);
            if (window->keyDownCallback != nullptr) {
                window->keyDownCallback(static_cast<KEYCODE>(event->key.keysym.scancode));
            }
        }
    } else if (event->type == SDL_KEYUP) {
        if (event->key.windowID == SDL_GetWindowID(window->window)) {
            for (unsigned int i = 0; i < window->keyboardState.size(); ++i) {
                if (window->keyboardState[i] == event->key.keysym.scancode) {
                    std::swap(window->keyboardState[i], window->keyboardState.back());
                    window->keyboardState.pop_back();
                    break;
                }
            }
            if (window->keyUpCallback != nullptr) {
                window->keyUpCallback(static_cast<KEYCODE>(event->key.keysym.scancode));
            }
        }
    } else if (event->type == SDL_WINDOWEVENT) {
        if (event->window.windowID == SDL_GetWindowID(window->window)) {
            switch (event->window.event) {
                case SDL_WINDOWEVENT_SIZE_CHANGED:
                    window->pendingWidth = event->window.data1;
                    window->pendingHeight = event->window.data2;
                    if (window->windowResizeCallback != nullptr) {
                        window->windowResizeCallback(event->window.data1, event->window.data2);
                    } else {
                        window->applyResize();
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
    for (unsigned int i = 0; i < keyboardState.size(); ++i) {
        if (keyboardState[i] == static_cast<int>(key)) {
            return true;
        }
    }

    return false;
}

bool Window::isMouseDown(MOUSE_BUTTON button) {
    for (unsigned int i = 0; i < mouseState.size(); ++i) {
        if (mouseState[i] == static_cast<int>(button)) {
            return true;
        }
    }

    return false;
}

void Window::getMousePosition(double& x, double& y) {
    x = mouseX;
    y = mouseY;
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
    return parentWindow != nullptr;
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

bool Window::isShaderActive(const Shader& shader) const {
    if (currentWindow != this) {
        return false;
    }

    return (activeShader == &shader);
}

void Window::setShaderActive(const Shader& shader, bool active) {
    if (currentWindow != this) {
        throw Exception("Unable to make shader active, window is not active in calling thread");
    }

    if (active) {
        activeShader = &shader;
    } else {
        if (activeShader != &shader) {
            throw Exception("Tried to deactivate shader that wasn't active");
        }
        activeShader = NULL;
    }
}
