#include "DebounceButton.h"

DebounceButton::DebounceButton(uint8_t pin, BUTTON_CONNECTED c, BUTTON_NORMAL n)
	: pin(pin), connectedTo(c), normalState(n)
{
}

DebounceButton::~DebounceButton()
{
}

uint8_t DebounceButton::getPin() const
{
	return pin;
}

void DebounceButton::setEventHandler(BtnEventHandler eventHandler)
{
	this->eventHandler = eventHandler;
}

void DebounceButton::clearEventHandler()
{
	this->eventHandler = nullptr;
}

bool DebounceButton::check()
{
	buttonFlags.currrentState = getPinState() ^ static_cast<bool>(connectedTo) ^ static_cast<bool>(normalState);

	unsigned long now = getTicks();

	// dispatch events if button is debounced
	if (eventHandler && debounce(now))
	{
		processPressed(now);
		processReleased(now);
	}

	return buttonFlags.currrentState;
}

bool DebounceButton::debounce(unsigned long now)
{
	if (buttonFlags.currrentState == buttonFlags.previousState)
	{
		// check if button has the same state at least debounce delay 
		if (!buttonFlags.debounced && (now - lastDebounceTime) > delayDebounce)
		{
			buttonFlags.debounced = true;
		}
	}
	else
	{
		// button state is changed - still bouncing
		buttonFlags.previousState = buttonFlags.currrentState;
		lastDebounceTime = now;
		buttonFlags.debounced = false;
	}

	return buttonFlags.debounced;
}

void DebounceButton::processPressed(unsigned long now)
{
	// check if button pressed
	if (buttonFlags.currrentState == false)
		return;

	// Released to Pressed pulse
	if (lastPressedTime <= lastReleasedTime)
	{
		lastPressedTime = now;
		lastReleasedTime = 0;

		eventHandler(this, BUTTON_EVENT::Pressed);

		buttonFlags.clicks++;

		return;
	}

	// check if button is long pressed
	if (buttonFlags.clicks > 0 && (now - lastPressedTime) > delayLongPress)
	{
		// decrement count of clicks
		if (buttonFlags.clicks == 2)
			eventHandler(this, BUTTON_EVENT::Clicked);
		else if (buttonFlags.clicks == 3)
			eventHandler(this, BUTTON_EVENT::DoubleClicked);

		eventHandler(this, BUTTON_EVENT::LongPressed);

		buttonFlags.clicks = 0; // reset clicks after long press
	}
}

void DebounceButton::processReleased(unsigned long now)
{
	// check if button released
	if (buttonFlags.currrentState == true)
		return;

	//  Pressed to Released pulse
	if (lastPressedTime > lastReleasedTime)
	{
		lastReleasedTime = now;
		eventHandler(this, BUTTON_EVENT::Released);

		// button is repeatedly pressed
		if (buttonFlags.clicks > 2)
		{
			eventHandler(this, BUTTON_EVENT::RepeatClicked);
		}

		return;
	}

	// process clicks
	if (buttonFlags.clicks == 0) return;

	// chek if repeated clicks are still possible
	if ((now - lastPressedTime) > delayRepeatedClick)
	{
		// no more repeated clicks possible
		if (buttonFlags.clicks == 1)
			eventHandler(this, BUTTON_EVENT::Clicked);
		else if (buttonFlags.clicks == 2)
			eventHandler(this, BUTTON_EVENT::DoubleClicked);

		buttonFlags.clicks = 0; // reset clicks
	}
}
