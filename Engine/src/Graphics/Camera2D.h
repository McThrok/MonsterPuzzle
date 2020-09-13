#pragma once
#include "GameObjects/GameObject.h"
#include "Components/Transform.h"

namespace fe
{

	class Camera2D
	{
	public:
		void SetProjectionValues(float width, float height, float nearZ, float farZ);
		const DirectX::SimpleMath::Matrix& GetProjectionMatrix() const { return this->projectionMatrix; }
		const DirectX::SimpleMath::Matrix& GetViewMatrix() const { return this->viewMatrix; }

	private:
		sm::Matrix projectionMatrix;
		sm::Matrix viewMatrix;

		//float pixelWidth;
		//float pixelHeight;
		//float aspectRatio;
		//float nearZ, farZ;
	};
}
