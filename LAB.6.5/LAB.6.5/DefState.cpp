#include "DefState.h"

DefState::DefState() {

}

DefState::~DefState() {
	
}

IState * DefState::process(char x) {
	return nullptr;
}

void DefState::addTransit(ITransit *transit) {
	transits.addElement(transit);
}

bool DefState::deleteTransit(size_t i) {
	return transits.deleteElement(i);
}

bool DefState::deleteTransit(ITransit *transit) {
	return transits.deleteElement(transit);
}
