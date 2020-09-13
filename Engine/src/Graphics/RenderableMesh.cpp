#include "pch.h"
#include "RenderableMesh.h"

namespace fe
{
	RenderableMesh::RenderableMesh(Mesh* mesh,VertexShader* vs, PixelShader* ps, Texture* texture)
	{
		this->mesh = mesh;
		this->vertexShader = vs;
		this->pixelShader = ps;
		this->texture = texture;
	}
}
