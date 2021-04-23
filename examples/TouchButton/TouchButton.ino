/*
  Button example
 */

#include "ArduinoDebounceButton.h"

#define BTN_PIN 2

ArduinoDebounceButton btn(BTN_PIN, BUTTON_CONNECTED::VCC, BUTTON_NORMAL::OPEN);

void handleButtonEvent(const DebounceButton* button, BUTTON_EVENT eventType)
{
	switch (eventType)
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

void setup()
{
	Serial.begin(115200);

	btn.initPin();

	Serial.print(F("Button state while booting: "));
	Serial.println(btn.check());

	btn.setEventHandler(handleButtonEvent);
}

void loop()
{
	btn.check();
}
