#include "Application/Application.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	try
	{
		HRESULT hr = CoInitialize(NULL);
		ERROR_IF_FAILED(hr, "Failed to call CoInitialize.");

		Application game;
		if (game.Initialize(hInstance, "Saqqarah: The Pyramid of Puzzles", "MyWindowClass", 1024,768))
		{
			game.Start();
			while (game.ProcessMessages())
			{
				game.Update();
				game.RenderFrame();
			}
		}
	}
	catch (fe::COMException & exception)
	{
		fe::ErrorLogger::Log(exception);
		return -1;
	}

	return 0;
}
