/*
 * Widget.h
 * 	Coconut
*/

#pragma once

#include <string>

using std::string;

namespace Coconut
{

    class AppState;
    class Widget
    {
    public:
        Widget(AppState* state, string name, bool visible = true);
        virtual ~Widget();
        virtual void Draw() = 0;

        bool GetVisible() const;
        void SetVisible(bool);
        bool* VisiblePointer();

        string GetName() const;
        void setName(string name);

    protected:
        AppState* mAppState;
        string mName;
        bool mVisible;
    };
}
