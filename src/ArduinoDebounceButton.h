#ifndef _ARDUINODEBOUNCEBUTTON_H
#define _ARDUINODEBOUNCEBUTTON_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "DebounceButton.h"

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

	virtual bool getPinState() override
	{
		return static_cast<bool>(digitalRead(pin));
	}

	virtual unsigned long getTicks() override
	{
		return millis();
	}
};

#endif
