#pragma once

#include "constants.h"
#include "helpers.h"

enum class NotifierType
{
	eMap
};

class Observer;

class Notifier
{
public:
	Notifier(NotifierType type, std::vector<RefWrapper<Observer>> const& observers);
	~Notifier();

	void addObserver(Observer& observer);
	void removeObserver(Observer& observer);

	void notify(size_t notification);

private:
	NotifierType type;
	std::vector<RefWrapper<Observer>> observers;
};

class Observer
{
public:
	Observer();
	~Observer();

	std::vector<std::pair<NotifierType, size_t>> getNotifications();

	friend class Notifier;

private:
	void addNotifier(Notifier& notifier);
	void removeNotifier(Notifier& notifier);

	std::vector<std::pair<NotifierType, size_t>> notifications;
	std::vector<RefWrapper<Notifier>> notifiers;
};