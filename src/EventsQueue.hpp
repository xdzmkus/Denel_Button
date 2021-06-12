#ifndef _EVENTS_QUEUE_HPP_
#define _EVENTS_QUEUE_HPP_

template <typename EVENT, uint8_t MAX>
class EventsQueue
{
public:
	EventsQueue();
	~EventsQueue();

	void push(EVENT event);
	EVENT pop();

	uint8_t length();

private:

	EVENT events[MAX];

	uint8_t nextIdx = 0;
};

template <typename EVENT, uint8_t MAX>
EventsQueue<EVENT, MAX>::EventsQueue()
{
}

template <typename EVENT, uint8_t MAX>
EventsQueue<EVENT, MAX>::~EventsQueue()
{
}

template <typename EVENT, uint8_t MAX>
inline void EventsQueue<EVENT, MAX>::push(EVENT event)
{
	if (nextIdx >= MAX)
		return;

	events[nextIdx] = event;
	++nextIdx;
}

template <typename EVENT, uint8_t MAX>
inline EVENT EventsQueue<EVENT, MAX>::pop()
{
	if (nextIdx == 0)
		return static_cast<EVENT>(-1);

	EVENT firstEvnt = events[0];
	
	--nextIdx;

	for (uint8_t i = 0; i < nextIdx; i++)
	{
		events[i] = events[i + 1];
	}

	return firstEvnt;
}

template <typename EVENT, uint8_t MAX>
inline uint8_t EventsQueue<EVENT, MAX>::length()
{
	return nextIdx;
}

#endif
