// f function
#include <iostream>
#include <Windows.h>
#include <tchar.h>
using namespace std;

float getF(float x);

int main(int argc, char* argv[]) {
	MessageBoxA(NULL, "Attach now", NULL, MB_OK);
	string funcName(argv[0]);
	float x = atof(argv[1]);
	
	if (funcName != "") {
		float res = getF(x);
	}

	return 0;
}

float getF(float x) {
	Sleep(10000);
	return 0.0f;
}
