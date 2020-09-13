#include "pch.h"
#include "ShaderCollection.h"

namespace fe
{
	void ShaderCollection::Initialize(Device* device)
	{
		this->device = device;
	}

	void ShaderCollection::Add(VertexShader* t, std::string name)
	{
		VertexShaders.push_back(std::make_shared<VertexShader>(*t));
		if (!name.empty())
			namesVS[name] = t;
	}
	void ShaderCollection::Add(PixelShader* t, std::string name)
	{
		PixelShaders.push_back(std::make_shared<PixelShader>(*t));
		if (!name.empty())
			namesPS[name] = t;
	}
	void ShaderCollection::Add(GeometryShader* t, std::string name)
	{
		GeometryShaders.push_back(std::make_shared<GeometryShader>(*t));
		if (!name.empty())
			namesGS[name] = t;
	}
	VertexShader* ShaderCollection::GetVS(std::string name) const
	{
		auto t = namesVS.find(name);
		return t != namesVS.end() ? t->second : nullptr;
	}
	PixelShader* ShaderCollection::GetPS(std::string name) const
	{
		auto t = namesPS.find(name);
		return t != namesPS.end() ? t->second : nullptr;
	}
	GeometryShader* ShaderCollection::GetGS(std::string name) const
	{
		auto t = namesGS.find(name);
		return t != namesGS.end() ? t->second : nullptr;
	}
	VertexShader* ShaderCollection::GetVSByPath(std::wstring path) const
	{
		auto t = pathsVS.find(path);
		return t != pathsVS.end() ? t->second : nullptr;
	}
	PixelShader* ShaderCollection::GetPSByPath(std::wstring path) const
	{
		auto t = pathsPS.find(path);
		return t != pathsPS.end() ? t->second : nullptr;
	}
	GeometryShader* ShaderCollection::GetGSByPath(std::wstring path) const
	{
		auto t = pathsGS.find(path);
		return t != pathsGS.end() ? t->second : nullptr;
	}
	VertexShader* ShaderCollection::AddFromFileVS(std::wstring path, D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT numElements, std::string name)
	{
		auto shader = new VertexShader();
		pathsVS[path] = shader;
		shader->Initialize(device->get(), path, layoutDesc, numElements);
		Add(shader, name);

		return shader;
	}
	PixelShader* ShaderCollection::AddFromFilePS(std::wstring path, std::string name)
	{
		auto shader = new PixelShader();
		pathsPS[path] = shader;
		shader->Initialize(device->get(), path);
		Add(shader, name);

		return shader;
	}
	GeometryShader* ShaderCollection::AddFromFileGS(std::wstring path, std::string name)
	{
		auto shader = new GeometryShader();
		pathsGS[path] = shader;
		shader->Initialize(device->get(), path);
		Add(shader, name);

		return shader;
	}
}
