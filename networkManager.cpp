// 
// 
// 

#include "networkManager.h"
#include "Configuration.h"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
HTTPClient http;

String dayStamp;
String formattedDate;
String temperature;

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

	timeClient.begin();
	timeClient.setTimeOffset(3600);

	// wait for NTP sync
	while (!timeClient.update()) {
		Serial.println("NTP forceupdate()");
		timeClient.forceUpdate();
	}

}

String getNTPdate() {

	formattedDate = timeClient.getFormattedDate();
	int splitT = formattedDate.indexOf("T");
	String dateStamp = formattedDate.substring(0, splitT);
	dateStamp = formattedDate.substring(8, 10) + "/" + formattedDate.substring(5, 7) + "/" + formattedDate.substring(2, 4);


	//char myString[100] = String::c_str(timeClient.getFormattedDate());


	return dateStamp;
}

char* getNTPtimechr() {
	unsigned long rawTime = timeClient.getEpochTime();
	unsigned long hours = (rawTime % 86400L) / 3600;
	//String hoursStr = hours < 10 ? "0" + String(hours) : String(hours);

	unsigned long minutes = (rawTime % 3600) / 60;
	//String minuteStr = minutes < 10 ? "0" + String(minutes) : String(minutes);

	unsigned long seconds = rawTime % 60;
	//String secondStr = seconds < 10 ? "0" + String(seconds) : String(seconds);
	
	char buffer[20];
	sprintf(buffer, "%d:%d:%d", hours, minutes, seconds);
	Serial.println(buffer);
	return buffer;
}

String getNTPtime() {

	formattedDate = timeClient.getFormattedDate();
	int splitT = formattedDate.indexOf("T");
	return formattedDate.substring(splitT + 1, formattedDate.length() - 1);

}

String HTTPrequestTemperature() {

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

	//const char* ServerTime = domo_deviceResp_doc["ServerTime"]; // "2020-01-31 23:37:03"
	JsonObject result_0 = domo_deviceResp_doc["result"][0];
	const char* result_0_Data = domo_deviceResp_doc["Data"]; // device temp "-127.0 C"
	http.end();

	return result_0_Data;
}

float requestTemperatureVal() {
	//TO TEST
	String data = HTTPrequestTemperature();
	data = data.substring(data.length() - 1, data.length());
	return atof(data.c_str());
}
