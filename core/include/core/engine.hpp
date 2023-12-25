#pragma once

#include "input.hpp"

namespace Tektite
{
    class Engine
    {
      public:
        Engine();
        ~Engine();

        void update();
        void render();

        Input *getInput() const { return m_input.get(); }
        bool isRunning() const { return m_running; }

      private:
        std::unique_ptr<Input> m_input;
        bool m_running;
    };
} // namespace Tektite
