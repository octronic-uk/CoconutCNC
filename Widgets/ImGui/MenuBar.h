#pragma once

#include "ImGuiWidget.h"

namespace Coconut
{
    class MenuBar : public ImGuiWidget
    {
    public:
        MenuBar(AppState*);
        ~MenuBar() override;
        void Draw() override;

    protected:
        void DrawFileMenu();
        void DrawViewMenu();
        void DrawWizardsMenu();
        void DrawLoggingMenu();


        void HandleFileMenuActions();
        void HandleWizardMenuActions();

        void FileOpenAction();
        void FileCloseAction();
        void FileSettingsAction();
        void FileQuitAction();

    private:
        bool mFileSettingsClicked;
        bool mFileOpenClicked;
        bool mFileQuitClicked;
        bool mFileCloseClicked;
    };
}
