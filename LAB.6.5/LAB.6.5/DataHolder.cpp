#include "DataHolder.h"
using namespace std;

DataHolder::DataHolder(const char *path) {
	this->input = ifstream(path);
	while (!input.eof()) {
		string data;
		input >> data;
		dataHolder.addElement(new Data(data));
	}
}

bool DataHolder::compareStr(const string &str) {
	for (size_t i = 0; i < dataHolder.getLength(); i++)
		if (dataHolder[i]->areEq(str)) return true;
	return false;
}
