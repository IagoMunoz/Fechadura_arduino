#include <SPI.h>
#include <MFRC522.h>

#define BOTAO_IN_PIN 2   // Estabelece o pino do rele, possível alterar
#define BOTAO_OUT_PIN 7  // Estabelece o pino do botão, possível alterar

bool atvBotaoIn = false;
bool atvBotaoOut = false;

void setup() {
  Serial.begin(9600);   // Inicia a serial
  pinMode(BOTAO_IN_PIN, INPUT_PULLUP);
  pinMode(BOTAO_OUT_PIN, INPUT_PULLUP);
}

void loop() {

  atvBotao = digitalRead(BOTAO_IN_PIN);
  if (atvBotao == LOW) {
    Serial.println("  ");
    Serial.print("Botão de entradan apertado");
    Serial.println("  ");
    AtivarRele();
  }

  atvBotao = digitalRead(BOTAO_OUT_PIN);
  if (atvBotao == LOW) {
    Serial.println("  ");
    Serial.print("Botão de saida apertado");
    Serial.println("  ");
    AtivarRele();
  }

// Função de ativação do relé
void AtivarRele() {
  Serial.println("Abrindo porta");

  for (int i = 0; i < 8; i++) {  // Loop que repete 8 vezes
    pinMode(RELE_PIN, LOW);  // Ativa o relé
    delay(8);
    pinMode(RELE_PIN, LOW); // Desativa o relé
    delay(8); 
  }
  return;
  }
}

//Iago rodrigues Munoz
//CASIN - UFSC - 06/2024