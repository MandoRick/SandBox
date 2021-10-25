#include <Arduino.h>
#include "tf.h"

TrafficLights::TrafficLights(uint8_t red, uint8_t yellow, uint8_t green, States default_state)
{
    _pin_red = red;
    _pin_yellow = yellow;
    _pin_green = green;

    pinMode(_pin_red, OUTPUT);
    pinMode(_pin_yellow, OUTPUT);
    pinMode(_pin_green, OUTPUT);

    _state = default_state;
    _direction = (_state == States::RED) ? Directions::DOWN : Directions::UP;
    _state_index = 0;
}

void TrafficLights::nextState()
{
    States *tmp_states = _directions[static_cast<uint8_t>(_direction)];

    _state = tmp_states[_state_index];
    _next_state = _state;
    _state_index++;
    if (_state_index >= _dir_len)
    {
        _state_index = 0;
        if (_direction == Directions::UP)
        {
            _direction = Directions::DOWN;
        }
        else
        {
            _direction = Directions::UP;
        }
    }
}

void TrafficLights::setState(States s)
{
    _state = s;
}

void TrafficLights::turnOnRed()
{
    digitalWrite(_pin_red, HIGH);
}
void TrafficLights::turnOffRed()
{
    digitalWrite(_pin_red, LOW);
}
void TrafficLights::turnOnYellow()
{
    digitalWrite(_pin_yellow, HIGH);
}
void TrafficLights::turnOffYellow()
{
    digitalWrite(_pin_yellow, LOW);
}
void TrafficLights::turnOnGreen()
{
    digitalWrite(_pin_green, HIGH);
}
void TrafficLights::turnOffGreen()
{
    digitalWrite(_pin_green, LOW);
}

void TrafficLights::turnOffAll()
{
    turnOffRed();
    turnOffYellow();
    turnOffGreen();
}

void TrafficLights::nextStateAfter(States ss, uint32_t d)
{
    _waitDelay = d;
    _next_state = ss;
    _state = States::WAIT;
    _prevTime = _currentTime;
}

void TrafficLights::run()
{
    _currentTime = millis();

    switch (_state)
    {
    case States::WAIT:
        if (_currentTime - _prevTime > _waitDelay)
        {
            _state = _next_state;
            _prevTime = _currentTime;
        }
        break;
    case States::RED:
        blinks = 0;
        turnOffAll();
        turnOnRed();
        break;
    case States::YELLOW:
        blinks = 0;
        if(_direction == Directions::UP) 
            turnOffAll();
        turnOnYellow();
        break;
    case States::GREEN:
        blinks = 0;
        turnOffAll();
        turnOnGreen();
        break;
    case States::BLINK_YELLOW:
        turnOffAll();
        nextStateAfter(States::BLINK_YELLOW, 1000 / 4);
        blinks = !blinks;
        if (blinks)
            turnOnYellow();
        break;
    case States::BLINK_GREEN:
        turnOffAll();
        nextStateAfter(States::BLINK_GREEN, 1000 / 4);
        blinks = !blinks;
        if (blinks)
            turnOnGreen();
        break;
    case States::NOOP:
        break;
    }
}
