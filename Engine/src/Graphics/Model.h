#pragma once
#include "Mesh.h"
#include "Texture.h"

namespace fe
{
	namespace sm = DirectX::SimpleMath;

	class Model
	{
	public:
		Model() { }
		Model(Model* model);
		~Model();
		bool Initialize(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext);

		size_t GetMeshesSize() { return meshes.size(); }
		Mesh* GetMesh(size_t idx) { return meshes[idx]; }

	private:
		std::vector<Mesh*> meshes;

		bool LoadModel(const std::string& filePath);
		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh* ProcessMesh(aiMesh* mesh);
		std::vector<Texture> LoadMaterialTextures(aiMaterial* pMaterial, aiTextureType textureType, const aiScene* pScene);

		ID3D11Device* device = nullptr;
		ID3D11DeviceContext* deviceContext = nullptr;
	};
}
