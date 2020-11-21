#include <PubSubClient.h>
#include <WiFi.h>
#include <ESP32Servo.h>
#include "Ultrasonic.h"

#define TRIG 27
#define ECHO 26


Servo servo;
Ultrasonic sensor(TRIG, ECHO);
WiFiClient espClient;
PubSubClient client(espClient);



int test;
char msg[25];
int pos = 0;

const char* usuario = "Silva";
const char* senha = "Lutecia1973";


const char* mqttServer = "ioticos.org";
const int mqttPort = 1883;
const char* mqttUser = "M6bt5rZzrpQ37Cw";
const char* mqttPassword = "pAcCb75SApCT5aZ";
const char* root_topic_subscribe = "bW5mpaOpKfOAiWc/esp32";
const char* root_topic_publish = "bW5mpaOpKfOAiWc/esp32";




void callback(char* topic, byte* payload, unsigned int length);

void setup() {
  servo.attach(15);
  servo.write(0);
  Serial.begin(115200);
  WiFi.begin(usuario, senha);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("Conectando ao wifi:");
    Serial.println(usuario);
  }

  Serial.println("Conectado à rede wifi");
  Serial.println("");
  Serial.println("Endereço do ip: ");
  Serial.println(WiFi.localIP());
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while (!client.connected()) {
    Serial.println("Conectando-se ao MQTT…");
    String clientId = "ESP32-projeto";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str(), mqttUser, mqttPassword )) {
      Serial.println("conectado");
    } else {
      Serial.print("falha na conexão");
      Serial.print(client.state());
      delay(2000);
    }
  }

  Serial.print("Tentando enviar a mensagem");
  client.publish(root_topic_publish, msg);
  client.subscribe(root_topic_subscribe);

}


void callback(char* topic, byte* payload, unsigned int length) {
 
  if (payload[0] == '0') {
    pos = 90;
    servo.write(pos);
    delay(15);
  }
  if (payload[0] == '1') {
    pos = 0;
    servo.write(pos);
    delay(15);

  }
}

void loop() { 
  sensor.measure();
  test = sensor.get_cm();
  Serial.println(sensor.get_cm());
  delay(50);

  if (test < 7 && pos == 0){
    pos = 90;
    servo.write(pos);
    delay(3000);
    pos = 0;
    servo.write(pos);
    delay(15);
  }
  delay(15);
  
  client.loop();
}
