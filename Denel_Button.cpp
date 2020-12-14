#include "Denel_Button.h"

bool Denel_Button::check()
{
	buttonFlags.currrentState = digitalRead(pin) ^ static_cast<int>(connectedTo) ^ static_cast<int>(normalState);

    unsigned long now = millis();

    // check debounce the button and dispatch events
    if (eventHandler && isDebounced(now))
    {
        processPressed(now);
        processReleased(now);
        processClicks(now);
    }
        
	return buttonFlags.currrentState;
}

void Denel_Button::processPressed(unsigned long now)
{
    // check if button pressed
    if (buttonFlags.currrentState == false)
        return;

    // Released to Pressed pulse
    if (lastPressedTime < lastReleasedTime)
    {
        clicks++;
        eventHandler(this, BUTTON_EVENT::Pressed);
        lastPressedTime = now;
        return;
    }
        
    // check if button holded
    if (now - lastPressedTime > delayLongPress)
    {
        eventHandler(this, BUTTON_EVENT::LongPressed);
    }

}
    
void Denel_Button::processReleased(unsigned long now)
{
    // check if button released
    if (buttonFlags.currrentState == true)
        return;

    //  Pressed to Released pulse
    if (lastPressedTime >= lastReleasedTime)
    {
        lastReleasedTime = now;
        eventHandler(this, BUTTON_EVENT::Released);

        // button is repeatedly pressed
        if (clicks > 2)
        {
            eventHandler(this, BUTTON_EVENT::RepeatClicked);
            return;
        }
    }
}

void Denel_Button::processClicks(unsigned long now)
{
    if (clicks == 0)
        return;

    // chek if repeated clicks are still possible
    if ((now - lastPressedTime) > delayRepeatedClick)
    {
        // no more repeated clicks possible
        if (clicks == 1)
            eventHandler(this, BUTTON_EVENT::Clicked);
        else if (clicks == 2)
            eventHandler(this, BUTTON_EVENT::DoubleClicked);
            
        clicks = 0; // reset clicks
    }
}

bool Denel_Button::isDebounced(unsigned long now)
{
    if (buttonFlags.currrentState == buttonFlags.previousState)
    {
        // check if timer overloaded
        if (now < lastDebounceTime)
        {
            lastDebounceTime = lastPressedTime = lastReleasedTime = now;
        }
        else if ((now - lastDebounceTime) > delayDebounce)
        {
            // button has the same state at least debounce delay 
            buttonFlags.debounced = true;
        }

        if (buttonFlags.debounced)
            return true;
    }
    else
    {
        // button state is changed - still debouncing
        buttonFlags.debounced = false;
        buttonFlags.previousState = buttonFlags.currrentState;
        lastDebounceTime = now;
    }

    return false;
}
