
#define _CRT_SECURE_NO_WARNINGS
#include<stdlib.h>
#include<Windows.h>
#include<stdio.h>	


int main() {
	char szBuffer[256] = { 0 };
	char szHandle[8] = { 0 };

	SECURITY_ATTRIBUTES ie_sa_p;
	ie_sa_p.nLength = sizeof(ie_sa_p);
	ie_sa_p.lpSecurityDescriptor = NULL;
	ie_sa_p.bInheritHandle = TRUE;

	SECURITY_ATTRIBUTES ie_sa_t;
	ie_sa_t.nLength = sizeof(ie_sa_t);
	ie_sa_t.lpSecurityDescriptor = NULL;
	ie_sa_t.bInheritHandle = TRUE;
	//创建一个可以被继承的内核对象,此处是个进程								
	STARTUPINFO ie_si = { 0 };
	PROCESS_INFORMATION ie_pi;
	ie_si.cb = sizeof(ie_si);

	TCHAR szCmdline[] = TEXT("C:\\Program Files\\Google\\Chrome\\Application\\chrome.exe");
	CreateProcess(
		NULL,
		szCmdline,
		&ie_sa_p,
		&ie_sa_t,
		TRUE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL, &ie_si, &ie_pi);
	sprintf(szHandle, "%x %x", ie_pi.hProcess, ie_pi.hThread);


	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi;
	si.cb = sizeof(si);

	//创建子进程				
	BOOL res = CreateProcess(
		TEXT("C:\\ie.exe"),
		szHandle,
		NULL,
		NULL,
		TRUE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL, &si, &pi);

}
