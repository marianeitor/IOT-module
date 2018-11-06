
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>
#include <EEPROM.h>
#include "config.h"
#include "webserver.h"

const char* ssid = "IOT-test";
const char* password = "";

config_t conf;

DNSServer dnsServer;


IPAddress apIP(192, 168, 4, 1);
IPAddress netMsk(255, 255, 255, 0);

int led = 13;

void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);

  EEPROM.begin(sizeof(config_t));
  // EEPROM.put(0, conf);
  // EEPROM.commit();  
  
  EEPROM.get(0, conf);  

  Serial.begin(115200);
  WiFi.softAPConfig(apIP, apIP, netMsk);
  WiFi.softAP("TestIOT-rmm", "");

  // Wait for connection

  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  } else {
    Serial.println("mDNS responder started");
    // Add service to MDNS-SD
    MDNS.addService("http", "tcp", 80);
  }
  WebServer_init();

  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(53, "*", apIP);

  Serial.println("HTTP server started");
}

void loop(void) {
  //DNS
  dnsServer.processNextRequest();
  //HTTP
  WebServer_loop();
}
