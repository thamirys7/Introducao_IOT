// Importando bibliotecas e arquivos (#include)
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Pino conectado ao DHT11 (#define cria constantes ou macros)
#define DHTPIN 15
#define DHTTYPE DHT11

// Inicializa o sensor DHT
DHT dht(DHTPIN, DHTTYPE);

// Parâmetros do display OLED (128x64)
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);
  dht.begin();

  // Inicializa o display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Falha ao inicializar o display OLED"));
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Iniciando sensor DHT11...");
  display.display();
  delay(2000);
}

void loop() {
  float temperatura = dht.readTemperature();
  float umidade = dht.readHumidity();

  // Verifica se a leitura falhou
  if (isnan(temperatura) || isnan(umidade)) {
    Serial.println("Falha na leitura do DHT11");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Erro na leitura");
    display.display();
  } else {
    
    // Imprime na serial
    Serial.printf("Temperatura: %.1f °C \t Umidade: %.0f%%\n", temperatura, umidade);

    // Exibe no display
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Leitura DHT11:");
    display.printf("Temperatura: %.1f C\n", temperatura);
    display.printf("Umidade: %.0f %%\n", umidade);
    display.display();
  }

  delay(1000);
}