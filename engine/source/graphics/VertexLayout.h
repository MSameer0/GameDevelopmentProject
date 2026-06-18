#pragma once
#include <vector>

namespace eng {
    struct VertexElement {
        GLuint index;   //attribue location
        GLuint size;    //number of components
        GLuint type;    //data type example GL_FLOAT
        uint32_t offset;    //bytes offset from the start of vertex
    };

    struct VertexLayout {
        std::vector<VertexElement> elements;
        uint32_t stride = 0;    //total size of a single vertex
    };
}