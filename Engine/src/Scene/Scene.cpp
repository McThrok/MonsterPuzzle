#include "pch.h"
#include "Scene.h"

namespace fe
{
	void Scene::OnStart()
	{
		for (size_t i = 0; i < gameObjects.size(); i++)
		{
			gameObjects[i]->OnStart();

			for (size_t child_idx = 0; child_idx < gameObjects[i]->children.size(); child_idx++)
				gameObjects[i]->children[child_idx]->OnStart();
		}
	}

	void Scene::OnUpdate()
	{
		for (size_t i = 0; i < gameObjects.size(); i++)
		{
			gameObjects[i]->OnUpdate();

			for (size_t child_idx = 0; child_idx < gameObjects[i]->children.size(); child_idx++)
				gameObjects[i]->children[child_idx]->OnUpdate();
		}
	}

	std::vector<GameObject*> Scene::GetActiveGameObjects()
	{
		std::vector<GameObject*> out;

		for (auto& go : gameObjects)
			TryAddActiveGO(go, out);

		return out;
	}

	void Scene::TryAddActiveGO(GameObject* go, std::vector<GameObject*>& activeGOs)
	{
		if (!go->active)
			return;

		activeGOs.push_back(go);

		for (auto& goChild : go->children)
			TryAddActiveGO(goChild, activeGOs);
	}

	Scene::~Scene()
	{
		for (size_t i = 0; i < gameObjects.size(); i++)
		{
			for (size_t child_idx = 0; child_idx < gameObjects[i]->children.size(); child_idx++)
				delete gameObjects[i]->children[child_idx];

			delete gameObjects[i];
		}
	}
}
