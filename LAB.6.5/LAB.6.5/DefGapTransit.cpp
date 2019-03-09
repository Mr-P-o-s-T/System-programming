#include "DefGapTransit.h"

DefGapTransit::DefGapTransit(char x, char y, const char *out, IState *nextState) : DefTransit(x, out, nextState) {
	this->y = y;
	if (x > y) {
		char tmp = x; 
		x = y; 
		y = tmp;
	}
}

bool DefGapTransit::operator==(const ITransit & transit) const {
	return false;
}

bool DefGapTransit::operator>(const ITransit &transit) const {
	return this->x > transit.getX();
}

bool DefGapTransit::operator<(const ITransit & transit) const {
	return this->y < transit.getX();
}

bool DefGapTransit::operator==(const IGapTransit & transit) const {
	return (this->x == transit.getX()) && (this->y == transit.getY());
}

bool DefGapTransit::operator>(const IGapTransit & transit) const {
	return this->x > transit.getY();
}

bool DefGapTransit::operator<(const IGapTransit & transit) const {
	return this->y < transit.getX();
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
