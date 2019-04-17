#include "Arduino.h"
#include "DRINA_TOOLS.h"

void WaitLoop::start() {
  _start = millis();
}

void WaitLoop::wait() {
  long delta = _waitMilliSec - (millis() - _start);
  blink();
  DEBUG_PRINT(_start);
  DEBUG_PRINT(", ");
  DEBUG_PRINTLN(delta);
  if (delta>0) delay(delta);
}


void blink() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(250);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  //delay(500);                       // wait for a second
}
