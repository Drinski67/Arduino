#include "TaPecLib.h"
#ifndef DRINA_TaPecLib_CPP
#define DRINA_TaPecLib_CPP


TaPec::TaPec(const char *fileName, const int pinSD_CS, Adafruit_MAX31855 *thermocouple) {
  this->_fileName = fileName;
  this->_pinSD_CS = pinSD_CS;
  this->_thermocouple = thermocouple;
  this->_logToFile = false;  //podrazumeva se da se odmah podaci beleze u fajl

}

boolean TaPec::begin() {
  if (!SD.begin(_pinSD_CS)) {
    DEBUG_PRINTLN(F("TaPec::begin(): SD Module setup failed!"));
    _logToFile = false;   //ako nije inicijalizovana SD kartica, nemoj ni da logujes u fajl
    return (false);
  }
  _logToFile = true;     //podrazumeva se da se odmah podaci beleze u fajl
  return (true);
}

void TaPec::formatLog() {
  char fStr[] = "+000.0";

  _logStr[0] = (char)0;

  sprintf(_logStr, "%d-%d-%d %d:%d:%d, ", _year, _month, _day, _hour, _minute, _second);

  dtostrf(_tempExternal, 4, 1, fStr);
  strcat(_logStr, fStr);
  strcat(_logStr, ", ");
  dtostrf(_tempInternal, 4, 1, fStr);
  strcat(_logStr, fStr);

}

boolean TaPec::write(const char *dataString) {
  DEBUG_PRINT(F("TaPec::write(const char *dataString): "));
  File dataFile = SD.open(_fileName, FILE_WRITE);
  if (dataFile) {
    dataFile.print(dataString);
    dataFile.close();
    DEBUG_PRINTLN(dataString);
    return (true);
  }
  else { // if the file isn't open, pop up an error:
    DEBUG_PRINTLN(F("error opening datalog.txt"));
    return (false);

  }
}

boolean TaPec::writeln(const char *dataString) {
  DEBUG_PRINT(F("TaPec::writeln(const char *dataString): "));
  File dataFile = SD.open(_fileName, FILE_WRITE);
  if (dataFile) {
    dataFile.print(dataString);
    dataFile.println();
    dataFile.close();
    DEBUG_PRINTLN(dataString);
    return (true);
  }
  else { // if the file isn't open, pop up an error:
    DEBUG_PRINTLN(F("error opening datalog.txt"));
    return (false);

  }
}

int TaPec::readSensors() {
  int retVal = 0;
  DS3231 Clock;
  bool Century = false;
  bool h12;
  bool PM;

  _year = Clock.getYear();
  _month = Clock.getMonth(Century);
  _day = Clock.getDate();
  _hour = Clock.getHour(h12, PM);
  _minute = Clock.getMinute();
  _second = Clock.getSecond();

  _tempInternal = _thermocouple->readInternal();    //interna temperatura
  _tempExternal = _thermocouple->readCelsius();     //temperatura senzora

  if (isnan(_tempExternal)) {
    retVal = _thermocouple->readError();
    DEBUG_PRINT(F("TaPec::readSensors(): thermocouple.readError() = "));
    DEBUG_PRINTLN(retVal);
  }

  return (retVal);
}

// write internal temp. and thermocouple temperature to SD card
int TaPec::logSensors() {
  int retVal = 0;

  retVal = readSensors();
  formatLog();

  if (retVal == 0 && _logToFile) { //ako nije bilo greske u ocitavanju temperature i trazeno je logovanje u fajl
    DEBUG_PRINT(F("logSensors(): "));
    File dataFile = SD.open(_fileName, FILE_WRITE);
    if (dataFile) {
      dataFile.println(_logStr);
      dataFile.close();
      DEBUG_PRINTLN(_logStr);
    }
    else { // if the file isn't open, pop up an error:
      DEBUG_PRINT(F("error opening "));
      DEBUG_PRINTLN(_fileName);
      retVal = -1;
    }
  }
  return (retVal);

}

int  TaPec::dumpLogFile() {
  File dataFile = SD.open(_fileName);
  if (dataFile) {
    while (dataFile.available()) {
      Serial.write(dataFile.read());
    }
    dataFile.close();
    return (0);
  } else {
    DEBUG_PRINT(F("error opening dump log file"));
    DEBUG_PRINTLN(_fileName);
    return (-1);
  }

}

int  TaPec::deleteLogFile() {
  File dataFile = SD.open(_fileName);
  if (SD.exists(_fileName)) {
    SD.remove(_fileName);
    return (0);
  } else {
    DEBUG_PRINT(F("error deleting log file "));
    DEBUG_PRINTLN(_fileName);
    return (-1);
  }

}
#endif
