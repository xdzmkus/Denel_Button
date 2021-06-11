#ifndef _BTN_EVENT_QUEUE_HPP_
#define _BTN_EVENT_QUEUE_HPP_

#include "BtnProperties.h"

template <uint8_t MAX>
class BtnEventQueue
{
public:
	BtnEventQueue();
	~BtnEventQueue();

	void push(BUTTON_EVENT event);
	BUTTON_EVENT pop();

	uint8_t length();

private:

	BUTTON_EVENT events[MAX];

	uint8_t nextIdx = 0;
};

template <uint8_t MAX>
BtnEventQueue<MAX>::BtnEventQueue()
{
}

template <uint8_t MAX>
BtnEventQueue<MAX>::~BtnEventQueue()
{
}

template <uint8_t MAX>
inline void BtnEventQueue<MAX>::push(BUTTON_EVENT event)
{
	if (nextIdx >= MAX)
		return;

	events[nextIdx] = event;
	++nextIdx;
}

template <uint8_t MAX>
inline BUTTON_EVENT BtnEventQueue<MAX>::pop()
{
	if (nextIdx == 0)
		return static_cast<BUTTON_EVENT>(-1);

	BUTTON_EVENT firstEvnt = events[0];
	
	--nextIdx;

	for (uint8_t i = 0; i < nextIdx; i++)
	{
		events[i] = events[i + 1];
	}

	return firstEvnt;
}

template<uint8_t MAX>
inline uint8_t BtnEventQueue<MAX>::length()
{
	return nextIdx;
}

#endif
