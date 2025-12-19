#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <Servo.h>
#include <DHT.h>
#include <RTClib.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// ================= CONSTANTS =================
#define DHT_PIN 2
#define DHT_TYPE DHT22
#define SERVO_PIN 9
#define SD_CS_PIN 10
#define BATTERY_PIN A0

#define FREEFALL_THRESHOLD 0.3   // g
#define FAILSAFE_TIME 2000       // ms
#define LOG_INTERVAL 1000        // ms

// ================= OBJECTS =================
DHT dht(DHT_PIN, DHT_TYPE);
RTC_DS1307 rtc;
Adafruit_MPU6050 mpu;
Servo chuteServo;
File logFile;

// ================= VARIABLES =================
bool parachuteDeployed = false;
bool released = false;
unsigned long releaseTime = 0;
unsigned long lastLogTime = 0;

// ================= FUNCTIONS =================

float readBatteryVoltage() {
  int raw = analogRead(BATTERY_PIN);
  float voltage = raw * (5.0 / 1023.0);
  voltage *= 2.0;   // voltage divider ratio (adjust if needed)
  return voltage;
}

void deployParachute() {
  if (!parachuteDeployed) {
    chuteServo.write(90);  // OPEN position
    parachuteDeployed = true;
  }
}

void logData(DateTime now, sensors_event_t a, sensors_event_t g) {
  float temp = dht.readTemperature();
  float hum  = dht.readHumidity();
  float batt = readBatteryVoltage();

  logFile.print(now.timestamp());
  logFile.print(",");

  logFile.print(a.acceleration.x); logFile.print(",");
  logFile.print(a.acceleration.y); logFile.print(",");
  logFile.print(a.acceleration.z); logFile.print(",");

  logFile.print(g.gyro.x); logFile.print(",");
  logFile.print(g.gyro.y); logFile.print(",");
  logFile.print(g.gyro.z); logFile.print(",");

  logFile.print(temp); logFile.print(",");
  logFile.print(hum); logFile.print(",");
  logFile.println(batt);

  logFile.flush();
}

// ================= SETUP =================

void setup() {
  Serial.begin(9600);
  Wire.begin();

  // Servo
  chuteServo.attach(SERVO_PIN);
  chuteServo.write(0);   // LOCKED

  // DHT
  dht.begin();

  // RTC
  if (!rtc.begin()) {
    Serial.println("RTC ERROR");
    while (1);
  }

  if (!rtc.isrunning()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // MPU6050
  if (!mpu.begin()) {
    Serial.println("MPU6050 ERROR");
    while (1);
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  // SD Card
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("SD CARD ERROR");
    while (1);
  }

  logFile = SD.open("CANSAT.CSV", FILE_WRITE);
  if (!logFile) {
    Serial.println("FILE ERROR");
    while (1);
  }

  // CSV Header
  logFile.println("Time,Ax,Ay,Az,Gx,Gy,Gz,Temp,Humidity,Battery");
  logFile.flush();

  Serial.println("SYSTEM READY");
}

// ================= LOOP =================

void loop() {
  sensors_event_t a, g, tempEvent;
  mpu.getEvent(&a, &g, &tempEvent);

  float accMag = sqrt(
    sq(a.acceleration.x) +
    sq(a.acceleration.y) +
    sq(a.acceleration.z)
  ) / 9.81;

  unsigned long nowMillis = millis();

  // ===== RELEASE DETECTION =====
  if (!released && accMag < FREEFALL_THRESHOLD) {
    released = true;
    releaseTime = nowMillis;
  }

  // ===== PARACHUTE DEPLOYMENT =====
  if (released && !parachuteDeployed) {
    if (accMag > FREEFALL_THRESHOLD) {
      deployParachute();
    }
    else if (nowMillis - releaseTime > FAILSAFE_TIME) {
      deployParachute();
    }
  }

  // ===== DATA LOGGING =====
  if (nowMillis - lastLogTime >= LOG_INTERVAL) {
    lastLogTime = nowMillis;
    DateTime now = rtc.now();
    logData(now, a, g);
  }
}
