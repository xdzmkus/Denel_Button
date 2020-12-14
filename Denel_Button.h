#ifndef _DENEL_BUTTON_H
#define _DENEL_BUTTON_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class Denel_Button;

/**
* @param Released - button pulse Pressed->Released
* @param Pressed - button pulse Released->Pressed
* @param Clicked - button Pressed+Released(or hold)
* @param DoubleClicked - button Pressed+Released(or hold) twice
* @param RepeatClicked - button Pressed+Released continiously
* @param LongPressed - button hold
*/
enum class BUTTON_EVENT { Released, Pressed, Clicked, DoubleClicked, RepeatClicked, LongPressed };

/**
* @param VCC - button connected to VCC, mcu pin connected to GND, pinMode - INPUT
* @param GND - button connected to GND, mcu pin connected to VCC, pinMode - INPUT_PULLUP
*/
enum class BUTTON_CONNECTED { VCC = 0, GND = 1 };

/**
* @param OPEN - button normally open
* @param CLOSE - button normally closed
*/
enum class BUTTON_NORMAL { OPEN = 0, CLOSE = 1 };

/**
	* The event handler procedure.
	*
	* @param button - pointer to the Button that generated the event
	* @param eventType - the event type which trigger the call
	*/
typedef void (*ButtonEventHandler)(const Denel_Button* button, BUTTON_EVENT eventType);

class Denel_Button
{
protected:

	// Button pin number connected to 
	uint8_t pin;

	// Button level connected to
	BUTTON_CONNECTED connectedTo;

	// Button normal state
	BUTTON_NORMAL normalState;

	// The event handler for button
	ButtonEventHandler eventHandler = nullptr;

	unsigned long lastDebounceTime = 0;
	unsigned long lastPressedTime = 0;
	unsigned long lastReleasedTime = 0;

	uint8_t clicks = 0;

	// Internal flags
	struct ButtonFlags
	{
		bool currrentState : 1;
		bool previousState : 1;
		bool debounced : 1;
		bool longPressed : 1;
	} buttonFlags;

private:

	// Disable copy-constructor and assignment operator
	Denel_Button(const Denel_Button&) = delete;
	Denel_Button& operator=(const Denel_Button&) = delete;

public:
	Denel_Button(uint8_t pin, BUTTON_CONNECTED c = BUTTON_CONNECTED::VCC, BUTTON_NORMAL n = BUTTON_NORMAL::OPEN)
		: pin(pin), connectedTo(c), normalState(n)
	{
		(connectedTo == BUTTON_CONNECTED::VCC) ? pinMode(pin, INPUT) : pinMode(pin, INPUT_PULLUP);
	};

	/**
		* Get the button's pin number.
		* @return pin number
		*/
	uint8_t getPin() const { return pin; }

	/**
		* Check state of button and trigger event processing.
		* This method should be called from the loop() method about every 50 ms.
		* If function is called less often, the debouncing algorithm may not work correctly,
		* which may cause other event detection algorithms to fail.
		*
		* @return true if button pressed
		*/
	bool check();

	// Install the ButtonEventHandler function pointer
	void setEventHandler(ButtonEventHandler eventHandler) { this->eventHandler = eventHandler; }

	// Default value of debounce delay
	static const uint16_t delayDebounce = 50;

	// Default value of double click delay
	static const uint16_t delayRepeatedClick = 600;

	// Default value of long press delay
	static const uint16_t delayLongPress = 1000;

protected:

	bool isDebounced(unsigned long now);
	void processPressed(unsigned long now);
	void processReleased(unsigned long now);
	void processClicks(unsigned long now);
};

#endif
