#pragma once

#include "Vertex.h"
#include "Buffers/VertexBuffer.h"
#include "Buffers/IndexBuffer.h"
#include "Buffers/ConstantBuffer.h"
#include "Shaders.h"
#include "Device.h"

namespace fe
{
	class Graphics
	{
	public:
		bool Initialize(HWND hwnd, float width, float height);
		void OnResize(float width, float height);
		//void InitializeGameTexture(Texture* game_texture);
		void RestoreContext();

		void OnPreRender();
		void OnPostRender();
		void SetFullScreen(bool fullScreen);

		//Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView_engine;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;

		Device device;
		VertexShader pureVertexShader;
		PixelShader purePixelShader;

	private:
		bool InitializeDirectX(HWND hwnd, float width, float height);
		bool InitializeShaders();
		bool InitializeScene();

		void UpdateViewport(float width, float height);
		void UpdateRenderTargetView();
		void UpdateDepthStencilView(float width, float height);

		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
		//, renderTargetView_engine;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;
		//, depthStencilBuffer_engine;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
		Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;

		//CD3D11_VIEWPORT vieport_engine;
		CD3D11_VIEWPORT viewport;
		//Texture* game_texture;

		float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	};
}
