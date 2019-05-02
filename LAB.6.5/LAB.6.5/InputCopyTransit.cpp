#include "InputCopyTransit.h"

#include <ctype.h>
using namespace std;

InputCopyTransit::InputCopyTransit(char x, IState * nextState) : DefTransit(x, nullptr, nextState) {
	char *tmp = new char[2];
	tmp[0] = this->x;
	tmp[1] = '\0';

	out = tmp;
}

InputCopyTransit::~InputCopyTransit() {
	delete[] out;
}
