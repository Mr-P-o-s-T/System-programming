#include "DefaultCaseTransit.h"

#include <typeinfo>

DefaultCaseTransit::DefaultCaseTransit(const char *out, IState *nextState) : DefGapTransit(0, 255, out, nextState) {

}

DefaultCaseTransit::~DefaultCaseTransit() {

}

bool DefaultCaseTransit::operator==(const ITransit & transit) const {
	try {
		const IGapTransit &gapTransit = dynamic_cast<const IGapTransit &>(transit);
		return *this > gapTransit;
	}
	catch (std::bad_cast exception) {
		return true;
	}
}

bool DefaultCaseTransit::operator>(const ITransit & transit) const {
	try {
		const IGapTransit &gapTransit = dynamic_cast<const IGapTransit &>(transit);
		return *this > gapTransit;
	}
	catch (std::bad_cast exception) {
		return true;
	}
}

bool DefaultCaseTransit::operator<(const ITransit & transit) const {
	try {
		const IGapTransit &gapTransit = dynamic_cast<const IGapTransit &>(transit);
		return *this > gapTransit;
	}
	catch (std::bad_cast exception) {
		return false;
	}
}

bool DefaultCaseTransit::operator==(const IGapTransit & transit) const {
	return false;
}

bool DefaultCaseTransit::operator>(const IGapTransit & transit) const {
	return true;
}

bool DefaultCaseTransit::operator<(const IGapTransit & transit) const {
	return false;
}
