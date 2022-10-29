#include "ESC.h"
ESC::ESC(byte ESC_pin, int outputMin, int outputMax, int armVal) {
  oPin = ESC_pin;
  oMin = outputMin;
  oMax = outputMax;
  oArm = armVal;
}

ESC::~ESC()
{
  // Nothing to destruct
}

void ESC::calib(void) {
  myESC.attach(oPin);
  myESC.writeMicroseconds(oMax);
  delay(calibrationDelay);
  myESC.writeMicroseconds(oMin);
  delay(calibrationDelay);
  arm();
}

void ESC::arm(void) {
  myESC.attach(oPin);  			// attaches the ESC on pin oPin to the ESC object
  myESC.writeMicroseconds (oArm);
}

void ESC::stop(void) {
  myESC.writeMicroseconds (stopPulse);
}

void ESC::speed(int outputESC) {
  oESC = constrain(outputESC, oMin, oMax);
  myESC.writeMicroseconds(oESC);
}

void ESC::setCalibrationDelay(uint32_t calibration_delay) {
  calibrationDelay = calibration_delay;
}

void ESC::setStopPulse(uint32_t stop_pulse) {
  stopPulse = stop_pulse;
}
