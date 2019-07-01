#include "OriginDrawer.h"

#include <glm/vec4.hpp>
#include "../../AppState.h"
#include "../../Model/Grbl/GrblMachineModel.h"

using glm::vec4;

namespace Coconut
{
	OriginDrawer::OriginDrawer(AppState* state)
        : GLWidget (state,"OriginDrawer"),
          mLineWidth(1.f),
          mAxisLineLength(45.f),
          mArrowSize(5.f)
	{
	}

    void OriginDrawer::Update()
    {

    }

	bool OriginDrawer::Init()
	{
        GLWidget::Init();

		static vec4 red = vec4(1.0, 0.0, 0.0, 1.0);
		static vec4 green = vec4(0.0, 1.0, 0.0, 1.0);
		static vec4 blue = vec4(0.0, 0.0, 1.0, 1.0);
		static vec4 white = vec4(1.0,1.0,1.0,1.0);

        ClearTriangleVertexBuffer();

		// X-axis
        // Lentgh
        AddTriangleVertex({vec3(              0,  mLineWidth, 0), red});
        AddTriangleVertex({vec3(mAxisLineLength,  mLineWidth, 0), red});
        AddTriangleVertex({vec3(              0, -mLineWidth, 0), red});

        AddTriangleVertex({vec3(mAxisLineLength,  mLineWidth, 0), red});
        AddTriangleVertex({vec3(mAxisLineLength, -mLineWidth, 0), red});
        AddTriangleVertex({vec3(              0, -mLineWidth, 0), red});

        AddTriangleVertex({vec3(              0, 0,  mLineWidth), red});
        AddTriangleVertex({vec3(mAxisLineLength, 0,  mLineWidth), red});
        AddTriangleVertex({vec3(              0, 0, -mLineWidth), red});

        AddTriangleVertex({vec3(mAxisLineLength,  0,  mLineWidth), red});
        AddTriangleVertex({vec3(mAxisLineLength,  0, -mLineWidth), red});
        AddTriangleVertex({vec3(               0, 0, -mLineWidth), red});


        // tip
		AddTriangleVertex({vec3(mAxisLineLength+mArrowSize,   0.0, 0.0), red});
		AddTriangleVertex({vec3(mAxisLineLength,  mArrowSize, 0.0), red});
		AddTriangleVertex({vec3(mAxisLineLength, -mArrowSize, 0.0), red});

 		AddTriangleVertex({vec3(mAxisLineLength+mArrowSize,   0.0, 0.0), red});
		AddTriangleVertex({vec3(mAxisLineLength,  0, mArrowSize), red});
		AddTriangleVertex({vec3(mAxisLineLength, 0, -mArrowSize), red});


       	// Y-axis
        // Lentgh
        AddTriangleVertex({vec3(0,               0,  mLineWidth), green});
        AddTriangleVertex({vec3(0, mAxisLineLength,  mLineWidth), green});
        AddTriangleVertex({vec3(0,               0, -mLineWidth), green});

        AddTriangleVertex({vec3(0, mAxisLineLength,  mLineWidth), green});
        AddTriangleVertex({vec3(0, mAxisLineLength, -mLineWidth), green});
        AddTriangleVertex({vec3(0,               0, -mLineWidth), green});

        AddTriangleVertex({vec3( mLineWidth, 0, 0), green});
        AddTriangleVertex({vec3( mLineWidth, mAxisLineLength, 0), green});
        AddTriangleVertex({vec3(-mLineWidth, 0, 0), green});

        AddTriangleVertex({vec3( mLineWidth, mAxisLineLength, 0), green});
        AddTriangleVertex({vec3(-mLineWidth, mAxisLineLength, 0), green});
        AddTriangleVertex({vec3(-mLineWidth, 0, 0), green});


        // tip
		AddTriangleVertex({vec3(0, mAxisLineLength+mArrowSize, 0.0), green});
		AddTriangleVertex({vec3(0, mAxisLineLength,  mArrowSize), green});
		AddTriangleVertex({vec3(0, mAxisLineLength, -mArrowSize), green});

        AddTriangleVertex({vec3(          0, mAxisLineLength+mArrowSize, 0.0), green});
		AddTriangleVertex({vec3( mArrowSize, mAxisLineLength, 0.0), green});
		AddTriangleVertex({vec3(-mArrowSize, mAxisLineLength, 0.0), green});


       	// Z-axis
        // Lentgh
        AddTriangleVertex({vec3( mLineWidth, 0, 0), blue});
        AddTriangleVertex({vec3( mLineWidth, 0, mAxisLineLength), blue});
        AddTriangleVertex({vec3(-mLineWidth, 0, 0), blue});

        AddTriangleVertex({vec3( mLineWidth, 0, mAxisLineLength), blue});
        AddTriangleVertex({vec3(-mLineWidth, 0, mAxisLineLength), blue});
        AddTriangleVertex({vec3(-mLineWidth, 0, 0), blue});

        AddTriangleVertex({vec3(0,  mLineWidth, 0), blue});
        AddTriangleVertex({vec3(0,  mLineWidth, mAxisLineLength), blue});
        AddTriangleVertex({vec3(0, -mLineWidth, 0), blue});

        AddTriangleVertex({vec3(0,  mLineWidth, mAxisLineLength), blue});
        AddTriangleVertex({vec3(0, -mLineWidth, mAxisLineLength), blue});
        AddTriangleVertex({vec3(0, -mLineWidth, 0), blue});


        // tip
		AddTriangleVertex({vec3(        0.0, 0.0, mAxisLineLength+mArrowSize), blue});
		AddTriangleVertex({vec3( mArrowSize, 0.0, mAxisLineLength), blue});
		AddTriangleVertex({vec3(-mArrowSize, 0.0, mAxisLineLength), blue});

        AddTriangleVertex({vec3(0.0,       0.0, mAxisLineLength+mArrowSize), blue});
		AddTriangleVertex({vec3(0,  mArrowSize, mAxisLineLength), blue});
		AddTriangleVertex({vec3(0, -mArrowSize, mAxisLineLength), blue});


        SubmitTriangleVertexBuffer();

		return true;
	}
}
