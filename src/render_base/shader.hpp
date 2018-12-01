#ifndef SHADER_HPP
#define SHADER_HPP

#define GLEW_STATIC

#include <string>
#include <sstream>
#include <unordered_map>

#include <GLEW/glew.h>

#include "math/vector4.hpp"
#include "math/matrix4x4.hpp"

#include "render_base/color.hpp"

namespace Render3D {
    class Window; // forward declaration

    struct KeyPairHash {
        size_t operator()(const std::pair<std::string, GLuint>& key) const {
            std::stringstream sstream(key.first);
            sstream << key.second;
            return std::hash<std::string>()(sstream.str());
        }
    };

    struct KeyPairEqual {
        bool operator()(const std::pair<std::string, GLuint>& lhs, const std::pair<std::string, GLuint>& rhs) const {
            return lhs.first == rhs.first && lhs.second == rhs.second;
        }
    };

	class Shader {
	 public:
		Shader() {}

		Shader(const GLchar* vertexPath, const GLchar* fragmentPath);

        void prepareContent(Window* win);
		void use(Window* win);

		void setVariable(Window* win, const char *variableName, int number);
		void setVariable(Window* win, const char *variableName, float number);
		void setVariable(Window* win, const char *variableName, const Math3D::Matrix4x4& matrix);
		void setVariable(Window* win, const char *variableName, const Math3D::Vector4& vector);
		void setVariable(Window* win, const char *variableName, const Color& color);

	 private:
        std::string vertexSource;
        std::string fragmentSource;
        std::unordered_map<GLuint, GLuint> programIDs;

        typedef std::pair<std::string, GLuint> KeyPair;
        typedef std::unordered_map<KeyPair, std::pair<GLuint, int>, KeyPairHash, KeyPairEqual> IntegerMap;
        typedef std::unordered_map<KeyPair, std::pair<GLuint, float>, KeyPairHash, KeyPairEqual> FloatMap;
        typedef std::unordered_map<KeyPair, std::pair<GLuint, Math3D::Matrix4x4>, KeyPairHash, KeyPairEqual> MatrixMap;
        typedef std::unordered_map<KeyPair, std::pair<GLuint, Math3D::Vector4>, KeyPairHash, KeyPairEqual> VectorMap;
        typedef std::unordered_map<KeyPair, std::pair<GLuint, Color>, KeyPairHash, KeyPairEqual> ColorMap;
        IntegerMap integers;
        FloatMap floats;
        MatrixMap matrices;
        VectorMap vectors;
        ColorMap colors;

        void compileProgram(GLuint clusterID);
		GLuint getProgramID(GLuint clusterID);
	};
}

#endif