#include "pch.h"
#include "Transform.h"
#include "Utils/Vector3Ext.h"

namespace fe
{
	CLASS_DEFINITION(Component, Transform)

	Transform::Transform(const sm::Vector3& position, const sm::Vector3& rotation, const sm::Vector3& scale) : Component()
	{
		this->position = position;
		this->rotation = rotation;
		this->scale = scale;
	}

	Transform& Transform::operator=(const Transform& t)
	{
		this->position = t.position;
		this->rotation = t.rotation;
		this->scale = t.scale;

		return *this;
	}

	sm::Vector3 Transform::GetForward(bool noY) const
	{
		sm::Matrix vecRotationMatrix = noY ? DirectX::XMMatrixRotationRollPitchYaw(0.0f, rotation.y, 0.0f) :
			DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, 0.0f);

		return XMVector3TransformCoord(Vector3Ext::DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
	}

	sm::Vector3 Transform::GetRight(bool noY) const
	{
		sm::Matrix vecRotationMatrix = noY ? DirectX::XMMatrixRotationRollPitchYaw(0.0f, rotation.y, 0.0f) :
			DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, 0.0f);

		return XMVector3TransformCoord(Vector3Ext::DEFAULT_RIGHT_VECTOR, vecRotationMatrix);
	}

	sm::Vector3 Transform::GetBackward(bool noY) const
	{
		sm::Matrix vecRotationMatrix = noY ? DirectX::XMMatrixRotationRollPitchYaw(0.0f, rotation.y, 0.0f) :
			DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, 0.0f);

		return XMVector3TransformCoord(Vector3Ext::DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
	}

	sm::Vector3 Transform::GetLeft(bool noY) const
	{
		sm::Matrix vecRotationMatrix = noY ? DirectX::XMMatrixRotationRollPitchYaw(0.0f, rotation.y, 0.0f) :
			DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, 0.0f);

		return XMVector3TransformCoord(Vector3Ext::DEFAULT_LEFT_VECTOR, vecRotationMatrix);
	}

	sm::Matrix Transform::GetModelMatrix()
	{
		return DirectX::XMMatrixScaling(this->scale.x, this->scale.y, this->scale.z) *
			DirectX::XMMatrixRotationRollPitchYaw(this->rotation.x, this->rotation.y, this->rotation.z) *
			DirectX::XMMatrixTranslation(this->position.x, this->position.y, this->position.z);
	}

	void Transform::SetLookAtPos(sm::Vector3 lookAtPos)
	{
		//Verify that look at pos is not the same as cam pos. They cannot be the same as that wouldn't make sense and would result in undefined behavior.
		if (lookAtPos.x == this->position.x && lookAtPos.y == this->position.y && lookAtPos.z == this->position.z)
			return;

		lookAtPos.x = this->position.x - lookAtPos.x;
		lookAtPos.y = this->position.y - lookAtPos.y;
		lookAtPos.z = this->position.z - lookAtPos.z;

		float pitch = 0.0f;
		if (lookAtPos.y != 0.0f)
		{
			const float distance = sqrt(lookAtPos.x * lookAtPos.x + lookAtPos.z * lookAtPos.z);
			pitch = atan(lookAtPos.y / distance);
		}

		float yaw = 0.0f;
		if (lookAtPos.x != 0.0f)
		{
			yaw = atan(lookAtPos.x / lookAtPos.z);
		}
		if (lookAtPos.z > 0)
			yaw += DirectX::XM_PI;

		this->SetRotation(pitch, yaw, 0.0f);
	}
}
