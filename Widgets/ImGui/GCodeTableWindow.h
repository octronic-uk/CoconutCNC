#pragma once

#include "ImGuiWidget.h"

#define MDI_BUFFER_SIZE 128

namespace Coconut
{
    class GCodeTableWindow : public ImGuiWidget
    {
    public:
        GCodeTableWindow(AppState* proj);
        ~GCodeTableWindow() override;
        void Draw() override;

    protected:
        void DrawMarkersPane();
        void DrawTablePane();


    };
}
