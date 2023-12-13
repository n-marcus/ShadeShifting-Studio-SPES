
void setupWifi() {
  // Connect to WiFi network
  DEBUG_PRINT();
  DEBUG_PRINT();
  Serial.print("Connecting to " + String(WIFI_SSID));
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  DEBUG_PRINT("");

  DEBUG_PRINT("WiFi connected");
  DEBUG_PRINT("IP address: ");
  DEBUG_PRINT(WiFi.localIP());

  DEBUG_PRINT("Starting UDP");
  Udp.begin(localPort);
  DEBUG_PRINT("Local port: ");
#ifdef ESP32
  DEBUG_PRINT(localPort);
#else
  DEBUG_PRINT(Udp.localPort());
#endif
}