#include "pch.h"
#include "TextureCollection.h"

namespace fe
{
	void TextureCollection::Initialize(Device* device)
	{
		this->device = device;
	}

	Texture* TextureCollection::Get(std::string name) const
	{
		auto t = names.find(name);
		return t != names.end() ? t->second : nullptr;
	}

	Texture* TextureCollection::GetByPath(std::string path) const
	{
		auto t = paths.find(path);
		return t != paths.end() ? t->second : nullptr;
	}

	void TextureCollection::Add(Texture* t, std::string name)
	{
		textures.push_back(std::make_shared<Texture>(*t));
		if (!name.empty())
			names[name] = t;
	}

	Texture* TextureCollection::AddFromFile(std::string path, std::string name, aiTextureType textureType)
	{
		Texture* t = new Texture(device->get(), PathConstants::TEXTURES_PATH + path, textureType);
		paths[path] = t;
		Add(t, name);
	
		return t;
	}

	Texture* TextureCollection::AddFromDesc(CD3D11_TEXTURE2D_DESC desc, std::string name, const sm::Color* colorData)
	{
		Texture* t = new Texture(device->get(), desc, colorData);
		Add(t, name);

		return t;
	}
}
