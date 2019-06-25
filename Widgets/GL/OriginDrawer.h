#pragma once
#include "GLWidget.h"

namespace Coconut
{
    class OriginDrawer : public GLWidget
    {
    public:
        OriginDrawer(AppState* state);

        vec3 GetSizes();
        vec3 GetMinimumExtremes();
        vec3 GetMaximumExtremes();
        int  GetVertexCount();

        vec3 GetPosition() const;
        void SetPosition(const vec3& position);

    protected:
        bool UpdateData();

    private:
        vec3 mPosition;
    };
}
