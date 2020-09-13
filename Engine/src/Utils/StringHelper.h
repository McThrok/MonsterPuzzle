#pragma once

namespace fe
{
	class StringHelper
	{
	public:
		static std::wstring StringToWide(std::string str);
		static std::string GetDirectoryFromPath(const std::string& filepath);
		static std::string GetFileExtension(const std::string& filename);
	};
}
