#include "Data.h"
#include <algorithm>
using namespace std;

Data::Data(string data) {
	this->data = data;
}

bool Data::areEq(const std::string &data) {
	string tmp(data);
	transform(tmp.begin(), tmp.end(), tmp.begin(), ::toupper);
	return tmp == this->data;
}

bool Data::operator==(const Data &data) const {
	return *this == data;
}

bool Data::operator>(const Data &data) const {
	return false;
}

bool Data::operator<(const Data &data) const {
	return false;
}
