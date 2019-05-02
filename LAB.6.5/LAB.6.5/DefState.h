#pragma once
#include "IState.h"
#include "IStateEditable.h"

template<class T>
class Container;

class ITransit;

class DefState : public IState, public IStateEditable {
public:
	DefState(const char *name);
	DefState(const char *name, ITransit **data, size_t length);
	virtual ~DefState();

	bool isSubAutomat() {
		return false;
	}
	bool isBuffState() {
		return false;
	}
	ITransit *getTransition(char x);

	void addTransit(ITransit *transit);
	bool deleteTransit(size_t i);
	bool deleteTransit(ITransit *transit);

	bool operator==(const IState &state) const;
	bool operator>(const IState &state) const;
	bool operator<(const IState &state) const;
protected:
	const char *name;
	Container<ITransit *> *transits;
};
