#pragma once

#include "Keyboard/Keyboard.h"
#include "Mouse/Mouse.h"

namespace fe
{
	class Input
	{
	public:
		Keyboard keyboard;
		Mouse mouse;

		void Update();
		bool ProcMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	};
}