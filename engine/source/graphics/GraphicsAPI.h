#pragma once
#include <memory>

namespace eng {
    class ShaderProgram;
    class GraphicsAPI {
    public:
        std::shared_ptr<ShaderProgram> createShaderProgram(const std::string& vertexSource,
            const std::string& fragmentSource);
        void bindShaderProgram(ShaderProgram* shaderProgram);
    };
}