#pragma once
#include <vector>

namespace eng {
    class Mesh;
    class Material;
    class GraphicsAPI;

    struct RenderCommand {
        Mesh* mesh = nullptr;
        Material* material = nullptr;
    };

    class RenderQueue {
        std::vector<RenderCommand> m_commands;
    public:
        void submit(const RenderCommand& command);
        void draw(GraphicsAPI& graphicsAPI);
    };
}