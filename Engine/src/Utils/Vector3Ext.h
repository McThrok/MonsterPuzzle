#pragma once

namespace fe
{
	namespace sm = DirectX::SimpleMath;

	class Vector3Ext
	{
	public:
		static const sm::Vector3 DEFAULT_FORWARD_VECTOR;
		static const sm::Vector3 DEFAULT_UP_VECTOR;
		static const sm::Vector3 DEFAULT_BACKWARD_VECTOR;
		static const sm::Vector3 DEFAULT_LEFT_VECTOR;
		static const sm::Vector3 DEFAULT_RIGHT_VECTOR;
	};
}
