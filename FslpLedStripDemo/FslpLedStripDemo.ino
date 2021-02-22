#define USE_SERIAL_PLOTTER// , split print

#include <Fslp.h>
#include <PololuLedStrip.h>
PololuLedStrip<12> ledStrip;
#define LED_COUNT 60
rgb_color colors[LED_COUNT];

// To measure position, the sense line must be connected to a
// pin capable of reading analog voltages.  For pressure,
// the sense line and drive line 2 must be connected to pins
// capable of reading analog voltages.  See the FSLP guide for
// more information.
const int fslpSenseLine = A2;
const int fslpDriveLine1 = 8;
const int fslpDriveLine2 = A3;
const int fslpBotR0 = 9;

Fslp fslp = Fslp(fslpSenseLine, fslpDriveLine1, fslpDriveLine2, fslpBotR0);

void setup()
{
  Serial.begin(115200);
  delay(250);
}

void loop()
{
  int pressure, position;

  pressure = fslp.fslpGetPressure();

  if (pressure == 0)
  {
    // There is no detectable pressure, so measuring
    // the position does not make sense.
    position = 0;
  }
  else
  {
    position = fslp.fslpGetPosition();  // Raw reading, from 0 to 1023.
  }

  char report[80];
#ifndef USE_SERIAL_PLOTTER
  sprintf(report, "pressure: %5d   position: %5d\n",
    pressure, position);
  Serial.print(report);
#else
  Serial.println(String(pressure) + "," + String(position));
#endif
  // Scale the position reading to be from 0 to the
  // number of LEDs.
  int adjustedPosition = (int32_t)position * LED_COUNT / 1000;

  // Scale the pressure reading to be from 0 to 255.
  int adjustedPressure = pressure * 8 / 10;
  if (adjustedPressure > 255)
  {
    adjustedPressure = 255;
  }

  for(int i = 0; i < LED_COUNT; i++)
  {
    if (i < adjustedPosition)
    {
      // Light up the LEDs with indices lower than the adjustedLocation.
      // Set the color of the LEDs based on the adjustedPressure
      colors[i] = (rgb_color){ adjustedPressure, 0, 255 - adjustedPressure };
    }
    else
    {
      // Turn off the other LEDs.
      colors[i] = (rgb_color){ 0, 0, 0 };
    }
  }

  // Update the LED strip with values from the above for loop.
  ledStrip.write(colors, LED_COUNT);

  delay(20);
}
