#include "render_base/context3d.hpp"

#include <cstring>
#include <iostream>
#include <utility>

#include "objects/primitive3d.hpp"

#include "render_base/exception.hpp"
#include "render_base/shader.hpp"
#include "render_base/shadervariableblock.hpp"
#include "render_base/texture.hpp"
#include "render_base/texturebuffer.hpp"
#include "render_base/window.hpp"

using namespace Render3D;
using namespace Math3D;

Context3D::Context3D() {
    window = nullptr;
}

Context3D::Context3D(Window* win) {
    window = win;
    window->context = this;
    window->makeCurrent();

    if (window->isChild()) {
        std::cout << "Using existing manager of window " << window->parentWindow << " for child window " << win << std::endl;
        // use existing shader and texture managers
        textureManager = window->parentWindow->context->textureManager;
        std::cout << "Used manager successfully, use_count is " << textureManager.use_count() << std::endl;
    } else {
        std::cout << "Creating manager for window " << win << std::endl;
        // create shader and texture managers
        textureManager = std::make_shared<TextureManager>();
        std::cout << "Created manager successfully, use_count is " << textureManager.use_count() << std::endl;
    }

    textureManager->addWindow(*window);
    textureManager->getDefaultTexture().prepareContent(*window, *textureManager);

    testBlock = ShaderVariableBlock{
        GLSL_TYPE::VEC4,
        GLSL_TYPE::VEC4,
        GLSL_TYPE::VEC4,
        GLSL_TYPE::FLOAT
    };

    testBlock.prepareContent(*window);
    testBlock.use(*window, uniformBufferManager);
}

Context3D::~Context3D() {
    window->makeCurrent();
    testBlock.destroyContent(*window);
    if (textureManager.use_count() == 1) {
        textureManager->getDefaultTexture().destroyContent(*window, *textureManager);
    }
    textureManager->removeWindow(*window);
}

void Context3D::addObject(Primitive3D* object) {
    std::pair<std::set<Primitive3D*>::iterator, bool> p = objects.insert(object);
    if (p.second) {
        window->makeCurrent();
        object->prepareContent(*window, *textureManager);
    }
}

void Context3D::removeObject(Primitive3D* object) {
    unsigned int numRemoved = objects.erase(object);
    if (numRemoved > 0) {
        window->makeCurrent();
        object->destroyContent(*window, *textureManager);
    }
}

void Context3D::clearObjects() {
    window->makeCurrent();
    for (std::set<Primitive3D*>::iterator it = objects.begin(); it != objects.end(); it++) {
        (*it)->destroyContent(*window, *textureManager);
    }
    objects.clear();
}

void Context3D::addShader(Shader* shader) {
    std::pair<std::set<Shader*>::iterator, bool> p = shaders.insert(shader);
    if (p.second) {
        window->makeCurrent();
        shader->prepareContent(*window);
        shader->bindVariableBlock(*window, uniformBufferManager, "TestBlock", testBlock);
    }
}

void Context3D::removeShader(Shader* shader) {
    unsigned int numRemoved = shaders.erase(shader);
    if (numRemoved > 0) {
        window->makeCurrent();
        shader->destroyContent(*window);
    }
}

void Context3D::clearShaders() {
    window->makeCurrent();
    for (std::set<Shader*>::iterator it = shaders.begin(); it != shaders.end(); it++) {
        (*it)->destroyContent(*window);
    }
    shaders.clear();
}

void Context3D::addTexture(Texture* texture) {
    std::pair<std::set<Texture*>::iterator, bool> p = textures.insert(texture);
    if (p.second) {
        window->makeCurrent();
        texture->prepareContent(*window, *textureManager);
    }
}

void Context3D::removeTexture(Texture* texture) {
    unsigned int numRemoved = textures.erase(texture);
    if (numRemoved > 0) {
        window->makeCurrent();
        texture->destroyContent(*window, *textureManager);
    }
}

void Context3D::clearTextures() {
    window->makeCurrent();
    for (std::set<Texture*>::iterator it = textures.begin(); it != textures.end(); it++) {
        (*it)->destroyContent(*window, *textureManager);
    }
    textures.clear();
}

Camera* const Context3D::getCamera() {
    return &camera;
}

TextureManager* const Context3D::getTextureManager() {
    return textureManager.get();
}

void Context3D::render() {
    window->makeCurrent();

    Matrix4x4 cameraCFrame = camera.getCFrame();
    Vector4 cameraPosition = cameraCFrame.position();
    Matrix4x4 cameraInverse = cameraCFrame.inverse();
    Matrix4x4 projection = camera.getPerspective(window->getAspectRatio());

    Vector4 lightPosition = Vector4(40, 300, 150);
    Color lightColor = Color(1, 1, 1);
    float ambient = 0.15f;

    testBlock.getVariable<Vector4>(0)->setValue(lightPosition);
    testBlock.getVariable<Vector4>(1)->setValue(cameraPosition);
    testBlock.getVariable<Color>(2)->setValue(lightColor);
    testBlock.getVariable<float>(3)->setValue(ambient);
    testBlock.updateContent(*window);

    Shader* currentShader = nullptr;

    Texture& defaultTex = textureManager->getDefaultTexture();
 
    for (std::set<Primitive3D*>::iterator it = objects.begin(); it != objects.end(); ++it) {
        Primitive3D* object = *it;

        if (object->getShader() == nullptr) {
            throw Exception("Tried to render object with no shader set");
        }

        if (object->getShader() != currentShader) {
            currentShader = object->getShader();
            currentShader->use(*window);

            defaultTex.resetDiffAndSpec(*currentShader, *window, *textureManager); // reset textures

            // Scene Lighting Data
            //currentShader->getVariable<Vector4>("lightPos")->setValue(*window, lightPosition);
            /*currentShader->getVariable<Color>("lightColor")->setValue(*window, lightColor);
            currentShader->getVariable<float>("ambientAmount")->setValue(*window, ambient);

            // Camera Data
            //currentShader->getVariable<Vector4>("cameraPos")->setValue(*window, cameraPosition);*/
            currentShader->getVariable<Matrix4x4>("cameraInverse")->setValue(*window, cameraInverse);

            // Projection Matrix
            currentShader->getVariable<Matrix4x4>("projection")->setValue(*window, projection);
        }
        object->render(*window, *textureManager);
    }

    if (currentShader != nullptr) {
        currentShader->unuse(*window);
    }
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Context3D::renderTexture(Texture& tex) {
    window->makeCurrent();

    if (tex.getShader() == nullptr) {
        throw Exception("Tried to render texture with no shader set");
    }

    tex.getShader()->use(*window);

    bool depthTest = window->isDepthTestEnabled();
    window->setDepthTestEnabled(false);
    // Note: If tex.render throws, then depthTest will be left set to false.
    //       I need to fix this in the future.
    tex.render(*window, *textureManager);
    window->setDepthTestEnabled(depthTest);

    tex.getShader()->unuse(*window);
    
    glBindVertexArray(0);
}
