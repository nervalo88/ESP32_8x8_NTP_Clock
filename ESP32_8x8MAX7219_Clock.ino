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

#include <OneWire.h>
#include <DallasTemperature.h>

#include <MD_MAXPanel.h>
#include <Button2.h>
#include "Fonts.h"

#include "Configuration.h"



MD_MAXPanel mp = MD_MAXPanel(HARDWARE_TYPE, CS_PIN, X_DEVICES, Y_DEVICES);
Button2 buttonSwitch = Button2(PUSH_BUTTON_1);
OneWire oneWire(ONE_WIRE_DS18B20_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature tempSensors(&oneWire);

uint32_t prevMillis;
bool swapScreen = false;
int swapcounter =0;
unsigned long timer =0;

char timestampChr[50] = { '\0' };
char datestampChr[80] = { '\0' };

float intTemp;
float extTemp;
float extTempHist =0;
float intTempHist =0;

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

	loopState = TIME_DISPLAY;

    Serial.begin(115200);
    Serial.println("Start");

	buttonSwitch.setClickHandler(onSwitchPressed);
	
	networkConnect();

    mp.begin();
    mp.setIntensity(1);

	tempSensors.begin();
	Serial.print("Found ");
	Serial.print(tempSensors.getDeviceCount(), DEC);
	Serial.println(" DS18B20.");

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

		case TEMP_DISPLAY:
			tempSensors.requestTemperatures();
			
			intTemp = tempSensors.getTempCByIndex(0);
			extTemp = HTTPrequestTemperature();

			if (intTemp != intTempHist || extTemp != extTempHist) {
				intTempHist = intTemp;
				extTempHist = extTemp;
				
				mp.setFont(_Fixed_5x3);
				mp.clear();
				char buffer[10];
				sprintf(buffer, "%0.1f$", intTemp);
				mp.drawText( 4, mp.getYMax()-2, buffer);
				Serial.print(buffer);
				Serial.print(" ");
				sprintf(buffer, "%0.1f$", extTemp);
				mp.drawText(27, mp.getYMax() - 2, buffer);
				Serial.println(buffer);
			}
			else {
				Serial.println(" - ");
			}
			
												
			swapcounter++;
			if (swapScreen || swapcounter > 5) {
				swapcounter = 0;
				swapScreen = false;
				loopState =  DATE_DISPLAY;
			}
			
			break;

        case DATE_DISPLAY:
            
			getNTPdatechr(datestampChr);
			
			mp.setFont(_renoFont8px);
            mp.clear();
            mp.drawText(0, mp.getYMax(), datestampChr);
						
			swapcounter++;
            if (swapScreen || swapcounter > 5) {
				swapcounter = 0;
                swapScreen = false;
				loopState = TIME_DISPLAY;
            }
			Serial.println(datestampChr);

            break;

        case TIME_DISPLAY:
			getNTPtimechr(timestampChr);
            
			mp.setFont(_renoFont8px);
            mp.clear();
            mp.drawText(4, mp.getYMax(), timestampChr);
			swapcounter++;
			if (swapScreen || swapcounter > 5) {
				swapcounter = 0;
				swapScreen = false;
				loopState = TEMP_DISPLAY;
			}
			Serial.println(timestampChr);
            break;

        }


    }
	delay(50);
   }
