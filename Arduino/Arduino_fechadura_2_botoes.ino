#include <SPI.h>
#include <MFRC522.h>     // É necessario baixar essa biblioteca

#define BOTAO_IN_PIN 2   // Estabelece o pino do rele, possível alterar
#define BOTAO_OUT_PIN 7  // Estabelece o pino do botão, possível alterar

bool atvBotaoIn = false;  // Declara o botao como nao ativado
bool atvBotaoOut = false; // Declara o botao como nao ativado

// Lista com as UIDs autorizadas
String AUT_UID[] = {
  
};

void setup() {
  Serial.begin(9600);                    // Inicia a serial
  pinMode(BOTAO_IN_PIN, INPUT_PULLUP);   // Configura o pino do botao de enrtada como entrada
  pinMode(BOTAO_OUT_PIN, INPUT_PULLUP);  // Configura o pino do botao de saida como entrada
}

void loop() {                           // Inicia o loop do sistema

  atvBotao = digitalRead(BOTAO_IN_PIN); // Verifica se o botao de entrada foi ativado
  if (atvBotao == LOW) {
    Serial.println("  ");
    Serial.print("Botão de entradan apertado");
    Serial.println("  ");
    AtivarRele();                       // Funçao de ativaçao do rele
  }

  atvBotao = digitalRead(BOTAO_OUT_PIN); // Verifica se o botao de saida foi ativado
  if (atvBotao == LOW) {
    Serial.println("  ");
    Serial.print("Botão de saida apertado");
    Serial.println("  ");
    AtivarRele();                         // Funçao de ativaçao do rele
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