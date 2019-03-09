#include "Automat.h"

#include "SubAutomat.h"
#include "DefTransit.h"
#include "DefGapTransit.h"
#include "DefState.h"
using namespace std;


Automat::Automat(const char *input, const char *output) {
	this->input = ifstream(input);
	this->output = ofstream(output);
	automat = buildAutomat();
}

Automat::~Automat() {
	delete automat;
}

void Automat::processSymbol() {
	char sym;
	input >> sym;
	ITransit * tmp = automat->getTransition(sym);
	output << tmp->output();
}

bool Automat::isDone() {
	return automat->isDone();
}

void Automat::reset() {
	automat->reset();
}

SubAutomat * Automat::buildAutomat() {
	SubAutomat *automat = new SubAutomat();

	{
		DefState *start = new DefState(), *end = new DefState();
		automat->setStartState(start);
		automat->setEndState(end);
		start->addTransit(new DefTransit('\0', "", end));
	}

	// TODO: Add build code

	return automat;
}
