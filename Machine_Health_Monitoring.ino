#include <DHT.h>

// ---------------- DHT SENSOR ----------------
#define DHTPIN PA1
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// ---------------- OUTPUT PINS ----------------
#define RELAY_PIN   PA4
#define MOTOR_PWM   PA8
#define VIB_PIN     PA0
#define BUZZER      PB0

// ---------------- TEMPERATURE THRESHOLDS ----------------
float tempLow = 25;
float tempHigh = 30;
float tempCritical = 35;

// ---------------- VIBRATION THRESHOLDS ----------------
int vibLow = 300;
int vibMedium = 600;
int vibHigh = 900;

// ---------------- STATE VARIABLES ----------------
float prevTemp = 0;

int motorPWM = 0;

// buzzer control
unsigned long prevMillis = 0;
bool buzzerState = LOW;
int beepInterval = 0;

// ---------------- NEW DIAGNOSTIC VARIABLES ----------------
String systemStatus = "NORMAL";
String tempTrend = "STABLE";
String motorLoad = "LOW";
int safetyScore = 100;

// ---------------- SETUP ----------------
void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(MOTOR_PWM, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(VIB_PIN, INPUT);

  digitalWrite(RELAY_PIN, HIGH);
  digitalWrite(BUZZER, LOW);
  analogWrite(MOTOR_PWM, 0);
}

// ---------------- MAIN LOOP ----------------
void loop() {

  float temp = dht.readTemperature();
  int vibValue = analogRead(VIB_PIN);

  if (isnan(temp)) {
    Serial.println("DHT Error!");
    return;
  }

  // =====================================================
  // 1. TEMPERATURE CONTROL
  // =====================================================
  bool fanState = false;
  String tempStatus = "NORMAL";

  if (temp < tempLow) {
    fanState = false;
  }
  else if (temp < tempHigh) {
    fanState = true;
    tempStatus = "MODERATE TEMP";
  }
  else if (temp < tempCritical) {
    fanState = true;
    tempStatus = "HIGH TEMP";
  }
  else {
    tempStatus = "CRITICAL TEMP";
  }

  digitalWrite(RELAY_PIN, fanState ? LOW : HIGH);

  // Predictive alert
  if ((temp - prevTemp) > 2) {
    Serial.print(" | ALERT: TEMP RISING FAST!");
  }

  // =====================================================
  // 2. VIBRATION CONTROL
  // =====================================================
  if (vibValue < vibLow) {
    motorPWM = 220;
    beepInterval = 0;
    digitalWrite(BUZZER, LOW);
  }
  else if (vibValue < vibMedium) {
    motorPWM = 160;
    beepInterval = 700;
  }
  else if (vibValue < vibHigh) {
    motorPWM = 90;
    beepInterval = 200;
  }
  else {
    motorPWM = 0;
    analogWrite(MOTOR_PWM, 0);
    digitalWrite(BUZZER, HIGH);

    Serial.print("VIB=");
    Serial.print(vibValue);
    Serial.println(" | CRITICAL VIBRATION 🚨");

    return;
  }

  analogWrite(MOTOR_PWM, motorPWM);

  // =====================================================
  // 3. EMERGENCY SHUTDOWN
  // =====================================================
  if (temp >= tempCritical && vibValue >= vibHigh) {
    digitalWrite(RELAY_PIN, HIGH);
    analogWrite(MOTOR_PWM, 0);
    digitalWrite(BUZZER, HIGH);

    Serial.println("EMERGENCY SHUTDOWN 🚨");
    return;
  }

  // =====================================================
  // 4. BUZZER HANDLING
  // =====================================================
  handleBuzzer();

  // =====================================================
  // 5. NEW: SYSTEM DIAGNOSTICS
  // =====================================================

  // System status
  if (temp >= tempCritical || vibValue >= vibHigh) {
    systemStatus = "CRITICAL";
    safetyScore = 20;
  }
  else if (temp >= tempHigh || vibValue >= vibMedium) {
    systemStatus = "WARNING";
    safetyScore = 60;
  }
  else {
    systemStatus = "NORMAL";
    safetyScore = 100;
  }

  // Temperature trend
  if (temp - prevTemp > 0.5) {
    tempTrend = "RISING";
  }
  else if (temp - prevTemp < -0.5) {
    tempTrend = "FALLING";
  }
  else {
    tempTrend = "STABLE";
  }

  // Motor load
  if (motorPWM > 180) motorLoad = "HIGH";
  else if (motorPWM > 100) motorLoad = "MEDIUM";
  else motorLoad = "LOW";

  // store temp
  prevTemp = temp;

  // =====================================================
  // 6. SERIAL OUTPUT (IMPROVED)
  // =====================================================
  Serial.print("Temp: ");
  Serial.print(temp);

  Serial.print(" | Vib: ");
  Serial.print(vibValue);

  Serial.print(" | Fan: ");
  Serial.print(fanState ? "ON" : "OFF");

  Serial.print(" | TempStatus: ");
  Serial.print(tempStatus);

  Serial.print(" | PWM: ");
  Serial.print(motorPWM);

  Serial.print(" | MotorLoad: ");
  Serial.print(motorLoad);

  Serial.print(" | BuzzerInterval: ");
  Serial.print(beepInterval);

  Serial.print(" | TempTrend: ");
  Serial.print(tempTrend);

  Serial.print(" | System: ");
  Serial.print(systemStatus);

  Serial.print(" | Safety: ");
  Serial.print(safetyScore);

  Serial.print(" | BuzzerState: ");
  Serial.println(buzzerState);

  delay(300);
}

// =====================================================
// BUZZER FUNCTION
// =====================================================
void handleBuzzer() {

  if (beepInterval == 0) return;

  unsigned long currentMillis = millis();

  if (currentMillis - prevMillis >= beepInterval) {
    prevMillis = currentMillis;
    buzzerState = !buzzerState;
    digitalWrite(BUZZER, buzzerState);
  }
}