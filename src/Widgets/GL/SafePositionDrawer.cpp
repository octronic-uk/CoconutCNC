#include "SafePositionDrawer.h"

#include <glm/vec4.hpp>

using glm::vec4;

namespace Coconut
{
	SafePositionDrawer::SafePositionDrawer(AppState* state)
        : GLWidget (state,"SafePositionDrawer")
	{
		mPosition = vec3(0.0,0.0,0.0);
	}

	bool SafePositionDrawer::UpdateData()
	{
		vec4 green = vec4(0.0, 1.0, 0.0, 1.0);

        ClearLineVertexBuffer();
        ClearTriangleVertexBuffer();

		// Z-axis
		AddTriangleVertex({vec3(mPosition.x,   mPosition.y, mPosition.z),    green});
		AddTriangleVertex({vec3(mPosition.x+1, mPosition.y, mPosition.z+4),  green});
		AddTriangleVertex({vec3(mPosition.x-1, mPosition.y, mPosition.z+4),  green});

		AddTriangleVertex({vec3(mPosition.x, mPosition.y, mPosition.z),      green});
		AddTriangleVertex({vec3(mPosition.x, mPosition.y+1, mPosition.z+4),  green});
		AddTriangleVertex({vec3(mPosition.x, mPosition.y-1, mPosition.z+4),  green});

		return true;
	}


	int SafePositionDrawer::GetVertexCount()
	{
	   return mLineVertexBuffer.size();
	}

	vec3 SafePositionDrawer::GetPosition() const
	{
		return mPosition;
	}

	void SafePositionDrawer::SetPosition(const vec3& position)
	{
		mPosition = position;
		Update();
	}
}
