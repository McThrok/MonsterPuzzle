#pragma once
#include "TextureCollection.h"
#include "ShaderCollection.h"
#include "MeshCollection.h"
#include "ModelCollection.h"

namespace fe
{
	class ResourceCollection
	{
	public:
		void Initialize(Device* device);

		TextureCollection Textures;
		ShaderCollection Shaders;
		MeshCollection Meshes;
		ModelCollection Models;
	};
}
