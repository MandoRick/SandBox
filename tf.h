#include <Arduino.h>

class TrafficLights
{

public:
    enum class States : uint8_t
    {
        WAIT,
        BLINK_YELLOW,
        BLINK_GREEN,
        RED,
        YELLOW,
        GREEN,
        NOOP
    };

    enum class Directions : uint8_t
    {
        UP,
        DOWN
    };

    States _directions[2][5] = {
        {States::BLINK_GREEN, States::YELLOW, States::RED, States::RED, States::RED},
        {States::RED, States::RED, States::RED, States::YELLOW, States::GREEN},
    };

    uint8_t _dir_len = sizeof(_directions[0])/sizeof(_directions[0][0]);

    TrafficLights(uint8_t red, uint8_t yellow, uint8_t green, States default_state);

    void setState(States s);
    void run();
    void nextState();

private:
    void turnOnRed();
    void turnOffRed();
    void turnOnYellow();
    void turnOffYellow();
    void turnOnGreen();
    void turnOffGreen();
    void turnOffAll();

    void nextStateAfter(States ss, uint32_t d);

    uint8_t _pin_red;
    uint8_t _pin_yellow;
    uint8_t _pin_green;

    States _state;
    States _next_state;

    uint32_t _waitDelay;
    uint32_t _currentTime;
    uint32_t _prevTime;

    Directions _direction;

    uint8_t _state_index;

    bool blinks = 0;
};
