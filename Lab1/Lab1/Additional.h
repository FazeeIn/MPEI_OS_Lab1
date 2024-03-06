#pragma once
#include "windows.h"
#include "stack"
#include "list"
using namespace std;

#define TextBufferSize					11000
extern OPENFILENAMEA ofn;
extern char filename[255];
extern char Buffer[TextBufferSize];
extern int verticesCount;
extern int firstVertex;
extern int secondVertex;
extern int* cycle;
extern int cycleLength;
extern int graph[100][100];
extern int shortPath[100];
extern int shortPathLength;

void SaveGraph(LPCSTR path);
void LoadGraph(LPCSTR path);
void SetOpenFileNameParam(HWND hWnd);
void PrintVertices(HWND hWnd, int verticesCount);
void JoinVertices(HWND hWnd, int firstVertex, int secondVertex, COLORREF pencolor);
void FillTheMatrix(char buffer[]);
void ConnectVertices(HWND hWnd, int matrix[100][100], int verticesCount);
void ClearGraph();
void FindPath(int matrix[100][100], int VerticesCount, int firstVertex, int secondVertex);
bool ContainsInMassive(int vertex, list<int>* cycle);
list<int> AdjacentVertices(int startVertex, int vertex, list<int>* cycle);
int* FindCycle();
void PrintPath(HWND hWnd);
void PrintCycle(HWND hWnd);
bool VerifyPath();
void ShowPathWarning();
void ShowCycleWarning();
void ShowZeroVerticesWarning();
void Clear();