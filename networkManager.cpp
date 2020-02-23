// 
// 
// 

#include "networkManager.h"
#include "Configuration.h"

#define LEAP_YEAR(Y)     ( (Y>0) && !(Y%4) && ( (Y%100) || !(Y%400) ) )

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
HTTPClient http;

// NTP server parameters
int8_t timeZone = 1;
int8_t minutesTimeZone = 0;
const PROGMEM char* ntpServer = "pool.ntp.org";

// DOMOTICZ_SERVER/json.htm?type=devices&rid=28 response object size
// use https://arduinojson.org/v6/assistant/ for parsing code
const size_t domo_deviceResp_capacity = 2 * JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(16) + JSON_OBJECT_SIZE(34) + 870;
DynamicJsonDocument domo_deviceResp_doc(domo_deviceResp_capacity);

void networkConnect() {

	WiFi.mode(WIFI_STA);
	WiFi.begin(WIFI_SSID, WIFI_PASS);

	while (!WiFi.isConnected()) {
		Serial.print(".");
		delay(1000);
	}
	Serial.println(WiFi.localIP());

	timeClient.begin();
	timeClient.setTimeOffset(3600);

	// wait for NTP sync
	while (!timeClient.update()) {
		Serial.println("NTP forceupdate()");
		timeClient.forceUpdate();
	}

}


void getNTPtimechr(char *buf) {
	unsigned long rawTime = timeClient.getEpochTime();
	unsigned long hours = (rawTime % 86400L) / 3600;

	unsigned long minutes = (rawTime % 3600) / 60;

	unsigned long seconds = rawTime % 60;
	
	sprintf(buf, "%02d:%02d:%02d", hours, minutes, seconds);
	
	//Serial.println(buffer);
}

void getNTPdatechr(char *buf) {

	struct tm ts;
	time_t rawTime = timeClient.getEpochTime();
	ts = *localtime(&rawTime);
	sprintf(buf,"%d/%02d/%d", ts.tm_mday, ts.tm_mon + 1, (ts.tm_year + 1900)%100);
}


float HTTPrequestTemperature() {

	http.begin("http://192.168.0.40/json.htm?type=devices&rid=28");
	int httpRespCode = http.GET();
	int httpRespSize = http.getSize();
	String httpRespStr = http.getString();
	DeserializationError error = deserializeJson(domo_deviceResp_doc, httpRespStr);

	if (error) {
		Serial.print(F("domo_deviceResp_doc deserializeJson() failed: "));
		Serial.println(error.c_str());
		http.end();
	}

	//DEBUG : to check if JSO object correctly received, test root response
	//const char* ServerTime = domo_deviceResp_doc["ServerTime"]; // "2020-01-31 23:37:03"
	//Serial.println(ServerTime);
	
	JsonObject result_0 = domo_deviceResp_doc["result"][0];
	float result_0_Temp = result_0["Temp"];
	http.end();
	
	return result_0_Temp;
}
