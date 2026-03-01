# 🔔 Campainha WiFi com ESP-01 + Sinric Pro + Alexa / Google Home

Projeto de campainha inteligente com ESP-01 (ESP8266) integrada à nuvem
via Sinric Pro, enviando notificações para assistentes de voz como
Amazon Alexa e Google Home.

O grande diferencial: não é necessário possuir um dispositivo físico
Alexa ou Google Home. Como o Sinric Pro é baseado em nuvem, você pode
conectar sua conta ao aplicativo da Alexa no smartphone e receber
notificações de qualquer lugar do mundo.

------------------------------------------------------------------------

## 🚀 Funcionalidades

-   Conexão WiFi com ESP-01
-   Integração com Sinric Pro (nuvem)
-   Disparo de evento tipo Switch ao pressionar o botão
-   Compatível com Alexa e Google Home
-   Permite criação de rotinas no app Alexa
-   Notificações no celular em qualquer local com internet
-   Sistema com debounce por software

------------------------------------------------------------------------

## 🧠 Como Funciona

1.  O botão é pressionado.\
2.  O ESP-01 envia um evento para o Sinric Pro.\
3.  O Sinric Pro comunica o evento à Alexa ou Google Home.\
4.  Uma rotina pode ser configurada para:
    -   Enviar notificação push\
    -   Falar "Tem alguém no portão"\
    -   Executar qualquer automação configurada

------------------------------------------------------------------------

## 🔧 Hardware Utilizado

-   ESP-01 (ESP8266)
-   Botão (ligado ao GPIO2)
-   Fonte 3.3V estável

### 📌 Ligação do Botão

  ESP-01   Botão
  -------- ---------------------
  GPIO2    Um lado do botão
  GND      Outro lado do botão

O pino está configurado como `INPUT_PULLUP`.

------------------------------------------------------------------------

## 🌐 Configuração do Sinric Pro

1.  Criar conta no site do Sinric Pro.\
2.  Criar um dispositivo do tipo **doorbell**.\
3.  Obter:
    -   APP_KEY\
    -   APP_SECRET\
    -   DEVICE_ID\
4.  Vincular conta ao app Alexa.\
5.  Criar rotina personalizada.

------------------------------------------------------------------------

## 📲 Criando a Rotina na Alexa

No aplicativo Alexa:

1.  Vá em **Mais → Rotinas**\
2.  Criar nova rotina\
3.  Evento:
    -   Quando dispositivo "Campainha" for ligado\
4.  Ação:
    -   Enviar notificação\
    -   Anunciar mensagem personalizada

Exemplo:

> "Tem alguém no portão!"

------------------------------------------------------------------------

## 💻 Código Utilizado

⚠️ **Importante:**\
Substitua pelos seus dados reais de WiFi e Sinric Pro.

``` cpp
#include <ESP8266WiFi.h>
#include <SinricPro.h>
#include <SinricProSwitch.h>

//////////////////////
// WIFI
//////////////////////
#define WIFI_SSID     "SEU_WIFI"
#define WIFI_PASSWORD "SUA_SENHA"

//////////////////////
// SINRIC
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

  SinricPro.handle();

  if (WiFi.status() != WL_CONNECTED) {
    setupWiFi();
  }

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
```

------------------------------------------------------------------------



## 📌 Possíveis Melhorias

-   Adicionar buzzer local\
-   Implementar modo deep sleep\
-   Adicionar LED indicador\
-   Implementar OTA (atualização remota)\
-   Criar versão com ESP32

------------------------------------------------------------------------

## 🎯 Conclusão

Este projeto transforma um simples botão em uma campainha inteligente
baseada em nuvem, eliminando a necessidade de dispositivos físicos
dedicados.

Simples, funcional e escalável.

------------------------------------------------------------------------

Se esse projeto te ajudou, deixe uma ⭐ no repositório!
