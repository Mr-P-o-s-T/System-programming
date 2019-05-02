#pragma once
#include "IAutomat.h"

template<class T>
class Container;

class SubAutomat : public IAutomat {
public:
	SubAutomat(const char* name);
	virtual ~SubAutomat();

	void addState(IState *state);
	bool deleteState(size_t i);
	bool deleteState(IState *state);

	void setStartState(IState *start);
	virtual IState * getStartState();
	void goTo(IState *state);
	IState * getCurrState();
	void setEndState(IState *end);
	virtual IState * getEndState();

	void reset();
	bool isDone();

	bool isSubAutomat() {
		return true;
	}
	bool isBuffState();
	ITransit* getTransition(char x);

	bool operator==(const IState &state) const;
	bool operator>(const IState &state) const;
	bool operator<(const IState &state) const;
protected:
	const char* name;
	IState *startState, *currState, *endState;
	Container<IState *> *states;
	bool subAutoEndSt = false;
};
