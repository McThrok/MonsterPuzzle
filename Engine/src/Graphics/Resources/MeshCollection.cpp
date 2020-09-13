#include "pch.h"
#include "MeshCollection.h"

namespace fe
{
	void MeshCollection::Initialize(Device* device)
	{
		this->device = device;
	}
	Mesh* MeshCollection::Get(std::string name) const
	{
		auto t = names.find(name);
		return t != names.end() ? t->second : nullptr;
	}

	void MeshCollection::AddFromData(std::vector<VertexPDS>& vertices, 
		std::vector<DWORD>& indices, std::string name)
	{
		meshes.push_back(std::make_shared<Mesh>(device->get(), device->context(), 
			vertices.data(), sizeof(VertexPDS), vertices.size(), indices));
		
		if (!name.empty())
			names[name] = meshes[meshes.size() - 1].get();
	}

	void MeshCollection::Add(Mesh* t, std::string name)
	{
		meshes.push_back(std::make_shared<Mesh>(*t));
		if (!name.empty())
			names[name] = t;
	}
}
