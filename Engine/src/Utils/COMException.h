#pragma once
#include "pch.h"
#include "StringHelper.h"

#define ERROR_IF_FAILED( hr, msg ) if( FAILED( hr ) ) throw fe::COMException( hr, msg, __FILE__, __FUNCTION__, __LINE__ )

#define ERROR_IF_TRUE( value, msg ) if(value) throw fe::COMException( E_FAIL, msg, __FILE__, __FUNCTION__, __LINE__ )
#define ERROR_IF_FALSE( value, msg ) if(!value) throw fe::COMException( E_FAIL, msg, __FILE__, __FUNCTION__, __LINE__ )

namespace fe
{
	class COMException
	{
	public:
		COMException(HRESULT hr, const std::wstring& msg, const std::string& file, const std::string& function, int line)
		{
			_com_error error(hr);
			whatmsg = L"Msg: " + msg + L"\n";
			whatmsg += error.ErrorMessage();
			whatmsg += L"\nFile: " + StringHelper::StringToWide(file);
			whatmsg += L"\nFunction: " + StringHelper::StringToWide(function);
			whatmsg += L"\nLine: " + StringHelper::StringToWide(std::to_string(line));
		}

		COMException(HRESULT hr, const std::string& msg, const std::string& file, const std::string& function, int line)
			:COMException(hr, StringHelper::StringToWide(std::string(msg)), file, function, line)
		{}

		const wchar_t* what() const
		{
			return whatmsg.c_str();
		}
	private:
		std::wstring whatmsg;
	};
}
