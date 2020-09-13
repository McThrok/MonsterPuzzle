#pragma once
#include "pch.h"
#include "ConstantBufferTypes.h"
#include "Utils/ErrorLogger.h"

namespace fe
{
	template<class T>
	class ConstantBuffer
	{
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
		ID3D11DeviceContext* deviceContext = nullptr;
		UINT count = 0;

	public:
		ConstantBuffer() {}

		ID3D11Buffer* Get()const
		{
			return buffer.Get();
		}

		ID3D11Buffer* const* GetAddressOf() const
		{
			return buffer.GetAddressOf();
		}

		UINT GetCount() const
		{
			return count;
		}

		void Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, UINT count = 1)
		{
			if (buffer.Get() != nullptr)
				buffer.Reset();

			this->count = count;
			this->deviceContext = deviceContext;

			D3D11_BUFFER_DESC desc;
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			desc.MiscFlags = 0;
			desc.ByteWidth = static_cast<UINT>(sizeof(T) * count + (16 - (sizeof(T) * count % 16)));
			desc.StructureByteStride = 0;

			HRESULT hr = device->CreateBuffer(&desc, 0, buffer.GetAddressOf());
			ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");
		}

		void Update(T* data)
		{
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			HRESULT hr = this->deviceContext->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			ERROR_IF_FAILED(hr, "Failed to map constant buffer.");

			CopyMemory(mappedResource.pData, data, sizeof(T) * count);
			this->deviceContext->Unmap(buffer.Get(), 0);
		}
	};
}
