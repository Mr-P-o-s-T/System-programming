#pragma once
#include "ITransit.h"

class IGapTransit;

class DefTransit : public virtual ITransit {
public:
	DefTransit(char x, const char *out = nullptr, IState *nextState = nullptr);
	virtual ~DefTransit() = default;

	const char *output() const;
	IState* goTo() const;

	virtual bool operator==(const ITransit &transit) const;
	virtual bool operator>(const ITransit &transit) const;

	virtual bool operator==(const IGapTransit &transit) const;
	virtual bool operator>(const IGapTransit &transit) const;
	virtual bool operator<(const IGapTransit &transit) const;
protected:
	char x;
	const char *out;
	IState *nextState;

	char getX() const;
};

