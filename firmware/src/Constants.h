#ifndef CONSTANTS_H
#define CONSTANTS_H

// ======== WIFI / WEB SERVER ========
#ifndef WIFI_SSID
#define WIFI_SSID "IFRI-Mascot"
#endif

#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD "ifri-mascot"
#endif

#ifndef WIFI_CHANNEL
#define WIFI_CHANNEL 6
#endif

#ifndef WIFI_MAX_CLIENTS
#define WIFI_MAX_CLIENTS 4
#endif

#ifndef WEB_SERVER_PORT
#define WEB_SERVER_PORT 80
#endif

#ifndef STATS_INTERVAL_MS
#define STATS_INTERVAL_MS 1000
#endif

#ifndef AUDIO_VOLUME_MAX
#define AUDIO_VOLUME_MAX 100
#endif

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
#define BATTERY_MIN_VOLTAGE      3.0f  // 0% pour Li-Po 1S
#define BATTERY_LOW_VOLTAGE      3.3f  // Alerte faible
#define BATTERY_CRITICAL_VOLTAGE 3.1f  // Alerte critique
#define BATTERY_MAX_VOLTAGE      4.2f  // 100% pour Li-Po 1S
#define BATTERY_DIVIDER_RATIO    2.0f  // R1=R2: tension batterie = tension ADC * 2
#define BATTERY_SAMPLE_COUNT     16
#define BATTERY_UPDATE_MS        500

#endif
