#define _CRT_SECURE_NO_WARNINGS
// WindowsProject3.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "WindowsProject3.h"
#include"Resource.h"
#include<stdio.h>
#include<Windows.h>
HANDLE g_hMutex;
HWND g_Hwnd;
HANDLE hHandleArr[5];
char string_buffer1[110];
char string_buffer2[110];
int g_total = -1;
int g_num1 = 0;
int g_num2 = 0;
int g_num3 = 0;
int g_t = -1; // 用来存放红包总大小
DWORD WINAPI ThreadProc1(LPVOID lpParameter);
DWORD WINAPI ThreadProc2(LPVOID lpParameter);
DWORD WINAPI ThreadProc3(LPVOID lpParameter);
DWORD WINAPI MainThreadProc(LPVOID lpParameter); 
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    g_hMutex = CreateMutexA(NULL, FALSE, "XYZ");//创建互斥体
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT3));
    DialogBox(hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX_MAIN), NULL, (DLGPROC)WndProc);
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


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    g_Hwnd = hWnd;
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
            case IDC_BUTTON_START:
                HWND hDlg=GetDlgItem(hWnd, IDC_EDIT_MONEYSUM);
                GetWindowTextA(hDlg, string_buffer1, 100);
                sscanf(string_buffer1, "%d", &g_t);
                HANDLE hThread =CreateThread(NULL, 0, MainThreadProc, NULL, 0, NULL);              
            }
            return true;
        }
    case WM_CLOSE:
        EndDialog(hWnd, 0);
        exit(0);
        break;
    }
    return 0;
}

DWORD WINAPI MainThreadProc(LPVOID lpParameter) {
    memset(hHandleArr, 0, sizeof(hHandleArr));
    GetWindowTextA(GetDlgItem(g_Hwnd, IDC_EDIT_MONEYSUM), string_buffer1, 100);
    sscanf(string_buffer1, "%d", &g_total);
    hHandleArr[0] =CreateThread(NULL, 0, ThreadProc1, NULL, 0, NULL);
    hHandleArr[1] =CreateThread(NULL, 0, ThreadProc2, NULL, 0, NULL);
    hHandleArr[2] =CreateThread(NULL, 0, ThreadProc3, NULL, 0, NULL);
    WaitForMultipleObjects(3, hHandleArr, TRUE, -1);//一直等，直到所有的线程结束
    CloseHandle(hHandleArr[0]);
    CloseHandle(hHandleArr[1]);
    CloseHandle(hHandleArr[2]); // 防止内核对象泄漏
    g_total = -1;
    g_num1 = g_num2 = g_num3 = 0;// 设置为0，方便下一次抢红包
    sprintf(string_buffer1, "红包总额%d已全部抢完。", g_t);
    MessageBoxA(0, string_buffer1, ("提示"), 0);
    return 0;
}
DWORD WINAPI ThreadProc1(LPVOID lpParameter) {
    while (true)
    {
        WaitForSingleObject(g_hMutex, INFINITE);
        if (g_total <= 0) {
            ReleaseMutex(g_hMutex);
            break;
        }
        if (g_total>=50)
        {
            g_total -= 50;
            g_num1 += 50;
        }
        else
        {
            g_num1 += g_total;
            g_total = 0;
        }
        sprintf(string_buffer1, "%d", g_total);
        sprintf(string_buffer2, "%d", g_num1);
        SetWindowTextA(GetDlgItem(g_Hwnd, IDC_EDIT_P1), string_buffer2);
        SetWindowTextA(GetDlgItem(g_Hwnd, IDC_EDIT_MONEYSUM), string_buffer1);
        ReleaseMutex(g_hMutex);
        Sleep(50);
    }

    return 0;

}

DWORD WINAPI ThreadProc2(LPVOID lpParameter) {
    while (true)
    {
        WaitForSingleObject(g_hMutex, INFINITE);
        if (g_total <= 0) {
            ReleaseMutex(g_hMutex);
            break;
        }
        if (g_total >= 50)
        {
            g_total -= 50;
            g_num2 += 50;
        }
        else
        {
            g_num2 += g_total;
            g_total = 0;
        }
        sprintf(string_buffer1, "%d", g_total);
        sprintf(string_buffer2, "%d", g_num2);
        SetWindowTextA(GetDlgItem(g_Hwnd, IDC_EDIT_P2), string_buffer2);
        SetWindowTextA(GetDlgItem(g_Hwnd, IDC_EDIT_MONEYSUM), string_buffer1);
        ReleaseMutex(g_hMutex);
        Sleep(50);
    }

    return 0;

}
DWORD WINAPI ThreadProc3(LPVOID lpParameter) {
    while (true)
    {
        WaitForSingleObject(g_hMutex, INFINITE);
        if (g_total <= 0) {
            ReleaseMutex(g_hMutex);
            break;
        }
        if (g_total >= 50)
        {
            g_total -= 50;
            g_num3 += 50;
        }
        else
        {
            g_num3 += g_total;
            g_total = 0;
        }
        sprintf(string_buffer1, "%d", g_total);
        sprintf(string_buffer2, "%d", g_num3);
        SetWindowTextA(GetDlgItem(g_Hwnd, IDC_EDIT_P3), string_buffer2);
        SetWindowTextA(GetDlgItem(g_Hwnd, IDC_EDIT_MONEYSUM), string_buffer1);
        ReleaseMutex(g_hMutex);
        Sleep(50);
    }

    return 0;

}
