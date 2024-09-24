#include "Observable.h"

void Observable::Subscribe(const IObserver* observer)
{
	mObservers.push_back(observer);
}
void Observable::Unsubscribe(const IObserver* observer)
{
	mObservers.erase(std::remove(mObservers.begin(), mObservers.end(), observer), mObservers.end());
}