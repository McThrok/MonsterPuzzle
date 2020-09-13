#include "pch.h"
#include "Texture.h"
#include "Utils/ErrorLogger.h"

namespace fe
{
	Texture::Texture(ID3D11Device* device, CD3D11_TEXTURE2D_DESC desc, const sm::Color* colorData, aiTextureType type)
	{
		Initialize(device, desc, colorData, type);
	}

	Texture::Texture(ID3D11Device* device, std::string filePath, aiTextureType type)
	{
		this->type = type;
		std::string ext = StringHelper::GetFileExtension(filePath);
		if (StringHelper::GetFileExtension(filePath) == "dds")
		{
			HRESULT hr = DirectX::CreateDDSTextureFromFile(device, StringHelper::StringToWide(filePath).c_str(), texture.GetAddressOf(), this->textureView.GetAddressOf());
			ERROR_IF_FAILED(hr, "Failed to load texture.");
		}
		else
		{
			HRESULT hr = DirectX::CreateWICTextureFromFile(device, StringHelper::StringToWide(filePath).c_str(), texture.GetAddressOf(), this->textureView.GetAddressOf());
			ERROR_IF_FAILED(hr, "Failed to load texture.");
			//ErrorLogger::Log("Cannot load " + filePath + ". Only .dds texture format is supported!");
		}
	}

	void Texture::Initialize(ID3D11Device* device, CD3D11_TEXTURE2D_DESC desc, const sm::Color* colorData, aiTextureType type)
	{
		this->width = desc.Width;
		this->height = desc.Height;
		this->type = type;

		ID3D11Texture2D* p2DTexture = nullptr;
		D3D11_SUBRESOURCE_DATA initialData{};

		initialData.pSysMem = colorData ? colorData : (new sm::Color[width * height]); //allocate memory if empty data
		initialData.SysMemPitch = width * sizeof(sm::Color);

		try
		{
			HRESULT hr = device->CreateTexture2D(&desc, &initialData, &p2DTexture);
			ERROR_IF_FAILED(hr, "Failed to create texture buffer.");
			texture = static_cast<ID3D11Texture2D*>(p2DTexture);

			CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(D3D11_SRV_DIMENSION_TEXTURE2D, desc.Format);
			hr = device->CreateShaderResourceView(texture.Get(), &srvDesc, textureView.GetAddressOf());
			ERROR_IF_FAILED(hr, "Failed to create texture view.");
		}
		catch (COMException& exception)
		{
			ErrorLogger::Log(exception);
			return;
		}

		if (!colorData)
			delete[] initialData.pSysMem;
	}

	CD3D11_TEXTURE2D_DESC Texture::GetRenderTexDesc(UINT width, UINT height)
	{
		CD3D11_TEXTURE2D_DESC desc(DXGI_FORMAT_R32G32B32_FLOAT, width, height);
		desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.MipLevels = 1;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.CPUAccessFlags = 0;
		desc.ArraySize = 1;
		desc.MiscFlags = 0;

		return desc;
	}

	CD3D11_TEXTURE2D_DESC Texture::GetDefaultTexDesc(UINT width, UINT height)
	{
		return CD3D11_TEXTURE2D_DESC(DXGI_FORMAT_R32G32B32A32_FLOAT, width, height);
	}

	CD3D11_TEXTURE2D_DESC Texture::GetOneColorTexDesc()
	{
		return GetDefaultTexDesc(1, 1);
	}

	CD3D11_TEXTURE2D_DESC Texture::GetDepthStencilTexDesc(UINT width, UINT height)
	{
		CD3D11_TEXTURE2D_DESC desc;
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		return desc;
	}
}
