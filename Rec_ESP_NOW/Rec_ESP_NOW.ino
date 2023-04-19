#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h>

#define servoA_pin 13
#define servoB_pin 12
Servo myservoA;
Servo myservoB;

#define LED_BUILTIN 2

//uint8_t broadcastAddress[] = {0x40,0x22,0xD8,0x5F,0xF7,0xFC};// mac metalico
//uint8_t broadcastAddress[] = {0x58,0xBF,0x25,0x81,0x3E,0x98};//mac aberto 

// Definir a estrutura para o pacote de dados
typedef struct struct_message {
  int servoA, servoB;
  bool led;
} struct_message;

// Inicializar a estrutura do pacote de dados
struct_message myData;

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  digitalWrite(LED_BUILTIN, myData.led);

  /* Serial.println("=== Data ===");
  Serial.print("endereço MAC: ");
  for (int i = 0; i < 6; i++) {
      Serial.print("0x");
      Serial.print(mac[i], HEX);
      Serial.print(":");
  } */

  Serial.println(myData.servoA);
  Serial.println(myData.servoB);

  myservoA.write(myData.servoA);
  myservoB.write(myData.servoB);
  //Serial.println();
}//end OnDataSent

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,LOW);

  ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
  myservoA.setPeriodHertz(50);
  myservoB.setPeriodHertz(50);
  myservoA.attach(servoA_pin, 600, 2400);
  myservoB.attach(servoB_pin, 600, 2400);

  myservoA.write(0);
  myservoB.write(0);

  WiFi.mode(WIFI_STA);
  Serial.print("WiFi.macAddress: ");
  Serial.println(WiFi.macAddress());
  /*
      Available ESP32 RF power parameters:
      WIFI_POWER_19_5dBm    // 19.5dBm (For 19.5dBm of output, highest. Supply current ~150mA)
      WIFI_POWER_19dBm      // 19dBm
      WIFI_POWER_18_5dBm    // 18.5dBm
      WIFI_POWER_17dBm      // 17dBm
      WIFI_POWER_15dBm      // 15dBm
      WIFI_POWER_13dBm      // 13dBm
      WIFI_POWER_11dBm      // 11dBm
      WIFI_POWER_8_5dBm     //  8dBm
      WIFI_POWER_7dBm       //  7dBm
      WIFI_POWER_5dBm       //  5dBm
      WIFI_POWER_2dBm       //  2dBm
      WIFI_POWER_MINUS_1dBm // -1dBm( For -1dBm of output, lowest. Supply current ~120mA)
      Available ESP8266 RF power parameters:
      0    (for lowest RF power output, supply current ~ 70mA
      20.5 (for highest RF power output, supply current ~ 80mA
  */ 
  WiFi.setTxPower(WIFI_POWER_19_5dBm);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  /* 
  esp_now_peer_info_t peerInfo;
  memset(&peerInfo, 0, sizeof(peerInfo));
  for (int ii = 0; ii < 6; ++ii )
  {
    peerInfo.peer_addr[ii] = (uint8_t) broadcastAddress[ii];
  }
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  } */
  esp_now_register_recv_cb(OnDataRecv);
}//end setup

void loop() {
  delay(1);
}//end loop