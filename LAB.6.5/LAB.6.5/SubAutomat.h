#pragma once
#include "IAutomat.h"

template<class T>
class Container;

class SubAutomat : public IAutomat {
public:
	SubAutomat();
	virtual ~SubAutomat();

	void addState(IState *state);
	bool deleteState(size_t i);
	bool deleteState(IState *state);

	void setStartState(IState *start);
	void goTo(IState *state);
	void setEndState(IState *end);

	void reset();
	bool isDone();

	bool isSubAutomat() {
		return true;
	}
	ITransit* getTransition(char x);

	bool operator==(const IState &state) const;
	bool operator>(const IState &state) const;
protected:
	IState *startState, *currState, *endState;
	Container<IState *> *states;
	Container<IAutomat *> *subAutomats;
};

