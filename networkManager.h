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
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <HTTPClient.h>

void networkConnect();

String getNTPdate();

void getNTPtimechr(char * buf);

String HTTPrequestTemperature();
