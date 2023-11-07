#include <arduino.h>
#include <PubSubClient.h>
#include "WifiHandler.cpp"

// Pin outs =======================================================
#define PIN_ESP_LED2 2

// WiFi setup =====================================================
const char *SSID = "S23+ de Gabriel";
const char *PASSWORD = "12345678";
WifiHandler WIFI(SSID, PASSWORD);
WiFiClient wifiClient;
// Broker setup ===================================================
PubSubClient MQTT(wifiClient);
const char *MQTT_ID = "dc7745de-0834-4c36-9860-a831468121fd";
const char *BROKER_MQTT = "test.mosquitto.org";
const uint16_t BROKER_PORT = 1883;
// Subscribe topics ===============================================
const char *SUBSCRIBE_TOPIC_ = "";
// Publish topics =================================================
const char *PUBLISH_TOPIC_ = "PucMinas_IoT1_TI_G4_T_Teste";

// Functions Declaration =========================================
void MqttCallback(char *topic, byte *payload, unsigned int length);
void BrokerKeepAlive();

void setup()
{
  Serial.begin(115200);

  // pinModes
  pinMode(PIN_ESP_LED2, OUTPUT);

  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
  MQTT.setCallback(MqttCallback);

  digitalWrite(PIN_ESP_LED2, HIGH);
  delay(1000);
  digitalWrite(PIN_ESP_LED2, LOW);
}

void loop()
{
  BrokerKeepAlive();

  char test[10] = "test";

  MQTT.publish(PUBLISH_TOPIC_, test);

  Serial.println(test);

  delay(2000);
}

void MqttCallback(char *topic, byte *payload, unsigned int length)
{
  // String msg;
  // String ele = "L";

  // /* obtem a string do payload recebido */
  // for (int i = 0; i < length; i++)
  // {
  //   char c = (char)payload[i];
  //   msg += c;
  // }

  // Serial.print("Chegou a seguinte string via MQTT: ");
  // Serial.println(msg);
  // /* toma ação dependendo da string recebida */
  // // if(msg.equals("L")
  // if (msg[0] == 'L')
  // {
  //   digitalWrite(PIN_ESP_LED2, HIGH);
  //   Serial.println("LED aceso mediante comando MQTT");
  // }
  // else
  //   // if (msg.equals("D"))
  //   if (msg[0] == 'D')
  //   {
  //     digitalWrite(PIN_ESP_LED2, LOW);
  //     Serial.println("LED apagado mediante comando MQTT");
  //   }
  //   else
  //     Serial.println("Não identificou comando MQTT");
}

void BrokerKeepAlive()
{
  while(!WIFI.IsConnected())
  {
    WIFI.Connect();
  }

  while (!MQTT.connected())
  {
    Serial.print("Connecting to MQTT Broker: ");
    Serial.println(BROKER_MQTT);

    if (MQTT.connect(MQTT_ID))
    {
      Serial.println("Connected!");
    }
    else
    {
      Serial.println("Fail in connection. Trying again in 2s...");
      delay(2000);
    }
  }

  MQTT.loop();
}