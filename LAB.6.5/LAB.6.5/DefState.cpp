#include "DefState.h"

#include "DefTransit.h"
#include "Container.h"

DefState::DefState(const char *name) {
	this->name = name;
	transits = new Container<ITransit *>();
}

DefState::DefState(const char *name, ITransit ** data, size_t length) {
	this->name = name;
	transits = new Container<ITransit *>(data, length);
}

DefState::~DefState() {
	delete transits;
}

ITransit * DefState::getTransition(char x) {
	DefTransit tmp(x);
	ITransit *transit = (*transits)[0];
	for (size_t i = 0; transit; i++, transit = (*transits)[i])
		if (*transit == tmp) return transit;
	return nullptr;
}

void DefState::addTransit(ITransit *transit) {
	transits->addElement(transit);
}

bool DefState::deleteTransit(size_t i) {
	return transits->deleteElement(i);
}

bool DefState::deleteTransit(ITransit *transit) {
	return transits->deleteElement(transit);
}

inline bool DefState::operator==(const IState & state) const {
	return this == &state;
}

inline bool DefState::operator>(const IState & state) const {
	return false;
}

bool DefState::operator<(const IState & state) const {
	return false;
}
