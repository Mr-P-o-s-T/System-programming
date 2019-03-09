#pragma once

template<class ComparableWith>
class IComparable {
public:
	virtual bool operator==(const ComparableWith &transit) const = 0;
	virtual bool operator>(const ComparableWith &transit) const = 0;
	virtual bool operator<(const ComparableWith &transit) const {
		return !(*this >= transit);
	}

	virtual bool operator!=(const ComparableWith &transit) const {
		return !(*this == transit);
	};
	virtual bool operator>=(const ComparableWith &transit) const {
		return (*this > transit) || (*this == transit);
	};
	virtual bool operator<=(const ComparableWith &transit) const {
		return (*this < transit) || (*this == transit);
	}
protected:
	IComparable() = default;
	IComparable(const IComparable &) = delete;
	IComparable(IComparable &&) = delete;

	virtual ~IComparable() = default;

	IComparable & operator=(const IComparable &) = delete;
	IComparable & operator=(IComparable &&) = delete;
};
