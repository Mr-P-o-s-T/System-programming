#pragma once

class IState;

class ITransit {
public:
	virtual ~ITransit() = default;

	virtual IState* goTo(char c) = 0;

	virtual bool operator==(const ITransit &transit) const = 0;
	virtual bool operator!=(const ITransit &transit) const {
		return !(*this == transit);
	};
	virtual bool operator>(const ITransit &transit) const = 0;
	virtual bool operator>=(const ITransit &transit) const {
		return (*this > transit) || (*this == transit);
	};
	virtual bool operator<(const ITransit &transit) const {
		return !(*this >= transit);
	}
	virtual bool operator<=(const ITransit &transit) const {
		return !(*this > transit);
	}
protected:
	ITransit() = default;
	ITransit(const ITransit&) = delete;
	ITransit& operator=(const ITransit&) = delete;
};
