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

        void FileOpenAction();
        void FileCloseAction();
        void FileQuitAction();

    private:
        bool mOpenButtonClicked;
        bool mQuitButtonClicked;
        bool mCloseButtonClicked;
    };
}
