#include "pch.h"
#include "ResourceCollection.h"

namespace fe
{
	void ResourceCollection::Initialize(Device* device)
	{
		Textures.Initialize(device);
		Shaders.Initialize(device);
		Models.Initialize(device);
		Meshes.Initialize(device);
	}
}
