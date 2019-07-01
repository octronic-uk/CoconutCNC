#pragma once

#include "ImGuiWidget.h"

namespace Coconut
{
    class SettingsWindow : public ImGuiWidget
    {
    public:
        SettingsWindow(AppState* proj);
        ~SettingsWindow() override;
        void Draw() override;
    protected:
        void DrawConnectionSettings();
        void DrawInterfaceSettings();
        void DrawMachineSettings();
        void DrawToolHolderSettings();
        void DrawToolSettings();
        void DrawGrblSettings();

    private:
        int mConnectionBaudRateComboIndex;
        int mSelectedToolSettingsIndex;
        int mToolSettingsIndexToRemove;
        int mSelectedToolHolderSettingsIndex;
        int mToolHolderSettingsIndexToRemove;
        bool mUpdateToolDrawer;
    };
}
