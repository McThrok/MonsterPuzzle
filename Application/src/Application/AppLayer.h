#pragma once
#include <Graphics/Resources/ResourceCollection.h>
#include <Layers/Layer.h>
#include <Input/Input.h>
#include <Scene/Scene.h>
#include <unordered_map>
#include <Audio/AudioEngine.h>
#include "Graph.h"
#include "BagOfBugs.h"
#include "OpacityAnimation.h"
#include <Graphics/Components/AudioComponent.h>
#include <Engine.h>

namespace sm = DirectX::SimpleMath;

class AppLayer : public fe::Layer
{
public:
	AppLayer() : fe::Layer("game layer") { }

	bool Initialize(fe::ResourceCollection* resources, fe::Scene* scene, fe::Input* input, fe::Engine* engine, float width, float height);
	void ProcssMouseInput() override;
	void ProcssKeyboardInput() override;
	void Update() override;

private:
	bool fullScreen = false;
	bool keyTaken = false;

	float backgrondLayer = -5;
	float keyLayer = -0.5;
	float width, height;

	fe::Scene* scene;
	fe::Input* input;
	fe::ResourceCollection* resources;
	fe::Engine* engine;
	fe::AudioEngine* audio;

	fe::GameObject* gold;
	std::unique_ptr<Graph> graph;
	std::unique_ptr<BagOfBugs> bugs;
	fe::GameObject* key;
	std::vector<fe::GameObject*> fires;
	fe::AudioComponent* go;
	fe::AudioComponent* back;

	sm::Vector2 keyPosition = sm::Vector2(510, 345);
	sm::Vector2 backgroundSize= sm::Vector2(1024, 612);

	void InitScene();
	void InitBugs();
	void InitFire();
	void Reset();
	
	bool CheckHit(int idx, fe::MousePoint point);
	bool CheckHit(sm::Vector2 pos, fe::MousePoint point);
	void HandleLeftClick(fe::MousePoint point);

	std::unique_ptr<OpacityAnimation> showKey;
};
