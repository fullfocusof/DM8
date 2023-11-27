#pragma once
#include <iostream>
#include <fstream>

#include <Windows.h>
#include <conio.h>
#include <algorithm>

#include <list>
#include <queue>
#include <vector>

#define MAXV 1000

using namespace std;

class GraphInteraction
{
public:

	vector<vector<bool>> adjacencyM;
	int verts, edges;

	GraphInteraction();

	void printQuit();
	void Read_g();
	void Print_g();
	
	vector<vector<int>> BFSComps();
	void PrintComps(vector<vector<int>>& parents);

	vector<vector<int>> BFSPaths(int vert);
	void PrintPaths(vector<vector<int>>& paths, int vert);

	vector<int> isCyclicBFS();
	
	bool isColored(vector<int>& colors);

	void DFSComps(vector<int>& comp, vector<bool>& status, int curVert);
	vector<vector<int>> FindCompsDFS();

	bool DFSCycle(vector<int>& visited, vector<int>& path, vector<int>& parents, int curVert);
	vector<int> isCyclicDFS();
};