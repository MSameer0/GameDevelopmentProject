#include "Application.h"

namespace eng {
    void Application::setNeedsToBeClosed(bool value) {
        m_needsToBeClosed = value;
    }

    bool Application::needsToBeClosed() const {
        return m_needsToBeClosed;
    }
}
