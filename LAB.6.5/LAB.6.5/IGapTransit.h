#pragma once
#include "ITransit.h"

class IGapTransit : public virtual ITransit {
public:
	virtual ~IGapTransit() = default;

	virtual char getY() const = 0;
	virtual void setX(char x) = 0;
	virtual void setY(char y) = 0;
protected:
	IGapTransit() = default;
	IGapTransit(const IGapTransit &) = delete;
	IGapTransit(IGapTransit &&) = delete;
	IGapTransit & operator=(const IGapTransit &) = delete;
	IGapTransit & operator=(IGapTransit &&) = delete;
};
