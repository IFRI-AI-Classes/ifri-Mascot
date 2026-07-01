#include "WebServer.h"
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include "Constants.h"
#include "Pinout.h"
#include "BatteryDriver.h"
#include "UltrasonicDriver.h"
#include "MotionEngine.h"

static AsyncWebServer server(WEB_SERVER_PORT);

static String currentState = "Arret";
static String currentSpeed = "Normal";
static bool isMuted = false;
static int audioVolume = 50;
static String currentMoveCmd = "stop";

static const char* motionModeToString(MotionMode mode) {
    switch (mode) {
        case MOTION_WALK_FORWARD:
            return "Avant";
        case MOTION_WALK_BACKWARD:
            return "Arriere";
        case MOTION_TURN_LEFT:
            return "Gauche";
        case MOTION_TURN_RIGHT:
            return "Droite";
        case MOTION_ROLL:
            return "Roulis";
        case MOTION_IDLE:
        default:
            return "Arret";
    }
}

static bool applyMoveCommand(const String& cmd) {
    if (cmd == "forward") {
        motionStart(MOTION_WALK_FORWARD, WALK_SPEED_NORMAL);
    } else if (cmd == "backward") {
        motionStart(MOTION_WALK_BACKWARD, WALK_SPEED_NORMAL);
    } else if (cmd == "left") {
        motionStart(MOTION_TURN_LEFT, WALK_SPEED_NORMAL);
    } else if (cmd == "right") {
        motionStart(MOTION_TURN_RIGHT, WALK_SPEED_NORMAL);
    } else if (cmd == "stop") {
        motionStop();
    } else {
        return false;
    }

    currentMoveCmd = cmd;
    currentState = motionModeToString(motionGetCurrentMode());
    currentSpeed = motionIsMoving() ? "Normal" : "Arret";
    return true;
}

void webServerInit() {
    if (!SPIFFS.begin(true)) {
        Serial.println("[WebServer] ERREUR: Impossible de monter SPIFFS");
        return;
    }
    Serial.println("[WebServer] SPIFFS monte avec succes");

    WiFi.softAP(WIFI_SSID, WIFI_PASSWORD, WIFI_CHANNEL, 0, WIFI_MAX_CLIENTS);
    IPAddress ip = WiFi.softAPIP();
    Serial.print("[WebServer] AP demarre: ");
    Serial.println(WIFI_SSID);
    Serial.print("[WebServer] IP: ");
    Serial.println(ip);

    server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

    server.on("/api/move", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (!request->hasParam("cmd")) {
            request->send(400, "application/json", "{\"status\":\"error\",\"msg\":\"param cmd manquant\"}");
            return;
        }

        String cmd = request->getParam("cmd")->value();

        Serial.print("[API] Move: ");
        Serial.println(cmd);

        if (!applyMoveCommand(cmd)) {
            request->send(400, "application/json", "{\"status\":\"error\",\"msg\":\"commande inconnue\"}");
            return;
        }

        String json = "{\"status\":\"ok\",\"cmd\":\"" + cmd + "\"}";
        request->send(200, "application/json", json);
    });

    server.on("/api/action", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (!request->hasParam("type")) {
            request->send(400, "application/json", "{\"status\":\"error\",\"msg\":\"param type manquant\"}");
            return;
        }

        String type = request->getParam("type")->value();
        String msg = "Action: " + type;

        if (type == "music") {
            msg = "Musique activee";
            motionDanceSequence();
        } else if (type == "wave") {
            msg = "Salut !";
            motionGreetSequence();
        } else if (type == "sleep") {
            msg = "Mode veille";
            motionStop();
            currentState = "Veille";
        } else if (type == "light") {
            msg = "Lumiere togglee";
        } else if (type == "auto") {
            msg = "Mode autonome";
            currentState = "Autonome";
        } else if (type == "follow") {
            msg = "Mode suivi";
            currentState = "Suivi";
        } else if (type == "avoid") {
            msg = "Evitement obstacles";
            currentState = "Evitement";
        } else if (type == "speak") {
            msg = "Parle...";
        } else if (type == "volume_up") {
            audioVolume = min(audioVolume + 20, AUDIO_VOLUME_MAX);
            msg = "Volume: " + String(audioVolume) + "%";
        } else if (type == "mute") {
            isMuted = !isMuted;
            msg = isMuted ? "Muet" : "Son active";
        }

        Serial.print("[API] Action: ");
        Serial.println(type);

        String json = "{\"status\":\"ok\",\"msg\":\"" + msg + "\"}";
        request->send(200, "application/json", json);
    });

    server.on("/api/status", HTTP_GET, [](AsyncWebServerRequest *request) {
        currentState = motionModeToString(motionGetCurrentMode());
        currentSpeed = motionIsMoving() ? "Normal" : "Arret";

        String json = "{";
        json += "\"distance\":" + String(ultrasonicDriverGetDistanceCm(), 1) + ",";
        json += "\"distanceValid\":" + String(ultrasonicDriverHasValidMeasure() ? "true" : "false") + ",";
        json += "\"obstacle\":" + String(ultrasonicDriverIsObstacleDetected() ? "true" : "false") + ",";
        json += "\"battery\":" + String(batteryGetPercent()) + ",";
        json += "\"batteryVoltage\":" + String(batteryGetVoltage(), 2) + ",";
        json += "\"batteryLow\":" + String(batteryIsLow() ? "true" : "false") + ",";
        json += "\"batteryCritical\":" + String(batteryIsCritical() ? "true" : "false") + ",";
        json += "\"state\":\"" + currentState + "\",";
        json += "\"speed\":\"" + currentSpeed + "\",";
        json += "\"move\":\"" + currentMoveCmd + "\",";
        json += "\"muted\":" + String(isMuted ? "true" : "false") + ",";
        json += "\"volume\":" + String(audioVolume);
        json += "}";

        request->send(200, "application/json", json);
    });

    server.begin();
    Serial.println("[WebServer] Serveur HTTP demarre sur le port " + String(WEB_SERVER_PORT));
}

void webServerUpdate() {
    static unsigned long lastUpdate = 0;
    if (millis() - lastUpdate > STATS_INTERVAL_MS) {
        lastUpdate = millis();
    }
}
