#pragma once
#include "GLWidget.h"

namespace Coconut
{
    class OriginDrawer : public GLWidget
    {
    public:
        OriginDrawer(AppState* state);

        bool Init() override;
        void Update() override;
    private:
        float mLineWidth;
        float mAxisLineLength;
        float mArrowSize;
    };
}
