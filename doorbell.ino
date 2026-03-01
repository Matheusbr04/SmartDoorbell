#include <ESP8266WiFi.h>
#include <SinricPro.h>
#include <SinricProSwitch.h>

//////////////////////
// WIFI (SUBSTITUA PELOS SEUS DADOS)
//////////////////////
#define WIFI_SSID     "SEU_WIFI"
#define WIFI_PASSWORD "SUA_SENHA"

//////////////////////
// SINRIC PRO (SUBSTITUA PELOS SEUS DADOS)
//////////////////////
#define APP_KEY       "SUA_APP_KEY"
#define APP_SECRET    "SEU_APP_SECRET"
#define DEVICE_ID     "SEU_DEVICE_ID"

//////////////////////
// PINOS
//////////////////////
#define BUTTON_PIN 2   // GPIO2

//////////////////////
// VARIÁVEIS
//////////////////////
bool lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

//////////////////////
// CONECTA WIFI
//////////////////////
void setupWiFi() {
  Serial.print("Conectando ao WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("\nWiFi conectado!");
  Serial.println(WiFi.localIP());
}

//////////////////////
// EVENTO CAMPANHA
//////////////////////
void sendDoorbellEvent() {
  Serial.println("Campainha acionada!");

  SinricProSwitch &mySwitch = SinricPro[DEVICE_ID];
  mySwitch.sendPowerStateEvent(true);
  delay(200);
  mySwitch.sendPowerStateEvent(false);
}

//////////////////////
// SETUP
//////////////////////
void setup() {
  Serial.begin(115200);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  setupWiFi();

  SinricPro.begin(APP_KEY, APP_SECRET);
  SinricPro.restoreDeviceStates(true);
}

//////////////////////
// LOOP
//////////////////////
void loop() {

  // Mantém conexão ativa
  SinricPro.handle();

  // Reconecta WiFi se cair
  if (WiFi.status() != WL_CONNECTED) {
    setupWiFi();
  }

  // Leitura botão com debounce
  bool reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {

    if (reading == LOW && lastButtonState == HIGH) {
      sendDoorbellEvent();
    }
  }

  lastButtonState = reading;
}