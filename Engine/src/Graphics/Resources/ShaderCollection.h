#pragma once
#include "Graphics/Device.h"
#include "Graphics/Shaders.h"

namespace fe
{
	class ShaderCollection
	{
	private:
		Device* device;
		std::vector<std::shared_ptr<VertexShader>> VertexShaders;
		std::vector<std::shared_ptr<PixelShader>> PixelShaders;
		std::vector<std::shared_ptr<GeometryShader>> GeometryShaders;

		std::map<std::string, VertexShader*> namesVS;
		std::map<std::string, PixelShader*> namesPS;
		std::map<std::string, GeometryShader*> namesGS;

		std::map<std::wstring, VertexShader*> pathsVS;
		std::map<std::wstring, PixelShader*> pathsPS;
		std::map<std::wstring, GeometryShader*> pathsGS;

	public:
		void Initialize(Device* device);

		void Add(VertexShader* t, std::string name = "");
		void Add(PixelShader* t, std::string name = "");
		void Add(GeometryShader* t, std::string name = "");
		VertexShader* GetVS(std::string name) const;
		PixelShader* GetPS(std::string name) const;
		GeometryShader* GetGS(std::string name) const;
		VertexShader* GetVSByPath(std::wstring path) const;
		PixelShader* GetPSByPath(std::wstring path) const;
		GeometryShader* GetGSByPath(std::wstring path) const;
		VertexShader* AddFromFileVS(std::wstring path, D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT numElements, std::string name = "");
		PixelShader* AddFromFilePS(std::wstring path, std::string name = "");
		GeometryShader* AddFromFileGS(std::wstring path, std::string name = "");
	};
}