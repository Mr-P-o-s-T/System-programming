// main file
#define ERROR_CONST 100000

#include <Windows.h>
#include <iostream>
using namespace std;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	STARTUPINFO si[2];
	PROCESS_INFORMATION pi[2];

	ZeroMemory(&si, sizeof(si[0]) * 2);
	si[0].cb = si[1].cb = sizeof(si[0]);
	ZeroMemory(&pi, sizeof(pi[0]) * 2);

	WCHAR a[256] = L"getF 0.0";

	// Start the child process. 
	if (!CreateProcess(L"..\\x64\\Debug\\function.exe", a, NULL, NULL, FALSE, 0, NULL, NULL, &si[0], &pi[0])) {
		DWORD error = GetLastError();
		return NULL;
	}

	if (!CreateProcess(L"..\\x64\\Debug\\function.exe", a, NULL, NULL, FALSE, 0, NULL, NULL, &si[1], &pi[1])) {
		DWORD error = GetLastError();
		return NULL;
	}

	CONST HANDLE hP[2] = { pi[0].hProcess, pi[1].hProcess };

	// Wait until child process exits.
	WaitForMultipleObjects(2, hP, true, ERROR_CONST);

	// Close process and thread handles. 
	CloseHandle(pi[0].hProcess);
	CloseHandle(pi[0].hThread);

	CloseHandle(pi[1].hProcess);
	CloseHandle(pi[1].hThread);
	
	return NULL;
}