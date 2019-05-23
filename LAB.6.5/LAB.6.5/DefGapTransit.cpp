#include "DefGapTransit.h"

#include <iostream>

DefGapTransit::DefGapTransit(char x, char y, const char *out, IState *nextState) : DefTransit(x, out, nextState) {
	this->y = toupper(static_cast<unsigned char>(y));
	if (x > y) {
		char tmp = x; 
		x = y; 
		y = tmp;
	}
}

bool DefGapTransit::operator==(const ITransit & transit) const {
	try {
		const IGapTransit &gapTransit = dynamic_cast<const IGapTransit &>(transit);
		return *this == gapTransit;
	}
	catch (std::bad_cast exception) {
		return static_cast<unsigned char>(x) <= static_cast<unsigned char>(transit.getX()) 
			&& static_cast<unsigned char>(transit.getX()) <= static_cast<unsigned char>(y);
	}
}

bool DefGapTransit::operator>(const ITransit &transit) const {
	try {
		const IGapTransit &gapTransit = dynamic_cast<const IGapTransit &>(transit);
		return *this > gapTransit;
	}
	catch (std::bad_cast exception) {
		return static_cast<unsigned char>(this->x) > static_cast<unsigned char>(transit.getX());
	}
}

bool DefGapTransit::operator<(const ITransit & transit) const {
	try {
		const IGapTransit &gapTransit = dynamic_cast<const IGapTransit &>(transit);
		return *this < gapTransit;
	}
	catch (std::bad_cast exception) {
		return static_cast<unsigned char>(this->y) < static_cast<unsigned char>(transit.getX());
	}
}

bool DefGapTransit::operator==(const IGapTransit & transit) const {
	return (static_cast<unsigned char>(this->x) == static_cast<unsigned char>(transit.getX())) 
		&& (static_cast<unsigned char>(this->y) == static_cast<unsigned char>(transit.getY()));
}

bool DefGapTransit::operator>(const IGapTransit & transit) const {
	return static_cast<unsigned char>(this->x) > static_cast<unsigned char>(transit.getY());
}

bool DefGapTransit::operator<(const IGapTransit & transit) const {
	return static_cast<unsigned char>(this->y) < static_cast<unsigned char>(transit.getX());
}

char DefGapTransit::getY() const {
	return y;
}

void DefGapTransit::setX(char x) {
	this->x = x;
}

void DefGapTransit::setY(char y) {
	this->y = y;
}
