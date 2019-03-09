#pragma once

template <class T>
class Container {
public:
	Container(int capacity = 4) : capacity(capacity) {
		data = new T[capacity];
	}
	Container(const Container &container) : length(container.length), capacity(container.capacity) {
		data = new T[capacity];
		copy(container.data, data, length);
	}
	Container(Container &&container) {
		{
			T *tmp = data;
			data = container.data;
			container.data = tmp;
		}
		{
			size_t tmp = length;
			length = container.length;
			container.length = tmp;

			tmp = capacity;
			capacity = container.capacity;
			container.capacity = tmp;
		}
	}
	Container(T *data, size_t length) : data(data), length(length), capacity(length) { }

	~Container() {
		for (size_t i = 0; i < length; i++)	if (data[i] != nullptr) delete data[i];
		if (data) delete[] data;
	}

	Container & operator=(const Container &container) {
		length = container.length;
		capacity = container.capacity;
		data = new T[capacity];
		copy(container.data, data, length);
	}
	Container & operator=(Container &&container) {
		{
			T *tmp = data;
			data = container.data;
			container.data = tmp;
		}
		{
			size_t tmp = length;
			length = container.length;
			container.length = tmp;

			tmp = capacity;
			capacity = container.capacity;
			container.capacity = tmp;
		}
	}

	void addElement(T element) {
		if (length >= capacity) extend();
		size_t i;
		for (i = 0; i < length; i++)
			if (*(data[i]) > *element) {
				copy(data + i, data + i + 1, length - i, true);
				/*int len = length - i;
				for (T *currV = (data + length - 1); len > 0; len--, currV--) *(currV + 1) = *currV;*/
				length++;
				break;
			}
		if (i == length) {
			data[i] = element;
			length++;
		}
	}
	T operator[](size_t i) {
		if (0 <= i && i < length) return data[i];
		return nullptr;
	}
	size_t getIndex(T element) {
		for (size_t i = 0; i < length; i++)
			if (data[i] == element) return i;
		return length;
	}
	bool deleteElement(int i) {
		T deletable = (*this)[i];
		if (deletable) {
			delete deletable;
			copy(data + i + 1, data + i, length - i - 1);
			/*int len = length - i - 1;
			for (T *curV = (data + i); len > 0; curV++, len--) *curV = *(curV + 1);*/
			length--;
		}
		return deletable;
	}
	bool deleteElement(T element) {
		if (deleteElement(getIndex(element))) return true;
		delete element;
		return false;
	}

	size_t getLength() {
		return length;
	}
	size_t getCapacity() {
		return capacity;
	}
private:
	T *data = nullptr;
	size_t length = 0, capacity = 0;

	static void copy(T *origin, T *destination, size_t length, bool reverse = false) {
		if (reverse) for (T *newV = destination, *oldV = origin; length > 0; newV--, oldV--, length--) *newV = *oldV;
		else for (T *newV = destination, *oldV = origin; length > 0; newV++, oldV++, length--) *newV = *oldV;
	}
	void extend() {
		T *tmp = new T[capacity <<= 1];
		copy(data, tmp, length);
		delete[] data;
		data = tmp;
	}
};