#include "BagOfBugs.h"
#include <Graphics/Components/Transform.h>
#include <Graphics/Components/Component.h>

BagOfBugs::BagOfBugs(
	const std::vector<fe::GameObject*>& browns,
	const std::vector<fe::GameObject*>& blues,
	fe::GameObject* gold,
	const std::unordered_map<int, std::unordered_map<int, fe::GameObject*>>& edges
)
{
	this->bugPositions =
	{
		{129, 414}, {195, 478}, {266, 414}, {333, 478}, {408, 414},
		{512, 510}, {511, 426}, {615, 414}, {690, 478}, {757, 414},
		{246, 301}, {439, 298}, {511, 349}, {511, 222}, {584, 298},
		{777, 300}, {275, 164}, {341, 105}, {414, 165}, {511, 122},
		{609, 165}, {682, 105}, {748, 164}, {822, 103}, {881, 162}
	};


	for (auto& edgeFrom : edges)
	{
		for (auto& edgeTo : edgeFrom.second)
		{
			int from = edgeFrom.first;
			int to = edgeTo.first;
			this->edges[from][to] = &edgeTo.second->GetComponent<fe::Transform>();
			this->edges[to][from] = this->edges[from][to];
		}
	}

	for (int i = 0; i < N; i++)
	{
		this->browns.push_back(&browns[i]->GetComponent<fe::Transform>());
		this->blues.push_back(&blues[i]->GetComponent<fe::Transform>());
	}

	this->gold = &gold->GetComponent<fe::Transform>();

	Reset();
}

void BagOfBugs::Reset()
{
	sm::Vector3 scale = sm::Vector3(bugSize.x, bugSize.y, 1);

	gold->SetScale(scale);
	gold->SetRotation(0, 0, 0);
	gold->SetPosition(100, 100, bugLayer);
	Hide(gold);
	
	rotationCounts.clear();
	for (int i = 0; i < N; i++)
	{
		sm::Vector3 pos(bugPositions[i].x, bugPositions[i].y, bugLayer);
		browns[i]->SetPosition(pos);
		browns[i]->SetScale(scale);
		browns[i]->SetRotation(0, 0, 0);
		ShowBug(browns[i]);

		blues[i]->SetPosition(pos);
		blues[i]->SetScale(scale);
		blues[i]->SetRotation(0, 0, 0);
		Hide(blues[i]);

		rotationCounts.push_back(std::vector<sm::Vector3>());
	}

	for (auto& edgeFrom : edges)
		for (auto& edgeTo : edgeFrom.second)
		{
			int from = edgeFrom.first;
			int to = edgeTo.first;

			if (from > to)
				continue;

			auto a = bugPositions[from];
			auto b = bugPositions[to];
			auto tr = edgeTo.second;

			sm::Vector3 dir;
			sm::Vector3::UnitZ.Cross(sm::Vector3(b.x - a.x, b.y - a.y, 0), dir);
			dir.Normalize();
			float angle = acosf(sm::Vector3::Up.Dot(dir));

			sm::Vector3 crs;
			sm::Vector3::UnitY.Cross(dir, crs);
			if (crs.z < 0)
				angle *= -1;

			tr->SetRotation(0, 0, angle);
			tr->SetScale((a - b).Length(), this->edgeWidth, 1);

			sm::Vector3 pos((a.x + b.x) / 2, (a.y + b.y) / 2, edgeLayer);
			tr->SetPosition(pos);
			Hide(tr);
		}
}

void BagOfBugs::MoveGold(int prev, int next)
{
	if (prev != -1)
	{
		ShowBug(blues[prev]);
		ShowEdge(edges[prev][next]);
		gold->SetRotation(GetRotation(prev, next));
		blues[prev]->SetRotation(gold->GetRotation());
		rotationCounts[prev].push_back(blues[prev]->GetRotation());
	}

	Hide(browns[next]);
	Hide(blues[next]);
	gold->SetPosition({ bugPositions[next].x , bugPositions[next].y , bugLayer });
	ShowBug(gold);
}

void BagOfBugs::RevertGold(int prev, int next)
{

	if (rotationCounts[next].size() == 0)
	{
		ShowBug(browns[next]);
	}
	else
	{
		blues[next]->SetRotation(rotationCounts[next][rotationCounts[next].size() - 1]);
		ShowBug(blues[next]);
	}


	if (prev == -1)
	{
		Hide(gold);
		gold->SetRotation(0, 0, 0);
	}
	else
	{
		Hide(blues[prev]);
		rotationCounts[prev].pop_back();
		gold->SetRotation(blues[prev]->GetRotation());
		gold->SetPosition({ bugPositions[prev].x , bugPositions[prev].y , bugLayer });
		Hide(edges[prev][next]);
	}
}

void BagOfBugs::HideBoxElemets()
{
	Hide(blues[12]);
	Hide(edges[11][12]);
	Hide(edges[12][14]);
}

sm::Vector2 BagOfBugs::GetPosition(int idx)
{
	return bugPositions[idx];
}

void BagOfBugs::Hide(fe::Transform* transform)
{
	transform->GetOwner().active = false;
	//auto pos = transform->GetPosition();
	//pos.z = hidden;
	//transform->SetPosition(pos);
}

void BagOfBugs::ShowBug(fe::Transform* transform)
{
	transform->GetOwner().active = true;
	//auto pos = transform->GetPosition();
	//pos.z = visible;
	//transform->SetPosition(pos);
}
void BagOfBugs::ShowEdge(fe::Transform* transform)
{
	transform->GetOwner().active = true;
	//auto pos = transform->GetPosition();
	//pos.z = visible - 0.1f;
	//transform->SetPosition(pos);
}

sm::Vector3 BagOfBugs::GetRotation(int prev, int next)
{
	auto a = bugPositions[prev];
	auto b = bugPositions[next];

	sm::Vector3 dir(b.x - a.x, b.y - a.y, 0);
	dir.Normalize();
	float angle = acosf(sm::Vector3::Up.Dot(dir));

	sm::Vector3 crs;
	sm::Vector3::UnitY.Cross(dir, crs);
	if (crs.z < 0)
		angle *= -1;


	return sm::Vector3(0, 0, angle);
}
