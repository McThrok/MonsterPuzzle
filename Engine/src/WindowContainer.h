#pragma once
#include "RenderWindow.h"
#include "Input/Input.h"
#include "Graphics/Graphics.h"

namespace fe
{
	class WindowContainer
	{
	public:
		WindowContainer();
		LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		void Exit();
		void SetFullScreen(bool fullScreen);

	protected:
		virtual void WindowResized() { }

		int windowWidth = 0;
		int windowHeight = 0;
		bool minimized = false;

		RenderWindow renderWindow;
		Input input;
		Graphics graphics;
	};
}
