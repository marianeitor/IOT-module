#include "webserver.h"
#include "index.html.gz.h"

ESP8266WebServer server(80);

void handleRoot() {
  server.sendHeader("Content-Encoding", "gzip");
  server.send_P(200, "text/html", index_html_gz, index_html_gz_len);
}

void handleConfig() {
  String essid(server.arg("essid"));

  server.send(200,"text/plain",essid);
  //server.sendHeader("Content-Encoding", "gzip");
  //server.send_P(200, "text/html", index_html_gz, index_html_gz_len);
}

void handleNotFound() {
  String message = "File Not Found\n\nURI: ";
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
}

void WebServer_init() {
  server.on("/", handleRoot);
  server.on("/config", handleConfig);
  server.onNotFound(handleNotFound);
  server.begin();
}

void WebServer_stop() {
  server.stop();
}

void WebServer_loop() {
  server.handleClient();
}
