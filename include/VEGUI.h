#pragma once

#include "VEInclude.h"
#include "VESystem.h"
#include "VECS.h"
#include "VEFfmpegManager.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>
#include <libswscale/swscale.h>
}

#include <fstream>

namespace vve {

    class GUI : public System {

    public:
        GUI(std::string systemName, Engine& engine, std::string windowName);
        ~GUI();

    private:
        bool OnAnnounce(Message message);
        bool OnKeyDown(Message message);
        bool OnKeyUp(Message message);
        bool OnMouseButtonDown(Message message);
        bool OnMouseButtonUp(Message message);
        bool OnMouseMove(Message message);
        bool OnMouseWheel(Message message);
        bool OnFrameEnd(Message message);
        void GetCamera();

        FfmpegManager m_ffmpegManager;
        std::string m_windowName;
        bool m_mouseButtonDown = false;
        bool m_shiftPressed = false;
        int m_x = -1;
        int m_y = -1;
        vecs::Handle m_cameraHandle{};
        vecs::Handle m_cameraNodeHandle{};
        bool m_makeScreenshot{ false };
        bool m_enableStreaming{ true };
        bool m_makeScreenshotDepth{ false };
        int m_numScreenshot{ 1 };

    };

};  // namespace vve
