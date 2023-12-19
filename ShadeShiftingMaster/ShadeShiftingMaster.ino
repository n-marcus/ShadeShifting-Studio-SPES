#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>
#include <elapsedMillis.h>

// Set WiFi credentials
#define WIFI_SSID "StudioSPES"
#define WIFI_PASS "FridayDance2020!"

WiFiUDP Udp;
const IPAddress outIp(255, 255, 255, 255);  // remote IP (not needed for receive)
const unsigned int outPort = 8888;
const unsigned int localPort = 9999;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  setupWifi();
  setupScenes();

  printMotorData();
}

void loop() {
  // put your main code here, to run repeatedly:

  sendRandomRotationToNode(0);
  sendRandomRotationToNode(1);

  delay(5000);
}

void sendRandomRotationToNode(int node) {
  OSCMessage msg("/moveToAngle");
  msg.add(node);
  msg.add(int(random(360)));
  Udp.beginPacket(outIp, outPort);
  msg.send(Udp);
  Udp.endPacket();
  msg.empty();
}
