#pragma once
#include "Graphics/Model.h"
#include "Graphics/Shaders.h"
#include "Graphics/Texture.h"

namespace fe
{
	class RenderableMesh
	{
	public:
		RenderableMesh(Mesh* mesh, VertexShader* vs, PixelShader* ps, Texture* texture = nullptr);

		Mesh* mesh;
		VertexShader* vertexShader;
		PixelShader* pixelShader;
		Texture* texture;
	};
}
