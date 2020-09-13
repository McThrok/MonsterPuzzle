#pragma once
#include <Graphics/Components/MeshRenderer.h>
#include <Graphics/Resources/ResourceCollection.h>
#include <Graphics/Components/Transform.h>
#include <Graphics/GameObjects/GameObject.h>
#include <unordered_map>
#include "Graph.h"

namespace sm = DirectX::SimpleMath;

class BagOfBugs
{
public:
	BagOfBugs(
		const std::vector<fe::GameObject*>& browns, 
		const std::vector<fe::GameObject*>& blues, 
		fe::GameObject* gold,
		const std::unordered_map<int, std::unordered_map<int, fe::GameObject*>>& edges
	);
	void Reset();

	void MoveGold(int prev, int next);
	void RevertGold(int prev, int next);
	void HideBoxElemets();
	sm::Vector2 GetPosition(int idx);


private:
	static const int N = Graph::N;
	const float edgeWidth = 30;
	const float bugLayer = -2;
	const float edgeLayer = bugLayer - 0.1;
	sm::Vector2 bugSize = sm::Vector2(50, 50);

	std::unordered_map<int, std::unordered_map<int, fe::Transform*>> edges;
	std::vector<fe::Transform*> browns;
	std::vector<fe::Transform*> blues;
	fe::Transform* gold;
	std::vector<std::vector<sm::Vector3>> rotationCounts;
	std::vector<sm::Vector2> bugPositions;

	void Hide(fe::Transform* transform);
	void ShowBug(fe::Transform* transform);
	void ShowEdge(fe::Transform* transform);
	sm::Vector3 GetRotation(int prev, int next);
	
};

