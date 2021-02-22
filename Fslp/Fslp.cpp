#include "Arduino.h"
#include "Fslp.h"

// To measure position, the sense line must be connected to a
// pin capable of reading analog voltages.  For pressure,
// the sense line and drive line 2 must be connected to pins
// capable of reading analog voltages.  See the FSLP guide for
// more information.
Fslp::Fslp(int _fslpSenseLine, int _fslpDriveLine1, int _fslpDriveLine2, int _fslpBotR0){
  fslpSenseLine = _fslpSenseLine;
  fslpDriveLine1 = _fslpDriveLine1;
  fslpDriveLine2 = _fslpDriveLine2;
  fslpBotR0 = _fslpBotR0;
  
}

// This function follows the steps described in the FSLP
// integration guide to measure the position of a force on the
// sensor.  The return value of this function is proportional to
// the physical distance from drive line 2, and it is between
// 0 and 1023.  This function does not give meaningful results
// if fslpGetPressure is returning 0.
int Fslp::fslpGetPosition()
{
  // Step 1 - Clear the charge on the sensor.
  pinMode(fslpSenseLine, OUTPUT);
  digitalWrite(fslpSenseLine, LOW);

  pinMode(fslpDriveLine1, OUTPUT);
  digitalWrite(fslpDriveLine1, LOW);

  pinMode(fslpDriveLine2, OUTPUT);
  digitalWrite(fslpDriveLine2, LOW);

  pinMode(fslpBotR0, OUTPUT);
  digitalWrite(fslpBotR0, LOW);

  // Step 2 - Set up appropriate drive line voltages.
  digitalWrite(fslpDriveLine1, HIGH);
  pinMode(fslpBotR0, INPUT);
  pinMode(fslpSenseLine, INPUT);

  // Step 3 - Wait for the voltage to stabilize.
  delayMicroseconds(10);

  // Step 4 - Take the measurement.
  analogReset();
  return analogRead(fslpSenseLine);
}

// This function follows the steps described in the FSLP
// integration guide to measure the pressure on the sensor.
// The value returned is usually between 0 (no pressure)
// and 500 (very high pressure), but could be as high as
// 32736.
int Fslp::fslpGetPressure()
{
  // Step 1 - Set up the appropriate drive line voltages.
  pinMode(fslpDriveLine1, OUTPUT);
  digitalWrite(fslpDriveLine1, HIGH);

  pinMode(fslpBotR0, OUTPUT);
  digitalWrite(fslpBotR0, LOW);

  pinMode(fslpSenseLine, INPUT);

  pinMode(fslpDriveLine2, INPUT);

  // Step 2 - Wait for the voltage to stabilize.
  delayMicroseconds(10);

  // Step 3 - Take two measurements.
  analogReset();
  int v1 = analogRead(fslpDriveLine2);
  analogReset();
  int v2 = analogRead(fslpSenseLine);

  // Step 4 - Calculate the pressure.
  // Detailed information about this formula can be found in the
  // FSLP Integration Guide.
  if (v1 == v2)
  {
    // Avoid dividing by zero, and return maximum reading.
    return 32 * 1023;
  }
  return 32 * v2 / (v1 - v2);
}

// Performs an ADC reading on the internal GND channel in order
// to clear any voltage that might be leftover on the ADC.
// Only works on AVR boards and silently fails on others.
void Fslp::analogReset()
{
#if defined(ADMUX)
#if defined(ADCSRB) && defined(MUX5)
    // Code for the ATmega2560 and ATmega32U4
    ADCSRB |= (1 << MUX5);
#endif
    ADMUX = 0x1F;

    // Start the conversion and wait for it to finish.
    ADCSRA |= (1 << ADSC);
    loop_until_bit_is_clear(ADCSRA, ADSC);
#endif
}

int Fslp::fixedPressure(int _pressure)
{
  if(_pressure >= 32 * 1023 || _pressure < 0){
    return 0;
  }
  else{
    return _pressure;
  }
}
