#pragma once
#include "DefTransit.h"
#include "IGapTransit.h"

class DefGapTransit : public virtual DefTransit, public virtual IGapTransit {
public:
	DefGapTransit(char x, char y, const char *out = nullptr, IState *nextState = nullptr);
	virtual ~DefGapTransit() = default;

	bool operator==(const ITransit &transit) const;
	bool operator>(const ITransit &transit) const;
	bool operator<(const ITransit &transit) const;

	bool operator==(const IGapTransit &transit) const;
	bool operator>(const IGapTransit &transit) const;
	bool operator<(const IGapTransit &transit) const;
protected:
	char y;

	char getY() const;
	void setX(char x);
	void setY(char y);
};

