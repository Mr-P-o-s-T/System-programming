#pragma once
#include "DefTransit.h"
class InputCopyTransit : public DefTransit {
public:
	InputCopyTransit(char x, IState *nextState);
	virtual ~InputCopyTransit();
};
