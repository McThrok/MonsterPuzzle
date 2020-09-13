#pragma once
#include "Graphics/Vertex.h"
#include "Component.h"

namespace sm = DirectX::SimpleMath;

namespace fe
{
	class ColorComponent : public Component
	{
		CLASS_DECLARATION(ColorComponent)
	public:

		ColorComponent(sm::Vector4 color) :Color(color) {}

		sm::Vector4 Color;
	};
}

