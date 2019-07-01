#pragma once

#include "GLWidget.h"

#include <glm/vec4.hpp>

using glm::vec4;

namespace Coconut
{
	class SelectionDrawer : public GLWidget
	{
	public:
		SelectionDrawer(AppState* state);

		vec3 StartPosition() const;
		void SetStartPosition(const vec3 &startPosition);

		vec4 Color() const;
		void SetColor(const vec4 &color);

		vec3 EndPosition() const;
		void SetEndPosition(const vec3 &endPosition);

		void Update() override;

	private:
		vec3 m_startPosition;
		vec3 m_endPosition;
		vec4 m_color;
	};
}
