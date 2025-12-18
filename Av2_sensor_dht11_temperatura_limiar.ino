#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>


#define DHTPIN 15
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


#define LEDPIN 14 // LED Red
#define BOTAO_UP 2 // Botão 6 (aumenta o limiar)
#define BOTAO_DOWN 3 // Botão 5 (diminuie o limiar)


int limiar = 30;
float atual = 0;
unsigned long lastDebounce = 0;
const unsigned long debounceDelay = 50;

void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(LEDPIN, OUTPUT);
  pinMode(BOTAO_UP, INPUT_PULLUP);
  pinMode(BOTAO_DOWN, INPUT_PULLUP);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Falha ao inicializar o display OLED"));
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Iniciando sistema...");
  display.display();
  delay(1000);
}

void loop() {

  float temperatura = dht.readTemperature();

  atual = temperatura;


  if (millis() - lastDebounce > debounceDelay) {
    if (digitalRead(BOTAO_UP) == LOW) {
      limiar++;
      lastDebounce = millis();
    }
    if (digitalRead(BOTAO_DOWN) == LOW) {
      limiar--;
      if (limiar < 0) limiar = 0;
      lastDebounce = millis();
    }
  }


  if (isnan(temperatura)) {
    Serial.println("Falha na leitura do DHT11");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Erro na leitura");
    display.display();
  } else {
  
    if (atual >= limiar) {
      digitalWrite(LEDPIN, HIGH);
    } else {
      digitalWrite(LEDPIN, LOW);
    }

  
    Serial.printf("Temperatura --> Atual: %.1f °C | Limiar: %d °C\n",
                   atual, limiar);

  
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Temperatura");
    display.printf("Atual: %.1f C\n", atual);
    display.printf("Limiar: %d C\n", limiar);
    display.display();
  }

  delay(1000);
}
