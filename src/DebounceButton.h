#ifndef _DEBOUNCEBUTTON_H_
#define _DEBOUNCEBUTTON_H_

#include "BtnProperties.h"

#include <stdint.h>

class DebounceButton
{
public:

    // Default value of debounce delay
    static const uint16_t delayDebounce = 50;

    // Default value of double click delay
    static const uint16_t delayRepeatedClick = 600;

    // Default value of long press delay
    static const uint16_t delayLongPress = 1000;

    DebounceButton(uint8_t pin, BUTTON_CONNECTED c = BUTTON_CONNECTED::VCC, BUTTON_NORMAL n = BUTTON_NORMAL::OPEN);

    virtual ~DebounceButton();

    /**
    * Get the button's pin number.
    * 
    * @return pin number
    */
    uint8_t getPin() const;

    /**
    * Check current state of button and apply debouncing algorithm.
    * This method should be called from the code about every 50 ms.
    * If function is called less often, the debouncing algorithm may not work correctly,
    * which may cause other event detection algorithms to fail.
    *
    * @return true if button pressed (not debounced)
    */
    bool check();

    /**
    * Install the BtnEventHandler function pointer
    * 
    * @param eventHandler - the event handler function 
    */
    void setEventHandler(BtnEventHandler eventHandler);

    /**
    * Remove the BtnEventHandler function pointer
    * 
    */
    void clearEventHandler();

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
    BtnEventHandler eventHandler = nullptr;

    unsigned long lastDebounceTime = 0;
    unsigned long lastPressedTime = 0;
    unsigned long lastReleasedTime = 0;

    // Internal flags
    struct ButtonFlags
    {
        bool currrentState  : 1;
        bool previousState  : 1;
        bool debounced      : 1;
        uint8_t clicks      : 5;
    }
    buttonFlags = { 0 };

    bool debounce(unsigned long now);

    void processPressed(unsigned long now);
    void processReleased(unsigned long now);

private:

    // Disable copy-constructor and assignment operator
    DebounceButton(const DebounceButton&) = delete;
    DebounceButton& operator=(const DebounceButton&) = delete;
};

#endif
