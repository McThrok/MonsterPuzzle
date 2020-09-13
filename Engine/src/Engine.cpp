#include "pch.h"
#include "Engine.h"
#include "Time/Time.h"
#include "Graphics/Components/ParticleSystem.h"

namespace fe
{
	Engine::~Engine()
	{
		for (int i = layers.size() - 1; i >= 0; i--)
			delete layers[i];
	}

	bool Engine::Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height)
	{
		timer.Start();

		if (!this->renderWindow.Initialize(this, hInstance, window_title, window_class, width, height))
			return false;

		if (!graphics.Initialize(this->renderWindow.GetHWND(), this->windowWidth, this->windowHeight))
			return false;

		resources.Initialize(&graphics.device);
		renderer.Initialize(&scene, &graphics.device, &resources);
		audioEngine.Initialize();

		LoadShaders();

		scene.camera.SetProjectionValues(windowWidth, windowHeight, 0.1f, 100.0f);

		return true;
	}

	void Engine::Start()
	{
		scene.OnStart();
	}

	bool Engine::ProcessMessages()
	{
		return this->renderWindow.ProcessMessages();
	}

	void Engine::Update()
	{
		UpdateTime();

		for (auto& layer : layers)
		{
			layer->ProcssKeyboardInput();
			layer->ProcssMouseInput();
			layer->Update();
		}

		scene.OnUpdate();
		input.Update();


		for (size_t i = 0; i < scene.gameObjects.size(); i++)
		{
			auto pss = scene.gameObjects[i]->GetComponents<ParticleSystem>();
			for (auto ps : pss)
				ps->Update(&graphics.device);
		}
	}

	void Engine::UpdateTime()
	{
		Time::deltaTime = timer.GetMilisecondsElapsed();
		Time::timeSinceLevelLoad += Time::deltaTime;

		timer.Restart();
	}

	void Engine::LoadShaders()
	{
		resources.Shaders.Add(&graphics.pureVertexShader, "PureShader");
		resources.Shaders.Add(&graphics.purePixelShader, "PureShader");
	}

	void Engine::WindowResized()
	{
		float width = static_cast<float>(this->windowWidth);
		float height = static_cast<float>(this->windowHeight);

		graphics.OnResize(width, height);
	}

	void Engine::RenderFrame()
	{
		this->graphics.OnPreRender();
		this->graphics.RestoreContext();
		this->renderer.Render();
		this->graphics.OnPostRender();
	}
}
