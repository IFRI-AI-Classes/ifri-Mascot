# IFRI Mascot

Robot mascotte du Club AI & IoT IFRI, base Otto Ninja, pilote par ESP32.

Le projet contient le firmware ESP32, l'interface web embarquee, les drivers
materiels, les modeles 3D et la documentation d'assemblage.

## Etat logiciel

Fonctionnalites actuellement integrees dans `main`:

- interface web en WiFi AP (`IFRI-Mascot`);
- controle web des mouvements;
- verrouillage a un seul controleur actif;
- driver 4 servos;
- driver batterie ADC;
- driver HC-SR04;
- serveur SPIFFS pour l'application mobile;
- optimisation de latence des commandes.

Branche experimentale a valider au labo:

- `feature/continuous-wheel-drive`: adapte le mouvement a 2 servos roues 360 + 2 servos supports/pattes.

## Documentation utile

- [Guide technique firmware](docs/technical_firmware.md)
- [Guide de calibration materielle](docs/calibration.md)
- [Planning](docs/planning.md)
- [Roles](docs/roles.md)
- [Guide d'assemblage](hardware/assembly/assembly_guide.md)

## Commandes rapides

Compiler:

```powershell
arduino-cli compile --fqbn esp32:esp32:esp32 firmware
```

Televerser le firmware, en remplacant `COM3` par le port detecte:

```powershell
arduino-cli upload -p COM3 --fqbn esp32:esp32:esp32 firmware
```

Generer l'image SPIFFS de l'interface web:

```powershell
& 'C:\Users\PRECIEUX\Documents\ArduinoData\packages\esp32\tools\mkspiffs\0.2.3\mkspiffs.exe' -c firmware\data -b 4096 -p 256 -s 0x160000 spiffs.bin
```

Televerser SPIFFS:

```powershell
& 'C:\Users\PRECIEUX\Documents\ArduinoData\packages\esp32\tools\esptool_py\4.5.1\esptool.exe' --chip esp32 --port COM3 --baud 921600 write_flash 0x290000 .\spiffs.bin
```

## Licence

MIT License - Voir [LICENSE](LICENSE).
