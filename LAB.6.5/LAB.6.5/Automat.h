#pragma once
#include <fstream>
#include <string>
#include "HTMLParser.h"

class SubAutomat;

class Automat {
public:
	Automat(const char *input, const char *output);
	virtual ~Automat();

	void ProcessFile();
private:
	std::ifstream input;
	std::ofstream output;
	std::string buff;
	HTMLParser *parser;

	SubAutomat *automat;

	void processSymbol();

	bool isDone();

	void reset();

	SubAutomat *buildAutomat();
};

