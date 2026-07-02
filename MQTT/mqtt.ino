#include <WiFi.h>
#include <PubSubClient.h>

// ========== WiFi Credentials ==========
const char* ssid = "wifi";
const char* password = "password";

// ========== MQTT Broker & Unique Topics ==========
const char* mqtt_server = "://hivemq.com";
const int mqtt_port = 1883;

// CHANGE "myuniquebot99" to any random word/number sequence to prevent others from hijacking your topics
#define TOPIC_MOVEMENT "myuniquebot99/robot/movement"
#define TOPIC_BRUSH    "myuniquebot99/robot/brush"
#define TOPIC_PUMP1    "myuniquebot99/robot/pump1"
#define TOPIC_PUMP2    "myuniquebot99/robot/pump2"

WiFiClient espClient;
PubSubClient client(espClient);

// ========== TRACK MOTORS (L298N #1) ==========
#define IN1 25
#define IN2 26
#define IN3 27
#define IN4 14
#define ENA 32
#define ENB 33

// ========== BRUSH MOTOR (L298N #2) ==========
#define BRUSH_IN1 18
#define BRUSH_IN2 19
#define BRUSH_ENA 21

// ========== PUMP RELAY PINS ==========
#define PUMP1_PIN 4   // Water Pump Relay
#define PUMP2_PIN 5   // Soap Pump Relay

// ========== PWM SETTINGS ==========
#define FREQ 1000
#define RESOLUTION 8

// Forward declarations of movement functions so compiler knows they exist
void moveForward(int speed);
void moveBackward(int speed);
void turnLeft(int speed);
void turnRight(int speed);
void stopMotors();
void brushOn(int speed);
void brushOff();

// ===================================================
// WiFi Setup
// ===================================================
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

// ===================================================
// MQTT Message Callback
// ===================================================
void callback(char* topic, byte* message, unsigned int length) {
  // Convert payload bytes into a clean string
  String msg = "";
  for (int i = 0; i < length; i++) {
    msg += (char)message[i];
  }
  msg.trim(); // Automatically strip out accidental newlines or trailing spaces

  // Debug Print: Check if ANY message reaches the board
  Serial.println("\n--- [NEW MQTT PACKET] ---");
  Serial.print("Topic: ");
  Serial.println(topic);
  Serial.print("Payload: [");
  Serial.print(msg);
  Serial.println("]");

  String topicStr = String(topic);

  // ---- Movement Commands ----
  if (topicStr == TOPIC_MOVEMENT) {
    if (msg == "forward") {
      Serial.println("Action: Moving Forward!");
      moveForward(200);
    } 
    else if (msg == "backward") {
      Serial.println("Action: Moving Backward!");
      moveBackward(200);
    } 
    else if (msg == "left") {
      Serial.println("Action: Turning Left!");
      turnLeft(180);
    } 
    else if (msg == "right") {
      Serial.println("Action: Turning Right!");
      turnRight(180);
    } 
    else if (msg == "stop") {
      Serial.println("Action: Stopping Motors!");
      stopMotors();
    }
  }

  // ---- Brush Commands ----
  else if (topicStr == TOPIC_BRUSH) {
    if (msg == "on") {
      Serial.println("Action: Brush ON!");
      brushOn(200);
    } 
    else if (msg == "off") {
      Serial.println("Action: Brush OFF!");
      brushOff();
    }
  }

  // ---- Pump 1 (Water) Commands ----
  else if (topicStr == TOPIC_PUMP1) {
    if (msg == "on") {
      Serial.println("Action: Water Pump ON!");
      digitalWrite(PUMP1_PIN, HIGH);
    } 
    else if (msg == "off") {
      Serial.println("Action: Water Pump OFF!");
      digitalWrite(PUMP1_PIN, LOW);
    }
  }

  // ---- Pump 2 (Soap) Commands ----
  else if (topicStr == TOPIC_PUMP2) {
    if (msg == "on") {
      Serial.println("Action: Soap Pump ON!");
      digitalWrite(PUMP2_PIN, HIGH);
    } 
    else if (msg == "off") {
      Serial.println("Action: Soap Pump OFF!");
      digitalWrite(PUMP2_PIN, LOW);
    }
  }
}

// ===================================================
// MQTT Reconnect Logic
// ===================================================
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    // Generate a strictly random Client ID to prevent server kicks
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("Connected to MQTT Broker!");

      // Resubscribe using unique topics
      client.subscribe(TOPIC_MOVEMENT);
      client.subscribe(TOPIC_BRUSH);
      client.subscribe(TOPIC_PUMP1);
      client.subscribe(TOPIC_PUMP2);
      
      Serial.println("Topics Subscribed Successfully.");

    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" trying again in 5 seconds");
      delay(5000);
    }
  }
}

// ===================================================
// Track Motor Functions (Updated for ESP32 3.0+ Core API)
// ===================================================
void moveForward(int speed) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  ledcWrite(ENA, speed); // Core 3.0+ expects the Pin directly if attached via ledcAttach
  ledcWrite(ENB, speed);
}

void moveBackward(int speed) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  ledcWrite(ENA, speed);
  ledcWrite(ENB, speed);
}

void turnLeft(int speed) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  ledcWrite(ENA, speed);
  ledcWrite(ENB, speed);
}

void turnRight(int speed) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  ledcWrite(ENA, speed);
  ledcWrite(ENB, speed);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  ledcWrite(ENA, 0);
  ledcWrite(ENB, 0);
}

void brushOn(int speed) {
  digitalWrite(BRUSH_IN1, HIGH);
  digitalWrite(BRUSH_IN2, LOW);
  ledcWrite(BRUSH_ENA, speed);
}

void brushOff() {
  digitalWrite(BRUSH_IN1, LOW);
  digitalWrite(BRUSH_IN2, LOW);
  ledcWrite(BRUSH_ENA, 0);
}

// ===================================================
// Setup
// ===================================================
void setup() {
  Serial.begin(115200);

  // Track motor pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Brush motor pins
  pinMode(BRUSH_IN1, OUTPUT);
  pinMode(BRUSH_IN2, OUTPUT);

  // Pump pins
  pinMode(PUMP1_PIN, OUTPUT);
  pinMode(PUMP2_PIN, OUTPUT);
  digitalWrite(PUMP1_PIN, LOW);
  digitalWrite(PUMP2_PIN, LOW);

  // Modern ESP32 Core PWM setup (attaches frequency/resolution directly to the physical pin)
  ledcAttach(ENA, FREQ, RESOLUTION);
  ledcAttach(ENB, FREQ, RESOLUTION);
  ledcAttach(BRUSH_ENA, FREQ, RESOLUTION);

  // Run initialization routines
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

// ===================================================
// Main Loop
// ===================================================
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
