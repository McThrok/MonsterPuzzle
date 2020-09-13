#pragma once
#include "Graphics/Device.h"
#include "Graphics/Mesh.h"

namespace fe
{
	class MeshCollection
	{
	private:
		Device* device;
		std::vector<std::shared_ptr<Mesh>> meshes;
		std::map<std::string, Mesh*> names;

	public:
		void Initialize(Device* device);

		void Add(Mesh* t, std::string name = "");
		Mesh* Get(std::string name) const;
		void AddFromData(std::vector<VertexPDS>& vertices,
			std::vector<DWORD>& indices, std::string name);
	};
}
