#pragma once
#include <memory>
#include <unordered_map>
#include <string>

namespace eng {
    class ShaderProgram;
    class Material {
        std::shared_ptr<ShaderProgram> m_shaderProgram;
        std::unordered_map<std::string, float> m_floatParams;
    public:
        void setShaderProgram(std::shared_ptr<ShaderProgram>& shaderProgram);
        void setParam(const std::string& name, float value);
        void bind();
    };
}