#pragma once
#include "IState.h"
#include "IStateEditable.h"

template<class T>
class Container;

class ITransit;

class DefState : public IState, public IStateEditable {
public:
	DefState();
	DefState(ITransit **data, size_t length);
	virtual ~DefState();

	bool isSubAutomat() {
		return false;
	}
	ITransit *getTransition(char x);

	void addTransit(ITransit *transit);
	bool deleteTransit(size_t i);
	bool deleteTransit(ITransit *transit);

	bool operator==(const IState &state) const;
	bool operator>(const IState &state) const;
protected:
	Container<ITransit *> *transits;
};
