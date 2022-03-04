#include<stdlib.h>
#include<Windows.h>
#include<stdio.h>	
VOID TestCreateProcessByAPPName()
{
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi;

	si.cb = sizeof(si);

	TCHAR szApplicationName[] = TEXT("C:\\Program Files\\Google\\Chrome\\Application\\chrome.exe");

	BOOL res = CreateProcess(
		szApplicationName,
		NULL,
		NULL,
		NULL,
		FALSE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL, &si, &pi);

}
VOID TestCreateProcessByCmdline()
{
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi;

	si.cb = sizeof(si);

	TCHAR szCmdline[] = TEXT("C:\\Program Files\\Google\\Chrome\\Application\\chrome.exe http://www.ifeng.com");

	BOOL res = CreateProcess(
		NULL,
		szCmdline,
		NULL,
		NULL,
		FALSE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL, &si, &pi);
}
VOID TestCreateProcess()
{
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi;

	si.cb = sizeof(si);

	TCHAR szCmdline[] = TEXT("http://www.ifeng.com");

	BOOL res = CreateProcess(
		TEXT("C:\\Program Files\\Google\\Chrome\\Application\\chrome.exe"),
		szCmdline,
		NULL,
		NULL,
		FALSE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL, &si, &pi);
}

int main() {
	TestCreateProcess();

}
