#include "InputGapCopyTransit.h"

#include <ctype.h>

InputGapCopyTransit::InputGapCopyTransit(char x, char y, IState *nextState) : DefGapTransit(x, y, nullptr, nextState) {
	data = new char [2];
	data[1] = '\0';
}

InputGapCopyTransit::~InputGapCopyTransit() {
	delete[] data;
}

const char * InputGapCopyTransit::output(char x) const {
	data[0] = x;
	return data;
}
