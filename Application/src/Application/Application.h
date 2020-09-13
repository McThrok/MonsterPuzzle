#pragma once
#include <Engine.h>

class Application : public fe::Engine
{
public:
	bool Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class,
		int width, int height) override;

};
