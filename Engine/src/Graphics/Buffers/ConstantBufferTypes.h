#pragma once
#include "pch.h"

namespace fe
{
	// VERTEX SHADER

	/// <summary>
	/// Constant Buffer used in Vertex Shader under index 0
	/// </summary>
	struct CB_VS_mvpMatrices
	{
		DirectX::SimpleMath::Matrix modelMatrix;
		DirectX::SimpleMath::Matrix viewMatrix;
		DirectX::SimpleMath::Matrix invViewMatrix;
		DirectX::SimpleMath::Matrix projMatrix;
	};

	// PIXEL SHADER

	/// <summary>
	/// Constant Buffer used in Pixel Shader under index 0
	/// </summary>
	struct CB_PS_color
	{
		DirectX::SimpleMath::Vector4 color;
	};
}
