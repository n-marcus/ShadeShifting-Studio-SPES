void setupWifi() {
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connecting to ");
  lcd.setCursor(0, 1);
  lcd.print(WIFI_SSID);

  bool LEDOn = true;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    LEDOn = !LEDOn;
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.print(".");
  }
  Serial.println("");

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port: ");
#ifdef ESP32
  Serial.println(localPort);
#else
  Serial.println(Udp.localPort());
#endif

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connected to ");
  lcd.setCursor(0, 1);
  lcd.print(WIFI_SSID);
  delay(500);
}