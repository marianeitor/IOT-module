#include "index.html.gz.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>

const char* ssid = "IOT-test";
const char* password = "";

ESP8266WebServer server(80);
// DNS server
const byte DNS_PORT = 53;
DNSServer dnsServer;


IPAddress apIP(192, 168, 4, 1);
IPAddress netMsk(255, 255, 255, 0);

const int led = 13;

void handleRoot() {
  digitalWrite(led, 1);
  
  server.sendHeader("Content-Encoding", "gzip");
  server.send_P(200, "text/html", index_html_gz, index_html_gz_len);
  digitalWrite(led, 0);
}

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  // WiFi.mode(WIFI_AP);
  // WiFi.begin(ssid, password);
  WiFi.softAPConfig(apIP, apIP, netMsk);
  WiFi.softAP("TestIOT","");

 
  Serial.println("");

  // Wait for connection

  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();

  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(DNS_PORT, "*", apIP);
  
  Serial.println("HTTP server started");
}

void loop(void) {
  //DNS
  dnsServer.processNextRequest();
  //HTTP
  server.handleClient();
}
