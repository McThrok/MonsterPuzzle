#pragma once
#include <vector>
#include <numeric>
#include <unordered_map>
#include <algorithm> 

class Graph
{
public:
	static const int N = 25;

	Graph();

	void SetNext(int idx);
	std::vector<int> GetPossibilities();
	int GetPrevious();
	int GetCurret();
	void RevertLast();
	void Reset();
	bool Finished();

	std::vector<std::vector<int>> Connections;

private:
	std::unordered_map<int, std::unordered_map<int, bool>> edgeUsed;
	std::vector<int> history;
	bool& GetUsed(int from, int to);
};

