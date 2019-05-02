#pragma once
#include <fstream>
#include "Container.h"
#include "Data.h"

class DataHolder {
public:
	DataHolder(const char *path);
	~DataHolder() = default;
	bool compareStr(const std::string &str);
private:
	std::ifstream input;
	Container<Data *> dataHolder;
};
