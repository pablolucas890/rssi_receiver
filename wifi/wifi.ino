#include "Arduino.h"
#include "ESP8266.h"

#include "SoftwareSerial.h"
SoftwareSerial	ConsoleOut(6, 7);

#define SSID	"Pablo Wifi"
#define PWD		"zoeeamora"

void setup() {
	char	*ipAddress, ap[31];
  Serial.begin(9600);

	WiFi.reset(WIFI_RESET_HARD);
	WiFi.begin(9600);
	if (WiFi.join(SSID, PWD) == WIFI_ERR_OK) {
		ipAddress = WiFi.ip(WIFI_MODE_STA);
		ConsoleOut.print(F("\n\rIP address:"));
		ConsoleOut.print(ipAddress);
		ConsoleOut.print(':');
		if (WiFi.isConnect(ap))
			ConsoleOut.println(ap);
		else
			ConsoleOut.println(F("not found"));
	} else
    Serial.println("Erro");

}

void loop() {
	if (WiFi.connect((char *)"www.google.co.jp", 80) == WIFI_ERR_CONNECT) {

		if (WiFi.send((const uint8_t *)"GET / HTTP/1.1\r\n\r\n") == WIFI_ERR_OK) {
			int16_t	c;
			uint16_t len = WiFi.listen(10000UL);
			while (len)
				if ((c = WiFi.read()) > 0) {
          Serial.println("Foiiii");
					ConsoleOut.write((char)c);
					len--;
				}
		} else
			ConsoleOut.println(F("Send fail"));

		WiFi.close();

	} else
    Serial.println("Erro");
		ConsoleOut.println(F("TCP connect fail"));

	WiFi.disconnect();

	while (1);
}