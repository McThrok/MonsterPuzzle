#include "pch.h"
#include "Camera2D.h"
#include "Utils/Vector3Ext.h"

namespace fe
{
	//float Camera2D::pixelWidth = 0;
	//float Camera2D::pixelHeight = 0;

	void Camera2D::SetProjectionValues(float width, float height, float nearZ, float farZ)
	{
		//this->pixelWidth = width;
		//this->pixelHeight = height;
		//this->farZ = farZ;
		//this->nearZ = nearZ;
		//this->aspectRatio = width / height;
		//this->projectionMatrix = sm::Matrix::CreateOrthographicOffCenter(0.0f, width, height, 0.0f, nearZ, farZ);
		this->projectionMatrix = sm::Matrix::CreateOrthographicOffCenter(0.0f, width, 0.0f, height, nearZ, farZ);
		sm::Vector3 pos(0, -156/2, 0);
		this->viewMatrix = sm::Matrix::CreateLookAt(pos, pos - sm::Vector3::UnitZ, sm::Vector3::UnitY);
	}
}