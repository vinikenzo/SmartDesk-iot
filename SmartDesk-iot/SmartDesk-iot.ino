/********************************************************************
 * Projeto: SmartDesk - Envio de Dados MQTT com ESP32 (WiFi + NTP)
 * Placa: DOIT ESP32 DEVKIT V1
 * 
 * Descrição:
 * Este projeto conecta o ESP32 ao Wi-Fi e a um Broker MQTT.
 * A cada ciclo, envia dados simulados de ocupação de 3 mesas:
 *  - desk01
 *  - desk02
 *  - desk03
 ********************************************************************/

//----------------------------------------------------------
// Bibliotecas nativas ESP32

#include <WiFi.h>
#include <time.h>

//----------------------------------------------------------
// Bibliotecas externas 

#include <PubSubClient.h>
#include <ArduinoJson.h>

//----------------------------------------------------------

#define boardLED 2  

// Wi-Fi
const char* SSID      = "Wokwi-GUEST";
const char* PASSWORD  = "";

// MQTT Broker
const char* BROKER_MQTT  = "broker.hivemq.com";
const int   BROKER_PORT  = 1883;

//----------------------------------------------------------

WiFiClient espClient;
PubSubClient MQTT(espClient);
char buffer[300]; 

//----------------------------------------------------------

void setup() {
    Serial.begin(115200);
    pinMode(boardLED, OUTPUT);
    digitalWrite(boardLED, LOW);

    initWiFi();
    initNTP();
    initMQTT();
}

//----------------------------------------------------------


void loop() {

    verificaConexoesWiFiEMQTT();

    struct tm timeinfo;
    getLocalTime(&timeinfo);

    char data[11];
    char hora[9];
    strftime(data, sizeof(data), "%Y-%m-%d", &timeinfo);
    strftime(hora, sizeof(hora), "%H:%M:%S", &timeinfo);

    // Mesas monitoradas
    const char* desks[3] = {"desk01", "desk02", "desk03"};

    // Envia dados de cada mesa
    for (int i = 0; i < 3; i++) {

        bool ocupado = random(0, 2); 

        StaticJsonDocument<300> doc;

        doc["site"]       = "salaA";
        doc["desk_id"]    = desks[i];
        doc["Data"]       = data;
        doc["Hora"]       = hora;
        doc["occupied"]   = ocupado;
        doc["ip"]         = WiFi.localIP().toString();

        serializeJson(doc, buffer);

        Serial.println(buffer);

        String topico = String("smartdesk/salaA/") + desks[i] + "/sensor";

        MQTT.publish(topico.c_str(), buffer);

        delay(500);
    }

  
    piscaLed();

    delay(5000);
}

//----------------------------------------------------------

void initWiFi() {
    WiFi.begin(SSID, PASSWORD);
    Serial.print("Conectando ao Wi-Fi");

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }

    Serial.println("\nWi-Fi conectado!");
    Serial.print("IP Local: ");
    Serial.println(WiFi.localIP());
}

void reconectaWiFi() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Reconectando Wi-Fi...");
        initWiFi();
    }
}

//----------------------------------------------------------

void initNTP() {
    configTime(-3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

    struct tm timeinfo;
    while (!getLocalTime(&timeinfo)) {
        Serial.println("Aguardando sincronização NTP...");
        delay(1000);
    }

    Serial.println("Horário sincronizado via NTP.");
}

//----------------------------------------------------------

void initMQTT() {
    MQTT.setServer(BROKER_MQTT, BROKER_PORT);

    while (!MQTT.connected()) {
        Serial.println("Conectando ao Broker MQTT...");

        if (MQTT.connect("SmartDeskSender")) {
            Serial.println("Conectado ao Broker!");
        } else {
            Serial.print("Falha MQTT. Estado: ");
            Serial.println(MQTT.state());
            delay(2000);
        }
    }
}

void verificaConexoesWiFiEMQTT() {
    reconectaWiFi();
    if (!MQTT.connected()) {
        initMQTT();
    }
    MQTT.loop();
}

//----------------------------------------------------------

void piscaLed() {
    digitalWrite(boardLED, HIGH);
    delay(300);
    digitalWrite(boardLED, LOW);
}
