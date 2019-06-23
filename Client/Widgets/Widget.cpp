#include "Widget.h"

namespace Coconut
{
    Widget::Widget(AppState* state, string name, bool visible) :
      mAppState(state),
      mName(name),
      mVisible(visible)
    {

    }

    Widget::~Widget()
    {

    }

    bool Widget::GetVisible() const
    {
        return mVisible;
    }

    bool* Widget::VisiblePointer()
    {
        return &mVisible;
    }

    void Widget::SetVisible (bool v)
    {
        mVisible = v;
    }

    string Widget::GetName() const
	{
    	return mName;
    }

    void Widget::setName(string name)
    {
       	mName = name;
    }
}
