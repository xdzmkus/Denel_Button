#ifndef _BTN_PROPERTIES_H_
#define _BTN_PROPERTIES_H_

class DebounceButton;

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
typedef void (*BtnEventHandler)(const DebounceButton* button, BUTTON_EVENT eventType);

#endif
