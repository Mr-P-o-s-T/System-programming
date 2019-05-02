#include "SubAutomat.h"

#include "Container.h"
#include "ITransit.h"

SubAutomat::SubAutomat(const char* name) {
	this->name = name;
	states = new Container<IState *>();
}

SubAutomat::~SubAutomat() {
	delete states;
}

void SubAutomat::addState(IState *state) {
	return states->addElement(state);
}

bool SubAutomat::deleteState(size_t i) {
	return states->deleteElement(i);
}

bool SubAutomat::deleteState(IState *state) {
	return states->deleteElement(state);
}

void SubAutomat::setStartState(IState *start) {
	addState(start);
	startState = currState = start;
}

IState * SubAutomat::getStartState() {
	return startState;
}

void SubAutomat::goTo(IState *state) {
	currState = state;
}

IState * SubAutomat::getCurrState() {
	return currState;
}

void SubAutomat::setEndState(IState * end) {
	addState(end);
	endState = end;
}

IState * SubAutomat::getEndState() {
	return endState;
}

void SubAutomat::reset() {
	currState = startState;
}

bool SubAutomat::isDone() {
	return currState == endState;
}

bool SubAutomat::isBuffState() {
	return currState->isBuffState();
}

ITransit * SubAutomat::getTransition(char x) {
	ITransit *tmp = currState->getTransition(x);
	if (isDone()) reset();
	else if (!currState->isSubAutomat()) goTo(tmp->goTo());
	else {
		IAutomat *autmp = dynamic_cast<IAutomat *>(currState);
		if (autmp->isDone() || subAutoEndSt) 
			if (subAutoEndSt) {
				goTo(tmp->goTo());
				subAutoEndSt = false;
			}
			else subAutoEndSt = true;
	}
	return tmp;
}

bool SubAutomat::operator==(const IState & state) const {
	return false;
}

bool SubAutomat::operator>(const IState & state) const {
	return false;
}

bool SubAutomat::operator<(const IState & state) const
{
	return false;
}
