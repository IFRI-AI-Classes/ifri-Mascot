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
#include "src/WebServer.h"

#ifndef WIFI_SSID
#define WIFI_SSID "IFRI-Mascot"
#endif

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("\n=================================");
  Serial.println("  IFRI Mascot - Demarrage");
  Serial.println("=================================\n");

  // Initialisation des modules (ordre important)
  Serial.println("[Setup] Initialisation ServoDriver...");
  servoDriverInit();

  Serial.println("[Setup] Initialisation MotionEngine...");
  motionEngineInit();

  Serial.println("[Setup] Initialisation UltrasonicDriver...");
  ultrasonicDriverInit();

  Serial.println("[Setup] Initialisation AudioDriver...");
  audioDriverInit();

  Serial.println("[Setup] Initialisation WebServer...");
  webServerInit();

  Serial.println("\n[Setup] Tous les modules initialises.");
  Serial.println("[Setup] Connectez-vous au WiFi: " + String(WIFI_SSID));
  Serial.println("[Setup] Puis ouvrez l'adresse IP affichee ci-dessus.\n");
}

void loop() {
  // Mise a jour non-bloquante de tous les modules.
  servoDriverUpdate();
  motionEngineUpdate();
  ultrasonicDriverUpdate();
  audioDriverUpdate();
  webServerUpdate();

  // Petit delai pour laisser le CPU gerer le WiFi.
  delay(1);
}
