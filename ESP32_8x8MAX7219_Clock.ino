/*
 Name:		ESP32_8x8MAX7219_Clock.ino
 Created:	26/01/2020 21:48:30
 Author:	renau
*/

//#include <SPI.h>



#include <ArduinoJson.hpp>
#include <ArduinoJson.h>
#include <MD_MAX72xx.h>
#include "networkManager.h"

#include <MD_MAXPanel.h>
#include <Button2.h>
#include "Fonts.h"

#include "Configuration.h"



MD_MAXPanel mp = MD_MAXPanel(HARDWARE_TYPE, CS_PIN, X_DEVICES, Y_DEVICES);
Button2 buttonSwitch = Button2(PUSH_BUTTON_1);

uint32_t prevMillis;
bool swapScreen;
int swapcounter;
unsigned long timer;

char timestampChr[50];
char datestampChr[80];

void onSwitchPressed(Button2& btn) {
	swapScreen = true;
}

enum State {
    UNDEFINED,
    DATE_DISPLAY,
    TIME_DISPLAY,
    TEMP_DISPLAY
};

State loopState = TIME_DISPLAY;

// the setup function runs once when you press reset or power the board
void setup() {

    Serial.begin(115200);
    Serial.println("Start");

	buttonSwitch.setClickHandler(onSwitchPressed);
	
	networkConnect();

    mp.begin();
    mp.setIntensity(1);

	timer = millis();

}

// the loop function runs over and over again until power down or reset
void loop() {
	//heap_caps_check_integrity_all(true);
	//heap_caps_print_heap_info(MALLOC_CAP_DEFAULT);

	buttonSwitch.loop();

    if (millis() - timer > 1000 ||swapScreen) {
		timer = millis();

        switch (loopState) {

		default:
			break;

        case UNDEFINED:
            loopState = TIME_DISPLAY;
            break;

        case DATE_DISPLAY:
            
			getNTPdatechr(datestampChr);
			
			mp.setFont(_Fixed_5x3);
            mp.clear();
            mp.drawText(0, mp.getYMax(), datestampChr);

			char buffer[10];
			sprintf(buffer, "%0.1f",HTTPrequestTemperature());
            mp.drawText(31, 4, buffer);
			mp.drawText(44,4, "$");
			swapcounter++;
            if (swapScreen || swapcounter > 5) {
				swapcounter = 0;
                swapScreen = false;
                loopState = TIME_DISPLAY;
            }
			Serial.print(datestampChr);
			Serial.print(" - ");
			Serial.println(buffer);
            break;

        case TIME_DISPLAY:
			getNTPtimechr(timestampChr);
            
			mp.setFont(_renoFont8px);
            mp.clear();
            mp.drawText(4, mp.getYMax(), timestampChr);
            if (swapScreen) {
                swapScreen = false;
                loopState = DATE_DISPLAY;
            }
			Serial.println(timestampChr);
            break;

        }


    }
	delay(50);
   }
