#include "OriginDrawer.h"

#include <glm/vec4.hpp>
#include "../../AppState.h"
#include "../../Model/Grbl/GrblMachineModel.h"

using glm::vec4;

namespace Coconut
{
	OriginDrawer::OriginDrawer(AppState* state)
        : GLWidget (state,"OriginDrawer")
	{
	}

    void OriginDrawer::Update()
    {

    }

	bool OriginDrawer::Init()
	{
        GLWidget::Init();

        vec3 pos = mAppState->GetGrblMachineModel().GetWorkPosition();

		vec4 red = vec4(1.0, 0.0, 0.0, 1.0);
		vec4 green = vec4(0.0, 1.0, 0.0, 1.0);
		vec4 blue = vec4(0.0, 0.0, 1.0, 1.0);
		vec4 white = vec4(1.0,1.0,1.0,1.0);

        ClearLineVertexBuffer();
        ClearTriangleVertexBuffer();

		// X-axis
		AddLineVertex({vec3(pos.x,    pos.y,     pos.z), red });
		AddLineVertex({vec3(pos.x+8,  pos.y,     pos.z), red });

		AddTriangleVertex({vec3(pos.x+10, pos.y,     pos.z), red});
		AddTriangleVertex({vec3(pos.x+8,  pos.y+0.5, pos.z), red});
		AddTriangleVertex({vec3(pos.x+8,  pos.y-0.5, pos.z), red});

		AddTriangleVertex({vec3(pos.x+10, pos.y, pos.z),     red});
		AddTriangleVertex({vec3(pos.x+8,  pos.y, pos.z+0.5), red});
		AddTriangleVertex({vec3(pos.x+8,  pos.y, pos.z-0.5), red});

		// Y-axis
		AddLineVertex({vec3(pos.x, pos.y,   pos.z), green});
		AddLineVertex({vec3(pos.x, pos.y+9, pos.z), green});

		AddTriangleVertex({vec3(pos.x,     pos.y+10, pos.z), green});
		AddTriangleVertex({vec3(pos.x+0.5, pos.y+8,  pos.z), green});
		AddTriangleVertex({vec3(pos.x-0.5, pos.y+8,  pos.z), green});

		AddTriangleVertex({vec3(pos.x, pos.y+10, pos.z),     green});
		AddTriangleVertex({vec3(pos.x, pos.y+8, pos.z+0.5),  green});
		AddTriangleVertex({vec3(pos.x, pos.y+8, pos.z-0.5),  green});

		// Z-axis
		AddLineVertex({vec3(pos.x, pos.y, pos.z),    blue, });
		AddLineVertex({vec3(pos.x, pos.y, pos.z+9),  blue, });

		AddTriangleVertex({vec3(pos.x,     pos.y, pos.z+10), blue});
		AddTriangleVertex({vec3(pos.x+0.5, pos.y, pos.z+8),  blue});
		AddTriangleVertex({vec3(pos.x-0.5, pos.y, pos.z+8),  blue});

		AddTriangleVertex({vec3(pos.x, pos.y, pos.z+10),     blue});
		AddTriangleVertex({vec3(pos.x, pos.y+0.5, pos.z+8),  blue});
		AddTriangleVertex({vec3(pos.x, pos.y-0.5, pos.z+8),  blue});

        SubmitLineVertexBuffer();
        SubmitTriangleVertexBuffer();

		return true;
	}
}
