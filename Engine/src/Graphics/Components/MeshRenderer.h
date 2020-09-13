#pragma once
#include "Component.h"
#include "Graphics/RenderableMesh.h"

namespace fe
{
	class MeshRenderer : public Component
	{
		CLASS_DECLARATION(MeshRenderer)

	public:
		MeshRenderer(Mesh* mesh, VertexShader* vs, PixelShader* ps, Texture* texture = nullptr);
		MeshRenderer(Model* model, VertexShader* vs, PixelShader* ps, Texture* texture = nullptr);

		size_t GetMeshesSize() { return renderableMesh.size(); }
		RenderableMesh& GetMesh(size_t idx) { return renderableMesh[idx]; }

	private:
		std::vector<RenderableMesh> renderableMesh;
	};
}
