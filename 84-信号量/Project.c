#define _CRT_SECURE_NO_WARNINGS
// WindowsProject8.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "WindowsProject8.h"
#include<stdio.h>
//线程数组
HANDLE hThread[5];
HWND hDlg;
HWND hEdit_RC;
HWND hEdit_B1;
HWND hEdit_B2;
HWND hEdit_C1;
HWND hEdit_C2;
HWND hEdit_C3;
HWND hEdit_C4;
//临界区
CRITICAL_SECTION g_Buffer_CS;
//存储消费者文本框句柄
HWND hEditCustomer[4];
//缓冲区文本框数组
HWND hEditBuffer[2];
//信号量：缓冲区有空闲
HANDLE hSemaphoreEmpty;
//信号量：缓冲区有资源
HANDLE hSemaphoreFull;

// 此代码模块中包含的函数的前向声明:
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

//生产者线程
DWORD WINAPI ThreadProduct(LPVOID lpParameter) {

   //先判断资源长度
    char szBuffer[256];
    char szTempszBuffer[2];
    char szTemp[2];
    DWORD dwLength = 0;
    memset(szBuffer, 0, 256);
    GetWindowTextA(hEdit_RC, szBuffer, 256);
    dwLength = strlen(szBuffer);
    for (size_t i = 0; i < dwLength; i++)
    {
        memset(szTemp, 0, 2);
        memcpy(szTemp, &szBuffer[i], 1);
        //等待缓冲区可写资源的信息
        WaitForSingleObject(hSemaphoreEmpty, INFINITE);
        //将字母存储到缓冲区中
        for (size_t j = 0; j < 2; j++)
        {
            //读取缓冲区的值，判断是否为0
            EnterCriticalSection(&g_Buffer_CS);
            memset(szTempszBuffer, 0, 2);
            GetWindowTextA(hEditBuffer[j], szTempszBuffer, 2);
            if (strcmp((char*)szTempszBuffer, "0") == 0)
            {
                Sleep(1000);
                //写入缓冲区
                SetWindowTextA(hEditBuffer[j], szTemp);
                LeaveCriticalSection(&g_Buffer_CS);
                break;
            }
            LeaveCriticalSection(&g_Buffer_CS);
        }	
        //消费者信号+1,
        ReleaseSemaphore(hSemaphoreFull, 1, NULL);
    }
    return TRUE;
}
//消费者线程
DWORD WINAPI ThreadCustomer(LPVOID lpParameter) {
    char szTemp[2];
    char szBuffer[256];
    char newBuffer[256];
    DWORD dwIndex;
    dwIndex = (DWORD)lpParameter;
    while (true)
    {
    //从缓冲区中读取字母
        WaitForSingleObject(hSemaphoreFull, 10000);
        for (size_t i = 0; i < 2; i++)
        {
            EnterCriticalSection(&g_Buffer_CS);
            GetWindowTextA(hEditBuffer[i], szTemp, 2);
            if (strcmp((char*)szTemp, "0") != 0)
            {
                Sleep(1000);
                DWORD dwCusIndex = (DWORD)lpParameter;
                memset(szBuffer, 0, 256);
                GetWindowTextA(hEditCustomer[dwCusIndex], szBuffer, 256);
                sprintf(newBuffer, "%s-%s", szBuffer, szTemp);
                //写到自己的文本框中
                SetWindowTextA(hEditCustomer[dwIndex], newBuffer);
                //将缓冲区置为0
                SetWindowTextA(hEditBuffer[i], "0");
                LeaveCriticalSection(&g_Buffer_CS);
                break;
            }
            LeaveCriticalSection(&g_Buffer_CS);
        }
        ReleaseSemaphore(hSemaphoreEmpty, 1, NULL);
    }
    return TRUE;
}
DWORD WINAPI ThreadMain(LPVOID lpParameter) {
    //初始化临界区
    InitializeCriticalSection(&g_Buffer_CS);

    //创建信号量
    hSemaphoreEmpty = CreateSemaphore(NULL, 2, 2, NULL);
    hSemaphoreFull = CreateSemaphore(NULL, 0, 2, NULL);
    //创建一个生产者线程
    hThread[0] = CreateThread(NULL, 0, ThreadProduct,NULL, 0, NULL);
    //创建四个消费者线程
    hThread[1] = CreateThread(NULL, 0, ThreadCustomer, (void*)0, 0, NULL);
    hThread[2] = CreateThread(NULL, 0, ThreadCustomer, (void*)1, 0, NULL);
    hThread[3] = CreateThread(NULL, 0, ThreadCustomer, (void*)2, 0, NULL);
    hThread[4] = CreateThread(NULL, 0, ThreadCustomer, (void*)3, 0, NULL);

    //等待所有线程结束
    WaitForMultipleObjects(5, hThread, TRUE, INFINITE);
    //关闭句柄
    CloseHandle(hThread[0]);
    CloseHandle(hThread[1]);
    CloseHandle(hThread[2]);
    CloseHandle(hThread[3]);
    CloseHandle(hThread[4]);
    CloseHandle(hSemaphoreEmpty);
    CloseHandle(hSemaphoreFull);
    DeleteCriticalSection(&g_Buffer_CS);
    return TRUE;

}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。



    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT8));
    DialogBoxW(hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), NULL, (DLGPROC)WndProc);
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






//
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
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case IDC_BUTTON_START: {
                HANDLE hThread = CreateThread(NULL, 0, ThreadMain, NULL, 0, NULL);
                CloseHandle(hThread);
                return TRUE;
            }
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_INITDIALOG:
    {

        hEdit_RC = GetDlgItem(hDlg, IDC_EDIT_SOURCE);
        hEdit_B1 = GetDlgItem(hDlg, IDC_EDIT_BUFFER1);
        hEdit_B2 = GetDlgItem(hDlg, IDC_EDIT_BUFFER2);
        hEditBuffer[0] = hEdit_B1;
        hEditBuffer[1] = hEdit_B2;
        hEdit_C1 = GetDlgItem(hDlg, IDC_EDIT_EATA);
        hEdit_C2 = GetDlgItem(hDlg, IDC_EDIT_EATB);
        hEdit_C3 = GetDlgItem(hDlg, IDC_EDIT_EATC);
        hEdit_C4 = GetDlgItem(hDlg, IDC_EDIT_EATD);
        hEditCustomer[0] = hEdit_C1;
        hEditCustomer[1] = hEdit_C2;
        hEditCustomer[2] = hEdit_C3;
        hEditCustomer[3] = hEdit_C4;
        SetWindowTextA(hEdit_RC, "0");
        SetWindowTextA(hEdit_B1, "0");
        SetWindowTextA(hEdit_B2, "0");
        SetWindowTextA(hEdit_C1, "");
        SetWindowTextA(hEdit_C2, "");
        SetWindowTextA(hEdit_C3, "");
        SetWindowTextA(hEdit_C4, "");
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

