#include "pch.h"
#include "WindowContainer.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace fe
{
	WindowContainer::WindowContainer()
	{
		static bool raw_input_initialized = false;
		if (raw_input_initialized == false)
		{
			RAWINPUTDEVICE rid;

			rid.usUsagePage = 0x01; //Mouse
			rid.usUsage = 0x02;
			rid.dwFlags = 0;
			rid.hwndTarget = NULL;

			if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE)
			{
				ErrorLogger::Log(GetLastError(), "Failed to register raw input devices.");
				exit(-1);
			}

			raw_input_initialized = true;
		}
	}

	LRESULT WindowContainer::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
			return true;

		if (input.ProcMessage(uMsg, wParam, lParam))
			return 0;

		switch (uMsg)
		{
		case WM_SIZE:
		{
			this->windowWidth = LOWORD(lParam);
			this->windowHeight = HIWORD(lParam);

			if (this->windowWidth == 0 || this->windowHeight == 0)
			{
				minimized = true;
			}
			else
			{
				minimized = false;
				WindowResized();
			}
		}
		default:
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}
	void WindowContainer::Exit()
	{
		PostMessageA(renderWindow.GetHWND(), WM_CLOSE, 0, 0);
	}
	void WindowContainer::SetFullScreen(bool fullScreen)
	{
		graphics.SetFullScreen(fullScreen);
	}
}
