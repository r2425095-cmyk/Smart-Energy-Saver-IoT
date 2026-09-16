const int ldrPin = A0;
const int tempPin = A1;

const int fanPin = 3;

const int occupiedLightPin = 8;   // Main room light
const int ldrLightPin = 9;        // LDR-controlled light

const int lightThreshold = 300;
const float tempThreshold = 20.0;

bool occupied = false;

// Main room light state
bool occupiedLightState = true;   
// Assume light starts ON

void setup() {
  Serial.begin(9600);

  pinMode(fanPin, OUTPUT);
  pinMode(occupiedLightPin, OUTPUT);
  pinMode(ldrLightPin, OUTPUT);

  // Initial light state
  digitalWrite(occupiedLightPin, HIGH);
}

void loop() {

  // =========================================
  // 1. Read occupancy data from Python
  // =========================================
  if (Serial.available() > 0) {
    char data = Serial.read();

    if (data == '1') {
      occupied = true;
    }

    else if (data == '0') {
      occupied = false;
    }
  }

  // =========================================
  // 2. Read sensors
  // =========================================
  int ldrValue = analogRead(ldrPin);

  int voltage = analogRead(tempPin);
  float tempO =voltage* (5.0 / 1023.0);
  float tempC = tempO * 100;

  // =========================================
  // DEBUG OUTPUT

  // =========================================
  // LIGHT 1: Occupancy-controlled light
  // =========================================

  // If room becomes unoccupied,
  // force the light OFF
  if (!occupied) {
    occupiedLightState = false;
  }

  // If room becomes occupied again,
  // restore light ON
  else {
    occupiedLightState = true;
  }

  digitalWrite(occupiedLightPin, occupiedLightState);

  // =========================================
  // LIGHT 2: LDR-only light
  // =========================================

  if (ldrValue < lightThreshold) {
    digitalWrite(ldrLightPin, HIGH);
  }
  else {
    digitalWrite(ldrLightPin, LOW);
  }

  // =========================================
  // FAN: Occupancy + Temperature
  // =========================================

  if (occupied && tempC > tempThreshold) {
    digitalWrite(fanPin, HIGH);
  }
  else {
    digitalWrite(fanPin, LOW);
  }
  // =========================================
// SEND DATA TO PYTHON (Streamlit)
// Format: T, L, F
// =========================================

  Serial.print(tempC);
  Serial.print(",");
  Serial.print(ldrValue);
  Serial.print(",");
  Serial.println(digitalRead(fanPin)); 
  
  delay(5000);
}