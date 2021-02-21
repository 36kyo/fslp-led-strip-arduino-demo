/* FslpLedStripDemo: Example Arduino sketch that shows
 * how to use an Interlink force-sensing linear potentiometer
 * (FSLP) to control an Addressable RGB LED strip from Pololu.
 *
 * This code requires the PololuLedStrip library, which is
 * available from:
 *
 *   https://github.com/pololu/pololu-led-strip-arduino
 *
 * To use this code, you will need to plug an Addressable RGB LED
 * strip from Pololu into pin 12.
 *
 * You will need an additional resistor and four connections from
 * the FSLP to the Arduino.  The FSLP's integration guide from
 * Interlink can be used to make the correct connections as
 * follows:
 *
 * - Sense line (SL) to pin A2
 * - Drive line 1 (D1) to pin 8
 * - Drive line 1 (D2) to pin A3
 * - the connection made at the bottom of resistor R0 to pin 9
 *
 * After uploading the sketch, the Arduino will control the LED
 * strip using position and pressure readings from the FSLP.  The
 * position reading will determine the number of LEDs that light
 * up, while the pressure reading will determine the color of the
 * LEDs.
 *
 * This sketch also reports the force and pressure readings over
 * serial so you can read them on your computer using a terminal
 * program such as the Serial Monitor in the Arduino IDE.
 *
 * Without connecting an LED strip, this sketch can still be used
 * to take readings from the FSLP and see them on your computer.
 */

#ifndef FLSP_H
#define FLSP_H

class Fslp{
  public:
    Fslp(int _fslpSenseLine, int _fslpDriveLine1, int _fslpDriveLine2, int _fslpBotR0);
    int fslpGetPosition();
    int fslpGetPressure();
    void analogReset();
    
  private:
    int fslpSenseLine;
    int fslpDriveLine1;
    int fslpDriveLine2;
    int fslpBotR0;
  
};
#endif
 
