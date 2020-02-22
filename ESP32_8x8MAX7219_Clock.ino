/*
 Name:		ESP32_8x8MAX7219_Clock.ino
 Created:	26/01/2020 21:48:30
 Author:	renau
*/

//#include <SPI.h>



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

//String timeStamp;
//String Previous_timeStamp;

char timestampChr[50];
//char Previous_timeStamp[50]; 

void onSwitchPressed(Button2& btn) {
	swapScreen = true;
}

enum State {
    UNDEFINED,
    DATE_DISPLAY,
    TIME_DISPLAY,
    TEMP_DISPLAY
};

State loopState = UNDEFINED;

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
		getNTPtimechr(timestampChr);
        switch (loopState) {
		default:
			break;
        case UNDEFINED:
            loopState = TIME_DISPLAY;
            break;
        case DATE_DISPLAY:
            Serial.println("DATE_DISPLAY");
            
			mp.setFont(_Fixed_5x3);
            mp.clear();
            mp.drawText(0, mp.getYMax(), getNTPdate().c_str());

            mp.drawText(31, 4, HTTPrequestTemperature().c_str());
			mp.drawText(44,4, "$");
			swapcounter++;
            if (swapScreen || swapcounter > 5) {
				swapcounter = 0;
                swapScreen = false;
                loopState = TIME_DISPLAY;
            }
            break;
        case TIME_DISPLAY:
            Serial.println("TIME_DISPLAY");
            mp.setFont(_renoFont8px);
			//timeStamp = getNTPtime();
            mp.clear();
            mp.drawText(4, mp.getYMax(), timestampChr);
            if (swapScreen) {
                swapScreen = false;
                loopState = DATE_DISPLAY;
            }
            break;

        }


    }
	yield();
    delay(50);
   }
