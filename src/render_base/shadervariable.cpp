#include <utility>

#include "render_base/shadervariable.hpp"
#include "render_base/window.hpp"
#include "render_base/shader.hpp"

using namespace Math3D;
using namespace Render3D;

template <>
void ShaderVariable<int>::setValue(const Window& win, const int& val) {
    GLuint clusterID = win.getClusterID();
	for (unsigned int i = 0; i < variables.size(); ++i) {
		if (variables[i].first == clusterID) {
			if (val != variables[i].second.second) {
				glUniform1i(variables[i].second.first, val);
				variables[i].second.second = val;
			}
			return;
		}
	}

	VariablePair variable(glGetUniformLocation(shader->getProgramID(clusterID), name.c_str()), val);
	glUniform1i(variable.first, val);
	variables.push_back(std::pair<GLuint, VariablePair>(clusterID, std::move(variable)));
}

template <>
void ShaderVariable<float>::setValue(const Window& win, const float& val) {
    GLuint clusterID = win.getClusterID();
	for (unsigned int i = 0; i < variables.size(); ++i) {
		if (variables[i].first == clusterID) {
			if (val != variables[i].second.second) {
				glUniform1f(variables[i].second.first, val);
				variables[i].second.second = val;
			}
			return;
		}
	}

	VariablePair variable(glGetUniformLocation(shader->getProgramID(clusterID), name.c_str()), val);
	glUniform1f(variable.first, val);
	variables.push_back(std::pair<GLuint, VariablePair>(clusterID, std::move(variable)));
}

template <>
void ShaderVariable<Matrix4x4>::setValue(const Window& win, const Matrix4x4& val) {
	GLuint clusterID = win.getClusterID();
	for (unsigned int i = 0; i < variables.size(); ++i) {
		if (variables[i].first == clusterID) {
			if (val != variables[i].second.second) {
				glUniformMatrix4fv(variables[i].second.first, 1, GL_TRUE, val.getValues());
				variables[i].second.second = val;
			}
			return;
		}
	}

	VariablePair variable(glGetUniformLocation(shader->getProgramID(clusterID), name.c_str()), val);
	glUniformMatrix4fv(variable.first, 1, GL_TRUE, val.getValues());
	variables.push_back(std::pair<GLuint, VariablePair>(clusterID, std::move(variable)));
}

template <>
void ShaderVariable<Vector4>::setValue(const Window& win, const Vector4& val) {
	GLuint clusterID = win.getClusterID();
	for (unsigned int i = 0; i < variables.size(); ++i) {
		if (variables[i].first == clusterID) {
			if (val != variables[i].second.second) {
				glUniform4fv(variables[i].second.first, 1, val.getValues());
				variables[i].second.second = val;
			}
			return;
		}
	}

	VariablePair variable(glGetUniformLocation(shader->getProgramID(clusterID), name.c_str()), val);
	glUniform4fv(variable.first, 1, val.getValues());
	variables.push_back(std::pair<GLuint, VariablePair>(clusterID, std::move(variable)));
}

template <>
void ShaderVariable<Color>::setValue(const Window& win, const Color& val) {
	GLuint clusterID = win.getClusterID();
	for (unsigned int i = 0; i < variables.size(); ++i) {
		if (variables[i].first == clusterID) {
			if (val != variables[i].second.second) {
				glUniform4fv(variables[i].second.first, 1, val.getValues());
				variables[i].second.second = val;
			}
			return;
		}
	}

	VariablePair variable(glGetUniformLocation(shader->getProgramID(clusterID), name.c_str()), val);
	glUniform4fv(variable.first, 1, val.getValues());
	variables.push_back(std::pair<GLuint, VariablePair>(clusterID, std::move(variable)));
}
