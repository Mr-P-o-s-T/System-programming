#include "Automat.h"

#include "SubAutomat.h"
#include "BuffState.h"

#include "InputCopyTransit.h"
#include "InputGapCopyTransit.h"
#include "DefaultCaseTransit.h"
using namespace std;


Automat::Automat(const char *input, const char *output) {
	this->input = ifstream(input);
	this->output = ofstream(output);
	parser = new HTMLParser("dtb/resId.txt", "dtb/logExpr.txt");
	automat = buildAutomat();
}

Automat::~Automat() {
	if (input.is_open()) input.close();
	if (output.is_open()) output.close();
	delete parser;
	delete automat;
}

void Automat::ProcessFile() {
	reset();
	parser->prepareOutputFile(output);
	while (!isDone()) processSymbol();
	parser->finishOutputFile(output);
	input.close();
	output.close();
}

void Automat::processSymbol() {
	static char sym = '@';
	if ((!automat->isBuffState()) || (sym == '@')) {
		if (input.eof()) sym = '\0';
		else input.get(sym);
	}
	ITransit * tmp = automat->getTransition(sym);
	const char *out = tmp->output(sym);
	if (out[0] == '@') {
		output << parser->parse(buff, out);
		buff = "";
	}
	else buff.append(out);
}

bool Automat::isDone() {
	return automat->isDone();
}

void Automat::reset() {
	automat->reset();
}

SubAutomat * Automat::buildAutomat() {
	SubAutomat *automat = new SubAutomat("automat");
	DefState *id = new DefState("idState");
	SubAutomat *numAutomat = new SubAutomat("numberAutomat");
	DefState *fractPart = new DefState("fractPart");
	SubAutomat *logicExpr = new SubAutomat("logicExprAutomat");
	SubAutomat *operators = new SubAutomat("operationsAutomat");
	SubAutomat *strings = new SubAutomat("stringsAutomat");
	DefState *comments = new DefState("commentsState");
	

	{
		BuffState *start = new BuffState("automatStart"), 
			*end = new BuffState("automatEnd");
		automat->setStartState(start);
		automat->setEndState(end);
		start->addTransit(new DefTransit('\0', "", end));

		start->addTransit(new InputGapCopyTransit('A', 'Z', id));
		
		start->addTransit(new InputGapCopyTransit('0', '9', numAutomat));

		start->addTransit(new DefTransit('+', "", operators));
		start->addTransit(new DefTransit('-', "", operators));
		start->addTransit(new DefTransit(')', "", operators));
		start->addTransit(new DefTransit(',', "", operators));
		start->addTransit(new DefTransit('&', "", operators));
		start->addTransit(new DefTransit(';', "", operators));
		start->addTransit(new DefTransit('%', "", operators));
		start->addTransit(new DefTransit('=', "", operators));
		start->addTransit(new DefTransit('*', "", operators));
		start->addTransit(new DefTransit('/', "", operators));
		start->addTransit(new DefTransit('(', "", operators));
		start->addTransit(new DefTransit('>', "", operators));
		start->addTransit(new DefTransit('<', "", operators));
		start->addTransit(new DefTransit(':', "", operators));

		start->addTransit(new DefTransit('\'', "", strings));
		start->addTransit(new DefTransit('"', "", strings));

		start->addTransit(new InputCopyTransit('!', comments));

		DefState *dotUsage = new DefState("dotUsage");
		automat->addState(dotUsage);
		start->addTransit(new InputCopyTransit('.', dotUsage));
		dotUsage->addTransit(new InputGapCopyTransit('0', '9', fractPart));
		dotUsage->addTransit(new InputGapCopyTransit('A', 'Z', logicExpr));
		dotUsage->addTransit(new DefaultCaseTransit("@", start));

		DefState *sepHandler = new DefState("sepHandler");
		automat->addState(sepHandler);

		start->addTransit(new InputCopyTransit(' ', sepHandler));
		start->addTransit(new InputCopyTransit('\t', sepHandler));
		start->addTransit(new InputCopyTransit('\n', sepHandler));
		sepHandler->addTransit(new DefTransit(' ', "", sepHandler));
		sepHandler->addTransit(new InputCopyTransit('\t', sepHandler));
		sepHandler->addTransit(new InputCopyTransit('\n', sepHandler));
		sepHandler->addTransit(new DefaultCaseTransit("", automat->getStartState()));

		DefState *errHandl = new DefState("errorHandler");
		automat->addState(errHandl);

		start->addTransit(new InputGapCopyTransit(0, 255, errHandl));
		errHandl->addTransit(new DefaultCaseTransit("@", automat->getStartState()));
	}

	// TODO: Add build code

	{
		// Identificator
		automat->addState(id);

		id->addTransit(new InputGapCopyTransit('A', 'Z', id));
		id->addTransit(new InputGapCopyTransit('0', '9', id));
		id->addTransit(new InputCopyTransit('_', id));
		id->addTransit(new DefaultCaseTransit("@i", automat->getStartState()));
	}

	{
		// Number
		automat->addState(numAutomat);

		DefState *wholePart = new DefState("wholePartAutomat");
		SubAutomat *wholeSpeciePart = new SubAutomat("wholeSpeciePartAutomat");
		BuffState *end = new BuffState("numAutomatEnd");
		numAutomat->setStartState(wholePart);
		numAutomat->setEndState(end);

		DefState *dotNum = new DefState("dotNum");
		SubAutomat *stepPart = new SubAutomat("stepPartAutomat");
		SubAutomat *speciePart = new SubAutomat("speciePartAutomat");

		{
			// Whole part

			wholePart->addTransit(new InputGapCopyTransit('0', '9', wholePart));
			wholePart->addTransit(new InputCopyTransit('.', dotNum));
			wholePart->addTransit(new InputCopyTransit('E', stepPart));
			wholePart->addTransit(new InputCopyTransit('D', stepPart));
			wholePart->addTransit(new InputCopyTransit('_', wholeSpeciePart));
			wholePart->addTransit(new DefaultCaseTransit("", numAutomat->getEndState()));
		}

		{
			// Whole specie part
			numAutomat->addState(wholeSpeciePart);

			DefState *start = new DefState("wholeSpeciePartStart");
			BuffState *end = new BuffState("wholeSpeciePartEnd");
			wholeSpeciePart->setStartState(start);
			wholeSpeciePart->setEndState(end);
			start->addTransit(new DefTransit('\'', "", end));
			start->addTransit(new DefTransit('"', "", end));

			DefState *numChain = new DefState("wholeSpeciePartNumChain");
			wholeSpeciePart->addState(numChain);
			start->addTransit(new InputGapCopyTransit('0', '9', numChain));
			numChain->addTransit(new InputGapCopyTransit('0', '9', numChain));
			numChain->addTransit(new DefaultCaseTransit("", end));

			DefState *idChain = new DefState("wholeSpeciePartIdChain");
			wholeSpeciePart->addState(idChain);
			start->addTransit(new InputGapCopyTransit('A', 'Z', idChain));
			start->addTransit(new DefaultCaseTransit("@", end));
			idChain->addTransit(new InputGapCopyTransit('A', 'Z', idChain));
			idChain->addTransit(new InputGapCopyTransit('0', '9', idChain));
			idChain->addTransit(new InputCopyTransit('_', idChain));
			idChain->addTransit(new DefaultCaseTransit("", end));

			end->addTransit(new DefaultCaseTransit("", numAutomat->getEndState()));
		}

		{
			// Dot state
			numAutomat->addState(dotNum);

			dotNum->addTransit(new InputGapCopyTransit('0', '9', fractPart));
			dotNum->addTransit(new InputCopyTransit('E', stepPart));
			dotNum->addTransit(new InputCopyTransit('D', stepPart));
			dotNum->addTransit(new InputCopyTransit('_', speciePart));
			dotNum->addTransit(new DefaultCaseTransit("", numAutomat->getEndState()));
		}

		{
			// Fraction part
			numAutomat->addState(fractPart);

			fractPart->addTransit(new InputGapCopyTransit('0', '9', fractPart));
			fractPart->addTransit(new InputCopyTransit('E', stepPart));
			fractPart->addTransit(new InputCopyTransit('D', stepPart));
			fractPart->addTransit(new InputCopyTransit('_', speciePart));
			fractPart->addTransit(new DefaultCaseTransit("", numAutomat->getEndState()));
		}

		{
			// Step part
			numAutomat->addState(stepPart);

			DefState *start = new DefState("stepPartStart");
			BuffState *end = new BuffState("stepPartEnd");
			stepPart->setStartState(start);
			stepPart->setEndState(end);

			DefState *sign = new DefState("stepPartSign");
			stepPart->addState(sign);
			start->addTransit(new InputCopyTransit('+', sign));
			start->addTransit(new InputCopyTransit('-', sign));

			DefState *numChain = new DefState("stepPartNumChain");
			stepPart->addState(numChain);
			start->addTransit(new InputGapCopyTransit('0', '9', numChain));
			start->addTransit(new DefaultCaseTransit("@", end));
			sign->addTransit(new InputGapCopyTransit('0', '9', numChain));
			sign->addTransit(new DefaultCaseTransit("@", end));
			numChain->addTransit(new InputGapCopyTransit('0', '9', numChain));
			numChain->addTransit(new DefaultCaseTransit("", end));

			end->addTransit(new InputCopyTransit('_', speciePart));
			end->addTransit(new DefaultCaseTransit("", numAutomat->getEndState()));
		}

		{
			// Specie part
			numAutomat->addState(speciePart);

			DefState *start = new DefState("speciePartStart");
			BuffState *end = new BuffState("speciePartEnd");
			speciePart->setStartState(start);
			speciePart->setEndState(end);

			DefState *numChain = new DefState("speciePartNumChain");
			speciePart->addState(numChain);
			start->addTransit(new InputGapCopyTransit('0', '9', numChain));
			numChain->addTransit(new InputGapCopyTransit('0', '9', numChain));
			numChain->addTransit(new DefaultCaseTransit("", end));

			DefState *idChain = new DefState("speciePartIdChain");
			speciePart->addState(idChain);
			start->addTransit(new InputGapCopyTransit('A', 'Z', idChain));
			start->addTransit(new DefaultCaseTransit("@", end));
			idChain->addTransit(new InputGapCopyTransit('A', 'Z', idChain));
			idChain->addTransit(new InputGapCopyTransit('0', '9', idChain));
			idChain->addTransit(new InputCopyTransit('_', idChain));
			idChain->addTransit(new DefaultCaseTransit("", end));

			end->addTransit(new DefaultCaseTransit("", numAutomat->getEndState()));
		}

		end->addTransit(new DefTransit('\'', "", strings));
		end->addTransit(new DefTransit('"', "", strings));
		end->addTransit(new DefaultCaseTransit("@n", automat->getStartState()));
	}

	{
		// Logic operations && constants
		automat->addState(logicExpr);

		DefState *start = new DefState("logicExprStart");
		BuffState *end = new BuffState("logicExprEnd");
		logicExpr->setStartState(start);
		logicExpr->setEndState(end);

		start->addTransit(new InputGapCopyTransit('A', 'Z', start));

		DefState *dot = new DefState("logicExprDot");
		logicExpr->addState(dot);
		start->addTransit(new InputCopyTransit('.', dot));
		start->addTransit(new DefaultCaseTransit("@", end));
		dot->addTransit(new DefaultCaseTransit("", end));

		end->addTransit(new DefaultCaseTransit("@l", automat->getStartState()));
	}

	{
		// Operators
		automat->addState(operators);

		BuffState *start = new BuffState("operationsStart"); 
		BuffState *end = new BuffState("operationsEnd");
		operators->setStartState(start);
		operators->setEndState(end);

		DefState *onlyOpSym = new DefState("operationsOnlyOpSym");
		operators->addState(onlyOpSym);
		start->addTransit(new InputCopyTransit('+', onlyOpSym));
		start->addTransit(new InputCopyTransit('-', onlyOpSym));
		start->addTransit(new InputCopyTransit(')', onlyOpSym));
		start->addTransit(new InputCopyTransit(',', onlyOpSym));
		start->addTransit(new InputCopyTransit('&', onlyOpSym));
		start->addTransit(new InputCopyTransit(';', onlyOpSym));
		start->addTransit(new InputCopyTransit('%', onlyOpSym));
		onlyOpSym->addTransit(new DefaultCaseTransit("", end));

		DefState *equalSym = new DefState("operationsEqualSym");
		operators->addState(equalSym);
		start->addTransit(new InputCopyTransit('=', equalSym));
		
		DefState *greaterSym = new DefState("operationsGreaterSym");
		operators->addState(greaterSym);
		start->addTransit(new InputCopyTransit('>', greaterSym));

		DefState *lessSym = new DefState("operationsLessSym");
		operators->addState(lessSym);
		start->addTransit(new InputCopyTransit('<', lessSym));

		DefState *multSym = new DefState("operationsMultSym");
		operators->addState(multSym);
		start->addTransit(new InputCopyTransit('*', multSym));

		DefState *divSym = new DefState("operationsDivSym");
		operators->addState(divSym);
		start->addTransit(new InputCopyTransit('/', divSym));

		DefState *brkSym = new DefState("operationsBrkSym");
		operators->addState(brkSym);
		start->addTransit(new InputCopyTransit('(', brkSym));

		DefState *doublDotSym = new DefState("operationsDoublDotSym");
		operators->addState(doublDotSym);
		start->addTransit(new InputCopyTransit(':', doublDotSym));

		DefState *secondOpSym = new DefState("operationsSecondOpSym");
		operators->addState(secondOpSym);

		equalSym->addTransit(new InputCopyTransit('=', secondOpSym));
		equalSym->addTransit(new InputCopyTransit('>', secondOpSym));
		equalSym->addTransit(new DefaultCaseTransit("", end));

		greaterSym->addTransit(new InputCopyTransit('=', secondOpSym));
		greaterSym->addTransit(new DefaultCaseTransit("", end));

		lessSym->addTransit(new InputCopyTransit('=', secondOpSym));
		lessSym->addTransit(new DefaultCaseTransit("", end));

		multSym->addTransit(new InputCopyTransit('*', secondOpSym));
		multSym->addTransit(new DefaultCaseTransit("", end));

		divSym->addTransit(new InputCopyTransit(')', secondOpSym));
		divSym->addTransit(new InputCopyTransit('/', secondOpSym));
		divSym->addTransit(new InputCopyTransit('=', secondOpSym));
		divSym->addTransit(new DefaultCaseTransit("", end));

		brkSym->addTransit(new InputCopyTransit('/', secondOpSym));
		brkSym->addTransit(new DefaultCaseTransit("", end));

		doublDotSym->addTransit(new InputCopyTransit(':', secondOpSym));
		doublDotSym->addTransit(new DefaultCaseTransit("", end));

		secondOpSym->addTransit(new DefaultCaseTransit("", end));

		end->addTransit(new DefaultCaseTransit("@o", automat->getStartState()));
	}

	{
		// Strings
		automat->addState(strings);

		BuffState *start = new BuffState("stringsStart"), 
			*end = new BuffState("stringsEnd");
		strings->setStartState(start);
		strings->setEndState(end);

		DefState *strChain = new DefState("stringsStrChain");
		strings->addState(strChain);
		start->addTransit(new InputCopyTransit('\'', strChain));
		start->addTransit(new InputCopyTransit('"', strChain));
		strChain->addTransit(new DefTransit('\n', "@", end));
		strChain->addTransit(new DefTransit('\0', "@", end));

		DefState *sepUse = new DefState("stringsSepUse");
		strings->addState(sepUse);
		strChain->addTransit(new InputCopyTransit('\'', sepUse));
		strChain->addTransit(new InputCopyTransit('"', sepUse));
		sepUse->addTransit(new InputCopyTransit('\'', strChain));
		sepUse->addTransit(new InputCopyTransit('"', strChain));
		sepUse->addTransit(new DefaultCaseTransit("", end));

		DefState *amperUse = new DefState("stringsAmperUse");
		strings->addState(amperUse);
		strChain->addTransit(new InputCopyTransit('&', amperUse));
		strChain->addTransit(new InputGapCopyTransit(1, 255, strChain));
		amperUse->addTransit(new InputCopyTransit('&', amperUse));

		DefState *newStringUse = new DefState("stringsNewStringUse");
		strings->addState(newStringUse);
		amperUse->addTransit(new InputCopyTransit('\n', newStringUse));
		amperUse->addTransit(new InputGapCopyTransit(1, 255, strChain));
		newStringUse->addTransit(new InputCopyTransit('&', amperUse));
		newStringUse->addTransit(new InputCopyTransit('\t', newStringUse));
		newStringUse->addTransit(new InputCopyTransit(' ', newStringUse));
		newStringUse->addTransit(new DefaultCaseTransit("@", end));

		end->addTransit(new DefaultCaseTransit("@s", automat->getStartState()));
	}

	{
		// Comments
		automat->addState(comments);

		comments->addTransit(new DefTransit('\n', "@c", automat->getStartState()));
		comments->addTransit(new DefTransit('\0', "@c", automat->getStartState()));
		comments->addTransit(new InputGapCopyTransit(1, 255, comments));
	}

	return automat;
}
