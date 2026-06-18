#pragma once
#include <memory>
#include <vector>

#include "GL/glew.h"

namespace eng {
    class ShaderProgram;
    class Material;
    class Mesh;
    class GraphicsAPI {
    public:
        std::shared_ptr<ShaderProgram> createShaderProgram(const std::string& vertexSource,
            const std::string& fragmentSource);
        GLuint createVertexBuffer(const std::vector<float>& vertices);
        GLuint createIndexBuffer(const std::vector<uint32_t>& indices);

        void bindShaderProgram(ShaderProgram* shaderProgram);
        void bindMaterial(Material* material);
        void bindMesh(Mesh* mesh);
        void drawMesh(Mesh* mesh);
    };
}
