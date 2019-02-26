// main file
#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <time.h>
#include <iostream>
using namespace std;

#define ERROR_CONST 20000
#define PROMPT_DELAY 10000

#define UWM_CHECK_CALCULATION WM_APP
#define UWM_CANCELLED (WM_APP + 1)
#define UWM_IGNORED (WM_APP + 2)
#define UWM_START_CALCULATION (WM_APP + 3)
#define UWM_WAIT (WM_APP + 4)

#define BTN_START 10000
#define RD_BTN_0 10001
#define RD_BTN_1 10002
#define RD_BTN_2 10003
#define RD_BTN_3 10004
#define RD_BTN_4 10005
#define RD_BTN_5 10006
#define RD_BTN_6 10007
#define RD_BTN_7 10008
#define RD_BTN_8 10009
#define RD_BTN_9 10010

#define CLASS_NAME TEXT("LAB.1.14")
#define WINDOW_NAME CLASS_NAME

#define WIND_WIDTH 400
#define WIND_HEIGTH 250
#define RADIO_BTN_WIDTH (WIND_WIDTH / 2 - 7)
#define RADIO_BTN_HEIGTH 20
#define EDT_WIDTH (WIND_WIDTH - 15)
#define EDT_HEIGTH 30
#define BUTTN_WIDTH (WIND_WIDTH - 15)
#define BUTTN_HEIGTH 50

struct {
	int value;
	bool filled = false, stuck = true;
} x[3];

struct mainThread{
	HWND mainWindow, mainButton, mainEdit, mainRadButtons[2][5];
	DWORD mainThreadID;
} mThread;

PROCESS_INFORMATION pi[2] = { 0 };
STARTUPINFO si[2] = { 0 };
HANDLE hP[2] = { 0 }, promptThreadHandle = NULL;
bool isWait = false, isCancelled = false, isAnswer = false, isPrompted = false;
clock_t promptTime = clock();

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void createWindow(HINSTANCE hInst, LPCTSTR lpzClassName);
void launchChildProcesses(WCHAR *a, WCHAR *b);
BOOL killProcess(PROCESS_INFORMATION *pi);
void closeChildProcessesHandles(PROCESS_INFORMATION *pi);

DWORD promptThread(LPVOID x);
HANDLE createPromptThread();

int binOperation(int a, int b);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	mThread.mainThreadID = GetCurrentThreadId();

	createWindow(hInstance, CLASS_NAME);

	if (!mThread.mainWindow) return 2;

	BOOL bRet;
	MSG uMsg;

	while (bRet = GetMessage(&uMsg, NULL, 0, 0)) {
		if (bRet == -1) {
			killProcess(&pi[0]);
			killProcess(&pi[1]);
			closeChildProcessesHandles(pi);
			if (promptThreadHandle) CloseHandle(promptThread);
			ExitProcess(GetLastError());
		}
		else {
			TranslateMessage(&uMsg);
			DispatchMessage(&uMsg);
		}
	}
	
	killProcess(&pi[0]);
	killProcess(&pi[1]);
	closeChildProcessesHandles(pi);
	if (promptThreadHandle) CloseHandle(promptThreadHandle);
	return (int) uMsg.wParam;
}

int getChekedRadBtn(HWND *rbtnArr) {
	static int group = 0;
	for (int i = 0; i < 5; i++) 
		if (Button_GetState(rbtnArr[i]) == BST_CHECKED) {
			group = 1 - group;
			return (group ? RD_BTN_0 : RD_BTN_5) + i;
		}
	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	RegisterHotKey(hwnd, 1, 0, VK_ESCAPE);
	TCHAR res[256] = TEXT("\0");
	int len;
	static clock_t begin = 0, end = 0;

	switch (uMsg) {
	case WM_COMMAND:
		if ((int)wParam == BTN_START) PostMessage(mThread.mainWindow, UWM_START_CALCULATION, 0, 0);
		else if ((RD_BTN_0 <= (int)wParam) && ((int)wParam <= RD_BTN_4)) CheckRadioButton(hwnd, RD_BTN_0, RD_BTN_4, (int) wParam);
		else if ((RD_BTN_5 <= (int)wParam) && ((int)wParam <= RD_BTN_9)) CheckRadioButton(hwnd, RD_BTN_5, RD_BTN_9, (int)wParam);
		break;
	case WM_HOTKEY:
		if (wParam == 1) PostMessage(mThread.mainWindow, UWM_CANCELLED, 0, 0);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_COPYDATA:
		if (!isCancelled) {
			if ((DWORD)wParam == pi[0].dwThreadId) {
				x[1].value = *((int *)((COPYDATASTRUCT *)lParam)->lpData);
				x[1].filled = true;
				x[1].stuck = false;
				if (!x[1].value) {
					PostThreadMessage(GetThreadId(pi[1].hThread), WM_QUIT, 0, 0);
					PostMessage(mThread.mainWindow, UWM_CHECK_CALCULATION, false, 0);
				}
			}
			else if ((DWORD)wParam == pi[1].dwThreadId) {
				x[2].value = *(int *)((COPYDATASTRUCT *)lParam)->lpData;
				x[2].filled = true;
				x[2].stuck = false;
				if (!x[2].value) {
					PostThreadMessage(GetThreadId(pi[0].hThread), WM_QUIT, 0, 0);
					PostMessage(mThread.mainWindow, UWM_CHECK_CALCULATION, false, 0);
				}
			}
			else break;
			if (x[1].filled && x[2].filled) {
				isWait = false;
				PostMessage(mThread.mainWindow, UWM_CHECK_CALCULATION, false, 0);
			}
		}
		break;
	case UWM_CHECK_CALCULATION:
		if (!isPrompted) {
			if (!wParam) {
				if (x[1].filled && x[2].filled) {
					_stprintf(res, TEXT("f(%d) = %d\ng(%d) = %d\nResult is: %d\nTime: %f ms"), x[0].value, x[1].value, x[0].value, x[2].value, binOperation(x[1].value, x[2].value), ((double)clock() - promptTime) / CLOCKS_PER_SEC * 1000);
					if (!isCancelled && !isAnswer) {
						isAnswer = true;
						MessageBox(NULL, res, TEXT("Results:"), MB_ICONINFORMATION | MB_OK);
						PostMessage(mThread.mainWindow, WM_CLOSE, 0, 0);
					}
					else if (!isAnswer) PostMessage(mThread.mainWindow, UWM_CHECK_CALCULATION, 0, 0);
				}
				else if (x[1].filled && !x[1].value) {
					_stprintf(res, TEXT("f(%d) = %d\nResult is: %d\nTime: %f ms"), x[0].value, x[1].value, 0, ((double)clock() - promptTime) / CLOCKS_PER_SEC * 1000);
					if (!isCancelled && !isAnswer) {
						isAnswer = true;
						MessageBox(NULL, res, TEXT("Results:"), MB_ICONINFORMATION | MB_OK);
						PostMessage(mThread.mainWindow, WM_CLOSE, 0, 0);
					}
					else if (!isAnswer) PostMessage(mThread.mainWindow, UWM_CHECK_CALCULATION, 0, 0);
				}
				else if (x[2].filled && !x[2].value) {
					_stprintf(res, TEXT("g(%d) = %d\nResult is: %d\nTime: %f ms"), x[0].value, x[2].value, 0, ((double)clock() - promptTime) / CLOCKS_PER_SEC * 1000);
					if (!isCancelled && !isAnswer) {
						isAnswer = true;
						MessageBox(NULL, res, TEXT("Results:"), MB_ICONINFORMATION | MB_OK);
						PostMessage(mThread.mainWindow, WM_CLOSE, 0, 0);
					}
					else if (!isAnswer) PostMessage(mThread.mainWindow, UWM_CHECK_CALCULATION, 0, 0);
				}
			}
			else {
				if (isWait) {
					if (!isAnswer) {
						isWait = false;
						if (x[1].stuck && x[2].stuck) _stprintf(res, TEXT("Both processes are presumpted stuck..."));
						else if (x[1].stuck) _stprintf(res, TEXT("f(%d) - presumpted stuck...\ng(%d) = %d"), x[0].value, x[0].value, x[2].value);
						else if (x[2].stuck) _stprintf(res, TEXT("f(%d) = %d\ng(%d) - presumpted stuck..."), x[0].value, x[1].value, x[0].value);

						MessageBox(NULL, res, TEXT("Results:"), MB_ICONINFORMATION | MB_OK);
						PostMessage(mThread.mainWindow, WM_CLOSE, 0, 0);
					}
				}
				else {
					static bool inStart = true;
					if (inStart) {
						PostMessage(mThread.mainWindow, UWM_WAIT, 0, 0);
						inStart = false;
					}
					else {
						PostMessage(mThread.mainWindow, UWM_CHECK_CALCULATION, false, 0);
					}
				}
			}
		}
		else PostMessage(mThread.mainWindow, UWM_CHECK_CALCULATION, 0, 0);
		break;
	case UWM_CANCELLED:
		if (x[1].filled && x[2].filled) _stprintf(res, TEXT("f(%d) = %d\ng(%d) = %d\nResult is: %d"), x[0].value, x[1].value, x[0].value, x[2].value, binOperation(x[1].value, x[2].value));
		else if (x[1].filled) {
			if (!x[1].value) _stprintf(res, TEXT("f(%d) = %d\nResult is: %d"), x[0].value, x[1].value, 0);
			else _stprintf(res, TEXT("f(%d) = %d\ng(%d) - calculation cancelled by user..."), x[0].value, x[1].value, x[0].value);
		}
		else if (x[2].filled) {
			if (!x[2].value) _stprintf(res, TEXT("g(%d) = %d\nResult is: %d"), x[0].value, x[2].value, 0);
			else _stprintf(res, TEXT("f(%d) - calculation cancelled by user...\ng(%d) = %d"), x[0].value, x[0].value, x[2].value);
		}
		else _stprintf(res, TEXT("Calculation is cancelled by user..."));
		isCancelled = true;
		MessageBox(NULL, res, TEXT("Results:"), MB_ICONINFORMATION | MB_OK);
		PostMessage(mThread.mainWindow, WM_CLOSE, 0, 0);
		break;
	case UWM_IGNORED:
		if (!isAnswer) PostMessage(mThread.mainWindow, UWM_CHECK_CALCULATION, true, 0);
		break;
	case UWM_START_CALCULATION:
		if (len = GetWindowTextLength(mThread.mainEdit) + 1) {
			TCHAR buff[1024];
			GetWindowText(mThread.mainEdit, buff, len);
			x[0].value = _tstoi(buff);

			TCHAR a[256] = TEXT("\0"), b[256] = TEXT("\0");
			switch (getChekedRadBtn(mThread.mainRadButtons[0])) {
			case RD_BTN_0:
				_tcscat(a, buff);
				_tcscat(a, TEXT(" getZero"));
				break;
			case RD_BTN_1:
				_tcscat(a, buff);
				_tcscat(a, TEXT(" getNonZero"));
				break;
			case RD_BTN_2:
				_tcscat(a, buff);
				_tcscat(a, TEXT(" getZeroDelay"));
				break;
			case RD_BTN_3:
				_tcscat(a, buff);
				_tcscat(a, TEXT(" getNonZeroDelay"));
				break;
			case RD_BTN_4:
				_tcscat(a, buff);
				_tcscat(a, TEXT(" hang"));
				break;
			}

			switch (getChekedRadBtn(mThread.mainRadButtons[1])) {
			case RD_BTN_5:
				_tcscat(b, buff);
				_tcscat(b, TEXT(" getZero"));
				break;
			case RD_BTN_6:
				_tcscat(b, buff);
				_tcscat(b, TEXT(" getNonZero"));
				break;
			case RD_BTN_7:
				_tcscat(b, buff);
				_tcscat(b, TEXT(" getZeroDelay"));
				break;
			case RD_BTN_8:
				_tcscat(b, buff);
				_tcscat(b, TEXT(" getNonZeroDelay"));
				break;
			case RD_BTN_9:
				_tcscat(b, buff);
				_tcscat(b, TEXT(" hang"));
				break;
			}

			launchChildProcesses(a, b);
			hP[0] = pi[0].hProcess;
			hP[1] = pi[1].hProcess;

			promptThreadHandle = createPromptThread();

			ShowWindow(mThread.mainWindow, HIDE_WINDOW);
		}
		break;
	case UWM_WAIT:
		if (isWait) {
			end = clock();
			if (((double)end - begin) / CLOCKS_PER_SEC * 1000 > ERROR_CONST) PostMessage(mThread.mainWindow, UWM_CHECK_CALCULATION, true, 0);
			else PostMessage(mThread.mainWindow, UWM_WAIT, 0, 0);
		}
		else {
			if (x[1].stuck || x[2].stuck) {
				isWait = true;
				begin = clock();
				PostMessage(mThread.mainWindow, UWM_WAIT, 0, 0);
			}
		}
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}

ATOM createWindowClass(HINSTANCE hInst, LPCTSTR lpzClassName) {
	WNDCLASS wcWindowClass = { 0 };
	wcWindowClass.style = CS_HREDRAW | CS_VREDRAW;
	wcWindowClass.lpfnWndProc = WindowProc;
	wcWindowClass.hInstance = hInst;
	wcWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcWindowClass.hbrBackground = (HBRUSH)COLOR_APPWORKSPACE;
	wcWindowClass.lpszClassName = lpzClassName;
	return RegisterClass(&wcWindowClass);
}

void createRadBtnsGroup(HINSTANCE hInst, HWND *radBtnArr, HWND *parent) {
	static int group = 0;
	radBtnArr[0] = CreateWindow(TEXT("Button"), TEXT("Get zero value..."), WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, RADIO_BTN_WIDTH * group,
		RADIO_BTN_HEIGTH * 0, RADIO_BTN_WIDTH, RADIO_BTN_HEIGTH, *parent, (HMENU)(group ? RD_BTN_5 : RD_BTN_0), hInst, NULL);
	radBtnArr[1] = CreateWindow(TEXT("Button"), TEXT("Get non zero value..."), WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, RADIO_BTN_WIDTH * group,
		RADIO_BTN_HEIGTH * 1, RADIO_BTN_WIDTH, RADIO_BTN_HEIGTH, *parent, (HMENU)(group ? RD_BTN_6 : RD_BTN_1), hInst, NULL);
	radBtnArr[2] = CreateWindow(TEXT("Button"), TEXT("Get zero value (delay)..."), WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, RADIO_BTN_WIDTH * group,
		RADIO_BTN_HEIGTH * 2, RADIO_BTN_WIDTH, RADIO_BTN_HEIGTH, *parent, (HMENU)(group ? RD_BTN_7 : RD_BTN_2), hInst, NULL);
	radBtnArr[3] = CreateWindow(TEXT("Button"), TEXT("Get non zero value (delay)..."), WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, RADIO_BTN_WIDTH * group,
		RADIO_BTN_HEIGTH * 3, RADIO_BTN_WIDTH, RADIO_BTN_HEIGTH, *parent, (HMENU)(group ? RD_BTN_8 : RD_BTN_3), hInst, NULL);
	radBtnArr[4] = CreateWindow(TEXT("Button"), TEXT("Hanged function..."), WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, RADIO_BTN_WIDTH * group,
		RADIO_BTN_HEIGTH * 4, RADIO_BTN_WIDTH, RADIO_BTN_HEIGTH, *parent, (HMENU)(group ? RD_BTN_9 : RD_BTN_4), hInst, NULL);
	group = 1 - group;
}

void createWindow(HINSTANCE hInst, LPCTSTR lpzClassName) {
	createWindowClass(hInst, CLASS_NAME);

	RECT screen_rect;
	GetWindowRect(GetDesktopWindow(), &screen_rect);
	int x = screen_rect.right / 2 - 150;
	int y = screen_rect.bottom / 2 - 75;

	mThread.mainWindow = CreateWindow(CLASS_NAME, WINDOW_NAME, WS_OVERLAPPEDWINDOW | WS_VISIBLE, x, y, WIND_WIDTH, WIND_HEIGTH, NULL, NULL, hInst, NULL);
	mThread.mainEdit = CreateWindow(TEXT("Edit"), TEXT(""), WS_CHILD | WS_VISIBLE, 0, RADIO_BTN_HEIGTH * 5, EDT_WIDTH, EDT_HEIGTH, mThread.mainWindow, NULL, hInst, NULL);
	mThread.mainButton = CreateWindow(TEXT("Button"), TEXT("Start calculations"), WS_CHILD | WS_VISIBLE, 0, RADIO_BTN_HEIGTH * 5 + EDT_HEIGTH, BUTTN_WIDTH, BUTTN_HEIGTH, mThread.mainWindow, (HMENU) BTN_START, hInst, NULL);
	createRadBtnsGroup(hInst, mThread.mainRadButtons[0], &mThread.mainWindow);
	createRadBtnsGroup(hInst, mThread.mainRadButtons[1], &mThread.mainWindow);

	// BOOL a = RegisterHotKey(NULL, 0, 0, VK_ESCAPE);
}

void launchChildProcesses(WCHAR *a, WCHAR *b) {
	si[0].cb = si[1].cb = sizeof(si[0]);
	si[0].dwFlags = si[1].dwFlags = STARTF_USESHOWWINDOW;
	si[0].wShowWindow = si[1].wShowWindow = SW_HIDE;

	if (!CreateProcess(TEXT("..\\x64\\Debug\\function.exe"), a, NULL, NULL, FALSE, 0, NULL, NULL, &si[0], &pi[0])) return;
	if (!CreateProcess(TEXT("..\\x64\\Debug\\function.exe"), b, NULL, NULL, FALSE, 0, NULL, NULL, &si[1], &pi[1])) return;
}

BOOL killProcess(PROCESS_INFORMATION *pi) {
	return PostThreadMessage(GetThreadId(pi->hThread), WM_QUIT, 0, 0);
}

void closeChildProcessesHandles(PROCESS_INFORMATION *pi) {
	if (pi[0].hProcess) CloseHandle(pi[0].hProcess);
	if (pi[0].hThread) CloseHandle(pi[0].hThread);

	if (pi[1].hProcess) CloseHandle(pi[1].hProcess);
	if (pi[1].hThread) CloseHandle(pi[1].hThread);
}

DWORD promptThread(LPVOID x) {
	int answer = 0;

	do {
		promptTime = clock();
		Sleep(PROMPT_DELAY);
		isPrompted = true;
		if (!isAnswer) 
			answer = MessageBox(NULL, TEXT("Do you want to continue recieving this message, or cancel the calculations?"), TEXT("Confirmation"), MB_ICONQUESTION | MB_ABORTRETRYIGNORE | MB_DEFBUTTON1);
		isPrompted = false;
	} while (answer == IDRETRY && !isAnswer);
	if (answer == IDABORT) PostMessage(mThread.mainWindow, UWM_CANCELLED, 0, 0);
	else if (answer == IDIGNORE) PostMessage(mThread.mainWindow, UWM_IGNORED, 0, 0);
	return 0;
}

HANDLE createPromptThread() {
	return CreateThread(NULL, 0, promptThread, (LPVOID) &mThread, 0, NULL);
}

int binOperation(int a, int b) {
	return a * b;
}
