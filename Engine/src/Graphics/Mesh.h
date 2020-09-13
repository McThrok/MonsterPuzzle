#pragma once
#include "Vertex.h"
#include "Buffers\VertexBuffer.h"
#include "Buffers\IndexBuffer.h"
#include "Buffers\ConstantBuffer.h"

namespace fe
{
	class Mesh
	{
	public:
		Mesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, 
			void* vertices, int vertStride, int vertSize, std::vector<DWORD>& indices);
		Mesh(const Mesh& mesh);

		int VerticesCount() { return vertexbuffer.VertexCount(); }
		int IndicesCount() { return indexbuffer.IndexCount(); }
		int TrianglesCount() { return indexbuffer.IndexCount() / 3; }
		void Draw();

	private:
		VertexBuffer vertexbuffer;
		IndexBuffer indexbuffer;
		ID3D11DeviceContext* deviceContext;
	};
}