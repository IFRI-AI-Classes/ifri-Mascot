/*
 * OTTO NINJA - Robot Mascotte AIoT
 * Club AI & IoT IFRI
 * 
 * Firmware principal pour ESP32
 * Marche bipède avec 4 servos + capteurs
 */

#include "src/MotionEngine.h"
#include "src/ServoDriver.h"
#include "src/Constants.h"
#include "src/Pinout.h"

// ======== VARIABLES GLOBALES ========
unsigned long lastCommandTime = 0;
const unsigned long COMMAND_TIMEOUT = 5000; // Timeout après 5 secondes d'inactivité

// ======== SETUP ========

void setup() {
  Serial.begin(115200);
  delay(1000); // Laisser le temps au moniteur série de se connecter
  
  Serial.println("\n\n");
  Serial.println("╔════════════════════════════════════════╗");
  Serial.println("║      OTTO NINJA - AIoT IFRI            ║");
  Serial.println("║  Robot Mascotte - Initialisation...    ║");
  Serial.println("╚════════════════════════════════════════╝");
  Serial.println();
  
  // Initialiser le moteur de mouvement (servos, etc.)
  motionEngineInit();
  
  Serial.println();
  Serial.println("✓ Système prêt!");
  Serial.println("Tapez des commandes:");
  Serial.println("  'f' : Marche avant");
  Serial.println("  'b' : Marche arrière");
  Serial.println("  'l' : Virage gauche");
  Serial.println("  'r' : Virage droite");
  Serial.println("  'r' : Roulis");
  Serial.println("  's' : Arrêt");
  Serial.println("  'g' : Séquence d'accueil");
  Serial.println("  'd' : Danse");
  Serial.println();
}

// ======== MAIN LOOP ========

void loop() {
  // Mettre à jour les mouvements en cours
  motionEngineUpdate();
  
  // Vérifier les commandes série
  handleSerialCommands();
  
  // Timeout: arrêter après 5 secondes d'inactivité
  if (millis() - lastCommandTime > COMMAND_TIMEOUT && motionIsMoving()) {
    // motionStop(); // Décommenter pour activation
  }
  
  // Petite pause pour ne pas surcharger le processeur
  delay(10);
}

// ======== COMMAND HANDLING ========

void handleSerialCommands() {
  if (Serial.available()) {
    char command = Serial.read();
    lastCommandTime = millis();
    
    switch (command) {
      case 'f':
      case 'F':
        Serial.println("[CMD] Marche avant!");
        motionStart(MOTION_WALK_FORWARD, WALK_SPEED_NORMAL);
        break;
      
      case 'b':
      case 'B':
        Serial.println("[CMD] Marche arrière!");
        motionStart(MOTION_WALK_BACKWARD, WALK_SPEED_NORMAL);
        break;
      
      case 'l':
      case 'L':
        Serial.println("[CMD] Virage gauche!");
        motionStart(MOTION_TURN_LEFT, WALK_SPEED_NORMAL);
        break;
      
      case 'r':
      case 'R':
        Serial.println("[CMD] Virage droite!");
        motionStart(MOTION_TURN_RIGHT, WALK_SPEED_NORMAL);
        break;
      
      case 'o':
      case 'O':
        Serial.println("[CMD] Roulis!");
        motionStart(MOTION_ROLL, WALK_SPEED_NORMAL);
        break;
      
      case 's':
      case 'S':
        Serial.println("[CMD] Arrêt!");
        motionStop();
        break;
      
      case 'g':
      case 'G':
        Serial.println("[CMD] Séquence d'accueil!");
        motionGreetSequence();
        break;
      
      case 'd':
      case 'D':
        Serial.println("[CMD] Danse!");
        motionDanceSequence();
        break;
      
      case '1':
        Serial.println("[CMD] Vitesse lente");
        motionStop();
        motionStart(MOTION_WALK_FORWARD, WALK_SPEED_SLOW);
        break;
      
      case '2':
        Serial.println("[CMD] Vitesse normale");
        motionStop();
        motionStart(MOTION_WALK_FORWARD, WALK_SPEED_NORMAL);
        break;
      
      case '3':
        Serial.println("[CMD] Vitesse rapide");
        motionStop();
        motionStart(MOTION_WALK_FORWARD, WALK_SPEED_FAST);
        break;
      
      default:
        if (command >= 32 && command < 127) { // Caractères imprimables
          Serial.print("[CMD] Commande inconnue: ");
          Serial.println(command);
        }
        break;
    }
  }
}

// ======== UTILITY FUNCTIONS ========

void printStatus() {
  Serial.println("\n╔═══ STATUS ═══╗");
  Serial.print("Mode: ");
  Serial.println(motionGetCurrentMode());
  Serial.print("En mouvement: ");
  Serial.println(motionIsMoving() ? "OUI" : "NON");
  Serial.println("╚═════════════╝\n");
}
