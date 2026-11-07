//#include <WiFi.h>
//#include <PubSubClient.h>
//
//// ========== WiFi Credentials ==========
//const char* ssid = "realme 3";
//const char* password = "ansari123";
//
//// ========== MQTT Broker ==========
//const char* mqtt_server = "broker.hivemq.com";
//const int mqtt_port = 1883;
//
//WiFiClient espClient;
//PubSubClient client(espClient);
//
//// ========== TRACK MOTORS (L298N #1) ==========
//#define IN1 25
//#define IN2 26
//#define IN3 27
//#define IN4 14
//#define ENA 32
//#define ENB 33
//
//// ========== BRUSH MOTOR (L298N #2) ==========
//#define BRUSH_IN1 18
//#define BRUSH_IN2 19
//#define BRUSH_ENA 21
//
//// ========== PUMP RELAY PINS ==========
//#define PUMP1_PIN 4
//#define PUMP2_PIN 5
//
//// ========== PWM SETTINGS ==========
//#define FREQ 20000      // High frequency for brush motor
//#define TRACK_FREQ 1000 // Track motors frequency
//#define RESOLUTION 8
//
//void setup_wifi() {
//  delay(10);
//  Serial.println();
//  Serial.print("Connecting to WiFi: ");
//  Serial.println(ssid);
//  WiFi.begin(ssid, password);
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(500);
//    Serial.print(".");
//  }
//  Serial.println("");
//  Serial.println("WiFi Connected!");
//  Serial.print("IP Address: ");
//  Serial.println(WiFi.localIP());
//}
//
//void callback(char* topic, byte* message, unsigned int length) {
//  String msg = "";
//  for (int i = 0; i < length; i++) {
//    msg += (char)message[i];
//  }
//  Serial.print("Topic: ");
//  Serial.print(topic);
//  Serial.print(" | Message: ");
//  Serial.println(msg);
//
//  if (String(topic) == "robot/movement") {
//    if (msg == "forward") moveForward(200);
//    else if (msg == "backward") moveBackward(200);
//    else if (msg == "left") turnLeft(180);
//    else if (msg == "right") turnRight(180);
//    else if (msg == "stop") stopMotors();
//  }
//  if (String(topic) == "robot/brush") {
//    if (msg == "on") brushOn(255);
//    else if (msg == "off") brushOff();
//  }
//  if (String(topic) == "robot/pump1") {
//    if (msg == "on") digitalWrite(PUMP1_PIN, HIGH);
//    else if (msg == "off") digitalWrite(PUMP1_PIN, LOW);
//  }
//  if (String(topic) == "robot/pump2") {
//    if (msg == "on") digitalWrite(PUMP2_PIN, HIGH);
//    else if (msg == "off") digitalWrite(PUMP2_PIN, LOW);
//  }
//}
//
//void reconnect() {
//  while (!client.connected()) {
//    Serial.print("Attempting MQTT connection...");
//    String clientId = "ESP32Client-";
//    clientId += String(random(0xffff), HEX);
//    if (client.connect(clientId.c_str())) {
//      Serial.println("Connected to MQTT Broker!");
//      client.subscribe("robot/movement");
//      client.subscribe("robot/brush");
//      client.subscribe("robot/pump1");
//      client.subscribe("robot/pump2");
//    } else {
//      Serial.print("Failed, rc=");
//      Serial.print(client.state());
//      Serial.println(" trying again in 5 seconds");
//      delay(5000);
//    }
//  }
//}
//
//void moveForward(int speed) {
//  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
//  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
//  ledcWrite(ENA, speed); ledcWrite(ENB, speed);
//}
//
//void moveBackward(int speed) {
//  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
//  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
//  ledcWrite(ENA, speed); ledcWrite(ENB, speed);
//}
//
//void turnLeft(int speed) {
//  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
//  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
//  ledcWrite(ENA, speed); ledcWrite(ENB, speed);
//}
//
//void turnRight(int speed) {
//  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
//  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
//  ledcWrite(ENA, speed); ledcWrite(ENB, speed);
//}
//
//void stopMotors() {
//  ledcWrite(ENA, 0); ledcWrite(ENB, 0);
//}
//
//void brushOn(int speed) {
//  digitalWrite(BRUSH_IN1, HIGH); digitalWrite(BRUSH_IN2, LOW);
//  ledcWrite(BRUSH_ENA, speed);
//}
//
//void brushOff() {
//  ledcWrite(BRUSH_ENA, 0);
//}
//
//void setup() {
//  Serial.begin(115200);
//
//  // Pump pins pehle LOW
//  pinMode(PUMP1_PIN, OUTPUT);
//  pinMode(PUMP2_PIN, OUTPUT);
//  digitalWrite(PUMP1_PIN, LOW);
//  digitalWrite(PUMP2_PIN, LOW);
//
//  delay(500);
//
//  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
//  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
//  pinMode(BRUSH_IN1, OUTPUT); pinMode(BRUSH_IN2, OUTPUT);
//
//  // Track motors — 1000 Hz
//  ledcAttach(ENA, TRACK_FREQ, RESOLUTION);
//  ledcAttach(ENB, TRACK_FREQ, RESOLUTION);
//
//  // Brush motor — 20000 Hz (high frequency)
//  ledcAttach(BRUSH_ENA, FREQ, RESOLUTION);
//
//  setup_wifi();
//  client.setServer(mqtt_server, mqtt_port);
//  client.setCallback(callback);
//}
//
//void loop() {
//  if (!client.connected()) reconnect();
//  client.loop();
//}



#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "realme 3";
const char* password = "ansari123";

const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

// TRACK MOTORS (L298N #1)
#define IN1 25
#define IN2 26
#define IN3 27
#define IN4 14
#define ENA 32
#define ENB 33

// BRUSH MOTOR (BTS7960)
#define BRUSH_RPWM 18
#define BRUSH_LPWM 19
#define BRUSH_EN   21

// PUMP RELAY PINS
#define PUMP1_PIN 4
#define PUMP2_PIN 5

#define FREQ 1000
#define RESOLUTION 8

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void brushOn(int speed) {
  digitalWrite(BRUSH_LPWM, LOW);
  digitalWrite(BRUSH_EN, HIGH);
  ledcWrite(BRUSH_RPWM, speed);
}

void brushOff() {
  ledcWrite(BRUSH_RPWM, 0);
  digitalWrite(BRUSH_EN, LOW);
}

void moveForward(int speed) {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  ledcWrite(ENA, speed); ledcWrite(ENB, speed);
}

void moveBackward(int speed) {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  ledcWrite(ENA, speed); ledcWrite(ENB, speed);
}

void turnLeft(int speed) {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  ledcWrite(ENA, speed); ledcWrite(ENB, speed);
}

void turnRight(int speed) {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  ledcWrite(ENA, speed); ledcWrite(ENB, speed);
}

void stopMotors() {
  ledcWrite(ENA, 0); ledcWrite(ENB, 0);
}

void callback(char* topic, byte* message, unsigned int length) {
  String msg = "";
  for (int i = 0; i < length; i++) msg += (char)message[i];

  Serial.print("Topic: ");
  Serial.print(topic);
  Serial.print(" | Message: ");
  Serial.println(msg);

  if (String(topic) == "robot/movement") {
    if (msg == "forward") moveForward(200);
    else if (msg == "backward") moveBackward(200);
    else if (msg == "left") turnLeft(180);
    else if (msg == "right") turnRight(180);
    else if (msg == "stop") stopMotors();
  }
  if (String(topic) == "robot/brush") {
    if (msg == "on") brushOn(255);   // 255 = 100% duty = full speed
    else if (msg == "off") brushOff();
  }
  if (String(topic) == "robot/pump1") {
    if (msg == "on") digitalWrite(PUMP1_PIN, HIGH);
    else if (msg == "off") digitalWrite(PUMP1_PIN, LOW);
  }
  if (String(topic) == "robot/pump2") {
    if (msg == "on") digitalWrite(PUMP2_PIN, HIGH);
    else if (msg == "off") digitalWrite(PUMP2_PIN, LOW);
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("Connected to MQTT Broker!");
      client.subscribe("robot/movement");
      client.subscribe("robot/brush");
      client.subscribe("robot/pump1");
      client.subscribe("robot/pump2");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" trying again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(PUMP1_PIN, OUTPUT);
  pinMode(PUMP2_PIN, OUTPUT);
  digitalWrite(PUMP1_PIN, LOW);
  digitalWrite(PUMP2_PIN, LOW);

  // Brush driver pehle disable
  pinMode(BRUSH_EN, OUTPUT);
  pinMode(BRUSH_LPWM, OUTPUT);
  digitalWrite(BRUSH_EN, LOW);
  digitalWrite(BRUSH_LPWM, LOW);

  delay(500);

  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);

  ledcAttach(ENA, FREQ, RESOLUTION);
  ledcAttach(ENB, FREQ, RESOLUTION);
  ledcAttach(BRUSH_RPWM, FREQ, RESOLUTION);
  ledcWrite(BRUSH_RPWM, 0);

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();
}
