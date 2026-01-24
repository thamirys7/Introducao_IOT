#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define LDRPIN 1       // Pino do LDR
#define BOTAO_TELA 2   // Botão para alternar telas
#define BOTAO_PLANTA 3 // Botão para mudar de planta

#define DHTPIN 15
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Estrutura de plantas
struct Planta {
  const char* nome;
  int temperaturaMin;
  int temperaturaMax;
  int umidadeMin;
  int umidadeMax;
  int luminosidadeMin;
  int luminosidadeMax;
};

// Lista de plantas
Planta plantas[] = {
  {"Cacto", 20, 35, 10, 40, 8000, 15000},
  {"Girassol", 15, 30, 50, 60, 10000, 20000},
  {"Roseira", 15, 25, 50, 60, 10000, 25000},
  {"Samambaia", 18, 24, 60, 80, 1000, 5000}
};

int plantaAtual = 0; // índice da planta selecionada
int tela = 0;        // 0 = condições, 1 = seleção

// Função para mostrar condições atuais
void mostrarCondicoes(float temperatura, float umidade, int luminosidade) {
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("Planta: ");
  display.println(plantas[plantaAtual].nome);

  // Temperatura
  display.print("Temperatura: ");
  display.print(temperatura);
  display.println(" C");
  display.print("(");
  display.print(plantas[plantaAtual].temperaturaMin);
  display.print("-");
  display.print(plantas[plantaAtual].temperaturaMax);
  display.print(") ");
  if (temperatura < plantas[plantaAtual].temperaturaMin) display.print("Baixa");
  else if (temperatura > plantas[plantaAtual].temperaturaMax) display.print("Alta");
  else display.print("OK");
  display.println();

  // Umidade
  display.print("Umidade: ");
  display.print(umidade);
  display.println("%");
  display.print("(");
  display.print(plantas[plantaAtual].umidadeMin);
  display.print("-");
  display.print(plantas[plantaAtual].umidadeMax);
  display.print(") ");
  if (umidade < plantas[plantaAtual].umidadeMin) display.print("Baixa");
  else if (umidade > plantas[plantaAtual].umidadeMax) display.print("Alta");
  else display.print("OK");
  display.println();

  // Luminosidade
  display.print("Luz: ");
  display.print(luminosidade);
  display.println(" lx");
  display.print("(");
  display.print(plantas[plantaAtual].luminosidadeMin);
  display.print("-");
  display.print(plantas[plantaAtual].luminosidadeMax);
  display.print(") ");
  if (luminosidade < plantas[plantaAtual].luminosidadeMin) display.print("Baixa");
  else if (luminosidade > plantas[plantaAtual].luminosidadeMax) display.print("Alta");
  else display.print("OK");
  display.println();

  display.display();
}

// Função para selecionar planta
void selecionarPlanta() {
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Selecione planta:");
  for (int i=0; i<4; i++) {
    if (i == plantaAtual) display.print("> ");
    display.println(plantas[i].nome);
  }
  display.display();
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(BOTAO_TELA, INPUT_PULLUP);
  pinMode(BOTAO_PLANTA, INPUT_PULLUP);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Falha ao inicializar o display OLED"));
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Sistema iniciado...");
  display.display();
  delay(2000);
}

void loop() {
  float temperatura = dht.readTemperature();
  float umidade = dht.readHumidity();
  int ldrValor = analogRead(LDRPIN);
  int luminosidade = map(ldrValor,0,4095,0,1000);

  // Alterna tela ao pressionar BOTAO_TELA
  if (digitalRead(BOTAO_TELA) == LOW) {
    tela = (tela + 1) % 2; // alterna entre 0 e 1
    delay(300); // debounce
  }

  // Exibe tela correspondente
  if (tela == 0) {
    mostrarCondicoes(temperatura, umidade, luminosidade);
  } else if (tela == 1) {
    selecionarPlanta();
    // Troca planta com BOTAO_PLANTA
    if (digitalRead(BOTAO_PLANTA) == LOW) {
      plantaAtual = (plantaAtual + 1) % 4;
      delay(300);
    }
  }

  delay(500);
}
