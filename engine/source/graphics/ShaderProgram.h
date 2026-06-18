#pragma once
#include <string>
#include <unordered_map>

#include "GL/glew.h"

namespace eng {
    class ShaderProgram {
        GLuint m_shaderProgramID = 0;
        std::unordered_map<std::string, GLint> m_uniformLocationCache;
    public:
        explicit ShaderProgram(GLuint shaderProgramID);
        ShaderProgram() = delete;
        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram& operator=(const ShaderProgram&) = delete;
        ~ShaderProgram();

        void bind();
        GLint getUniformLocation(const std::string& name);
        void setUniform(const std::string& name, float value);
        void setUniform(const std::string& name, float v0, float v1);
    };
}
