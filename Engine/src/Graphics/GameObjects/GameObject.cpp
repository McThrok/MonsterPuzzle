#include "pch.h"
#include "GameObject.h"

namespace fe
{
	GameObject::GameObject()
	{
		this->name = "GameObject" + std::to_string(count++);
		active = true;
	}

	GameObject::GameObject(std::string name)
	{
		this->name = name;
		active = true;
	}

	void GameObject::AddChild(GameObject* go)
	{
		children.push_back(go);
	}

	int GameObject::count = 0;

	void GameObject::OnUpdate()
	{
	}
}
