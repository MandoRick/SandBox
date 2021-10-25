#include <Arduino.h>

#include <tf.h>

TrafficLights tf1(4, 3, 2, TrafficLights::States::RED);
TrafficLights tf2(7, 6, 5, TrafficLights::States::GREEN);

void setup()
{
  Serial.begin(9600);
}

uint32_t currentTime;
uint32_t prevTime;
uint32_t waitDelay = 1000;

void loop()
{

  currentTime = millis();

  tf1.run();
  tf2.run();

  if (currentTime - prevTime > waitDelay)
  {
    prevTime = currentTime;
    tf1.nextState();
    tf2.nextState();
  }
}
