#define _CRT_SECURE_NO_WARNINGS
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int main(int argc, char* argv[])
{
	DWORD dwProcessHandle = -1;
	DWORD dwThreadHandle = -1;
	char szBuffer[256] = { 0 };

	memcpy(szBuffer, argv[1], 8);
	sscanf(szBuffer, "%x", &dwProcessHandle);

	memset(szBuffer, 0, 256);
	memcpy(szBuffer, argv[2], 8);
	sscanf(szBuffer, "%x", &dwThreadHandle);
	printf("获取IE进程、主线程句柄\n");
	Sleep(10000);
	//挂起主线程								
	printf("挂起主线程\n");
	SuspendThread((HANDLE)dwThreadHandle);

	Sleep(10000);

	//恢复主线程								
	ResumeThread((HANDLE)dwThreadHandle);
	printf("恢复主线程\n");

	Sleep(10000);

	//关闭ID进程								
	TerminateProcess((HANDLE)dwProcessHandle, 1);
	WaitForSingleObject((HANDLE)dwProcessHandle, INFINITE);

	printf("ID进程已经关闭.....\n");

    return 0;
}
