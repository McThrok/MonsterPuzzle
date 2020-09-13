#include "pch.h"
#include "MeshRenderer.h"

namespace fe
{
	CLASS_DEFINITION(Component, MeshRenderer)

	MeshRenderer::MeshRenderer(Mesh* mesh, VertexShader* vs, PixelShader* ps, Texture* texture)
	{
		renderableMesh.emplace_back(mesh, vs, ps, texture);
	}

	MeshRenderer::MeshRenderer(Model* model, VertexShader* vs, PixelShader* ps, Texture* texture)
	{
		for (size_t i = 0; i < model->GetMeshesSize(); i++)
		{
			//renderableMesh.emplace_back(model->GetMesh(i), model->GetMaterial(i), vs, ps);
			renderableMesh.emplace_back(model->GetMesh(i), vs, ps, texture);
		}
	}
}
