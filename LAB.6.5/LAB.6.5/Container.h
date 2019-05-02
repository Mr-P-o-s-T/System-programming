#pragma once
#include <vector>

template<class T>
class Container {
public:
	Container(int capacity = 4) {
		data.reserve(capacity);
	}
	Container(const Container &container) {
		data = container.data;
	}
	Container(Container &&container) {
		{
			std::vector<T> tmp = data;
			data = container.data;
			container.data = tmp;
		}
	}
	Container(T *data, size_t size) {
		for (size_t i = 0; i < size; i++) this->data.push_back(data[i]);
	}

	~Container() {
		for (size_t i = 0; i < data.size(); i++)	
			if (data[i] != nullptr) delete data[i];
	}

	Container & operator=(const Container &container) {
		data = container.data;
	}
	Container & operator=(Container &&container) {
		{
			std::vector<T> tmp = data;
			data = container.data;
			container.data = tmp;
		}
	}

	void addElement(T element) {
		size_t i;
		for (i = 0; i < data.size(); i++)
			if (*(data[i]) > *element) {
				data.insert(data.cbegin() + i, element);
				break;
			}
		if (i == data.size()) data.push_back(element);
	}
	T operator[](size_t i) {
		try {
			return data.at(i);
		}
		catch (...) {
			return nullptr;
		}

	}
	size_t getIndex(T element) {
		for (size_t i = 0; i < data.size(); i++)
			if (data[i] == element) return i;
		return data.size();
	}
	bool deleteElement(int i) {
		T deletable = data[i];
		if (deletable) {
			data.erase(data.cbegin() + i);
			delete deletable;
		}
		return deletable;
	}
	bool deleteElement(T element) {
		if (deleteElement(getIndex(element))) return true;
		delete element;
		return false;
	}

	size_t getLength() {
		return data.size();
	}
	size_t getCapacity() {
		return data.capacity();
	}
private:
	std::vector<T> data;
};