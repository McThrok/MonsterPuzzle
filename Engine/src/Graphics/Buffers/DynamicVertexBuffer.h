#pragma once
#include "Utils/ErrorLogger.h"

namespace fe
{
	template<class T>
	class DynamicVertexBuffer
	{
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
		ID3D11DeviceContext* deviceContext = nullptr;
		UINT stride = sizeof(T);
		UINT vertexCount = 0;

	public:
		DynamicVertexBuffer() {}

		ID3D11Buffer* Get()const
		{
			return buffer.Get();
		}

		ID3D11Buffer* const* GetAddressOf()const
		{
			return buffer.GetAddressOf();
		}

		UINT VertexCount() const
		{
			return this->vertexCount;
		}

		const UINT Stride() const
		{
			return this->stride;
		}

		const UINT* StridePtr() const
		{
			return &this->stride;
		}

		void Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, T* data, UINT vertexCount)
		{
			if (buffer.Get() != nullptr)
				buffer.Reset();

			this->deviceContext = deviceContext;
			this->vertexCount = vertexCount;

			D3D11_BUFFER_DESC vertexBufferDesc;
			ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

			vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			vertexBufferDesc.ByteWidth = stride * vertexCount;
			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			vertexBufferDesc.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA vertexBufferData;
			ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
			vertexBufferData.pSysMem = data ? data : (new T[vertexCount]); //allocate memory if empty data

			HRESULT hr = device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, this->buffer.GetAddressOf());
			ERROR_IF_FAILED(hr, "Failed to initialize dynamic buffer.");

			if (!data)
				delete[] vertexBufferData.pSysMem;
		}

		bool Update(T* data)
		{
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			HRESULT hr = this->deviceContext->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			ERROR_IF_FAILED(hr, "Failed to map constant buffer.");

			CopyMemory(mappedResource.pData, data, sizeof(T) * vertexCount);
			this->deviceContext->Unmap(buffer.Get(), 0);
			return true;
		}
	};
}
