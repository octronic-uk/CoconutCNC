#include "SelectionDrawer.h"

namespace Coconut
{
	SelectionDrawer::SelectionDrawer(AppState* state)
        : GLWidget (state,"SelectionDrawer")
	{
		m_endPosition = vec3(NAN, NAN, NAN);
		m_color = vec4(0.5,0,0.5,1);
	}

	void SelectionDrawer::Update()
	{
        ClearPointVertexBuffer();
		GLWidgetVertex vertex;
		vertex.Color = m_color;
		vertex.Position = m_endPosition;
		AddPointVertex(vertex);
        SubmitPointVertexBuffer();
	}

	vec3 SelectionDrawer::EndPosition() const
	{
		return m_endPosition;
	}

	void SelectionDrawer::SetEndPosition(const vec3 &endPosition)
	{
		m_endPosition = endPosition;
	}

	vec4 SelectionDrawer::Color() const
	{
		return m_color;
	}

	void SelectionDrawer::SetColor(const vec4 &color)
	{
		m_color = color;
	}

	vec3 SelectionDrawer::StartPosition() const
	{
		return m_startPosition;
	}

	void SelectionDrawer::SetStartPosition(const vec3 &startPosition)
	{
		m_startPosition = startPosition;
	}

}
