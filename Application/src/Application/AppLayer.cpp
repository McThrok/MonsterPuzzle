#include <math.h>
#include <time.h>
#include <Graphics/Components/MeshRenderer.h>
#include <Graphics/Components/ParticleSystem.h>
#include <Graphics/Components/ColorComponent.h>
#include <Time/Time.h>
#include "AppLayer.h"
#include <Audio/AudioEngine.h>
#include <Engine.h>

bool AppLayer::Initialize(fe::ResourceCollection* resources, fe::Scene* scene, fe::Input* input, fe::Engine* engine, float width, float height)
{
	this->width = width;
	this->height = height;
	this->scene = scene;
	this->input = input;
	this->resources = resources;
	this->engine = engine;
	this->audio = &engine->audioEngine;

	graph = std::make_unique<Graph>();

	resources->Models.AddFromFile("square.obj", "Square");

	InitScene();
	InitBugs();
	InitFire();

	return true;
}

void AppLayer::InitFire()
{
	auto pt = resources->Textures.AddFromFile("particle.png");
	auto pcbt = resources->Textures.AddFromFile("particleColorBlur.png");

	std::vector<sm::Vector3> firePos =
	{ 
		{300, 800, -3.5},
		{400, 1400, -3.5},
		{700, 1100, -3.5},
		{800, 1550, -3.5},
		{1200, 950, -3.5},
		{ 1300, 1850, -3.5 },
		{1500, 1250, -3.5},
		{1600, 1700, -3.5}
	};

	for (size_t i = 0; i < firePos.size(); i++)
	{
		auto fire = new fe::GameObject("fire");

		fire->AddComponent<fe::Transform>(firePos[i]);
		fire->AddComponent<fe::ParticleSystem>(75, pt, pcbt, firePos[i]);
		scene->gameObjects.push_back(fire);
		fires.push_back(fire);
	}
}
void AppLayer::InitScene()
{
	{
		auto background = new fe::GameObject("Background");

		fe::Transform transform({ backgroundSize.x / 2, backgroundSize.y / 2, backgrondLayer });
		transform.SetScale(width, height, 1);
		background->AddComponent<fe::Transform>(transform);

		background->AddComponent<fe::AudioComponent>("background", audio, 0.15, true);
		background->AddComponent<fe::MeshRenderer>(
			resources->Models.Get("Square"),
			resources->Shaders.GetVS("PureShader"),
			resources->Shaders.GetPS("PureShader"),
			resources->Textures.AddFromFile("night.png"));

		scene->gameObjects.push_back(background);
		background->GetComponent<fe::AudioComponent>().Play();
	}

	{
		auto edges = new fe::GameObject("Edges");

		fe::Transform transform({ backgroundSize.x / 2, backgroundSize.y / 2, -3 });
		transform.SetScale(backgroundSize.x, backgroundSize.y, 1);
		edges->AddComponent<fe::Transform>(transform);

		edges->AddComponent<fe::ColorComponent>(sm::Vector4(1, 1, 1, 1));
		edges->AddComponent<fe::MeshRenderer>(
			resources->Models.Get("Square"),
			resources->Shaders.GetVS("PureShader"),
			resources->Shaders.GetPS("PureShader"),
			resources->Textures.AddFromFile("background.png"));

		scene->gameObjects.push_back(edges);
	}

	{
		key = new fe::GameObject("Key");

		fe::Transform transform({ keyPosition.x, keyPosition.y, keyLayer });
		transform.SetScale(130, 130, 1);
		key->AddComponent<fe::Transform>(transform);

		key->AddComponent<fe::ColorComponent>(sm::Vector4(1, 1, 1, 0));
		key->AddComponent<fe::AudioComponent>("gold", audio);
		key->AddComponent<fe::MeshRenderer>(
			resources->Models.Get("Square"),
			resources->Shaders.GetVS("PureShader"),
			resources->Shaders.GetPS("PureShader"),
			resources->Textures.AddFromFile("key.png"));

		key->active = false;
		scene->gameObjects.push_back(key);

		showKey = std::make_unique<OpacityAnimation>(&key->GetComponent<fe::ColorComponent>(), 1000, 0, 1, 650);
	}
}

void AppLayer::InitBugs()
{
	resources->Textures.AddFromFile("brown.png", "Brown");
	resources->Textures.AddFromFile("blue.png", "Blue");
	resources->Textures.AddFromFile("edge.bmpx", "Edge");

	std::vector<fe::GameObject*> blues;
	std::vector<fe::GameObject*> browns;

	for (int i = 0; i < Graph::N; i++)
	{
		{
			fe::GameObject* go = new fe::GameObject("Blue");

			go->AddComponent<fe::Transform>();
			go->AddComponent<fe::MeshRenderer>(
				resources->Models.Get("Square"),
				resources->Shaders.GetVS("PureShader"),
				resources->Shaders.GetPS("PureShader"),
				resources->Textures.Get("Blue"));

			blues.push_back(go);
			scene->gameObjects.push_back(go);
		}
		{
			fe::GameObject* go = new fe::GameObject("Brown");

			go->AddComponent<fe::Transform>();
			go->AddComponent<fe::MeshRenderer>(
				resources->Models.Get("Square"),
				resources->Shaders.GetVS("PureShader"),
				resources->Shaders.GetPS("PureShader"),
				resources->Textures.Get("Brown"));

			browns.push_back(go);
			scene->gameObjects.push_back(go);
		}
	}

	{
		gold = new fe::GameObject("Gold");
		gold->AddComponent<fe::AudioComponent>("go", audio);
		gold->AddComponent<fe::AudioComponent>("back", audio);
		gold->AddComponent<fe::Transform>();
		gold->AddComponent<fe::MeshRenderer>(
			resources->Models.Get("Square"),
			resources->Shaders.GetVS("PureShader"),
			resources->Shaders.GetPS("PureShader"),
			resources->Textures.AddFromFile("gold.png"));

		scene->gameObjects.push_back(gold);

		auto audios = gold->GetComponents<fe::AudioComponent>();
		if (audios[0]->soundName != "go")
			std::swap(audios[0], audios[1]);

		go = audios[0];
		back = audios[1];
	}

	std::unordered_map<int, std::unordered_map<int, fe::GameObject*>>edges;

	for (int i = 0; i < graph->Connections.size(); i++)
		for (int j = 0; j < graph->Connections[i].size(); j++)
		{
			int from = i;
			int to = graph->Connections[i][j];
			if (from > to)
				continue;

			fe::GameObject* go = new fe::GameObject("Edge");

			go->AddComponent<fe::Transform>();
			go->AddComponent<fe::MeshRenderer>(
				resources->Models.Get("Square"),
				resources->Shaders.GetVS("PureShader"),
				resources->Shaders.GetPS("PureShader"),
				resources->Textures.Get("Edge"));

			edges[from][to] = go;
			scene->gameObjects.push_back(go);
		}

	this->bugs = std::make_unique<BagOfBugs>(browns, blues, gold, edges);
}

void AppLayer::Reset()
{
	//box->active = false;
	key->active = false;
	keyTaken = false;
	graph->Reset();
	bugs->Reset();
	showKey->Reset();
	key->GetComponent<fe::ColorComponent>().Color.w = 0;
}

void AppLayer::ProcssMouseInput()
{
	while (!input->mouse.EventBufferIsEmpty())
	{
		fe::MouseEvent me = input->mouse.ReadEvent();
		if (me.GetType() == fe::MouseEvent::EventType::LPress)
		{
			auto pos = input->mouse.GetPos();
			//pos.y = 746 - pos.y - 65;
			pos.y = -pos.y + (backgroundSize.y + height) / 2;
			HandleLeftClick(pos);
		}
	}
}

void AppLayer::ProcssKeyboardInput()
{
	while (!input->keyboard.KeyBufferIsEmpty())
	{
		fe::KeyboardEvent ke = input->keyboard.ReadKey();
		if (ke.IsPress() && ke.GetKeyCode() == 'R')
			Reset();
		if (ke.IsPress() && ke.GetKeyCode() == 'Q')
			engine->Exit();
		if (ke.IsPress() && ke.GetKeyCode() == 'F')
			engine->SetFullScreen(fullScreen = !fullScreen);
	}
}

void AppLayer::Update()
{
	showKey->Update();

	float speed = 0.15;
	auto dir = sm::Vector3(1, 1, 0);
	dir.Normalize();

	for (size_t i = 0; i < fires.size(); i++)
	{
		auto& ps = fires[i]->GetComponent<fe::ParticleSystem>();
		if (ps.m_emitterPos.y < -100)
			ps.m_emitterPos.y += 800;

		if (ps.m_emitterPos.x < 0)
			ps.m_emitterPos.x += 1100;

		ps.m_emitterPos -= dir * speed * fe::Time::deltaTime;
		ps.EMITTER_DIR = dir;
	}
}

bool AppLayer::CheckHit(int idx, fe::MousePoint point)
{
	return CheckHit(bugs->GetPosition(idx), point);
}
bool AppLayer::CheckHit(sm::Vector2 pos, fe::MousePoint point)
{
	float clickRange = 45;
	pos.x -= point.x;
	pos.y -= point.y;

	return pos.LengthSquared() <= clickRange * clickRange;
}

void AppLayer::HandleLeftClick(fe::MousePoint point)
{
	if (graph->Finished())
	{
		if (keyTaken)
		{
			Reset();
		}
		else if (CheckHit(keyPosition, point))
		{
			key->GetComponent<fe::AudioComponent>().Play();
			key->active = false;
			keyTaken = true;
		}
		return;
	}

	int previous = graph->GetPrevious();
	int current = graph->GetCurret();
	if (previous != -1 && CheckHit(previous, point))
	{
		graph->RevertLast();
		sm::Vector2 pos = bugs->GetPosition(previous);
		bugs->RevertGold(previous, current);
		back->Play();
	}
	else if (previous == -1 && current != -1 && CheckHit(current, point))
	{
		graph->RevertLast();
		bugs->RevertGold(-1, current);
	}
	else
	{
		auto possibilities = graph->GetPossibilities();
		for (int& idx : possibilities)
		{
			if (CheckHit(idx, point))
			{
				sm::Vector2 pos = bugs->GetPosition(idx);
				bugs->MoveGold(current, idx);
				graph->SetNext(idx);
				if (current != -1)
					go->Play();

				break;
			}
		}
	}

	if (graph->Finished())
	{
		//box->active = true;
		key->active = true;
		//box->GetComponent<fe::AudioComponent>().Play();
		//bugs->HideBoxElemets();	
		showKey->Run();
	}
}
