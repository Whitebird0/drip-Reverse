#define _CRT_SECURE_NO_WARNINGS
#include "framework.h"
#include "WindowsProject7.h"
#include<stdio.h>

HANDLE hSemaphore;
HANDLE hThread[4];
HWND hEditSet;
HWND hEdit1;
HWND hEdit2;
HWND hEdit3;
HWND hEdit4;
HWND hDlg;

LRESULT CALLBACK   WndProc(HWND, UINT, WPARAM, LPARAM);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    DialogBox(hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), NULL, (DLGPROC)WndProc);
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT7));

    MSG msg;

    // 主消息循环:
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
DWORD WINAPI ThreadProc1(LPVOID lpParameter)
{
    char szBuffer[10];
    DWORD dwTimmer = 0;
    WaitForSingleObject(hSemaphore, INFINITE);
    while (dwTimmer < 100)
    {
        Sleep(100);
        memset(szBuffer, 0, 10);
        GetWindowTextA(hEdit1, szBuffer, 10);
        sscanf(szBuffer, "%d", &dwTimmer);
        dwTimmer++;
        memset(szBuffer, 0, 10);
        sprintf(szBuffer, "%d", dwTimmer);
        SetWindowTextA(hEdit1, szBuffer);
    }
    ReleaseSemaphore(hSemaphore,1, NULL);
    return 0;
}
DWORD WINAPI ThreadProc2(LPVOID lpParameter)
{
    char szBuffer[10];
    DWORD dwTimmer = 0;
    WaitForSingleObject(hSemaphore, INFINITE);
    while (dwTimmer < 100)
    {
        Sleep(100);
        memset(szBuffer, 0, 10);
        GetWindowTextA(hEdit2, szBuffer, 10);
        sscanf(szBuffer, "%d", &dwTimmer);
        dwTimmer++;
        memset(szBuffer, 0, 10);
        sprintf(szBuffer, "%d", dwTimmer);
        SetWindowTextA(hEdit2, szBuffer);
    }
    ReleaseSemaphore(hSemaphore, 1, NULL);
    return 0;
}
DWORD WINAPI ThreadProc3(LPVOID lpParameter)
{
    char szBuffer[10];
    DWORD dwTimmer = 0;
    WaitForSingleObject(hSemaphore, INFINITE);
    while (dwTimmer < 100)
    {
        Sleep(100);
        memset(szBuffer, 0, 10);
        GetWindowTextA(hEdit3, szBuffer, 10);
        sscanf(szBuffer, "%d", &dwTimmer);
        dwTimmer++;
        memset(szBuffer, 0, 10);
        sprintf(szBuffer, "%d", dwTimmer);
        SetWindowTextA(hEdit3, szBuffer);
    }
    ReleaseSemaphore(hSemaphore, 1, NULL);
    return 0;
}

DWORD WINAPI ThreadProc4(LPVOID lpParameter)
{
    char szBuffer[10];
    DWORD dwTimmer = 0;
    WaitForSingleObject(hSemaphore, INFINITE);
    while (dwTimmer < 100)
    {
        Sleep(100);
        memset(szBuffer, 0, 10);
        GetWindowTextA(hEdit4, szBuffer, 10);
        sscanf(szBuffer, "%d", &dwTimmer);
        dwTimmer++;
        memset(szBuffer, 0, 10);
        sprintf(szBuffer, "%d", dwTimmer);
        SetWindowTextA(hEdit4, szBuffer);
    }
    ReleaseSemaphore(hSemaphore, 1, NULL);
    return 0;
}
DWORD WINAPI ThreadBegin(LPVOID lpParameter)
{
    char szBuffer[10];
    DWORD dwMoney = 0;


    hSemaphore = CreateSemaphore(NULL, 0, 3, NULL);

    hThread[0] = CreateThread(NULL, 0, ThreadProc1, NULL, 0, NULL);
    hThread[1] = CreateThread(NULL, 0, ThreadProc2, NULL, 0, NULL);
    hThread[2] = CreateThread(NULL, 0, ThreadProc3, NULL, 0, NULL);
    hThread[3] = CreateThread(NULL, 0, ThreadProc4, NULL, 0, NULL);
    //开始准备红包			
    while (dwMoney < 1000)
    {
        memset(szBuffer, 0, 10);
        GetWindowTextA(hEditSet, szBuffer, 10);
        sscanf(szBuffer, "%d", &dwMoney);
        dwMoney++;
        memset(szBuffer, 0, 10);
        sprintf(szBuffer, "%d", dwMoney);
        SetWindowTextA(hEditSet, szBuffer);
    }
    ReleaseSemaphore(hSemaphore, 2, NULL);

    WaitForMultipleObjects(3, hThread, TRUE, INFINITE);
    CloseHandle(hSemaphore);

    return 0;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    hDlg = hWnd;
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
            case IDC_BUTTON_START:
                CreateThread(NULL, 0, ThreadBegin, NULL, 0, NULL);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_INITDIALOG:
    {
        hEditSet = GetDlgItem(hDlg, IDC_EDIT_P1);
        hEdit1 = GetDlgItem(hDlg, IDC_EDIT_P2);
        hEdit2 = GetDlgItem(hDlg, IDC_EDIT_P3);
        hEdit3 = GetDlgItem(hDlg, IDC_EDIT_P4);
        hEdit4 = GetDlgItem(hDlg, IDC_EDIT_P5);
        SetWindowTextA(hEditSet, "0");
        SetWindowTextA(hEdit1, "0");
        SetWindowTextA(hEdit2, "0");
        SetWindowTextA(hEdit3, "0");
        SetWindowTextA(hEdit4, "0");
        break;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

