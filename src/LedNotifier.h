#include <Arduino.h>

/// @brief Provides a various features to handle a led status
class LedNotifier
{
private:
    int _led;

    bool _active;
    bool _blinking;

    bool _sinkMode;

public:
    LedNotifier(int led, bool sinkMode = false)
    {

        pinMode(led, OUTPUT);

        _led = led;
        _active = false;
        _blinking = false;
        _sinkMode = sinkMode;
    }

    /// @brief Enables led statically
    void Enable()
    {
        if(_sinkMode)
            digitalWrite(_led, LOW);
            else
            digitalWrite(_led, HIGH);
    }
    /// @brief Disables led statically
    void Disable()
    {
            if(_sinkMode)
            digitalWrite(_led, HIGH);
            else
            digitalWrite(_led, LOW);
    }

    /// @brief blink the led for desired times and with the desired rate in milliseconds
    /// @param times 
    /// @param rate 
    void BlinkFor(int times, int rate){
        bool startingValue = true;

        for(int i = 0; i < times; ++i){
            if(startingValue)
                Enable();
            else
                Disable();
            
            startingValue = !startingValue;
            delay(rate);
        }

    }
};