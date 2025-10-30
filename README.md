int ldr;
const int LDR = 1;
const int LED_BLUE = 12;
const int LED_GREEN = 13;
const int LED_RED = 14;
const int BOTAO_LIGAR_ANALOG = 6;
const int BOTAO_MUDAR_COR = 4;
bool manualLigado = true;
int ledCorAtual = 0;
int luz;



void setup() {
  Serial.begin(9600);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BOTAO_LIGAR_ANALOG, INPUT_PULLUP);
  pinMode(BOTAO_MUDAR_COR, INPUT_PULLUP);
}

void loop() {
  ldr = digitalRead(LDR);
  Serial.println("LDR: " + String(ldr));
  

  if (digitalRead(BOTAO_LIGAR_ANALOG) == LOW) {
       

    if  (ldr > 6000) {
      digitalWrite(LED_BLUE, HIGH);
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_RED, LOW);
      Serial.println("Modo: Dia ");
    }
    else if (ldr >= 3000) {
      digitalWrite(LED_BLUE, LOW);
      digitalWrite(LED_GREEN, HIGH);
      digitalWrite(LED_RED, LOW);
      Serial.println("Modo: Crepúsculo ");
    }
    else if (ldr < 3000) {
      digitalWrite(LED_BLUE, LOW);
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_RED, HIGH);
      Serial.println("Modo: Noite ");
    }
  }
  else if (digitalRead(BOTAO_LIGAR_ANALOG) == HIGH){
    digitalWrite(LED_BLUE, LOW);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_RED, LOW);
    Serial.println("Modo Digital Desativado.");

    ledCorAtual = (ledCorAtual + 1) % 3;

      if  (ledCorAtual == 1) {
        digitalWrite(LED_BLUE, HIGH);
        digitalWrite(LED_GREEN, LOW);
        digitalWrite(LED_RED, LOW);
        Serial.println("Modo: Dia ");
      }
      else if (ledCorAtual == 2) {
        digitalWrite(LED_BLUE, LOW);
        digitalWrite(LED_GREEN, HIGH);
        digitalWrite(LED_RED, LOW);
        Serial.println("Modo: Crepúsculo ");
      }
      else if (ledCorAtual == 3) {
        digitalWrite(LED_BLUE, LOW);
        digitalWrite(LED_GREEN, LOW);
        digitalWrite(LED_RED, HIGH);
        Serial.println("Modo: Noite ");
      }
    
    

  }
}
