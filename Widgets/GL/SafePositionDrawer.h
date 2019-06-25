#pragma once

#include "GLWidget.h"

namespace Coconut
{

	class SafePositionDrawer : public GLWidget
	{
	public:
		SafePositionDrawer(AppState* state);

		int  GetVertexCount();

		vec3 GetPosition() const;
		void SetPosition(const vec3& position);

	protected:
		bool UpdateData();

	private:
		vec3 mPosition;
	};

}
