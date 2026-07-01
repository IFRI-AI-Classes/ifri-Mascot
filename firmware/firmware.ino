/*
  IFRI Mascot - Firmware principal
  Club AI & IoT IFRI | Robot Mascotte Otto Ninja
  Licence: MIT
*/

#include "src/Constants.h"
#include "src/Pinout.h"
#include "src/ServoDriver.h"
#include "src/MotionEngine.h"
#include "src/UltrasonicDriver.h"
#include "src/AudioDriver.h"
#include "src/BatteryDriver.h"
#include "src/BatterySafety.h"
#include "src/WebServer.h"

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("\n=================================");
  Serial.println("  IFRI Mascot - Demarrage");
  Serial.println("=================================\n");

  Serial.println("[Setup] Initialisation ServoDriver...");
  servoDriverInit();

  Serial.println("[Setup] Initialisation MotionEngine...");
  motionEngineInit();

  Serial.println("[Setup] Initialisation UltrasonicDriver...");
  ultrasonicDriverInit();

  Serial.println("[Setup] Initialisation AudioDriver...");
  audioDriverInit();

  Serial.println("[Setup] Initialisation BatteryDriver...");
  batteryDriverInit();

  Serial.println("[Setup] Initialisation WebServer...");
  webServerInit();

  Serial.println("\n[Setup] Tous les modules initialises.");
  Serial.println("[Setup] Connectez-vous au WiFi: " + String(WIFI_SSID));
  Serial.println("[Setup] Puis ouvrez l'adresse IP affichee ci-dessus.\n");
}

void loop() {
  servoDriverUpdate();
  motionEngineUpdate();
  ultrasonicDriverUpdate();
  audioDriverUpdate();
  batteryDriverUpdate();
  batterySafetyUpdate();
  webServerUpdate();

  yield();
}
