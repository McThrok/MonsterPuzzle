#include "AppLayer.h"
#include "Application.h"

bool Application::Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class,
	int width, int height)
{
	if (!Engine::Initialize(hInstance, window_title, window_class, width, height))
		return false;

	AppLayer* gameLayer = new AppLayer();
	gameLayer->Initialize(&resources, &scene, &input, this, width, height);

	AddLayer(gameLayer);

	return true;
}
