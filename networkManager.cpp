// 
// 
// 

#include "networkManager.h"
#include "Configuration.h"

#define LEAP_YEAR(Y)     ( (Y>0) && !(Y%4) && ( (Y%100) || !(Y%400) ) )

HTTPClient http;

// NTP server parameters
int8_t timeZone = 1;
int8_t minutesTimeZone = 0;
const PROGMEM char* ntpServer = "pool.ntp.org";

const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

// DOMOTICZ_SERVER/json.htm?type=devices&rid=28 response object size
// use https://arduinojson.org/v6/assistant/ for parsing code
const size_t domo_deviceResp_capacity = 2 * JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(16) + JSON_OBJECT_SIZE(34) + 870;
DynamicJsonDocument domo_deviceResp_doc(domo_deviceResp_capacity);
void printLocalTime()
{
	struct tm timeinfo;
	if (!getLocalTime(&timeinfo)) {
		Serial.println("Failed to obtain time");
		return;
	}
	Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}
void networkConnect() {
	WiFi.disconnect(true);
	WiFi.mode(WIFI_OFF);
	///esp_wifi_stop();
	delay(1000);
	WiFi.persistent(false);
	WiFi.mode(WIFI_STA);
	WiFi.begin(WIFI_SSID, WIFI_PASS);
	while (WiFi.waitForConnectResult() != WL_CONNECTED) {
		Serial.print("/");
		delay(1000);
	}
	while (!WiFi.isConnected()) {
		Serial.print(".");
		delay(1000);
	}
	Serial.println(WiFi.localIP());

	configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
	printLocalTime();

	
}


void getNTPtimechr(char *buf) {
	struct tm timeinfo;
	if (!getLocalTime(&timeinfo)) {
		Serial.println("Failed to obtain time");
		return;
	}
	/*unsigned long rawTime = timeClient.getEpochTime();
	unsigned long hours = (rawTime % 86400L) / 3600;

	unsigned long minutes = (rawTime % 3600) / 60;

	unsigned long seconds = rawTime % 60;
	
	sprintf(buf, "%02d:%02d:%02d", hours, minutes, seconds);*/
	sprintf(buf, "%02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
	//Serial.println(buffer);
}

void getNTPdatechr(char *buf) {

	struct tm timeinfo;
	if (!getLocalTime(&timeinfo)) {
		Serial.println("Failed to obtain time");
		return;
	}
	sprintf(buf,"%d/%02d/%d", timeinfo.tm_mday, timeinfo.tm_mon + 1, (timeinfo.tm_year + 1900)%100);
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
