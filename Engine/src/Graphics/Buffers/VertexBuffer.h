#pragma once
#include "pch.h"

namespace fe
{
	class VertexBuffer
	{
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
		UINT stride;
		UINT vertexCount = 0;

	public:
		VertexBuffer() {}

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

		HRESULT Initialize(ID3D11Device* _device, void* _data, UINT _stride, UINT _vertexCount)
		{
			if (buffer.Get() != nullptr)
				buffer.Reset();

			this->stride = _stride;
			this->vertexCount = _vertexCount;

			D3D11_BUFFER_DESC vertexBufferDesc;
			ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

			vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			vertexBufferDesc.ByteWidth = stride * _vertexCount;
			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertexBufferDesc.CPUAccessFlags = 0;
			vertexBufferDesc.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA vertexBufferData;
			ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
			vertexBufferData.pSysMem = _data;

			HRESULT hr = _device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, this->buffer.GetAddressOf());
			return hr;
		}
	};
}
