
/* Author: Milan Bozic
    https://www.circuito.io/app?components=512,11021,417986,821989,1671987

    Arduino UNO 3
      Arduino UNO use
        SPI pins: 11 (MOSI=DI), 12 (MISO=DO), 13 (SCK=CLK) su HW SPI pinovi, a SS (CS) biram da bude 9, jer cu 10 da koristim za SD citac
        I2C pins: SDA - A4, CSL - A5   (hardware I2C); every I2C IC has own address

    LCD 16x2 Display (I2C)
    I2C LCD Display Adapter (0x27 address)
      CSL - A5
      SDA - A4

    RTC DS3231 (I2C)                              (redosled pinova je u skladu sa fizickom izvedbom)
      CSL - A5
      SDA - A4

    MAX81355 Thermocouple K-type Amplifier (SPI)  (redosled pinova je u skladu sa fizickom izvedbom)
      DO  (MISO)  12
      CLK (SCK)   13
      CS  (SS)    9

    SD Card module (SPI)                          (redosled pinova je u skladu sa fizickom izvedbom)
      GND
      +3.3V
      +5V
      CS          10
      MOSI        11
      SCK (CLK)   13
      MISO        12
      GND

*/

// Import libraries
#include "DRINA_TOOLS.h"
#include "TaPecLib.h"


// Sampling rate in miliseconds
#define SAMPLING_RATE 5500

const char fileName[] = "TaPecLog.txt";
const int pinSD_CS = 10;
const int pinMAX31855_CS = 9;

Adafruit_MAX31855 thermocouple(pinMAX31855_CS);

TaPec taPec(fileName, pinSD_CS, &thermocouple);

void executeCommand() {
  if (Serial.available() > 0) {
    // read the incoming byte:
    char command = Serial.read();

    //Serial.print(F("Command: ")); Serial.println(command);
    switch (command) {

      case '0': // stop logging
        DEBUG_PRINTLN(F("0. Stop logging"));
        taPec._logToFile = false;
        break;

      case '1': // start logging
        DEBUG_PRINTLN(F("1. Start logging"));
        taPec._logToFile = true;
        break;

      case '2': // log one record to file
        DEBUG_PRINTLN(F("2. Log one record to file"));
        taPec.logSensors();
        break;

      case '3': // dump log file
        DEBUG_PRINTLN(F("3. Dump log file"));
        taPec.dumpLogFile();
        DEBUG_PRINTLN(F("3. Dump log file - END!"));
        break;

      case '4': // erase log file
        DEBUG_PRINTLN(F("2. Erase log file"));
        taPec.deleteLogFile();

        break;
        //    default:
        //DEBUG_PRINTLN(F("Rest - ignore"));
    }

  }
}


void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
  Wire.begin();
  DEBUG_BEGIN(9600);
  DEBUG_PRINTLN(F("TA PEC Setup..."));
  DEBUG_PRINTLN(F("TA PEC Commands:"));
  DEBUG_PRINTLN(F("0. Stop loging to file"));
  DEBUG_PRINTLN(F("1. Start loging to file"));
  DEBUG_PRINTLN(F("2. Lgg to file"));
  DEBUG_PRINTLN(F("3. Dump log file"));
  DEBUG_PRINTLN(F("4. Erase log file"));
  //DEBUG_PRINTLN(F("3*. List SD card")));

  if (!taPec.begin()) while (1);

}


void loop() {

  //  //Serial.println(millis());
  //  //waitLoop.start();
  //  // put your main code here, to run repeatedly:
  //  //readSensors();
  //  //logSensors();
  //  //delay(SAMPLING_RATE);
  //  //waitLoop.wait();

  //taPec.readSensors();
  //taPec.formatLog();
  //DEBUG_PRINTLN(taPec._logStr);

  taPec.logSensors();

  executeCommand();

  delay(5000);

}
