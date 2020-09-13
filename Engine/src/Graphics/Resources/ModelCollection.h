#pragma once
#include "Graphics/Device.h"
#include "Graphics/Model.h"
#include "Constants/PathConstants.h"

namespace fe
{
	class ModelCollection
	{
	private:
		Device* device;
		std::vector<std::shared_ptr<Model>> models;
		std::map<std::string, Model*> names;
		std::map<std::string, Model*> paths;

	public:
		void Initialize(Device* device);

		void Add(Model* t, std::string name = "");
		Model* Get(std::string name) const;
		Model* GetByPath(std::string path) const;
		Model* AddFromFile(std::string path, std::string name = "");
	};
}
