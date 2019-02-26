// f function
#include <iostream>
#include <Windows.h>
using namespace std;

#define CLASS_NAME "LAB.1.14"
#define WINDOW_NAME CLASS_NAME

#define DELAY 10000

struct {
	int x, answ;
	DWORD ID;
} x_and_ID;

int getZero(int x);
int getNonZero(int x);
int getZeroDelay(int x);
int getNonZeroDelay(int x);
int hang(int x);

DWORD workThreadFunction(LPVOID);
HANDLE createWorkThread(int (*)(int));

int main(int argc, char* argv[]) {
	x_and_ID.x = atoi(argv[0]);
	string funcName(argv[1]);

	if (funcName != "") {
		HANDLE workThreadHandle = NULL;
		if (funcName == "getZero") workThreadHandle = createWorkThread(getZero);
		else if (funcName == "getNonZero") workThreadHandle = createWorkThread(getNonZero);
		else if (funcName == "getZeroDelay") workThreadHandle = createWorkThread(getZeroDelay);
		else if (funcName == "getNonZeroDelay") workThreadHandle = createWorkThread(getNonZeroDelay);
		else if (funcName == "hang") workThreadHandle = createWorkThread(hang);

		BOOL bRet;
		MSG uMsg;

		while (bRet = GetMessage(&uMsg, NULL, 0, 0)) {
			if (bRet == -1) {
				CloseHandle(workThreadHandle);
				ExitProcess(GetLastError());
			}
			else if (uMsg.message == WM_CLOSE) {
				COPYDATASTRUCT data = { 0 };
				data.cbData = sizeof(x_and_ID);
				data.lpData = &x_and_ID.answ;
				SendMessage(FindWindow(CLASS_NAME, WINDOW_NAME), WM_COPYDATA, (WPARAM)GetCurrentThreadId(), (LPARAM)&data);

				PostQuitMessage(0);
			}
		}

		CloseHandle(workThreadHandle);
		ExitProcess(0);
	}
	return 0;
}

int getZero(int x) {
	return 0;
}

int getNonZero(int x) {
	return x * x;
}

int getZeroDelay(int x) {
	Sleep(DELAY);
	return 0;
}

int getNonZeroDelay(int x) {
	Sleep(DELAY);
	return x * x;
}

int hang(int x) {
	Sleep(INFINITE);
	return 0;
}

DWORD workThreadFunction(LPVOID getF) {
	x_and_ID.answ = ((int (*)(int))getF)(x_and_ID.x);
	PostThreadMessage(x_and_ID.ID, WM_CLOSE, 0, 0);
	return 0;
}

HANDLE createWorkThread(int (*getF) (int)) {
	x_and_ID.ID = GetCurrentThreadId();
	return CreateThread(NULL, 0, workThreadFunction, getF, 0, NULL);
}
