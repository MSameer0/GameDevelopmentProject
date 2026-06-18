#include "Material.h"
#include "../graphics/ShaderProgram.h"

namespace eng {
    void Material::setShaderProgram(std::shared_ptr<ShaderProgram> &shaderProgram) {
        m_shaderProgram = shaderProgram;
    }

    void Material::setParam(const std::string &name, float value) {
        m_floatParams[name] = value;
    }

    void Material::bind() {
        if (!m_shaderProgram) {
            return;
        }

        m_shaderProgram->bind();

        for (const auto& param : m_floatParams) {
            m_shaderProgram->setUniform(param.first, param.second);
        }
    }
}
