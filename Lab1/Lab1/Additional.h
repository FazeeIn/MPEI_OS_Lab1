#pragma once
#include "windows.h"
char filename[255];
OPENFILENAMEA ofn;
#define TextBufferSize					11000
char Buffer[TextBufferSize];

void SaveGraph(LPCSTR path);
void LoadGraph(LPCSTR path);
void SetOpenFileNameParams(HWND hWnd);
void FillTheMatrix(char buffer[], int size);

