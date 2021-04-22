#ifndef _ARDUINODEBOUNCEBUTTON_H
#define _ARDUINODEBOUNCEBUTTON_H

#include "DebounceButton.hpp"

class ArduinoDebounceButton : public DebounceButton
{
public:

	ArduinoDebounceButton(uint8_t pin, BUTTON_CONNECTED c = BUTTON_CONNECTED::VCC, BUTTON_NORMAL n = BUTTON_NORMAL::OPEN)
		: DebounceButton(pin, c, n)
	{
	}

	void initPin()
	{
		connectedTo == BUTTON_CONNECTED::VCC ? pinMode(pin, INPUT) : pinMode(pin, INPUT_PULLUP);
	}

protected:

	virtual bool getPinState()
	{
		return static_cast<bool>(digitalRead(pin));
	}

	virtual unsigned long getTicks()
	{
		return millis();
	}
};

#endif
