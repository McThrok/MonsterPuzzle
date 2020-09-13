#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <SimpleMath.h>
#include <assimp/material.h>
#include <DDSTextureLoader.h>
#include <Utils/StringHelper.h>

namespace fe
{
	enum class TextureStorageType
	{
		Invalid,
		None,
		EmbeddedIndexCompressed,
		EmbeddedIndexNonCompressed,
		EmbeddedCompressed,
		EmbeddedNonCompressed,
		Disk
	};

	namespace sm = DirectX::SimpleMath;

	class Texture
	{
	public:
		Texture() {}
		Texture(ID3D11Device* device, CD3D11_TEXTURE2D_DESC desc, const sm::Color* colorData = nullptr, aiTextureType type = aiTextureType::aiTextureType_UNKNOWN);
		Texture(ID3D11Device* device, std::string filePath, aiTextureType type);
		void Initialize(ID3D11Device* device, CD3D11_TEXTURE2D_DESC desc, const sm::Color* colorData = nullptr, aiTextureType type = aiTextureType::aiTextureType_UNKNOWN);

		ID3D11ShaderResourceView* GetTextureResourceView() { return this->textureView.Get(); }
		ID3D11ShaderResourceView** GetTextureResourceViewAddress() { return this->textureView.GetAddressOf(); }
		ID3D11Resource* GetTexture() { return this->texture.Get(); }

		aiTextureType GetType() { return type; }
		UINT GetWidth() const { return width; }
		UINT GetHeight() const { return height; }

		static CD3D11_TEXTURE2D_DESC GetRenderTexDesc(UINT width, UINT height);
		static CD3D11_TEXTURE2D_DESC GetDefaultTexDesc(UINT width, UINT height);
		static CD3D11_TEXTURE2D_DESC GetDepthStencilTexDesc(UINT width, UINT height);
		static CD3D11_TEXTURE2D_DESC GetOneColorTexDesc();

	private:

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureView = nullptr;
		Microsoft::WRL::ComPtr<ID3D11Resource> texture = nullptr;

		aiTextureType type = aiTextureType::aiTextureType_UNKNOWN;
		UINT width = 0, height = 0;
	};
}
