#include "DefState.h"

#include "DefTransit.h"
#include "Container.h"

DefState::DefState() {
	transits = new Container<ITransit *>();
}

DefState::DefState(ITransit ** data, size_t length) {
	transits = new Container<ITransit *>(data, length);
}

DefState::~DefState() {
	delete transits;
}

ITransit * DefState::getTransition(char x) {
	ITransit *tmp = new DefTransit(x), *transit = (*transits)[0];
	for (size_t i = 0; transit; i++, transit = (*transits)[i])
		if (*transit == *tmp) return transit;
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
