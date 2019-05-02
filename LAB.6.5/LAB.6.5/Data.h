#pragma once
#include "IComparable.h"

#include <string>

class Data : IComparable<Data> {
public:
	Data(std::string data);
	~Data() = default;

	bool areEq(const std::string &data);
	size_t getNext() {
		return data.size();
	}

	bool operator==(const Data &transit) const;
	bool operator>(const Data &transit) const;
	bool operator<(const Data &transit) const;
protected:
	std::string data;
};
