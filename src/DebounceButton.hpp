#ifndef _DEBOUNCEBUTTON_HPP
#define _DEBOUNCEBUTTON_HPP

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
typedef void (*ButtonEventHandler)(const DebounceButton* button, BUTTON_EVENT eventType);

class DebounceButton
{
public:

    // Default value of debounce delay
    static const uint16_t delayDebounce = 50;

    // Default value of double click delay
    static const uint16_t delayRepeatedClick = 600;

    // Default value of long press delay
    static const uint16_t delayLongPress = 1000;

    DebounceButton(uint8_t pin, BUTTON_CONNECTED c = BUTTON_CONNECTED::VCC, BUTTON_NORMAL n = BUTTON_NORMAL::OPEN)
        : pin(pin), connectedTo(c), normalState(n)
    {
    }

    virtual ~DebounceButton()
    {
    }

    /**
        * Get the button's pin number.
        * @return pin number
        */
    uint8_t getPin() const
    {
        return pin;
    }

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
    void setEventHandler(ButtonEventHandler eventHandler)
    {
        this->eventHandler = eventHandler;
    }

protected:

    virtual bool getPinState() = 0;

    virtual unsigned long getTicks() = 0;

    // Button pin number connected to 
    const uint8_t pin;

    // Button level connected to
    const BUTTON_CONNECTED connectedTo;

    // Button normal state
    const BUTTON_NORMAL normalState;

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

    bool isDebounced(unsigned long now);
    void processPressed(unsigned long now);
    void processReleased(unsigned long now);

private:

    // Disable copy-constructor and assignment operator
    DebounceButton(const DebounceButton&) = delete;
    DebounceButton& operator=(const DebounceButton&) = delete;

};

bool DebounceButton::check()
{
	buttonFlags.currrentState = getPinState() ^ static_cast<bool>(connectedTo) ^ static_cast<bool>(normalState);

    unsigned long now = getTicks();

    // check debounce the button and dispatch events
    if (eventHandler && isDebounced(now))
    {
        processPressed(now);
        processReleased(now);
    }
        
	return buttonFlags.currrentState;
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

        eventHandler(this, BUTTON_EVENT::Pressed);

        return;
    }
        
    // check if button holded
    if (now - lastPressedTime > delayLongPress)
    {
        eventHandler(this, BUTTON_EVENT::LongPressed);
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

        clicks++;

        // button is repeatedly pressed
        if (clicks > 2)
        {
            eventHandler(this, BUTTON_EVENT::RepeatClicked);
        }

        return;
    }

    // process clicks
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

bool DebounceButton::isDebounced(unsigned long now)
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

#endif