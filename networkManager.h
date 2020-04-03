// networkManager.h

#ifndef _NETWORKMANAGER_h
#define _NETWORKMANAGER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#endif

#include <ArduinoJson.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <HTTPClient.h>
#include <time.h>

void networkConnect();

void getNTPtimechr(char * buf);

void getNTPdatechr(char * buf);

float HTTPrequestTemperature();
