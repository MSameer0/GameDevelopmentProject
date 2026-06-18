#pragma once
#include <GL/glew.h>
#include "graphics/VertexLayout.h"

namespace eng {
    class Mesh {
        GLuint m_VBO = 0;
        GLuint m_EBO = 0;
        GLuint m_VAO = 0;

        VertexLayout m_vertexLayout;
        size_t m_vertexCount = 0;
        size_t m_indexCount = 0;
    public:
        Mesh(const VertexLayout& layout, const std::vector<float>& vertices, const std::vector<uint32_t>& indices);
        Mesh(const VertexLayout& layout, const std::vector<float>& vertices);
        Mesh(const Mesh&) = delete;
        Mesh& operator=(const Mesh&) = delete;

        void bind();
        void draw();
    };
}