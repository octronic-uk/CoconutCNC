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

    private:
        int mConnectionPortComboIndex;
        int mConnectionBaudRateComboIndex;
        vector<string> mConnectionPortList;
        vector<string> mConnectionBaudRateList;
        // Tool Settings Tab
        int mSelectedToolSettingsIndex;
    };
}
