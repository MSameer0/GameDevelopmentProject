#pragma once
#include <memory>
#include <unordered_map>
#include <string>

namespace eng {
    class ShaderProgram;
    class Material {
        std::shared_ptr<ShaderProgram> m_shaderProgram;
        std::unordered_map<std::string, float> m_floatParams;
        std::unordered_map<std::string, std::pair<float, float>> m_float2Params;
    public:
        void setShaderProgram(std::shared_ptr<ShaderProgram>& shaderProgram);
        void setParam(const std::string& name, float value);
        void setParam(const std::string& name, float v0, float v1);
        void bind();
    };
}