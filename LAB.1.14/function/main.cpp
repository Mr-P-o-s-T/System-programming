// f function
#include <iostream>
#include <Windows.h>
using namespace std;

#define CLASS_NAME "LAB.1.14"
#define WINDOW_NAME CLASS_NAME

#define DELAY 10000

struct {
	float x, answ;
	DWORD ID;
} x_and_ID;

float getZero(float x);
float getNonZero(float x);
float getZeroDelay(float x);
float getNonZeroDelay(float x);
float Hang(float x);

DWORD workThreadFunction(LPVOID);
HANDLE createWorkThread(float (*)(float));

int main(int argc, char* argv[]) {
	//MessageBoxA(NULL, "Attach now", "Debug", MB_OK);

	x_and_ID.x = atof(argv[0]);
	string funcName(argv[1]);

	if (funcName != "") {
		HANDLE workThreadHandle = NULL;
		if (funcName == "getZero") workThreadHandle = createWorkThread(getZero);
		else if (funcName == "getNonZero") workThreadHandle = createWorkThread(getNonZero);
		else if (funcName == "getZeroDelay") workThreadHandle = createWorkThread(getZeroDelay);
		else if (funcName == "getNonZeroDelay") workThreadHandle = createWorkThread(getNonZeroDelay);
		else if (funcName == "hang") workThreadHandle = createWorkThread(Hang);

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

float getZero(float x) {
	return 0.0f;
}

float getNonZero(float x) {
	return x * x;
}

float getZeroDelay(float x) {
	Sleep(DELAY);
	return 0.0f;
}

float getNonZeroDelay(float x) {
	Sleep(DELAY);
	return x * x;
}

float Hang(float x) {
	for ( ; ; );
}

DWORD workThreadFunction(LPVOID getF) {
	x_and_ID.answ = ((float (*)(float))getF)(x_and_ID.x);
	PostThreadMessage(x_and_ID.ID, WM_CLOSE, 0, 0);
	return 0;
}

HANDLE createWorkThread(float (*getF) (float)) {
	x_and_ID.ID = GetCurrentThreadId();
	return CreateThread(NULL, 0, workThreadFunction, getF, 0, NULL);
}
