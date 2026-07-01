# Guide technique firmware

Ce document decrit l'architecture logicielle du firmware ESP32, les commandes
de build/flash, les endpoints de l'interface web et les procedures de test
avant validation materielle.

## Architecture

Le firmware principal est dans `firmware/firmware.ino`. La boucle principale
appelle les modules de facon cooperative:

```cpp
servoDriverUpdate();
motionEngineUpdate();
ultrasonicDriverUpdate();
audioDriverUpdate();
batteryDriverUpdate();
webServerUpdate();
yield();
```

Le code doit rester non bloquant. Eviter les `delay()` dans les handlers web,
les drivers et les sequences appelees depuis l'interface. Les operations longues
doivent etre transformees en etats mis a jour par `millis()`.

## Modules

`ServoDriver`

- gere 4 sorties PWM via `ESP32Servo`;
- applique les angles progressivement avec `SERVO_STEP_DEGREES`;
- dans la branche experimentale `feature/continuous-wheel-drive`, une commande
  immediate est ajoutee pour les servos 360.

`MotionEngine`

- recoit les commandes de mouvement;
- dans `main`, le modele encore present est le modele historique de marche par
  phases;
- la branche `feature/continuous-wheel-drive` remplace ce modele par un mode
  roues continues: 2 servos 360 pour les roues et 2 servos positionnels pour les
  supports/pattes.

`WebServer`

- demarre l'ESP32 en point d'acces WiFi;
- sert l'interface depuis SPIFFS;
- expose les endpoints HTTP;
- verrouille le controle a une seule adresse IP active;
- libere le controle apres `CONTROL_SESSION_TIMEOUT_MS`.

`UltrasonicDriver`

- pilote le HC-SR04 en non bloquant avec interruption sur `ECHO`;
- applique un timeout;
- expose la distance, la validite de mesure et l'etat obstacle.

`BatteryDriver`

- lit l'ADC batterie;
- moyenne les mesures;
- convertit tension batterie en pourcentage;
- expose les etats faible et critique.

`AudioDriver`

- module present mais encore a completer pour les sons d'evenement.

## Pins ESP32

| Fonction | GPIO |
| --- | ---: |
| Servo 1 | 18 |
| Servo 2 | 19 |
| Servo 3 | 21 |
| Servo 4 | 22 |
| HC-SR04 ECHO | 14 |
| HC-SR04 TRIG | 15 |
| Audio LM386 | 25 |
| Batterie ADC | 35 |
| LED statut | 2 |

Important: si le HC-SR04 est alimente en 5 V, le signal `ECHO` doit etre
abaisse avant l'ESP32. L'ESP32 n'est pas tolerant 5 V sur ses GPIO.

## WiFi et controle

Parametres par defaut:

| Parametre | Valeur |
| --- | --- |
| SSID | `IFRI-Mascot` |
| Mot de passe | `ifri-mascot` |
| IP AP | `192.168.4.1` |
| Clients WiFi max | `1` |
| Timeout controle | `30 s` |

Le premier client qui appelle l'API devient le controleur. Les autres clients
recoivent une reponse HTTP `423 Locked`.

## Endpoints HTTP

`GET /api/move?cmd=...`

Commandes reconnues:

- `forward`
- `backward`
- `left`
- `right`
- `stop`

`GET /api/action?type=...`

Actions reconnues:

- `music`
- `wave`
- `sleep`
- `light`
- `auto`
- `follow`
- `avoid`
- `speak`
- `volume_up`
- `mute`

Certaines actions sont encore des placeholders cote firmware.

`GET /api/status`

Retourne notamment:

- `distance`
- `distanceValid`
- `obstacle`
- `battery`
- `batteryVoltage`
- `batteryLow`
- `batteryCritical`
- `state`
- `speed`
- `move`
- `controller`
- `locked`
- `muted`
- `volume`

## Build et flash

Identifier le port:

```powershell
arduino-cli board list
```

Compiler:

```powershell
arduino-cli compile --fqbn esp32:esp32:esp32 firmware
```

Televerser le firmware:

```powershell
arduino-cli upload -p COM3 --fqbn esp32:esp32:esp32 firmware
```

Generer SPIFFS:

```powershell
& 'C:\Users\PRECIEUX\Documents\ArduinoData\packages\esp32\tools\mkspiffs\0.2.3\mkspiffs.exe' -c firmware\data -b 4096 -p 256 -s 0x160000 spiffs.bin
```

Televerser SPIFFS:

```powershell
& 'C:\Users\PRECIEUX\Documents\ArduinoData\packages\esp32\tools\esptool_py\4.5.1\esptool.exe' --chip esp32 --port COM3 --baud 921600 write_flash 0x290000 .\spiffs.bin
```

Le firmware et SPIFFS sont deux zones differentes. Modifier `firmware/src/*`
necessite un upload firmware. Modifier `firmware/data/*` necessite un flash
SPIFFS.

## Test minimal apres flash

1. Ouvrir le moniteur serie a `115200`.
2. Verifier que l'AP `IFRI-Mascot` demarre.
3. Se connecter au WiFi depuis le telephone.
4. Ouvrir `http://192.168.4.1`.
5. Tester `stop` avant tout mouvement.
6. Tester `forward`, `backward`, `left`, `right`.
7. Verifier qu'un second telephone ne peut pas prendre le controle.
8. Verifier `/api/status` dans le navigateur.

## Regles de branches

Une branche par fonctionnalite:

- `feature/...` pour une nouvelle fonctionnalite;
- `fix/...` pour une correction;
- pas de merge dans `main` tant que la fonctionnalite n'a pas compile;
- validation materielle obligatoire pour les changements qui touchent le
  mouvement, les pins, l'alimentation ou les seuils capteurs.
