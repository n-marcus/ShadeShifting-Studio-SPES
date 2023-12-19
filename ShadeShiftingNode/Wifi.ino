
void setupWifi() {
  // Connect to WiFi network
  DEBUG_PRINT();
  DEBUG_PRINT();
  Serial.print("Connecting to " + String(WIFI_SSID));
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  bool connectingWifiLED = false;

  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
    connectingWifiLED = !connectingWifiLED;
    if (connectingWifiLED) {
      leds[0] = CHSV(0, 0, 0);
    } else {
      leds[0] = CRGB(0,0,255);
    }
    FastLED.show();
  }
  leds[0] = CRGB::Green;
  FastLED.show();

  DEBUG_PRINT("");

  DEBUG_PRINT("WiFi connected");
  DEBUG_PRINT("IP address: ");
  DEBUG_PRINT(WiFi.localIP());
  delay(100);

  DEBUG_PRINT("Starting UDP");
  Udp.begin(localPort);
  DEBUG_PRINT("Local port: ");
#ifdef ESP32
  DEBUG_PRINT(localPort);
#else
  DEBUG_PRINT(Udp.localPort());
#endif
}