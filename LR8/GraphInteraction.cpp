#include "GraphInteraction.h"

GraphInteraction::GraphInteraction()
{

}

void GraphInteraction::printQuit()
{
	cout << endl << endl << "Backspace - возврат в меню";

	int answ = _getch();
	bool press = false;

	while (!press)
	{
		if (answ == 8)
		{
			press = true;
			break;
		}
		else
		{
			answ = _getch();
		}
	}

	system("cls");
}

void GraphInteraction::Read_g()
{
	adjacencyM.resize(MAXV, vector<bool>(MAXV));

	ifstream ifs("data.txt");
	if (ifs.is_open())
	{
		ifs >> verts;
		ifs >> edges;
		while (!ifs.eof())
		{
			int v, u;
			ifs >> v;
			ifs >> u;
			adjacencyM[u][v] = true;
			adjacencyM[v][u] = true;
		}
		cout << "Данные успешно считаны";
	}
	else
	{
		cerr << "Ошибка открытия файла";
	}
	ifs.close();
}

void GraphInteraction::Print_g()
{
	for (int i = 1; i <= verts; i++)
	{
		cout << i << " вершина: ";
		vector<int> temp;
		for (int j = 1; j <= verts; j++)
		{
			if (adjacencyM[i][j])
			{
				temp.push_back(j);
				cout << j << " ";
			}
		}
		if (temp.empty())
		{
			cout << "<нет смежных вершин>";
		}
		cout << endl;
	}
}

void GraphInteraction::PrintComps(vector<vector<int>>& comps)
{
	for (int i = 0; i < comps.size(); i++)
	{
		cout << i + 1 << " компонент: ";
		for (auto& el : comps[i])
		{
			cout << el << " ";
		}
		if (comps[i].empty())
		{
			cout << "<нет вершин>";
		}
		cout << endl;
	}
}

vector<vector<int>> GraphInteraction::BFSComps()
{
	queue<int> qTemp;
	vector<bool> status(verts + 1);
	vector<vector<int>> result;

	int curNum = 1;
	while (curNum <= verts)
	{
		if (!status[curNum])
		{
			status[curNum] = true;
			qTemp.push(curNum);
			vector<int> temp = {curNum};
			while (!qTemp.empty())
			{
				int curVert = qTemp.front();
				qTemp.pop();

				for (int j = 1; j <= verts; j++)
				{
					if (adjacencyM[curVert][j] && !status[j])
					{
						status[j] = true;
						qTemp.push(j);
						temp.push_back(j);
					}					
				}
			}
			result.push_back(temp);
		}
		curNum++;
	}

	return result;
}

vector<vector<int>> GraphInteraction::BFSPaths(int vert)
{
	queue<int> qTemp;
	vector<bool> status(verts + 1);
	vector<vector<int>> result(verts + 1);

	status[vert] = true;
	qTemp.push(vert);
	result[vert].push_back(vert);

	while (!qTemp.empty())
	{
		int curVert = qTemp.front();
		qTemp.pop();

		for (int j = 1; j <= verts; j++)
		{
			if (adjacencyM[curVert][j] && !status[j])
			{
				status[j] = true;
				qTemp.push(j);
				result[j] = result[curVert];
				result[j].push_back(j);
			}
		}
	}

	return result;
}

void GraphInteraction::PrintPaths(vector<vector<int>>& paths, int vert)
{
	cout << "Кратчайшие пути от " << vert << " вершины" << endl;
	for (int i = 1; i < paths.size(); i++)
	{		
		cout << i << " вершина: ";
		for (auto& el : paths[i])
		{
			cout << el << " ";
		}
		if (paths[i].empty())
		{
			cout << "<не связаны>";
		}
		cout << endl;
	}
}

vector<int> GraphInteraction::isCyclicBFS()
{
	vector<bool> status(verts + 1);
	vector<int> parents(verts + 1, -1);
	vector<int> cycleVerts;

	for (int i = 1; i <= verts; i++)
	{
		if (!status[i])
		{
			queue<int> qTemp;
			status[i] = true;
			qTemp.push(i);

			while (!qTemp.empty())
			{
				int curVert = qTemp.front();
				qTemp.pop();

				for (int j = 1; j <= verts; j++)
				{
					if (adjacencyM[curVert][j] && !status[j])
					{
						status[j] = true;
						qTemp.push(j);
						parents[j] = curVert;
					}
					else if (adjacencyM[curVert][j] && parents[curVert] != j)
					{
						vector<int> parentsOfCur;
						int tempCur = curVert;
						while (tempCur != -1)
						{
							parentsOfCur.push_back(tempCur);
							tempCur = parents[tempCur];
						}

						vector<int> parentsOfJ;
						int tempJ = j;
						while (tempJ != -1)
						{
							parentsOfJ.push_back(tempJ);
							tempJ = parents[tempJ];
						}

						for (auto& el : parentsOfCur)
						{
							if (find(parentsOfJ.begin(), parentsOfJ.end(), el) != parentsOfJ.end())
							{
								if (el != parentsOfCur.back())
								{
									parentsOfCur.erase(parentsOfCur.begin() + el + 1, parentsOfCur.end());
								}
								if (el != parentsOfJ.back())
								{
									parentsOfJ.erase(parentsOfJ.begin() + el + 1, parentsOfJ.end());
								}

								reverse(parentsOfCur.begin(), parentsOfCur.end());

								cycleVerts.reserve(parentsOfCur.size() + parentsOfJ.size());
								cycleVerts.insert(cycleVerts.end(), parentsOfCur.begin(), parentsOfCur.end());
								cycleVerts.insert(cycleVerts.end(), parentsOfJ.begin(), parentsOfJ.end());

								break;
							}
						}

						return cycleVerts;
					}
				}
			}
		}
	}

	return vector<int>();
}

bool GraphInteraction::isColored(vector<int>& colors)
{
	for (int i = 1; i <= verts; i++)
	{
		if (colors[i] == -1)
		{
			queue<int> qTemp;
			colors[i] = 0;
			qTemp.push(i);

			while (!qTemp.empty())
			{
				int curVert = qTemp.front();
				qTemp.pop();

				for (int j = 1; j <= verts; j++)
				{
					if (adjacencyM[curVert][j] && colors[j] == -1)
					{
						colors[j] = 1 - colors[curVert];
						qTemp.push(j);
					}
					else if (adjacencyM[curVert][j] && colors[j] == colors[curVert])
					{
						return false;
					}
				}
			}
		}
	}

	return true;
}

void GraphInteraction::DFSComps(vector<int>& comp, vector<bool>& status, int curVert)
{
	status[curVert] = true;
	comp.push_back(curVert);

	for (int i = 1; i <= verts; i++)
	{
		if (adjacencyM[curVert][i] && !status[i]) 
		{
			DFSComps(comp, status, i);
		}
	}
}

vector<vector<int>> GraphInteraction::FindCompsDFS()
{
	vector<vector<int>> result;
	vector<bool> visited(verts + 1);

	for (int i = 1; i <= verts; i++)
	{
		if (!visited[i])
		{
			vector<int> component;
			DFSComps(component, visited, i);

			result.push_back(component);
		}
	}

	return result;
}

bool GraphInteraction::DFSCycle(vector<bool>& visited, vector<int>& path, int curVert)
{
	visited[curVert] = true;
	path.push_back(curVert);

	for (int i = 1; i <= verts; i++)
	{
		if (adjacencyM[curVert][i] && !visited[i])
		{
			if (DFSCycle(visited, path, i))
			{
				return true;
			}
		}
		else if (adjacencyM[curVert][i] && find(path.begin(), path.end(), i) != path.end())
		{
			return true;
		}
	}

	path.pop_back();

	return false;
}

vector<int> GraphInteraction::isCyclicDFS()
{
	vector<bool> visited(verts + 1);

	for (int i = 1; i <= verts; i++)
	{
		if (!visited[i])
		{
			vector<int> result;
			if (DFSCycle(visited, result, i))
			{
				return result;
			}
		}
	}

	return vector<int>();
}

