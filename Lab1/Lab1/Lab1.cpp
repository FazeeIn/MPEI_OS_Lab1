﻿// Lab1.cpp : Определяет точку входа для приложения.

// 13 вариант лабораторной работы
//Найти самый длинный цикл, имеющий две общие вершины с путем между двумя заданными вершинами

#include "Lab1.h"
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB1));
    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case IDM_GRAPH_IMPORT_FROM_FILE:
                if (GetOpenFileNameA(&ofn)) { LoadGraph(filename); }
                InvalidateRect(hWnd, NULL, TRUE);
                ConnectVertices(hWnd, graph, verticesCount);
                PrintVertices(hWnd, verticesCount);
                break;
            case IDM_GRAPH_EXPORT_TO_FILE:
                if (GetSaveFileNameA(&ofn)) { SaveGraph(filename); }
                break;
            case IDM_GRAPH_VERTICES_FROM_KEYBOARD:
                DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, (DLGPROC)WndMdlProc, 0);
                InvalidateRect(hWnd, NULL, TRUE);
                PrintVertices(hWnd,verticesCount);
                break;
            case IDM_GRAPH_EDGE_FROM_KEYBOARD:
                DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_DIALOG2), hWnd, (DLGPROC)WndMdlProc2, 0);
                InvalidateRect(hWnd, NULL, TRUE);
                JoinVertices(hWnd, firstVertex, secondVertex, RGB(0, 0, 0));
                break;
            case ID_GRAPH_ENTER_TWO_VERTICES:
                DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_DIALOG3), hWnd, (DLGPROC)WndMdlProc3, 0);
                break;
            case ID_EXECUTE:
                InvalidateRect(hWnd, NULL, TRUE);

                if (verticesCount == 0)
                {
                    ShowZeroVerticesWarning();
                    break;
                }

                FindPath(graph, 100, firstVertex, secondVertex);
                if (!VerifyPath())
                {
                    ShowPathWarning();
                    break;
                }

                cycle = FindCycle();
                if (cycleLength == 0)
                {
                    ShowCycleWarning();
                    break;
                }
                PrintPath(hWnd);
                PrintCycle(hWnd);
                PrintVertices(hWnd, verticesCount);
                break;
            case ID_GRAPH_CLEAR:
            {
                ClearGraph();
                InvalidateRect(hWnd, NULL, TRUE);
                break;
            }
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
                hdc = BeginPaint(hWnd, &ps);
                ConnectVertices(hWnd, graph, verticesCount);
                PrintPath(hWnd);
                PrintCycle(hWnd);
                PrintVertices(hWnd, verticesCount);
                EndPaint(hWnd, &ps);
        }
        break;

    case WM_CREATE:
        SetOpenFileNameParam(hWnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

LRESULT CALLBACK WndMdlProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_MDL_D1_BUTTON_OK:
            verticesCount = GetDlgItemInt(hWnd, IDC_MDL_D1_EDIT1, NULL, false);
            EndDialog(hWnd, 0);
            break;
        case IDOK_MDL_D1_BUTTON_CLOSE:
            EndDialog(hWnd, 0);
            break;
        }
        break;
    case WM_CLOSE:
        EndDialog(hWnd, 0);
        break;
    }
    return 0;
}

LRESULT CALLBACK WndMdlProc2(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_MDL_D2_BUTTON_OK:
            EndDialog(hWnd, 0);
            firstVertex = GetDlgItemInt(hWnd, IDC_MDL_D2_EDIT1, NULL, false);
            secondVertex = GetDlgItemInt(hWnd, IDC_MDL_D2_EDIT2, NULL, false);
            break;
        case IDOK_MDL_D2_BUTTON_CLOSE:
            EndDialog(hWnd, 0);
            break;
        }
        break;
    case WM_CLOSE:
        EndDialog(hWnd, 0);
        break;
    }
    return 0;
}

LRESULT CALLBACK WndMdlProc3(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_MDL_D3_BUTTON_OK:
            EndDialog(hWnd, 0);
            firstVertex = GetDlgItemInt(hWnd, IDC_MDL_D3_EDIT1, NULL, false);
            secondVertex = GetDlgItemInt(hWnd, IDC_MDL_D3_EDIT2, NULL, false);
            break;
        case IDOK_MDL_D3_BUTTON_CLOSE:
            EndDialog(hWnd, 0);
            break;
        }
        break;
    case WM_CLOSE:
        EndDialog(hWnd, 0);
        break;
    }
    return 0;
}