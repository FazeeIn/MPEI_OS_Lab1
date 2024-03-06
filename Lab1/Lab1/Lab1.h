#pragma once
#include "resource.h"
#include "windows.h"
#include "Math.h"
#include "Additional.h"
#define MAX_LOADSTRING 100
// Глобальные переменные:

HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
HWND hEdit1;
HWND hEdit2;
HWND hEdit3;
HWND hEdit4;
PAINTSTRUCT ps;
HDC hdc;
RECT windowRect;
HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    WndMdlProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    WndMdlProc2(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    WndMdlProc3(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);