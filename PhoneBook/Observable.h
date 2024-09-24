#pragma once
#include "IObserver.h"

class Observable
{
public:
	void Subscribe(const IObserver*);
	void Unsubscribe(const IObserver*);
	virtual void NotifyAll() const = 0;

	// Set observers protected for concrete Observables to access them
protected:
	std::vector<const IObserver*> mObservers;
};