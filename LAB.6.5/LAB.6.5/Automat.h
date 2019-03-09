#pragma once
#include <fstream>

class SubAutomat;

class Automat {
public:
	Automat(const char *input, const char *output);
	virtual ~Automat();

	void processSymbol();

	bool isDone();

	void reset();
private:
	std::ifstream input;
	std::ofstream output;

	SubAutomat *automat;

	SubAutomat *buildAutomat();
};

