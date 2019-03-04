#pragma once

class IState {
public:
	virtual ~IState() = default;

	virtual IState* process(char x) = 0;
protected:
	IState() = default;
	IState(const IState&) = delete;
	IState& operator=(const IState&) = delete;
};
