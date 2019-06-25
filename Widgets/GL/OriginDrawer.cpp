#include "OriginDrawer.h"

#include <glm/vec4.hpp>

using glm::vec4;

namespace Coconut
{
	OriginDrawer::OriginDrawer(AppState* state)
        : GLWidget (state,"OriginDrawer")
	{
		mPosition = vec3(0.0,0.0,0.0);
	}

	bool OriginDrawer::UpdateData()
	{
		vec4 red = vec4(1.0, 0.0, 0.0, 1.0);
		vec4 green = vec4(0.0, 1.0, 0.0, 1.0);
		vec4 blue = vec4(0.0, 0.0, 1.0, 1.0);
		vec4 white = vec4(1.0,1.0,1.0,1.0);

        ClearLineVertexBuffer();
        ClearTriangleVertexBuffer();

		// X-axis
		AddLineVertex({vec3(mPosition.x,    mPosition.y,     mPosition.z), red });
		AddLineVertex({vec3(mPosition.x+8,  mPosition.y,     mPosition.z), red });

		AddTriangleVertex({vec3(mPosition.x+10, mPosition.y,     mPosition.z), red});
		AddTriangleVertex({vec3(mPosition.x+8,  mPosition.y+0.5, mPosition.z), red});
		AddTriangleVertex({vec3(mPosition.x+8,  mPosition.y-0.5, mPosition.z), red});

		AddTriangleVertex({vec3(mPosition.x+10, mPosition.y, mPosition.z),     red});
		AddTriangleVertex({vec3(mPosition.x+8,  mPosition.y, mPosition.z+0.5), red});
		AddTriangleVertex({vec3(mPosition.x+8,  mPosition.y, mPosition.z-0.5), red});

		// Y-axis
		AddLineVertex({vec3(mPosition.x, mPosition.y,   mPosition.z), green});
		AddLineVertex({vec3(mPosition.x, mPosition.y+9, mPosition.z), green});

		AddTriangleVertex({vec3(mPosition.x,     mPosition.y+10, mPosition.z), green});
		AddTriangleVertex({vec3(mPosition.x+0.5, mPosition.y+8,  mPosition.z), green});
		AddTriangleVertex({vec3(mPosition.x-0.5, mPosition.y+8,  mPosition.z), green});

		AddTriangleVertex({vec3(mPosition.x, mPosition.y+10, mPosition.z),     green});
		AddTriangleVertex({vec3(mPosition.x, mPosition.y+8, mPosition.z+0.5),  green});
		AddTriangleVertex({vec3(mPosition.x, mPosition.y+8, mPosition.z-0.5),  green});

		// Z-axis
		AddLineVertex({vec3(mPosition.x, mPosition.y, mPosition.z),    blue, });
		AddLineVertex({vec3(mPosition.x, mPosition.y, mPosition.z+9),  blue, });

		AddTriangleVertex({vec3(mPosition.x,     mPosition.y, mPosition.z+10), blue});
		AddTriangleVertex({vec3(mPosition.x+0.5, mPosition.y, mPosition.z+8),  blue});
		AddTriangleVertex({vec3(mPosition.x-0.5, mPosition.y, mPosition.z+8),  blue});

		AddTriangleVertex({vec3(mPosition.x, mPosition.y, mPosition.z+10),     blue});
		AddTriangleVertex({vec3(mPosition.x, mPosition.y+0.5, mPosition.z+8),  blue});
		AddTriangleVertex({vec3(mPosition.x, mPosition.y-0.5, mPosition.z+8),  blue});

		return true;
	}

	vec3 OriginDrawer::GetSizes()
	{
	   return vec3();
	}

	vec3 OriginDrawer::GetMinimumExtremes()
	{
	   return vec3();
	}

	vec3 OriginDrawer::GetMaximumExtremes()
	{
	   return vec3();
	}

	int OriginDrawer::GetVertexCount()
	{
	   return mLineVertexBuffer.size();
	}

	vec3 OriginDrawer::GetPosition() const
	{
		return mPosition;
	}

	void OriginDrawer::SetPosition(const vec3& position)
	{
		mPosition = position;
		Update();
	}

}
