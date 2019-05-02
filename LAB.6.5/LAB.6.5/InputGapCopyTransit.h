#pragma once
#include "DefGapTransit.h"
class InputGapCopyTransit : public DefGapTransit {
public:
	InputGapCopyTransit(char x, char y, IState *nextState);
	virtual ~InputGapCopyTransit();

	 const char * output(char x) const;
protected:
	char *data;
};

