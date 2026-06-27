// pour tester les driver des servos moteur 
// A suppprimer plus tard

#include <Arduino.h>
#include "src/ServoDriver.h"

void setup() {
    Serial.begin(115200);
    Serial.println("=== TEST SERVOS OOTO NINJA ===");
    
    servoDriverInit();
    Serial.println("✅ Driver initialisé");

    // Position neutre
    servoDriverSetAngle(0, 90);
    servoDriverSetAngle(1, 90);
    servoDriverSetAngle(2, 90);
    servoDriverSetAngle(3, 90);
    servoDriverUpdate();
    
    Serial.println("✅ Servos à 90°");
    delay(1000);
}

void loop() {
    Serial.println("--- Cycle ---");
    
    // Servo 0 (Jambe gauche)
    Serial.println("Servo 0 → 0°");
    servoDriverSetAngle(0, 0);
    servoDriverUpdate();
    delay(1500);
    
    Serial.println("Servo 0 → 180°");
    servoDriverSetAngle(0, 180);
    servoDriverUpdate();
    delay(1500);
    
    Serial.println("Servo 0 → 90°");
    servoDriverSetAngle(0, 90);
    servoDriverUpdate();
    delay(1500);
    
    // Servo 1 (Jambe droite)
    Serial.println("Servo 1 → 0°");
    servoDriverSetAngle(1, 0);
    servoDriverUpdate();
    delay(1500);
    
    Serial.println("Servo 1 → 180°");
    servoDriverSetAngle(1, 180);
    servoDriverUpdate();
    delay(1500);
    
    Serial.println("Servo 1 → 90°");
    servoDriverSetAngle(1, 90);
    servoDriverUpdate();
    delay(1500);
    
    // Servo 2 (Pied gauche)
    Serial.println("Servo 2 → 0°");
    servoDriverSetAngle(2, 0);
    servoDriverUpdate();
    delay(1500);
    
    Serial.println("Servo 2 → 180°");
    servoDriverSetAngle(2, 180);
    servoDriverUpdate();
    delay(1500);
    
    Serial.println("Servo 2 → 90°");
    servoDriverSetAngle(2, 90);
    servoDriverUpdate();
    delay(1500);
    
    // Servo 3 (Pied droit)
    Serial.println("Servo 3 → 0°");
    servoDriverSetAngle(3, 0);
    servoDriverUpdate();
    delay(1500);
    
    Serial.println("Servo 3 → 180°");
    servoDriverSetAngle(3, 180);
    servoDriverUpdate();
    delay(1500);
    
    Serial.println("Servo 3 → 90°");
    servoDriverSetAngle(3, 90);
    servoDriverUpdate();
    delay(1500);
    
    // Tous ensemble
    Serial.println("TOUS → 0°");
    servoDriverSetAngle(0, 0);
    servoDriverSetAngle(1, 0);
    servoDriverSetAngle(2, 0);
    servoDriverSetAngle(3, 0);
    servoDriverUpdate();
    delay(2000);
    
    Serial.println("TOUS → 180°");
    servoDriverSetAngle(0, 180);
    servoDriverSetAngle(1, 180);
    servoDriverSetAngle(2, 180);
    servoDriverSetAngle(3, 180);
    servoDriverUpdate();
    delay(2000);
    
    Serial.println("TOUS → 90°");
    servoDriverSetAngle(0, 90);
    servoDriverSetAngle(1, 90);
    servoDriverSetAngle(2, 90);
    servoDriverSetAngle(3, 90);
    servoDriverUpdate();
    delay(2000);
}