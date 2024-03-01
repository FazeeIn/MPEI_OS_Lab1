// Lab1.cpp : Определяет точку входа для приложения.
//
// 13 вариант лабы
//Найти самый длинный цикл, имеющий две общие вершины с путем между двумя заданными вершинами
#include "windows.h"
#include "Lab1.h"
#include "Math.h"
#include "Additional.h"
#include "stack"
#include "list"
using namespace std;
#define MAX_LOADSTRING 100
// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    WndMdlProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    WndMdlProc2(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    WndMdlProc3(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int verticesCount = 0;
int firstVertex = 0;
int secondVertex = 0;
HWND hEdit1;
HWND hEdit2;
HWND hEdit3;
HWND hEdit4;
void PrintVerticies(HWND hWnd,int verticiesCount);
void JoinVertices(HWND hWnd, int firstVertex, int secondVertex, COLORREF pencolor);
int graph[100][100] = {};
void FillTheMatrix(char buffer[]);
void ConnectVertices(HWND hWnd, int matrix[100][100], int verticesCount);
PAINTSTRUCT ps;
HDC hdc;
void ClearGraph();
void FindPath(int matrix[100][100], int VerticesCount, int firstVertex, int secondVertex);
int shortPath[100] = {};
int shortPathLength = 0;
void PrintPath(HWND hWnd);
bool ContainsInMassive(int vertex, list<int>* cycle);
list<int> AdjacentVertices(int startVertex, int vertex, list<int>* cycle);
int* FindCycle();
int *cycle;
int cycleLength = 0;

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
            case IDM_INPUT_FROM_FILE:
                if (GetOpenFileNameA(&ofn)) { LoadGraph(filename); }
                InvalidateRect(hWnd, NULL, TRUE);
                ConnectVertices(hWnd, graph, verticesCount);
                PrintVerticies(hWnd, verticesCount);
                
                break;
            case IDM_OUTPUT_TO_FILE:
                if (GetSaveFileNameA(&ofn)) { SaveGraph(filename); }
                break;
            case IDM_DATA_FROM_KEYBOARD_0:
                DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, (DLGPROC)WndMdlProc, 0);
                InvalidateRect(hWnd, NULL, TRUE);
                PrintVerticies(hWnd,verticesCount);
                break;
            case IDM_DATA_FROM_KEYBOARD_1:
                DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_DIALOG2), hWnd, (DLGPROC)WndMdlProc2, 0);
                InvalidateRect(hWnd, NULL, TRUE);
                JoinVertices(hWnd, firstVertex, secondVertex, RGB(0, 0, 0));
                break;
            case ID_Two_Vertices:
                DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_DIALOG3), hWnd, (DLGPROC)WndMdlProc3, 0);
                break;
            case ID_Verify:
                FindPath(graph, 100, firstVertex, secondVertex);
                cycle = FindCycle();
                for (size_t i = 0; i < shortPathLength - 1; i++)
                    JoinVertices(hWnd, shortPath[i], shortPath[i + 1], RGB(255, 0, 0));

                
                for (int i = 0; i < cycleLength - 1; i++)
                {
                    JoinVertices(hWnd, cycle[i], cycle[i + 1], RGB(0, 255, 0));
                }
                PrintVerticies(hWnd, verticesCount);
                break;
            case ID_CLEAR_GRAPH:
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
            // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
            ConnectVertices(hWnd, graph, verticesCount);
            PrintVerticies(hWnd, verticesCount);
            EndPaint(hWnd, &ps);
        }
        break;

    case WM_CREATE:
        SetOpenFileNameParams(hWnd);
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
        case IDC_BUTTON_OK_MDL:
            verticesCount = GetDlgItemInt(hWnd, IDC_MDL_EDIT1, NULL, false);
            EndDialog(hWnd, 0);
            return 1;
        case IDOK_BUTTON_CLOSE_MDL:  
            EndDialog(hWnd, 0);
            return 1;
        }
        break;
    case WM_CLOSE:
        EndDialog(hWnd, 0);
        return 1;
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
        case IDOK_1:
            EndDialog(hWnd, 0);
            firstVertex = GetDlgItemInt(hWnd, IDC_EDIT2, NULL, false);
            secondVertex = GetDlgItemInt(hWnd, IDC_EDIT1, NULL, false);

            return 1;
        case IDCANCEL_1:
            EndDialog(hWnd, 0);
            return 1;
        }
        break;
    case WM_CLOSE:
        EndDialog(hWnd, 0);
        return 1;
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
        case IDOK_MDL3_1:
            EndDialog(hWnd, 0);
            firstVertex = GetDlgItemInt(hWnd, IDC_Input_V1, NULL, false);
            secondVertex = GetDlgItemInt(hWnd, IDC_Input_V2, NULL, false);

            return 1;
        case IDCANCEL_MDL3_1:
            EndDialog(hWnd, 0);
            return 1;
        }
        break;
    case WM_CLOSE:
        EndDialog(hWnd, 0);
        return 1;
    }
    return 0;
}

void PrintVerticies(HWND hWnd, int verticiesCount)
{
    HDC hdc = GetDC(hWnd);
    HBRUSH brush = CreateSolidBrush(RGB(222,20,220));
    LPRECT windowSize = {};
    RECT rc;

    SelectObject(hdc, brush);
    GetWindowRect(hWnd, &rc);

    int xWindow = rc.left;
    int yWindow = rc.top;
    int windowHeight = rc.bottom;
    int windowWeigt = rc.right;
    int XWindowFocus = (windowWeigt - xWindow) / 2;
    int YWindowFocus = (windowHeight - yWindow) / 2;
    int radius = 300;
    double alpha = 3.141592653589793 * 2 / verticiesCount;
   
    Ellipse(hdc, XWindowFocus,
                 YWindowFocus,
                 XWindowFocus + 20,
                 YWindowFocus - 20);

    brush = CreateSolidBrush(RGB(50, 50, 50));
    SelectObject(hdc, brush);

    for (int i = 0; i < verticiesCount; i++)
    {
        Ellipse(hdc, XWindowFocus + radius * cos(alpha * i),
                     YWindowFocus - radius * sin(alpha * i),
                     XWindowFocus + radius * cos(alpha * i) + 20,
                     YWindowFocus - radius * sin(alpha * i) - 20);



        wchar_t message[10] = { 0 };
        wsprintf(message, L"%d", i);
        RECT rect;
        rect.left = XWindowFocus + radius * cos(alpha * i) + 3; // закончил тут симметричное заполнение матрицы для 
        rect.top = YWindowFocus - radius * sin(alpha * i) - 18; // для правильной обработки и обновление окна
        rect.right = XWindowFocus + radius * cos(alpha * i) + 20;
        rect.bottom = YWindowFocus - radius * sin(alpha * i) - 20 ;

        SetTextColor(hdc, RGB(255,255,255));
        SetBkMode(hdc, TRANSPARENT); 


        DrawText(hdc, message, -1, &rect, DT_SINGLELINE | DT_NOCLIP | DT_CENTER);
    }
}
void JoinVertices(HWND hWnd, int firstVertex, int secondVertex, COLORREF pencolor)
{
    HDC hdc = GetDC(hWnd);
    HPEN pen = CreatePen(0, 2, pencolor);
    RECT rc;

    SelectObject(hdc, pen);
    GetWindowRect(hWnd, &rc);

    int xWindow = rc.left;
    int yWindow = rc.top;
    int windowHeight = rc.bottom;
    int windowWeigt = rc.right;
    int XWindowFocus = (windowWeigt - xWindow) / 2;
    int YWindowFocus = (windowHeight - yWindow) / 2;
    int radius = 300;
    double alpha = 3.141592653589793 * 2 / verticesCount;
    graph[firstVertex][secondVertex] = 1;
    graph[secondVertex][firstVertex] = 1;
    MoveToEx(hdc, XWindowFocus + radius * cos(alpha * (firstVertex)) + 10,
                  YWindowFocus - radius * sin(alpha * (firstVertex)) - 10,
                  NULL);

    LineTo(hdc, XWindowFocus + radius * cos(alpha * (secondVertex)) + 10,
                YWindowFocus - radius * sin(alpha * (secondVertex)) - 10);
    

}
void SetOpenFileNameParams(HWND hWnd)
{
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = filename;
    ofn.nMaxFile = sizeof(filename);
    ofn.lpstrFilter = "*.txt";
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = "C:\\Users\\danii\\Desktop\\3 курс\\2 сем\\Lab1";
    ofn.Flags = OFN_PATHMUSTEXIST || OFN_FILEMUSTEXIST;
}
void SaveGraph(LPCSTR path)
{
    HANDLE FileToSave = CreateFileA(
        path,
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    CloseHandle(FileToSave);
}
void LoadGraph(LPCSTR path)
{
    HANDLE FileToLoad = CreateFileA(
        path,
        GENERIC_READ,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    DWORD bytesIterated;
    ReadFile(FileToLoad, Buffer, TextBufferSize, &bytesIterated, NULL);
    FillTheMatrix(Buffer);
    CloseHandle(FileToLoad);
    
    
}
void FillTheMatrix(char buffer[]) 
{
    int k = 0;
    int step = 0;
    while (buffer[k] != '\r')
        k++;
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < k; j++)
            graph[i][j] = buffer[i * k + j + step] - 48;
        step += 2;
    }
    verticesCount = k;
}
void ConnectVertices(HWND hWnd,int matrix[100][100], int verticesCount)
{
    for (int i = 0; i < verticesCount; i++)
        for (int j = i; j < verticesCount; j++)
            if (matrix[i][j] == 1)
                JoinVertices(hWnd, i, j, RGB(0, 0, 0));
}
void ClearGraph()
{
    for (int i = 0; i < verticesCount; i++)
    {
        for (int j = 0; j < verticesCount; j++)
        {
            graph[i][j] = 0;
        }
    }
    verticesCount = 0;
}
void FindPath(int matrix[100][100],int VerticesCount, int firstVertex, int secondVertex)
{

    int backPath[100] = {  };
    for (size_t i = 0; i < 100; i++)
        backPath[i] = -1;

    int backPathLength = 0;

    int currVertex = firstVertex;

    int AllAdjacentVertices[100] = { };
    int AllAdjacentVerticesCount = 0;

    int adjacentVertices[100] = { };
    int adjacentVerticesCount = 0;

    int i = 0, M = 0;
    int j = 0, N = 0;

    int* m = (int*)matrix;
    int* aav = AllAdjacentVertices;
    int* av = adjacentVertices;
    int* pb = backPath;
    int* ps = shortPath;

    int queue[10000] = {};
    int* pq = queue;
    int queueCount = 0;
    int stackPopElement = 0;
    //код для нахождения вершин, смежных с currVertex
    __asm {
        // Main функция asm
        // _______Код основной программы на asm___________________________________
        mov ecx, firstVertex
        call enqueue
        while_stack : cmp queueCount, 0
        jz stack_empty
        call dequeue // значение лежит в ecx

        mov currVertex, ecx

        //jz end_main // если вершина равна конечной, добавляем в путь, поиск завершен (последняя вершина не добавляется в стек доделать)

        call find_All_Adjacent_Vertices
        call select_adjacent_vertices

        call AddAllAdjacentVertexToQueue // добавить все смежные вершины в очередь
        call completeBackPath

        cmp adjacentVerticesCount, 0
        mov eax, 0
        mov AllAdjacentVerticesCount, eax
        mov eax, 0
        mov adjacentVerticesCount, eax
        jz while_stack

        mov eax, shortPathLength
        mov ebx, 4
        mul ebx
        add eax, ps
        mov ebx, currVertex
        mov[eax], ebx
        mov eax, shortPathLength
        inc eax
        mov shortPathLength, eax

        // если смежных вершин нет, то прололжаем цикл
        // если смежные вершины есть, зписываем теккущую в путь

        jmp while_stack
        stack_empty : ret 

        end_main :pop ecx
                  mov eax, shortPathLength
                  mov ebx, 4
                  mul ebx
                  add eax, ps
                  mov ebx, currVertex
                  mov[eax], ebx
                  mov eax, shortPathLength
                  inc eax
                  mov shortPathLength, eax

        call highLightPath
            jmp end_proc


            //________________________________________________________________________________________________________________________//


        find_All_Adjacent_Vertices://(currVertex -> eax) Поиск всех смежных вершин для currVertex

        mov eax, AllAdjacentVerticesCount 
            mov eax, 0
            mov eax, adjacentVerticesCount
            mov eax, 0

            mov eax, VerticesCount
            mov ebx, 4
            mul ebx
            mov ebx, eax
            mov eax, currVertex

            //mov ebx, 12 // для 3-ех вершин заменить на 100 * 4 вершин
            mul ebx
            add eax, m //сместил указатель на начало 2-ой строки
            add ebx, eax
            mov ecx, aav
            mov edx, eax // указатель на нулевой элемент i-ой строки

            mov j, eax
            mov N, ebx
            mov i, edx

            m2 : cmp[eax], 1
            jnz m3


            mov j, eax
            mov N, ebx //сохранил j and  
            mov ebx, edx
            mov i, edx
            mov edx, 0
            sub eax, ebx
            mov ebx, 4
            div ebx // проверка на то, что номер вершины не совпадает с currVertex

            mov ebx, currVertex
            cmp ebx, eax
            jz m3

            mov[ecx], eax 
            add ecx, 4
            mov edx, AllAdjacentVerticesCount
            inc edx
            mov AllAdjacentVerticesCount, edx
            m3 : mov eax, j
            mov ebx, N //записал номер смежной вершины в массив
            mov edx, i
            add eax, 4
            mov j, eax


            cmp eax, ebx // eax < ebx
            jz faav_end
            jmp m2
            faav_end : ret

            //___________________________________________________Новывая версия метода____________________________________________

        find_Adjacent_Vertex: // кладет вершину в adjacentVertices, если она смежная для ebx и её нет в currPath
        // ebx - данная смежная вершина AllAdjacentVertices
        // ecx - указатель на первый элемент currPath
            // (AllAdjacentVertices -> ebx, currPath -> ecx)

        mov eax, shortPathLength 
            mov edx, 4
            mul edx
            add eax, ecx
            mov j, ecx
            mov N, eax
            mov eax, 0
             

            fav_for: cmp ecx, N
            JZ fav_add_vert_1

            fav_for_if : mov eax, [ebx]
            cmp eax, [ecx]
            JNZ fav_for_if_F 
            jmp fav_for_if_T 

            fav_for_if_F : add ecx, 4
            jmp fav_for


            fav_for_if_T : add ecx, 4
            jmp fav_end 

            fav_add_vert_1 :
        mov eax, adjacentVerticesCount
            mov edx, 4
            mul edx
            add eax, av
            mov edx, [ebx]
            mov[eax], edx
            mov eax, adjacentVerticesCount
            inc eax
            mov adjacentVerticesCount, eax
            fav_end : ret


            enqueue : 
        mov eax, queueCount
            mov ebx, 4
            mul ebx
            add eax, pq
            mov[eax], ecx
            inc queueCount
            ret

            dequeue :
        mov eax, pq

            mov ecx, [eax]
            mov stackPopElement, ecx

            mov eax, queueCount
            mov ebx, 4
            mul ebx
            add eax, pq
            mov ebx, eax
            mov eax, pq
            mov edx, eax
            add edx, 4

            dequeue_for: cmp eax, ebx
            jz end_dequeue
            mov ecx, [edx]
            mov[eax], ecx
            add eax, 4
            add edx, 4
            jmp dequeue_for

            end_dequeue :
        mov eax, queueCount
            dec eax
            mov queueCount, eax
            mov ecx, stackPopElement
            ret


            AddAllAdjacentVertexToQueue :
        mov eax, adjacentVerticesCount
            mov ebx, 4
            mul ebx
            add eax, av
            mov ebx, eax
            mov eax, av
            mov i, eax
            mov M, ebx
            queue_while : mov eax, i
            mov ebx, M
            cmp eax, ebx
            jz to_end
            mov ecx, [eax]
            call enqueue
            mov eax, i
            add eax, 4
            mov i, eax
            jmp queue_while
            to_end :
        ret

            select_adjacent_vertices :


        mov i, 0
            mov M, 0

            mov eax, AllAdjacentVerticesCount
            mov ebx, 4
            mul ebx

            mov ebx, aav
            add eax, ebx

            mov ecx, ps 

            mov i, ebx
            mov M, eax



        flag_for_1:
        call find_Adjacent_Vertex 
            mov eax, i
            mov edx, M
            add eax, 4
            mov i, eax
            mov ebx, eax
            cmp eax, edx
            jnz flag_for_1

            ret

            completeBackPath:
                mov edx, 0
                mov ecx, av
                mov eax, 0
       cmp_for: mov ebx, pb
                cmp eax, adjacentVerticesCount
                je cbp_end
                imul edx, [ecx], 4
                add ebx, edx
                mov edx, currVertex
                cmp [ebx], -1
                jnz cmp_pass
                mov[ebx], edx
      cmp_pass: mov ebx, secondVertex
                cmp [ecx], ebx
                jz end_main

                add ecx, 4
                inc eax
                jmp cmp_for

            cbp_end : ret

            highLightPath :
            mov shortPathLength, 1
            mov ebx, ps
            mov edx, secondVertex
            mov[ebx], edx
    hlp_m1: inc shortPathLength
            add ebx, 4
            imul eax, edx, 4
            mov ecx, pb
            add ecx, eax
            mov eax, [ecx]
            mov[ebx], eax
            mov edx, eax
            cmp eax, firstVertex
            jne hlp_m1

            ret

                end_proc:
           

    }

}
int* FindCycle()
{
    
    stack<list<int>*> niceCycleStack;
    for (int i = 0; i < shortPathLength; i++)
    {
        int currVertex = shortPath[i];
        stack<list<int>*> cycleStack;
        list<int>* startList = new list<int>();
        startList->push_back(currVertex);
        cycleStack.push(startList);
        cycleLength = 0;
        while (!cycleStack.empty())
        {
            list<int>* cycle = cycleStack.top();
            cycleStack.pop();
            int vertex = cycle->back();
            list<int> adjVertices = AdjacentVertices(currVertex, vertex, cycle);
            for (auto next = adjVertices.begin(); next != adjVertices.end(); next++)
            {
                list<int>* newCycle = new list<int>();
                newCycle->insert(newCycle->begin(), cycle->begin(), cycle->end());
                if (currVertex == *next)
                {
                    if (cycle->size() > 2)
                    {
                        newCycle->push_back(*next);
                        niceCycleStack.push(newCycle);
                    }
                    else
                        continue;
                }
                else
                {
                    newCycle->push_back(*next);
                    cycleStack.push(newCycle);
                }

            }

        }
    }
    
    list<int>* largestlist = NULL;
    int maxLenghCycle = 0;
    
    while (!niceCycleStack.empty())
    {
        int CountInputVertices = 0;
        list<int>* list = niceCycleStack.top();
        niceCycleStack.pop();
        for (int i = 0; i < shortPathLength; i++)
        {
            if (ContainsInMassive(shortPath[i], list))
                CountInputVertices++;
        }
        if (list->size() > maxLenghCycle && CountInputVertices == 2)
        {
            largestlist = list;
            maxLenghCycle = list->size();
        }  
    }

    int *niceCycle = new int[maxLenghCycle];
    for (auto it = largestlist->begin(); it != largestlist->end(); it++)
    {
        niceCycle[cycleLength] = *it;
        cycleLength++;
    }
    return niceCycle;

}
list<int> AdjacentVertices(int startVertex, int vertex, list<int>* cycle)
{
    list<int> adjacentVertex;
    for (int i = 0; i < verticesCount; i++)
    {
        if (graph[vertex][i] == 1  && (!ContainsInMassive(i, cycle) || i == startVertex))
            adjacentVertex.push_back(i);
    }
    return adjacentVertex;
}
bool ContainsInMassive(int vertex, list<int>* cycle)
{
    list<int>* newCycle = new list<int>();
    newCycle->insert(newCycle->begin(), cycle->begin(), cycle->end());

    for (auto it = newCycle->begin(); it != newCycle->end(); it++)
    {
        if (*it == vertex) 
            return true;
    }
    return false;
}

