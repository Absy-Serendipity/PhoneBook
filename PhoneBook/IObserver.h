#pragma once
#include "Record.h"

class IObserver
{
public:
	virtual void Update(const std::vector<Tuple>&) const = 0;
};


