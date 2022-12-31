#include "Observer.h"

Notifier::Notifier(std::vector<RefWrapper<Observer>> const& observers)
	:observers(observers)
{
	for (auto&& observer : observers)
	{
		observer.get().addNotifier(*this);
	}
}

Notifier::~Notifier()
{
	for (auto&& observer : observers)
	{
		observer.get().removeNotifier(*this);
	}
}

void Notifier::addObserver(Observer& observer)
{
	observers.push_back(refWrap(observer));
	observer.addNotifier(*this);
}

void Notifier::removeObserver(Observer& observer)
{
	std::erase(observers, refWrap(observer));
}

void Notifier::notify(size_t notification)
{
	for (auto&& observer : observers)
	{
		observer.get().notifications.push_back({ type, notification });
	}
}

Observer::Observer()
{}

Observer::~Observer()
{
	for (auto&& notifier : notifiers)
	{
		notifier.get().removeObserver(*this);
	}
}

auto Observer::getNotifications()
{
	auto result = notifications;
	notifications.clear();
	return result;
}

void Observer::addNotifier(Notifier& notifier)
{
	notifiers.push_back(refWrap(notifier));
}

void Observer::removeNotifier(Notifier& notifier)
{
	std::erase(notifiers, refWrap(notifier));
}
