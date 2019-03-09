#include "DefTransit.h"

#include <typeinfo>
#include <iostream>

#include "IState.h"
#include "DefGapTransit.h"
using std::cout;

DefTransit::DefTransit(char x, const char *out, IState *nextState) {
	this->x = x;
	this->out = out;
	this->nextState = nextState;
}

const char * DefTransit::output() const {
	return out;
}

IState * DefTransit::goTo() const {
	return nextState;
}

bool DefTransit::operator==(const ITransit &transit) const {
	return this->x == transit.getX();
}

bool DefTransit::operator>(const ITransit &transit) const {
	return this->x > transit.getX();
}

bool DefTransit::operator==(const IGapTransit & transit) const {
	return false;
}

bool DefTransit::operator>(const IGapTransit & transit) const {
	return transit.getY() < x;
}

bool DefTransit::operator<(const IGapTransit & transit) const {
	return transit.getX() > x;
}

char DefTransit::getX() const {
	return x;
}
