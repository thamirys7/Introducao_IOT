#include "WiFi.h"
#include <WebServer.h>
#include "DHT.h"
#include <ArduinoJson.h>

const char *ssid = "SSID";
const char *password = "PASSWORD";

const int LDR_PIN = 1;
const int LED_VENTILADOR = 13;   // LED simulador do ventilador (verde)
const int LED_REFLETOR = 12;     // LED simulador do refletor (azul)
const int LED_EXAUSTOR = 14;     // LED simulador do exaustor (vermelho)
const int DHT_PIN = 15;

int ldr;
int temperatura;
int umidade;
int velocidadeVentilador = 0; 
int intensidadeRefletor = 0; 
int potenciaExaustor = 0;     

WebServer server(80);
DHT dht(DHT_PIN, DHT11);

void setup() {
  Serial.begin(9600);
  dht.begin();


  pinMode(LED_VENTILADOR, OUTPUT);
  pinMode(LED_REFLETOR, OUTPUT);
  pinMode(LED_EXAUSTOR, OUTPUT);

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
  server.on("/refletor", HTTP_POST, handle_refletor);
  server.on("/exaustor", HTTP_POST, handle_exaustor);
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

  // Controla a velocidade do ventilador
  if (velocidadeVentilador > 0) {
    analogWrite(LED_VENTILADOR, velocidadeVentilador);
  } else {
    digitalWrite(LED_VENTILADOR, LOW);
  }

  // Controla a intensidade do refletor
   if (intensidadeRefletor > 0) {
    analogWrite(LED_REFLETOR, intensidadeRefletor);
  } else {
    digitalWrite(LED_REFLETOR, LOW);
  }

  // Controla potência do exaustor
  if (potenciaExaustor > 0) {
    analogWrite(LED_EXAUSTOR, potenciaExaustor);
  } else {
    digitalWrite(LED_EXAUSTOR, LOW);
  }
}


// GET /monitoramento
void handle_monitoramento() {
  StaticJsonDocument<200> doc;
  doc["luz"] = ldr;
  doc["temperatura"] = temperatura;
  doc["umidade"] = umidade;
  doc["ventilador"] = velocidadeVentilador;
  doc["refletor"] = intensidadeRefletor;
  doc["exaustor"] = potenciaExaustor;

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
  } else if (server.hasArg("plain")) {
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

    server.send(200, "text/plain", "Velocidade alterada");
    return;
  }

  server.send(400, "text/plain", "Erro...");
}

// POST /refletor
void handle_refletor() {
  addCORSHeaders();

  int novoValor = -1;

  if (server.hasArg("intensidade")) {
    novoValor = server.arg("intensidade").toInt();
  } else if (server.hasArg("plain")) {
    String body = server.arg("plain");
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, body);

    if (!error && doc.containsKey("intensidade")) {
      novoValor = doc["intensidade"];
    }
  }

  if (novoValor >= 0) {
    novoValor = constrain(novoValor, 0, 255);
    intensidadeRefletor = novoValor;

    server.send(200, "text/plain", "Intensidade alterada");
    return;
  }

  server.send(400, "text/plain", "Erro...");
}

// POST /exaustor
void handle_exaustor() {
  addCORSHeaders();

  int novoValor = -1;

  if (server.hasArg("potencia")) {
    novoValor = server.arg("potencia").toInt();
  } else if (server.hasArg("plain")) {
    String body = server.arg("plain");
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, body);

    if (!error && doc.containsKey("potencia")) {
      novoValor = doc["potencia"];
    }
  }

  if (novoValor >= 0) {
    novoValor = constrain(novoValor, 0, 255);
    potenciaExaustor = novoValor;

    server.send(200, "text/plain", "Exaustor alterado");
    return;
  }

  server.send(400, "text/plain", "Erro...");
}


void handle_not_found() {
  addCORSHeaders();
  server.send(404, "text/plain", "Página não encontrada");
}


void addCORSHeaders() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
}
