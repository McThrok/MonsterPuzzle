// This class is based on 
// https://stackoverflow.com/questions/44105058/how-does-unitys-getcomponent-work?fbclid=IwAR2H6nTXS-XHkxpqDxaf2sFOXjksJwE0VZbaA5LChQXW94h3YolGhSgFH6A

#pragma once
#include "Graphics/Model.h"
#include "Graphics/Components/Transform.h"
#include "Graphics/Components/ParticleSystem.h"

namespace fe
{
	namespace sm = DirectX::SimpleMath;

	class GameObject
	{
	public:
		GameObject();
		GameObject(std::string name);
		virtual ~GameObject() {}

		virtual void OnUpdate();
		virtual void OnStart() { }

		bool active;
		std::string name;
		std::vector<GameObject*> children;
		std::vector<std::unique_ptr<Component>> components;

		void AddChild(GameObject* go);

		template<class ComponentType, typename... Args >
		void AddComponent(Args&& ... params);

		template<class ComponentType>
		ComponentType& GetComponent() const;

		template<class ComponentType>
		bool RemoveComponent();

		template<class ComponentType>
		std::vector< ComponentType*> GetComponents();

		template<class ComponentType>
		int RemoveComponents();

	private:
		static int count;
	};

	//***************
	// GameObject::AddComponent
	// perfect-forwards all params to the ComponentType constructor with the matching parameter list
	//***************
	template< class ComponentType, typename... Args >
	void GameObject::AddComponent(Args&& ... params) {
		components.emplace_back(std::make_unique< ComponentType >(std::forward< Args >(params)...));
		components.back()->SetOwner(*this);
	}

	//***************
	// GameObject::GetComponent
	// returns the first component that matches the template type
	// or that is derived from the template type
	//***************
	template< class ComponentType >
	ComponentType& GameObject::GetComponent() const {
		for (auto&& component : components) {
			if (component->IsClassType(ComponentType::Type))
				return *static_cast<ComponentType*>(component.get());
		}

		return *std::unique_ptr< ComponentType >(nullptr);
	}

	//***************
	// GameObject::RemoveComponent
	// returns true on successful removal
	// returns false if components is empty, or no such component exists
	// returns false if components is empty, or no such component exists
	//***************
	template< class ComponentType >
	bool GameObject::RemoveComponent() {
		if (components.empty())
			return false;

		auto& index = std::find_if(components.begin(),
			components.end(),
			[classType = ComponentType::Type](auto& component) {
			return component->IsClassType(classType);
		});

		bool success = index != components.end();

		if (success)
		{
			components[index]->SetOwner(nullptr);
			components.erase(index);
		}

		return success;
	}

	//***************
	// GameObject::GetComponents
	// returns a vector of pointers to the the requested component template type following the same match criteria as GetComponent
	//***************
	template< class ComponentType >
	std::vector< ComponentType* > GameObject::GetComponents() {
		std::vector< ComponentType* > componentsOfType;

		for (auto&& component : components) {
			if (component->IsClassType(ComponentType::Type))
				componentsOfType.emplace_back(static_cast<ComponentType*>(component.get()));
		}

		return componentsOfType;
	}

	//***************
	// GameObject::RemoveComponents
	// returns the number of successful removals, or 0 if none are removed
	//***************
	template< class ComponentType >
	int GameObject::RemoveComponents() {
		if (components.empty())
			return 0;

		int numRemoved = 0;
		bool success = false;

		do {
			auto& index = std::find_if(components.begin(),
				components.end(),
				[classType = ComponentType::Type](auto& component) {
				return component->IsClassType(classType);
			});

			success = index != components.end();

			if (success) {
				components[index]->SetOwner(nullptr);
				components.erase(index);
				++numRemoved;
			}
		} while (success);

		return numRemoved;
	}
}
