#pragma once
#include "IState.h"
#include "IStateEditable.h"

class IAutomat : public IState {
public:
	virtual ~IAutomat() = default;

	virtual void addState(IState *) = 0;
	virtual bool deleteState(size_t i) = 0;
	virtual bool deleteState(IState *) = 0;

	virtual void setStartState(IState *) = 0;
	virtual void goTo(IState *) = 0;
	virtual void setEndState(IState *) = 0;

	virtual void reset() = 0;
	virtual bool isDone() = 0;
protected:
	IAutomat() = default;
	IAutomat(const IAutomat &) = delete;
	IAutomat(IAutomat &&) = delete;
	IAutomat & operator=(const IAutomat &) = delete;
	IAutomat & operator=(IAutomat &&) = delete;
};
