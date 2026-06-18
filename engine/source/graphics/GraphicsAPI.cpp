#include "GraphicsAPI.h"

#include <iostream>
#include <ostream>
#include <string>

#include "ShaderProgram.h"
#include "GL/glew.h"
#include "../render/Material.h"

namespace eng {
    std::shared_ptr<ShaderProgram> GraphicsAPI::createShaderProgram(const std::string &vertexSource,
        const std::string &fragmentSource) {
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        const char* vertexShaderCStr = vertexSource.c_str();
        glShaderSource(vertexShader, 1, &vertexShaderCStr, nullptr);
        glCompileShader(vertexShader);

        GLint success;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
            std::cerr << "ERROR:VERTEX_SHADER_COMPILATION_FAILED: " << infoLog << std::endl;

            return nullptr;
        }

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        const char* fragmentShaderCStr = fragmentSource.c_str();
        glShaderSource(fragmentShader, 1, &fragmentShaderCStr, nullptr);
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
            std::cerr << "ERROR:FRAGMENT_SHADER_COMPILATION_FAILED: " << infoLog << std::endl;

            return nullptr;
        }

        GLuint shaderProgramID = glCreateProgram();
        glAttachShader(shaderProgramID, vertexShader);
        glAttachShader(shaderProgramID, fragmentShader);
        glLinkProgram(shaderProgramID);

        glGetShaderiv(shaderProgramID, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetProgramInfoLog(shaderProgramID, 512, nullptr, infoLog);
            std::cerr << "ERROR:SHADER_PROGRAM_LINK_FAILED: " << infoLog << std::endl;

            return nullptr;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return std::make_shared<ShaderProgram>(shaderProgramID);
    }

    void GraphicsAPI::bindShaderProgram(ShaderProgram *shaderProgram) {
        if (shaderProgram) {
            shaderProgram->bind();
        }
    }

    void GraphicsAPI::bindMaterial(Material *material) {
        if (material) {
            material->bind();
        }
    }
}
