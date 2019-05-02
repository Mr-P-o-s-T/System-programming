#pragma once
#include <fstream>
#include <string>
#include "DataHolder.h"

class HTMLParser {
public:
	HTMLParser(const char *resIdPath, const char *logExprPath);
	~HTMLParser();

	void prepareOutputFile(std::ofstream &output);
	void finishOutputFile(std::ofstream &output);

	std::string parse(const std::string &str, const char *type);
private:
	DataHolder *resId, *logExpr;
};
