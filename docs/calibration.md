# Guide de calibration materielle

Ce guide sert a valider le robot au labo sans improviser. Chaque calibration
doit etre faite une seule variable a la fois, avec notes des valeurs finales.

## Securite avant tests

- Retirer les roues ou surelever le robot pour les premiers essais moteur.
- Alimenter les servos avec une alimentation adaptee, pas uniquement l'USB de
  l'ESP32.
- Relier les masses: GND alimentation servos et GND ESP32 doivent etre communs.
- Ne jamais envoyer 5 V directement sur un GPIO ESP32.
- Pour le HC-SR04 en 5 V, abaisser `ECHO` avec un diviseur de tension.
- Pour la batterie, utiliser un pont diviseur avant l'ADC.

## Calibration des servos 360

Un servo 360 ne recoit pas un angle physique. La valeur envoyee est une commande
de vitesse et de sens:

| Commande | Effet attendu |
| ---: | --- |
| `90` | arret |
| `> 90` | rotation dans un sens |
| `< 90` | rotation dans l'autre sens |

Sur certains servos, le vrai arret n'est pas exactement `90`. Il peut etre `88`,
`91`, `93`, etc.

Procedure:

1. Surelever le robot.
2. Envoyer la commande `stop`.
3. Observer chaque roue.
4. Si une roue bouge encore, ajuster le neutre correspondant.
5. Tester une valeur legerement au-dessus du neutre.
6. Tester une valeur legerement en-dessous du neutre.
7. Noter le sens reel de chaque roue.

Dans la branche `feature/continuous-wheel-drive`, les valeurs de depart sont:

```cpp
#define WHEEL_SERVO_STOP     90
#define WHEEL_SERVO_FORWARD  125
#define WHEEL_SERVO_BACKWARD 55
```

Si une roue tourne dans le mauvais sens, il ne faut pas recabler au hasard:
inverser d'abord les commandes logicielles de cette roue.

## Calibration des supports/pattes

Les deux servos de supports sont des servos positionnels classiques. Ici les
valeurs sont bien des angles physiques.

Objectif:

- mode repos: les disques/pattes sont poses a plat contre le sol;
- mode roulage: les disques/pattes sont souleves et laissent les roues porter
  le robot.

Dans la branche `feature/continuous-wheel-drive`, valeurs de depart:

```cpp
#define SUPPORT_LEFT_REST_ANGLE   90
#define SUPPORT_RIGHT_REST_ANGLE  90
#define SUPPORT_LEFT_DRIVE_ANGLE  35
#define SUPPORT_RIGHT_DRIVE_ANGLE 145
```

Procedure:

1. Tester `stop`: verifier la position repos.
2. Tester `forward`: verifier la position roulage.
3. Ajuster par pas de 5 degres.
4. Quand la position est proche, ajuster par pas de 1 ou 2 degres.
5. Noter les valeurs finales dans ce document ou dans un compte rendu.

## Calibration HC-SR04

Pins actuels:

| Signal | GPIO |
| --- | ---: |
| TRIG | 15 |
| ECHO | 14 |

Seuils:

```cpp
#define OBSTACLE_DISTANCE_MIN  20
#define OBSTACLE_DISTANCE_STOP 10
#define OBSTACLE_AVOID_TRIGGER_CM 15
```

Procedure:

1. Placer un obstacle a 30 cm, verifier `/api/status`.
2. Placer un obstacle a 20 cm, verifier `obstacle`.
3. Placer un obstacle a 10 cm, verifier la zone d'arret.
4. Si la mesure saute, verifier l'alimentation et le diviseur sur `ECHO`.

Evitement:

- le declenchement automatique commence a `OBSTACLE_AVOID_TRIGGER_CM`;
- la sequence actuelle est: stop, recul, rotation, recuperation;
- un appui manuel sur `stop` annule la sequence en cours;
- le son obstacle sera branche quand `AudioDriver` sera finalise.

## Calibration batterie

Pin actuel:

| Signal | GPIO |
| --- | ---: |
| ADC batterie | 35 |

Constantes:

```cpp
#define BATTERY_MIN_VOLTAGE      3.0f
#define BATTERY_LOW_VOLTAGE      3.3f
#define BATTERY_CRITICAL_VOLTAGE 3.1f
#define BATTERY_MAX_VOLTAGE      4.2f
#define BATTERY_DIVIDER_RATIO    2.0f
#define BATTERY_SAFETY_PERCENT   20
#define BATTERY_CRITICAL_PERCENT 5
```

Procedure:

1. Mesurer la batterie au multimetre.
2. Lire `batteryVoltage` dans `/api/status`.
3. Si la valeur firmware est fausse, ajuster `BATTERY_DIVIDER_RATIO`.
4. Ne pas descendre sous la tension minimale recommandee de la batterie.

Mode securite:

- sous `BATTERY_SAFETY_PERCENT`, le firmware limite la vitesse;
- sous `BATTERY_CRITICAL_PERCENT` ou tension critique, l'etat passe en critique;
- le son d'alerte sera branche quand `AudioDriver` sera finalise.

## Checklist de validation mouvement

| Test | Resultat attendu | Statut |
| --- | --- | --- |
| `stop` | roues arretees, supports en repos | A faire |
| `forward` | supports en roulage, robot avance | A faire |
| `backward` | supports en roulage, robot recule | A faire |
| `left` | rotation ou virage gauche coherent | A faire |
| `right` | rotation ou virage droite coherent | A faire |
| perte controleur | robot s'arrete apres timeout | A faire |
| second telephone | controle refuse | A faire |

## Notes de calibration

Remplir apres essais:

| Parametre | Valeur finale | Commentaire |
| --- | --- | --- |
| roue gauche stop |  |  |
| roue droite stop |  |  |
| roue gauche avant |  |  |
| roue droite avant |  |  |
| support gauche repos |  |  |
| support droit repos |  |  |
| support gauche roulage |  |  |
| support droit roulage |  |  |
| ratio batterie |  |  |
