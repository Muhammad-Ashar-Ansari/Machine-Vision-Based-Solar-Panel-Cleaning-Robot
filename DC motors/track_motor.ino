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

// ========== PWM SETTINGS ==========
#define FREQ 1000
#define RESOLUTION 8
#define CH_LEFT 0
#define CH_RIGHT 1
#define CH_BRUSH 2

void setup() {
  // Track Motor Pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Brush Motor Pins
  pinMode(BRUSH_IN1, OUTPUT);
  pinMode(BRUSH_IN2, OUTPUT);

  // PWM Setup
  ledcSetup(CH_LEFT, FREQ, RESOLUTION);
  ledcSetup(CH_RIGHT, FREQ, RESOLUTION);
  ledcSetup(CH_BRUSH, FREQ, RESOLUTION);

  ledcAttachPin(ENA, CH_LEFT);
  ledcAttachPin(ENB, CH_RIGHT);
  ledcAttachPin(BRUSH_ENA, CH_BRUSH);

  Serial.begin(115200);
  Serial.println("Robot Initialized!");
}

void moveForward(int speed) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  ledcWrite(CH_LEFT, speed);
  ledcWrite(CH_RIGHT, speed);
}

void brushOn(int speed) {
  digitalWrite(BRUSH_IN1, HIGH);
  digitalWrite(BRUSH_IN2, LOW);
  ledcWrite(CH_BRUSH, speed);
}

void stopAll() {
  ledcWrite(CH_LEFT, 0);
  ledcWrite(CH_RIGHT, 0);
  ledcWrite(CH_BRUSH, 0);
}

void loop() {
  Serial.println("Moving Forward + Brush ON");
  moveForward(200);
  brushOn(200);
  delay(5000);

  Serial.println("Stopping...");
  stopAll();
  delay(2000);
}
