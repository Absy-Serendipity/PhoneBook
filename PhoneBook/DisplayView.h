#pragma once
#include "IObserver.h"

class DisplayView final : public IObserver
{
	void Update(const std::vector<Tuple>&) const override;
};

