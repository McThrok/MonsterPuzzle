#include "pch.h"
#include "ModelCollection.h"

namespace fe
{
	void ModelCollection::Initialize(Device* device)
	{
		this->device = device;
	}
	Model* ModelCollection::Get(std::string name) const
	{
		auto t = names.find(name);
		return t != names.end() ? t->second : nullptr;
	}

	Model* ModelCollection::GetByPath(std::string path) const
	{
		auto t = paths.find(path);
		return t != paths.end() ? t->second : nullptr;
	}

	void ModelCollection::Add(Model* m, std::string name)
	{
		models.push_back(std::make_shared<Model>(*m));
		if (!name.empty())
			names[name] = m;
	}

	Model* ModelCollection::AddFromFile(std::string path, std::string name)
	{
		Model* m = new Model();
		m->Initialize(PathConstants::MODELS_PATH + path, device->get(), device->context());

		paths[path] = m;
		Add(m, name);

		return m;
	}
}
