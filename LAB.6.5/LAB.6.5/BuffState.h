#pragma once
#include "DefState.h"
class BuffState : public DefState {
public:
	BuffState(const char *name);
	virtual ~BuffState();

	bool isBuffState() final {
		return true;
	}
};

