#include "pch.h"
#include "Graphics.h"
#include "Constants/PathConstants.h"
#include "Constants/CBConstants.h"

namespace fe
{
	bool Graphics::Initialize(HWND hwnd, float width, float height)
	{
		if (!InitializeDirectX(hwnd, width, height))
			return false;

		if (!InitializeShaders())
			return false;

		if (!InitializeScene())
			return false;

		return true;
	}

		void Graphics::OnPreRender()
	{
		//device.context()->RSSetViewports(1, &vieport_engine);
		//device.context()->OMSetRenderTargets(1, this->renderTargetView_engine.GetAddressOf(), depthStencilView_engine.Get());

		//device.context()->ClearRenderTargetView(this->renderTargetView_engine.Get(), clearColor);
		//device.context()->ClearDepthStencilView(this->depthStencilView_engine.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		
		device.context()->OMSetDepthStencilState(this->depthStencilState.Get(), 0);
		//device.context()->OMSetBlendState(NULL, NULL, 0xFFFFFFFF);
		device.context()->OMSetBlendState(this->blendState.Get(), NULL, 0xffffffff);
		device.context()->PSSetSamplers(0, 1, this->samplerState.GetAddressOf());
	}

	void Graphics::RestoreContext()
	{
		device.context()->RSSetViewports(1, &viewport);
		device.context()->OMSetRenderTargets(1, this->renderTargetView.GetAddressOf(), this->depthStencilView.Get());

		device.context()->ClearRenderTargetView(this->renderTargetView.Get(), clearColor);
		device.context()->ClearDepthStencilView(this->depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	void Graphics::OnPostRender()
	{
		device.swapChain()->Present(1, 0);
	}

	void Graphics::OnResize(float width, float height)
	{
		if (!device.swapChain())
			return;

		device.context()->OMSetRenderTargets(0, 0, 0);

		// Release all outstanding references to the swap chain's buffers.
		renderTargetView->Release();
		// Preserve the existing buffer count and format.
		// Automatically choose the width and height to match the client rect for HWNDs.
		HRESULT hr = device.swapChain()->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

		UpdateViewport(width, height);
		UpdateRenderTargetView();
		UpdateDepthStencilView(width, height);
		
		device.context()->OMSetRenderTargets(1, this->renderTargetView.GetAddressOf(), this->depthStencilView.Get());
	}

	bool Graphics::InitializeDirectX(HWND hwnd, float width, float height)
	{
		try
		{
			device.Initialize(hwnd, width, height);

			UpdateViewport(width, height);
			UpdateRenderTargetView();
			UpdateDepthStencilView(width, height);

			device.context()->OMSetRenderTargets(1, this->renderTargetView.GetAddressOf(), this->depthStencilView.Get());

			//Create depth stencil state
			CD3D11_DEPTH_STENCIL_DESC depthstencildesc(D3D11_DEFAULT);
			depthstencildesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

			HRESULT hr = this->device->CreateDepthStencilState(&depthstencildesc, this->depthStencilState.GetAddressOf());
			ERROR_IF_FAILED(hr, "Failed to create depth stencil state.");

			//Create sampler description for sampler state
			CD3D11_SAMPLER_DESC sampDesc(D3D11_DEFAULT);
			sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			hr = this->device->CreateSamplerState(&sampDesc, this->samplerState.GetAddressOf());
			ERROR_IF_FAILED(hr, "Failed to create sampler state.");

			CD3D11_BLEND_DESC blendDesc(D3D11_DEFAULT);
			blendDesc.RenderTarget[0].BlendEnable = true;
			blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
			blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_DEST_ALPHA;
			blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
			hr = this->device->CreateBlendState(&blendDesc, this->blendState.GetAddressOf());
			ERROR_IF_FAILED(hr, "Failed to create blend state.");
		}
		catch (COMException& exception)
		{
			ErrorLogger::Log(exception);
			return false;
		}
		return true;
	}

	bool Graphics::InitializeShaders()
	{
		std::wstring root = StringHelper::StringToWide(PathConstants::SHADERS_PATH);

		pureVertexShader.Initialize(this->device, root + L"pureColor_vs.cso", VertexPNT::layout, ARRAYSIZE(VertexPNT::layout));
		purePixelShader.Initialize(this->device, root + L"pureColor_ps.cso");

		return true;
	}

	bool Graphics::InitializeScene()
	{
		return true;
	}

	void Graphics::SetFullScreen(bool fullScreen)
	{
		this->device.swapChain()->SetFullscreenState(fullScreen, NULL);
	}

	void Graphics::UpdateViewport(float width, float height)
	{
		viewport = CD3D11_VIEWPORT(0.0f, 0.0f, width, height);
		device.context()->RSSetViewports(1, &viewport);
	}

	void Graphics::UpdateRenderTargetView()
	{
		Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
		HRESULT hr = device.swapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
		ERROR_IF_FAILED(hr, "GetBuffer Failed.");

		hr = this->device->CreateRenderTargetView(backBuffer.Get(), NULL, this->renderTargetView.GetAddressOf());
		ERROR_IF_FAILED(hr, "Failed to create render target view.");
	}

	void Graphics::UpdateDepthStencilView(float width, float height)
	{
		auto depthStencilTextureDesc = Texture::GetDepthStencilTexDesc(width, height);

		HRESULT hr = this->device->CreateTexture2D(&depthStencilTextureDesc, NULL, this->depthStencilBuffer.ReleaseAndGetAddressOf());
		ERROR_IF_FAILED(hr, "Failed to create depth stencil buffer.");

		hr = this->device->CreateDepthStencilView(this->depthStencilBuffer.Get(), NULL, this->depthStencilView.ReleaseAndGetAddressOf());
		ERROR_IF_FAILED(hr, "Failed to create depth stencil view.");
	}
}
