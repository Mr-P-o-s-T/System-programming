#pragma once

class ITransit;

class IStateEditable {
public:
	virtual ~IStateEditable() = default;

	virtual void addTransit(ITransit *transit) = 0;
	virtual bool deleteTransit(size_t i) = 0;
	virtual bool deleteTransit(ITransit *transit) = 0;
protected:
	IStateEditable() = default;
	IStateEditable(const IStateEditable&) = delete;
	IStateEditable &operator=(const IStateEditable&) = delete;
};
