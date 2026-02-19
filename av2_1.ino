#include "WiFi.h"
#include <WebServer.h>
#include "DHT.h"
#include <ArduinoJson.h>

const char *ssid = "SSID";
const char *password = "PASSWORD";

const int LDR_PIN = 1;
const int LED_PIN = 13;   // LED simulador do ventilador
const int DHT_PIN = 15;

int ldr;
int temperatura;
int umidade;
int velocidadeVentilador = 0; // valor entre 0 e 255
int refletor = 0;
int exaustor = 0;

WebServer server(80);
DHT dht(DHT_PIN, DHT11);

void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(LED_PIN, OUTPUT);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Conectando...");
    delay(500);
  }

  Serial.println("Conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  // Endpoints
  server.on("/monitoramento", HTTP_GET, handle_monitoramento);
  server.on("/ventilador", HTTP_POST, handle_ventilador);
  server.onNotFound(handle_not_found);

  server.begin();
  Serial.println("Servidor ativo");
}

void loop() {
  server.handleClient();

  int temp_temperatura = dht.readTemperature();
  int temp_umidade = dht.readHumidity();
  if (!isnan(temp_temperatura)) temperatura = temp_temperatura;
  if (!isnan(temp_umidade)) umidade = temp_umidade;

  ldr = analogRead(LDR_PIN);

  // Liga ou desliga o ventilador conforme velocidade
  if (velocidadeVentilador > 0) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
}


// GET /monitoramento
void handle_monitoramento() {
  StaticJsonDocument<200> doc;
  doc["luz"] = ldr;
  doc["temperatura"] = temperatura;
  doc["umidade"] = umidade;
  doc["ventilador"] = velocidadeVentilador;
  doc["refletor"] = refletor;
  doc["exaustor"] = exaustor;

  String json;
  serializeJson(doc, json);

  addCORSHeaders();
  server.send(200, "application/json", json);
}

// POST /ventilador
void handle_ventilador() {
  addCORSHeaders();

  int novoValor = -1;


  if (server.hasArg("velocidade")) {
    novoValor = server.arg("velocidade").toInt();
  }

  else if (server.hasArg("plain")) {
    String body = server.arg("plain");
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, body);

    if (!error && doc.containsKey("velocidade")) {
      novoValor = doc["velocidade"];
    }
  }

  if (novoValor >= 0) {
    novoValor = constrain(novoValor, 0, 255);
    velocidadeVentilador = novoValor;


    if (velocidadeVentilador > 0) {
      digitalWrite(LED_PIN, HIGH);
    } else {
      digitalWrite(LED_PIN, LOW);
    }

    server.send(200, "application/json", "{\"msg\":\"Velocidade alterada\"}");
    return;
  }

  server.send(400, "application/json", "{\"erro\":\"Formato inválido\"}");
}


void handle_not_found() {
  addCORSHeaders();
  server.send(404, "application/json", "{\"erro\":\"Página não encontrada\"}");
}


void addCORSHeaders() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
}
