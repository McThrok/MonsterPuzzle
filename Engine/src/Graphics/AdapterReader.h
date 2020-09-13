#pragma once
#include "Utils\ErrorLogger.h"

namespace fe
{
	class AdapterData
	{
	public:
		AdapterData(IDXGIAdapter* pAdapter);
		IDXGIAdapter* pAdapter = nullptr;
		DXGI_ADAPTER_DESC description;
	};

	class AdapterReader
	{
	public:
		static std::vector<AdapterData> GetAdapters();
	private:
		static std::vector<AdapterData> adapters;
	};
}
