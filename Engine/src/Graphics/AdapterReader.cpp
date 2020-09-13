#include "pch.h"
#include "AdapterReader.h"
#include "Utils/COMException.h"

namespace fe
{
	std::vector<AdapterData> AdapterReader::adapters;

	std::vector<AdapterData> AdapterReader::GetAdapters()
	{
		if (adapters.size() > 0) //If already initialized
			return adapters;

		Microsoft::WRL::ComPtr<IDXGIFactory> pFactory;

		// Create a DXGIFactory object.
		HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(pFactory.GetAddressOf()));
		ERROR_IF_FAILED(hr, "Failed to create DXGIFactory for enumerating adapters.");

		IDXGIAdapter* pAdapter;
		UINT index = 0;
		while (SUCCEEDED(pFactory->EnumAdapters(index, &pAdapter)))
		{
			adapters.push_back(AdapterData(pAdapter));
			index += 1;
		}
		return adapters;
	}

	AdapterData::AdapterData(IDXGIAdapter* pAdapter)
	{
		this->pAdapter = pAdapter;
		HRESULT hr = pAdapter->GetDesc(&this->description);
		ERROR_IF_FAILED(hr, "Failed to Get Description for IDXGIAdapter.");
	}
}
