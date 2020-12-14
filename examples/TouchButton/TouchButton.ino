/*
  Button example
 */

#define BTN_PIN 10

#include "Denel_Button.h"

Denel_Button btn(BTN_PIN, BUTTON_CONNECTED::VCC, BUTTON_NORMAL::OPEN);

void handleButtonEvent(const Denel_Button* button, BUTTON_EVENT eventType)
{
	switch (eventType)
	{
	case BUTTON_EVENT::Pressed:
		Serial.println("Pressed");
		break;
	case BUTTON_EVENT::Released:
		Serial.println("Released");
		break;
	case BUTTON_EVENT::Clicked:
		Serial.println("Clicked");
		break;
	case BUTTON_EVENT::DoubleClicked:
		Serial.println("DoubleClicked");
		break;
	case BUTTON_EVENT::RepeatClicked:
		Serial.println("RepeatClicked");
		break;
	case BUTTON_EVENT::LongPressed:
		Serial.println("LongPressed");
		break;
	default:
		break;
	}
}

void setup()
{
	Serial.begin(115200);

	Serial.print("Button state while booting: ");
	Serial.println(btn.check());

	btn.setEventHandler(handleButtonEvent);
}

void loop()
{
    btn.check();
}
