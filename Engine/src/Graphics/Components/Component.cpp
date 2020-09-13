#include "pch.h"
#include "Component.h"

namespace fe
{
	const std::size_t Component::Type = std::hash<std::string>()(TO_STRING(Component));

	Component::Component() : owner(nullptr) { }

	bool Component::IsClassType(const std::size_t classType) const
	{
		return classType == Type;
	}

	GameObject& Component::GetOwner() const
	{
		return *owner;
	}

	void Component::SetOwner(GameObject& newOwner)
	{
		owner = &newOwner;
	}
}
