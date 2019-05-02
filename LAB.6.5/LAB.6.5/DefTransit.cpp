#include "DefTransit.h"

#include <typeinfo>
#include <iostream>

#include "IState.h"
#include "DefGapTransit.h"
using std::cout;

DefTransit::DefTransit(char x, const char *out, IState *nextState) {
	this->x = toupper(static_cast<char>(x));
	this->out = out;
	this->nextState = nextState;
}

const char * DefTransit::output(char x) const {
	return out;
}

IState * DefTransit::goTo() const {
	return nextState;
}

bool DefTransit::operator==(const ITransit &transit) const {
	try {
		const IGapTransit &gapTransit = dynamic_cast<const IGapTransit &>(transit);
		return *this == gapTransit;
	}
	catch (std::bad_cast exception) {
		return static_cast<unsigned char>(this->x) == static_cast<unsigned char>(transit.getX());
	}
}

bool DefTransit::operator>(const ITransit &transit) const {
	try {
		const IGapTransit &gapTransit = dynamic_cast<const IGapTransit &>(transit);
		return *this > gapTransit;
	}
	catch (std::bad_cast exception) {
		return static_cast<unsigned char>(this->x) > static_cast<unsigned char>(transit.getX());
	}
}

bool DefTransit::operator<(const ITransit & transit) const {
	try {
		const IGapTransit &gapTransit = dynamic_cast<const IGapTransit &>(transit);
		return *this < gapTransit;
	}
	catch (std::bad_cast exception) {
		return static_cast<unsigned char>(this->x) < static_cast<unsigned char>(transit.getX());
	}
}

bool DefTransit::operator==(const IGapTransit & transit) const {
	return false;
}

bool DefTransit::operator>(const IGapTransit & transit) const {
	return static_cast<unsigned char>(transit.getY()) < static_cast<unsigned char>(x);
}

bool DefTransit::operator<(const IGapTransit & transit) const {
	return static_cast<unsigned char>(transit.getX()) > static_cast<unsigned char>(x);
}

char DefTransit::getX() const {
	return x;
}
