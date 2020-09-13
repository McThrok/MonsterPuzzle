#include "pch.h"
#include "Shaders.h"

namespace fe
{
	void VertexShader::Initialize(ID3D11Device* device, std::wstring shaderpath, D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT numElements)
	{
		HRESULT hr = D3DReadFileToBlob(shaderpath.c_str(), this->shader_buffer.GetAddressOf());
		ERROR_IF_FAILED(hr, L"Failed to load shader: " + shaderpath);

		hr = device->CreateVertexShader(this->shader_buffer->GetBufferPointer(), this->shader_buffer->GetBufferSize(), NULL, this->shader.GetAddressOf());
		ERROR_IF_FAILED(hr, L"Failed to create vertex shader: " + shaderpath);

		hr = device->CreateInputLayout(layoutDesc, numElements, this->shader_buffer->GetBufferPointer(), this->shader_buffer->GetBufferSize(), this->inputLayout.GetAddressOf());
		ERROR_IF_FAILED(hr, L"Failed to create input layout.");
	}

	ID3D11VertexShader* VertexShader::GetShader()
	{
		return this->shader.Get();
	}

	ID3D10Blob* VertexShader::GetBuffer()
	{
		return this->shader_buffer.Get();
	}

	ID3D11InputLayout* VertexShader::GetInputLayout()
	{
		return this->inputLayout.Get();
	}

	void PixelShader::Initialize(ID3D11Device* device, std::wstring shaderpath)
	{
		HRESULT hr = D3DReadFileToBlob(shaderpath.c_str(), this->shader_buffer.GetAddressOf());
		ERROR_IF_FAILED(hr, L"Failed to load shader: " + shaderpath);

		hr = device->CreatePixelShader(this->shader_buffer.Get()->GetBufferPointer(), this->shader_buffer.Get()->GetBufferSize(), NULL, this->shader.GetAddressOf());
		ERROR_IF_FAILED(hr, L"Failed to create pixel shader: " + shaderpath);
	}

	ID3D11PixelShader* PixelShader::GetShader()
	{
		return this->shader.Get();
	}

	ID3D10Blob* PixelShader::GetBuffer()
	{
		return this->shader_buffer.Get();
	}

	void GeometryShader::Initialize(ID3D11Device* device, std::wstring shaderpath)
	{
		HRESULT hr = D3DReadFileToBlob(shaderpath.c_str(), this->shader_buffer.GetAddressOf());
		ERROR_IF_FAILED(hr, L"Failed to load shader: " + shaderpath);

		hr = device->CreateGeometryShader(this->shader_buffer.Get()->GetBufferPointer(), this->shader_buffer.Get()->GetBufferSize(), NULL, this->shader.GetAddressOf());
		ERROR_IF_FAILED(hr, L"Failed to create geometry shader: " + shaderpath);
	}

	ID3D11GeometryShader* GeometryShader::GetShader()
	{
		return this->shader.Get();
	}

	ID3D10Blob* GeometryShader::GetBuffer()
	{
		return this->shader_buffer.Get();
	}
}
