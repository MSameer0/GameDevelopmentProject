#include "RenderQueue.h"
#include "graphics/GraphicsAPI.h"
#include "render/Mesh.h"
#include "render/Material.h"

namespace eng {
    void RenderQueue::submit(const RenderCommand &command) {
        m_commands.push_back(command);
    }

    void RenderQueue::draw(GraphicsAPI &graphicsAPI) {
        for (auto& command : m_commands) {
            graphicsAPI.bindMaterial(command.material);
            graphicsAPI.bindMesh(command.mesh);

            graphicsAPI.drawMesh(command.mesh);
        }

        m_commands.clear();
    }
}
