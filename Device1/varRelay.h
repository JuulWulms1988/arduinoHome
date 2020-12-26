#ifndef varRelay_h
#define varRelay_h
#include "Arduino.h"

class varRelayCls {
public:
	varRelayCls(byte pinVar, unsigned int onVar, unsigned int offVar);
	void func(boolean nowVar);
	void timFunc(int tDiff);
private:
	boolean wacht = false;
	boolean staat = false;
	int timTel;
	volatile boolean timTelt = false;
	byte timTel2 = 0;
	unsigned int maxOn;
	unsigned int maxOff;
	byte relayPin;
	unsigned int maxF() { 
		if (maxOn > maxOff) return maxOn; return maxOff; 
	}
};

#endif
