#include "pch.h"
#include "Vector3Ext.h"

namespace fe
{
	const sm::Vector3 Vector3Ext::DEFAULT_FORWARD_VECTOR = sm::Vector3(0.0f, 0.0f, 1.0f);
	const sm::Vector3 Vector3Ext::DEFAULT_UP_VECTOR = sm::Vector3(0.0f, 1.0f, 0.0f);
	const sm::Vector3 Vector3Ext::DEFAULT_BACKWARD_VECTOR = sm::Vector3(0.0f, 0.0f, -1.0f);
	const sm::Vector3 Vector3Ext::DEFAULT_LEFT_VECTOR = sm::Vector3(-1.0f, 0.0f, 0.0f);
	const sm::Vector3 Vector3Ext::DEFAULT_RIGHT_VECTOR = sm::Vector3(1.0f, 0.0f, 0.0f);
}
