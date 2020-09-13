#include "Graph.h"

Graph::Graph()
{
	this->Connections =
	{
		{1,2},
		{0,2},
		{0,1,3,4,10},
		{2,4},
		{2,3,5,6,10,11},
		{4,7},
		{4,7},
		{5,6,8,9,14,15},
		{7,9},
		{7,8,15},

		{2,4,11,16},
		{4,10,12,13,16,18},
		{11,14},
		{11,14},
		{7,12,13,15,20,22},
		{7,9,14,22},

		{10,11,17,18},
		{16,18},
		{11,16,17,19},
		{18,20},
		{14,19,21,22},
		{20,22},
		{14,15,20,21,23,24},
		{22,24},
		{22,23},
	};

	Reset();
}

void Graph::SetNext(int idx)
{
	if (history.size() > 0)
	{
		int from = history[history.size() - 1];
		GetUsed(from, idx) = true;
	}
	history.push_back(idx);
}

std::vector<int> Graph::GetPossibilities()
{
	std::vector<int>result;

	if (history.size() == 0)
	{
		for (int i = 0; i < N; i++)
			result.push_back(i);
	}
	else
	{
		auto& cons = Connections[history[history.size() - 1]];
		int curr = GetCurret();

		for (auto& idx : cons)
			if (!GetUsed(curr, idx))
				result.push_back(idx);

	}
	return result;
}

int Graph::GetPrevious()
{
	if (history.size() < 2)
		return -1;

	return history[history.size() - 2];
}

int Graph::GetCurret()
{
	if (history.size() < 1)
		return -1;

	return history[history.size() - 1];
}

void Graph::RevertLast()
{
	if (history.size() > 1)
	{
		int from = history[history.size() - 1];
		int to = history[history.size() - 2];
		GetUsed(from, to) = false;
	}

	if (history.size() > 0)
		history.pop_back();
}

void Graph::Reset()
{
	this->history.clear();

	for (int i = 0; i < Connections.size(); i++)
		for (int j = 0; j < Connections[i].size(); j++)
		{
			int from = i;
			int to = Connections[i][j];
			GetUsed(from, to) = false;
		}
}

bool Graph::Finished()
{
	for (auto from : edgeUsed)
		for (auto to : from.second)
			if (!to.second)
				return false;

	return true;
}

bool& Graph::GetUsed(int from, int to)
{
	return edgeUsed[std::min(from, to)][std::max(from, to)];
}
