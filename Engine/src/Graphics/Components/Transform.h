#pragma once
#include "Component.h"

namespace fe
{
	class InspectorGui;
	namespace sm = DirectX::SimpleMath;

	class Transform : public Component
	{
		CLASS_DECLARATION(Transform)

	public:
		Transform() : Transform(sm::Vector3(0.0f, 0.0f, 0.0f)) { }
		Transform(const sm::Vector3& position) : Transform(position, sm::Vector3(0.0f, 0.0f, 0.0f),
			sm::Vector3(1.0f, 1.0f, 1.0f)) { }
		Transform(const sm::Vector3& position, const sm::Vector3& rotation, const sm::Vector3& scale);
		Transform(const Transform& t) : Transform(t.position, t.rotation, t.scale) { };
		Transform& operator=(const Transform& t);

		const sm::Vector3& GetPosition() const { return this->position; }
		const sm::Vector3& GetRotation() const { return this->rotation; }
		const sm::Vector3& GetScale() const { return this->scale; }

		sm::Vector3 GetForward(bool noY) const;
		sm::Vector3 GetRight(bool noY) const;
		sm::Vector3 GetBackward(bool noY) const;
		sm::Vector3 GetLeft(bool noY) const;
		sm::Matrix GetModelMatrix();

		void SetPosition(const sm::Vector3& pos) { this->position = pos; }
		void SetPosition(float x, float y, float z) { this->position = sm::Vector3(x, y, z); }
		void AdjustPosition(const sm::Vector3& pos) { this->position += pos; }
		void AdjustPosition(float x, float y, float z) { this->position += sm::Vector3(x, y, z); }

		void SetRotation(const sm::Vector3& rot) { this->rotation = rot; }
		void SetRotation(float x, float y, float z) { this->rotation = sm::Vector3(x, y, z); }
		void AdjustRotation(const sm::Vector3& rot) { this->rotation += rot; }
		void AdjustRotation(float x, float y, float z) { this->rotation += sm::Vector3(x, y, z); }

		void SetScale(const sm::Vector3& scale) { this->scale = scale; }
		void SetScale(float x, float y, float z) { this->scale = sm::Vector3(x, y, z); }

		void SetLookAtPos(sm::Vector3 lookAtPos);

		friend class InspectorGui;

	private:
		sm::Vector3 position;
		sm::Vector3 rotation;
		sm::Vector3 scale;
	};
}
