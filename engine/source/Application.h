#pragma once

namespace eng {
    class Application {
        bool m_needsToBeClosed = false;
    public:
        virtual bool init() = 0;
        virtual void update(float deltaTime) = 0;   //deltaTime is in seconds
        virtual void destroy() = 0;

        void setNeedsToBeClosed(bool value);
        bool needsToBeClosed() const;
    };
}