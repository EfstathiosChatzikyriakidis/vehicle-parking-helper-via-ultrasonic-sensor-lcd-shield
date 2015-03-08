/*
 *  Vehicle Parking Helper Via Ultrasonic Sensor & LCD Keypad Shield.
 *
 *  Copyright (C) 2010 Efstathios Chatzikyriakidis (contact@efxa.org)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/*
 *  In order to compile the sketch you will need to hack the LCD4Bit_mod library.
 *
 *  You should both add and implement the following
 *  method in order to clear specific lines of LCD:
 *
 *    void LCD4Bit_mod::clearLine (int line)
 *    {
 *      // Not implemented method (add your code here).
 *    }
 */

// include necessary headers.
#include <LCD4Bit_mod.h>

const int LCD_MAX_LINES = 2; // the number of LCD's lines.

// the object to manage the LCD.
LCD4Bit_mod lcd = LCD4Bit_mod(LCD_MAX_LINES);

const int echoPin = 2; // the PING sensor echo pin number (request).
const int trigPin = 3; // the PING sensor trigger pin number (reply).

const int RESULT_MAX_SIZE = 32; // the maximum result size in chars.
const int SAMPLES_LENGTH = 10;  // the number of PING sensor samples to fetch.
const long SAMPLE_DELAY = 80;   // the delay in ms for next PING sensor sample.

// startup point entry (runs once).
void
setup() {
  pinMode(echoPin, OUTPUT); // set PING sensor echo as output.
  pinMode(trigPin, INPUT);  // set PING sensor trigger as input.

  // initialize the LCD.
  lcd.init();

  // clear the screen.
  lcd.clear();

  // print a message.
  lcd.printIn("Object Distance:");
}

// loop the main sketch.
void
loop() {
  // the value of the sensor as string.
  char value[RESULT_MAX_SIZE];

  // get an average duration of back-and-forth ultrasonic PING signal.
  long dr = getPINGTime(trigPin, echoPin, SAMPLES_LENGTH, SAMPLE_DELAY);

  // calculate also the distance to inches and centimeters.
  long in = microsecondsToInches(dr);
  long cm = microsecondsToCentimeters(dr);

  // convert the value to string.
  sprintf(value, "%ld in, %ld cm", in, cm);

  // clear the second line.
  lcd.clearLine(2);

  // print to the LCD the appropriate data.
  lcd.printIn(value);
}

// convert duration time to distance in inches.
long
microsecondsToInches(const long ms) {
  return ms / 74 / 2;
}

// convert duration time to distance in centimeters.
long
microsecondsToCentimeters(const long ms) {
  return ms / 29 / 2;
}

// get an average duration time from the ultrasonic PING sensor.
long
getPINGTime(const int tPin, const int ePin, const int N, const long time) {
  static int curSample; // current sensor sample.
  static long curValue; // current sensor value.

  // current value variable first works as a sum counter.
  curValue = 0;

  // get sensor samples first with delay to calculate the sum of them.
  for (int i = 0; i < N; i++) {
    digitalWrite(ePin, HIGH);
    delayMicroseconds(10);
    digitalWrite(ePin, LOW);
    delayMicroseconds(2);

    // get sensor sample.
    curSample = pulseIn(tPin, HIGH);

    // add sample to the sum counter.
    curValue += curSample;

    // delay some time for the next sample.
    delay(time);
  }  

  // get an average sensor value.
  return (curValue / N);
}
