#ifndef DRINA_TaPecLib_H
#define DRINA_TaPecLib_H

#include <string.h>             // used only in formatLog()
#include "Arduino.h"
#include "DRINA_TOOLS.h"
#include <SPI.h>                // SPI protocol https://www.arduino.cc/en/reference/SPI
#include <SD.h>                 // SD module https://www.arduino.cc/en/reference/SD
#include "Adafruit_MAX31855.h"  // MAX31855 https://github.com/adafruit/Adafruit-MAX31855-library

#include <Wire.h>               //Clock library is using Wire.h
#include <DS3231.h>             //Clock library

// TaPec module declaration
class TaPec {
  
  private:
    char * _fileName;     // file name string
    int _pinSD_CS; // SD Module chip select pin
    Adafruit_MAX31855 *_thermocouple;
    
     
  public:
    
    //nakon testiranja prebaciti u private sekciju
    uint8_t _year, _month, _day, _hour, _minute, _second;     // date and time when temperature is read
    float _tempInternal = 0.0;  //internal temperature
    float _tempExternal = 0.0;  //sensor K-type thermocouple temperature
    
    char _logStr[32]="XXX"; //= "yy-mm-dd hh:mm:nn -001 -001 -001"; //day-time, clock temperature, sensor internal temperature, sensor external temperature
    void formatLog(); // formating _logStr

    boolean _logToFile; //write data to file on SD card or not
    
    TaPec(const char *fileName, const int pinSD_CS, Adafruit_MAX31855 *thermocouple);
    boolean begin();
    boolean write(const char *dataString);
    boolean writeln(const char *dataString);
    int readSensors();
    int logSensors();             // write internal temp. and thermocouple temperature to SD card
    int dumpLogFile();  
    int deleteLogFile();  
};

#endif //  DRINA_TaPecLib_H
