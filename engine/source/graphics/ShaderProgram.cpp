#include "ShaderProgram.h"

#include "GL/glew.h"

namespace eng {
    ShaderProgram::ShaderProgram(GLuint shaderProgramID) : m_shaderProgramID(shaderProgramID) {
    }

    ShaderProgram::~ShaderProgram() {
        glDeleteProgram(m_shaderProgramID);
    }

    void ShaderProgram::bind() {
        glUseProgram(m_shaderProgramID);
    }

    GLint ShaderProgram::getUniformLocation(const std::string& name) {
        auto it = m_uniformLocationCache.find(name);
        if (it != m_uniformLocationCache.end()) {
            return it->second;
        }

        GLint location = glGetUniformLocation(m_shaderProgramID, name.c_str());
        m_uniformLocationCache[name] = location;
        return location;
    }

    void ShaderProgram::setUniform(const std::string &name, float value) {
        auto location = getUniformLocation(name);
        glUniform1f(location, value);
    }
}
