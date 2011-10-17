/*
 
 ArdOSC 2.1 - OSC Library for Arduino.
 
 -------- Lisence -----------------------------------------------------------
 
 ArdOSC
 
 The MIT License
 
 Copyright (c) 2009 - 2011 recotana( http://recotana.com )　All right reserved
 
 */


#ifndef OSCClient_h
#define OSCClient_h

#include "OSCcommon.h"
#include "OSCMessage.h"
#include "OSCEncoder.h"

#define kDummyPortNumber 10000



class OSCClient{
	
private:
        
	uint8_t _sock;
	

    uint8_t *_sendData;
    
    OSCEncoder::OSCEncoder encoder;

    int16_t sockOpen(void);
	void sockClose(void);
	
	void flushSendData(void);
    
    
public:
    
	OSCClient(void);
	~OSCClient(void);
    

	int16_t send( OSCMessage *_message);
    
			
};


#endif