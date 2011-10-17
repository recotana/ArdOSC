/*
 
 ArdOSC 2.1 - OSC Library for Arduino.
 
 -------- Lisence -----------------------------------------------------------
 
 ArdOSC
 
 The MIT License
 
 Copyright (c) 2009 - 2011 recotana( http://recotana.com )　All right reserved
 
 */

#include <stdlib.h>
#include <string.h>
#include "OSCcommon/OSCServer.h"
#include "OSCcommon/OSCcommon.h"
#include "OSCCommon/OSCMessage.h"

#include <utility/socket.h>
#include <utility/w5100.h>


OSCServer::OSCServer(void){
    _sock = MAX_SOCK_NUM;
}

OSCServer::~OSCServer(void){
    stop();
     
}

int16_t OSCServer::begin(uint16_t _recievePort){
    
	if ( _sock != MAX_SOCK_NUM ) return -1;
    
    for (int i = 0 ; i < MAX_SOCK_NUM ; i++) {
        uint8_t s = W5100.readSnSR(i);
        if ( s == SnSR::CLOSED || s == SnSR::FIN_WAIT ) {
            _sock = i;
            break;
        }
    }
    
    if (_sock == MAX_SOCK_NUM) return -1;
    
    _port=_recievePort;
    
	if( socket( _sock , SnMR::UDP , _port, 0 ) ) return -1;
    
	rcvFlush();
	
    return 1;
    
}


void OSCServer::stop(void){

    if (_sock == MAX_SOCK_NUM) return;
    close(_sock);
    _sock = MAX_SOCK_NUM;
    
}

int16_t OSCServer::aviableCheck(void){

	if( !( W5100.readSnIR(_sock) && SnIR::RECV ) ) return -1;
    if( W5100.getRXReceivedSize(_sock) == 0 ) return -1;
    
    OSCMessage rcvMes;
    
    if ( recvfrom( _sock , _rcvData , 1 , rcvMes._ip , &rcvMes._port ) > kMaxRecieveData ) return -1;
    
	if( _decoder.decode( &rcvMes ,_rcvData ) < 0 ) return -1;
    
    _adrMatch.paternComp(&rcvMes);
	    
	return 1;
}



void OSCServer::rcvFlush(void){
    
	uint8_t byte;
    
    while ( W5100.getRXReceivedSize(_sock) ) recv( _sock , &byte , 1);
    
}


void OSCServer::addCallback(char *_adr , Pattern::AdrFunc _func ){
    _adrMatch.addOscAddress( _adr , _func );
}

