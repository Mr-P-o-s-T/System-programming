#pragma once
#include "DefGapTransit.h"
class DefaultCaseTransit : public DefGapTransit {
public:
	DefaultCaseTransit(const char *out, IState *nextState);
	virtual ~DefaultCaseTransit();

	bool operator==(const ITransit &transit) const;
	bool operator>(const ITransit &transit) const;
	bool operator<(const ITransit &transit) const;

	bool operator==(const IGapTransit &transit) const;
	bool operator>(const IGapTransit &transit) const;
	bool operator<(const IGapTransit &transit) const;
};
