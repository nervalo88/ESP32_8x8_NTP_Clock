#pragma once

#define DEBUG //enable Serial printing

#define  DELAYTIME  100  // in milliseconds

///--------------Wifi adapter configuration
#define WIFI_SSID "EnterYourSSIDhere"
#define WIFI_PASS "EnterYourWifiPwdHere"

// LCD PANNEL 
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES	6

#define CLK_PIN   18  // or SCK
#define DATA_PIN  23  // or MOSI
#define CS_PIN    15  // or SS

#define X_DEVICES 6
#define Y_DEVICES 1

/// OTHER PIN(S)

#define PUSH_BUTTON_1 22



#ifdef DEBUG
	#define PRINT(s, x) { Serial.print(F(s)); Serial.print(x); }
	#define PRINTS(x)   Serial.print(F(x))
	#define PRINTD(x)   Serial.print(x, DEC)

#else
	#define PRINT(s, x)
	#define PRINTS(x)
	#define PRINTD(x)

#endif