int ldr;
const int LDR = 1;
const int LED_BLUE = 12;
const int LED_GREEN = 13;
const int LED_RED = 14;
const int BOTAO_LIGAR_MANUAL = 6;
const int BOTAO_MUDAR_COR = 4;
bool manualLigado = true;
int ledCorAtual = 0;




void setup() {
  Serial.begin(9600);
  pinMode(LDR, INPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BOTAO_LIGAR_MANUAL, INPUT_PULLUP);
  pinMode(BOTAO_MUDAR_COR, INPUT_PULLUP);
}

void loop() {
  ldr = analogRead(LDR);
  Serial.println("LDR: " + String(ldr));
  

  if (digitalRead(BOTAO_LIGAR_MANUAL) == LOW) {
    manualLigado = !manualLigado;
    Serial.println("Modo Manual Desativado." + String(ldr));
    delay(100); 
  }

  if (!manualLigado) {
    Serial.println("Modo Manual Desativado.");
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
  else {
    Serial.println("Modo Manual Ativado.");
    if (digitalRead(BOTAO_MUDAR_COR) == LOW) {    
      ledCorAtual = (ledCorAtual + 1) % 4;
      delay(100);
    }
    switch (ledCorAtual) {
      case 0:
        digitalWrite(LED_BLUE, LOW);
        digitalWrite(LED_GREEN, LOW);
        digitalWrite(LED_RED, LOW);
        break;

      case 1: 
        digitalWrite(LED_BLUE, HIGH);
        digitalWrite(LED_GREEN, LOW);
        digitalWrite(LED_RED, LOW);
        Serial.println("Modo: Dia ");
        break;
      
      case 2:
        digitalWrite(LED_BLUE, LOW);
        digitalWrite(LED_GREEN, HIGH);
        digitalWrite(LED_RED, LOW);
        Serial.println("Modo: Crepúsculo ");
        break;
      
      case 3:
        digitalWrite(LED_BLUE, LOW);
        digitalWrite(LED_GREEN, LOW);
        digitalWrite(LED_RED, HIGH);
        Serial.println("Modo: Noite ");
        break;
   
    }    
  }
}
