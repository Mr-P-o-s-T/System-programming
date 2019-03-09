#include "SubAutomat.h"

#include "Container.h"
#include "ITransit.h"

SubAutomat::SubAutomat() {
	states = new Container<IState *>();
	subAutomats = new Container<IAutomat *>();
}

SubAutomat::~SubAutomat() {
	delete states;
	delete subAutomats;
}

void SubAutomat::addState(IState *state) {
	return states->addElement(state);
	if (state->isSubAutomat()) subAutomats->addElement(dynamic_cast<IAutomat *>(state));
}

bool SubAutomat::deleteState(size_t i) {
	if ((*states)[i]->isSubAutomat()) subAutomats->deleteElement(dynamic_cast<IAutomat *>((*states)[i]));
	return states->deleteElement(i);
}

bool SubAutomat::deleteState(IState *state) {
	if (state->isSubAutomat()) subAutomats->deleteElement(dynamic_cast<IAutomat *>(state));
	return states->deleteElement(state);
}

void SubAutomat::setStartState(IState *start) {
	startState = currState = start;
}

void SubAutomat::goTo(IState *state) {
	currState = state;
}

void SubAutomat::setEndState(IState * end) {
	endState = end;
}

void SubAutomat::reset() {
	currState = startState;
	for (size_t i = 0; i < subAutomats->getLength(); i++) (*subAutomats)[i]->reset();
}

bool SubAutomat::isDone() {
	return currState == endState;
}

ITransit * SubAutomat::getTransition(char x) {
	ITransit *tmp = currState->getTransition(x);
	if (isDone()) reset();
	else if (!currState->isSubAutomat()) goTo(tmp->goTo());
	return tmp;
}

bool SubAutomat::operator==(const IState & state) const {
	return false;
}

bool SubAutomat::operator>(const IState & state) const {
	return false;
}
