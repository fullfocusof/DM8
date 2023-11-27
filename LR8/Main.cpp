#include "GraphInteraction.h"

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_ENTER 13
#define KEY_ESC 27
#define KEY_BACKSPACE 8

HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

void GoToXY(short x, short y)
{
	SetConsoleCursorPosition(hStdOut, { x, y });
}

void ConsoleCursorVisible(bool show, short size)
{
	CONSOLE_CURSOR_INFO structCursorInfo;
	GetConsoleCursorInfo(hStdOut, &structCursorInfo);
	structCursorInfo.bVisible = show;
	structCursorInfo.dwSize = size;
	SetConsoleCursorInfo(hStdOut, &structCursorInfo);
}

int main()
{
	setlocale(LC_ALL, "ru");
	SetConsoleTitle(L"Графы");
	ConsoleCursorVisible(false, 100);

	GraphInteraction GI;

	int active_menu = 0;
	int keyInput;
	bool exitProg = false;

	while (!exitProg)
	{
		int x = 40, y = 12;
		GoToXY(x, y);

		vector<string> menu =
		{
			"Считать граф из файла",
			"Вывести граф на экран",
			"Найти количество компонент связности графа(BFS)",
			"Найти кратчайшие пути до остальных вершин(BFS)",
			"Проверка на циклы(BFS)",
			"Покраска графа(BFS)",
			"Найти количество компонент связности графа(DFS)",
			"Проверка на циклы(DFS)",
			"Выход"
		};

		for (int i = 0; i < menu.size(); i++)
		{
			if (i == active_menu)
			{
				SetConsoleTextAttribute(hStdOut, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			}
			else
			{
				SetConsoleTextAttribute(hStdOut, FOREGROUND_GREEN);
			}

			GoToXY(x, y++);
			cout << menu[i] << endl;
		}

		keyInput = _getch();
		switch (keyInput)
		{
			case KEY_ESC:
				exit(0);

			case KEY_UP:
			{
				if (active_menu > 0)
				{
					active_menu--;
				}
			}
			break;

			case KEY_DOWN:
			{
				if (active_menu < menu.size() - 1)
				{
					active_menu++;
				}
			}
			break;

			case KEY_ENTER:
			{
				switch (active_menu)
				{
					case 0:
					{
						system("cls");
						GI.Read_g();
						GI.printQuit();
					}
					break;

					case 1:
					{
						system("cls");
						if (GI.adjacencyM.empty())
						{
							cout << "Данные отсутствуют";
						}
						else
						{
							GI.Print_g();
						}
						GI.printQuit();
					}
					break;

					case 2:
					{
						system("cls");

						vector<vector<int>> comps = GI.BFSComps();
						GI.PrintComps(comps);

						GI.printQuit();
					}
					break;

					case 3:
					{
						system("cls");

						int vert;
						cout << "Введите номер вершины, для которой нужно подсчитать кратчайшие пути" << endl;
						cin >> vert;
						while (vert < 0 || vert > GI.verts)
						{
							system("cls");
							cout << "Данной вершины не существует в исходном графе" << endl;
							cout << "Введите номер вершины, для которой нужно подсчитать кратчайшие пути" << endl;
							cin >> vert;
						}

						system("cls");
						vector<vector<int>> paths = GI.BFSPaths(vert);
						GI.PrintPaths(paths, vert);

						GI.printQuit();
					}
					break;

					case 4:
					{
						system("cls");

						vector<int> cycleVerts = GI.isCyclicBFS();

						if (cycleVerts.empty())
						{
							cout << "В исходном графе нет циклов";
						}
						else
						{
							cout << "Длина цикла - " << cycleVerts.size() - 1 << endl;
							cout << "Маршрут цикла: ";
							for (auto& el : cycleVerts)
							{
								cout << el << " ";
							}
							cout << endl << endl;
						}

						GI.printQuit();
					}
					break;

					case 5:
					{
						system("cls");

						vector<int> colors(GI.verts + 1, -1);

						if (GI.isColored(colors))
						{
							for (int i = 1; i < colors.size(); i++)
							{
								cout << i << " вершина - ";
								if (colors[i] == 0)
								{
									cout << "черный";
								}
								else if (colors[i] == 1)
								{
									cout << "белый";
								}
								cout << endl;
							}
						}
						else
						{
							cout << "Данный граф невозможно покрасить";
						}

						GI.printQuit();
					}
					break;

					case 6:
					{
						system("cls");

						vector<vector<int>> comps = GI.FindCompsDFS();
						GI.PrintComps(comps);

						GI.printQuit();
					}
					break;

					case 7:
					{
						system("cls");

						vector<int> cycleVerts = GI.isCyclicDFS();

						if (cycleVerts.empty())
						{
							cout << "В исходном графе нет циклов";
						}
						else
						{
							cout << "Длина цикла - " << cycleVerts.size() - 1 << endl;
							cout << "Маршрут цикла: ";
							for (auto& el : cycleVerts)
							{
								cout << el << " ";
							}
							cout << endl << endl;
						}

						GI.printQuit();
					}
					break;

					case 8:
					{
						exitProg = true;
					}
					break;
				}
			}
			break;
		}
	}
}