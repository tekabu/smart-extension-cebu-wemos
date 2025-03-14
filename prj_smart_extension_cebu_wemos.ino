#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include "page.h"

const char* ssid = "Smart-Outlet";
const char* password = "Password123";

ESP8266WebServer server(80);
DNSServer dnsServer;

void handleRoot() {
  server.send(200, "text/html", htmlPage1);
}

void handlePage2() {
  server.send(200, "text/html", htmlPage2);
}

void handleCaptivePortal() {
  server.sendHeader("Location", "/", true);
  server.send(302, "text/plain", ""); // Redirect to root
}

void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.println();
  Serial.println("Configuring access point...");

  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  dnsServer.start(53, "*", myIP);

  server.on("/", handleRoot);
  server.on("/settings", handlePage2);
  server.on("/generate_204", handleCaptivePortal); //for newer android versions.
  server.on("/fwlink", handleCaptivePortal); //for windows
  server.on("/ncsi.txt", handleCaptivePortal); //for windows
  server.on("/connecttest.txt", handleCaptivePortal); //for windows
  server.on("/hotspot-detect.html", handleCaptivePortal); //for android
  server.on("/connectivitycheck.gstatic.com", handleCaptivePortal); //for android

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
}
