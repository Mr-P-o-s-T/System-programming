#pragma once
#include "IComparable.h"

class IState;
class IGapTransit;

class ITransit : public IComparable<ITransit>, public IComparable<IGapTransit> {
public:
	virtual ~ITransit() = default;

	virtual char getX() const = 0;
	virtual const char * output() const = 0;
	virtual IState* goTo() const = 0;
protected:
	ITransit() = default;
	ITransit(const ITransit &) = delete;
	ITransit(ITransit &&) = delete;

	ITransit & operator=(const ITransit &) = delete;
	ITransit & operator=(ITransit &&) = delete;
};
