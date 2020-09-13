#pragma once
#include "Graphics/Texture.h"
#include "Graphics/Device.h"
#include "Constants/PathConstants.h"

namespace fe
{
	namespace sm = DirectX::SimpleMath;

	class TextureCollection
	{
	private:
		Device* device;
		std::vector<std::shared_ptr<Texture>> textures;
		std::map<std::string, Texture*> names;
		std::map<std::string, Texture*> paths;

	public:
		void Initialize(Device* device);

		void Add(Texture* t, std::string name = "");
		Texture* Get(std::string name) const;
		Texture* GetByPath(std::string path) const;
		Texture* AddFromFile(std::string path, std::string name = "", aiTextureType textureType = aiTextureType::aiTextureType_NONE);
		Texture* AddFromDesc(CD3D11_TEXTURE2D_DESC desc, std::string name = "", const sm::Color* colorData = nullptr);
	};
}
