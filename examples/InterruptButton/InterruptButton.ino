/*
  Button example
 */

#include "ArduinoDebounceButton.h"
#include "EventsQueue.hpp"
#include <TimerOne.h>

#define BTN_PIN 4

ArduinoDebounceButton btn(BTN_PIN, BUTTON_CONNECTED::GND, BUTTON_NORMAL::OPEN);

EventsQueue<BUTTON_EVENT, 10> queue;

void handleButtonEvent(const DebounceButton* button, BUTTON_EVENT eventType)
{
	queue.push(eventType);
}

void check_button()
{
	btn.check();
}

void setup()
{
	Serial.begin(115200);

	btn.initPin();

	Serial.print(F("Button state while booting: "));
	Serial.println(btn.check());

	btn.setEventHandler(handleButtonEvent);

	Timer1.initialize(btn.delayDebounce / 2 * 1000);
	Timer1.attachInterrupt(check_button);
}

void loop()
{
	bool processBtnEvent;
	BUTTON_EVENT btnEvent;

	do
	{
		noInterrupts();

		processBtnEvent = queue.length();

		if (processBtnEvent)
		{
			btnEvent = queue.pop();
		}

		interrupts();

		if (processBtnEvent)
		{
			switch (btnEvent)
			{
			case BUTTON_EVENT::Pressed:
				Serial.println(F("Pressed"));
				break;
			case BUTTON_EVENT::Released:
				Serial.println(F("Released"));
				break;
			case BUTTON_EVENT::Clicked:
				Serial.println(F("Clicked"));
				break;
			case BUTTON_EVENT::DoubleClicked:
				Serial.println(F("DoubleClicked"));
				break;
			case BUTTON_EVENT::RepeatClicked:
				Serial.println(F("RepeatClicked"));
				break;
			case BUTTON_EVENT::LongPressed:
				Serial.println(F("LongPressed"));
				break;
			default:
				break;
			}
		}

	}
	while (processBtnEvent);

	delay(2000);
}

