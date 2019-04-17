#ifndef DRINA_TOOLS
  #define DRINA_TOOLS

  #define DEBUG
   
  #ifdef DEBUG
    #define DEBUG_BEGIN(baud) Serial.begin(baud); while(!Serial) {;}
    #define DEBUG_PRINTLN(x)  Serial.println (x); Serial.flush()
    #define DEBUG_PRINT(x)    Serial.print (x)
  #else
    #define DEBUG_BEGIN(baud)
    #define DEBUG_PRINTLN(x)
    #define DEBUG_PRINT(x)
  #endif

  
  class WaitLoop {
    
    unsigned int _waitMilliSec;   //number of milliseconds to wait before move on
    unsigned long _start;         //millis when loop is started
    
    public:
      WaitLoop(const unsigned int waitMilliSec) : _waitMilliSec(waitMilliSec) {};
      void start();
      void wait();
  };

  void blink();
#endif
