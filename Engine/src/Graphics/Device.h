#pragma once
#include "AdapterReader.h"

namespace fe
{
	class Device
	{
	public:
		void Initialize(HWND hwnd, int width, int height);

		ID3D11Device* operator->() const { return device.Get(); }
		operator ID3D11Device* () const { return device.Get(); }
		ID3D11Device* get() const { return device.Get(); }
		ID3D11DeviceContext* context() const { return deviceContext.Get(); }
		IDXGISwapChain* swapChain() const { return swapchain.Get(); }

	private:
		Microsoft::WRL::ComPtr<ID3D11Device> device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain;
	};
}
