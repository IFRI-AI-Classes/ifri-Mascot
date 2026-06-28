#ifndef CONSTANTS_H
#define CONSTANTS_H

// ======== SERVO RANGES ========
#define SERVO_MIN_ANGLE     0      // Angle minimum (degrés)
#define SERVO_MAX_ANGLE     180    // Angle maximum (degrés)
#define SERVO_CENTER        90     // Position centrale (neutrale)

// ======== WALKING GAIT PARAMETERS ========
// Angles pour la marche bipède (en degrés, relatif au centre)

// Phase de marche : Jambe gauche
#define WALK_LEFT_LEG_FORWARD   120   // Jambe gauche en avant
#define WALK_LEFT_LEG_NEUTRAL   90    // Jambe gauche neutre
#define WALK_LEFT_LEG_BACK      60    // Jambe gauche en arrière

// Phase de marche : Jambe droite
#define WALK_RIGHT_LEG_FORWARD  60    // Jambe droite en avant
#define WALK_RIGHT_LEG_NEUTRAL  90    // Jambe droite neutre
#define WALK_RIGHT_LEG_BACK     120   // Jambe droite en arrière

// Équilibre : Pieds
#define WALK_FOOT_LEFT_TILT     70    // Pied gauche incliné (équilibre)
#define WALK_FOOT_CENTER        90    // Pied neutre
#define WALK_FOOT_RIGHT_TILT    110   // Pied droit incliné (équilibre)

// ======== TIMING ========
#define WALK_STEP_DURATION      500   // Durée d'un pas en millisecondes
#define WALK_PHASE_COUNT        4     // Nombre de phases dans le cycle de marche

// Vitesse de marche (délai entre chaque étape)
#define WALK_SPEED_SLOW         800   // Marche lente
#define WALK_SPEED_NORMAL       500   // Marche normale
#define WALK_SPEED_FAST         250   // Marche rapide

// ======== ROLLING PARAMETERS ========
// Roulis : Le robot bascule sur les côtés pour progresser
#define ROLL_ANGLE_MAX          45    // Angle maximum de basculement
#define ROLL_SPEED              300   // Vitesse de roulis

// ======== TURNING ========
#define TURN_STEP_DURATION      600   // Durée d'un virage

// ======== OBSTACLE AVOIDANCE ========
#define OBSTACLE_DISTANCE_MIN   20    // Distance minimale avant obstacle (cm)
#define OBSTACLE_DISTANCE_STOP  10    // Distance d'arrêt d'urgence (cm)

// ======== BATTERY ========
#define BATTERY_LOW_VOLTAGE     3.0   // Tension batterie faible (V)
#define BATTERY_WARNING_VOLTAGE 3.3   // Tension d'avertissement (V)
#define BATTERY_MAX_VOLTAGE     4.2   // Tension maximale (V pour Li-Po)

#endif
