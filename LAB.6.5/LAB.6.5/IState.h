#pragma once
#include "IComparable.h"

class ITransit;

class IState : public IComparable<IState> {
public:
	virtual ~IState() = default;

	virtual bool isSubAutomat() = 0;
	virtual bool isBuffState() = 0;
	virtual ITransit* getTransition(char x) = 0;
protected:
	IState() = default;
	IState(const IState &) = delete;
	IState(IState &&) = delete;

	IState & operator=(const IState &) = delete;
	IState & operator=(IState &&) = delete;
};
