#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN   9   // Pinos para leitura
#define SS_PIN    10  // Pinos para leitura
#define RELE_PIN 2   // Estabelece o pino do rele, possível alterar
#define BOTAO_PIN 7  // Estabelece o pino do botão, possível alterar

bool atvBotao = false;
MFRC522 mfrc522(SS_PIN, RST_PIN);

// Lista com as UIDs autorizadas
String AUT_UID[] = {
  
};

void setup() {
  Serial.begin(9600);   // Inicia a serial
  SPI.begin();      // Inicia o bus SPI
  mfrc522.PCD_Init();   // Inicia o MFRC522      // Configura o pino do relé como saída
  pinMode(BOTAO_PIN, INPUT_PULLUP);
  pinMode(RELE_PIN, OUTPUT);        //Configura o pino do rele como saida
  digitalWrite(RELE, HIGH); 
}

void loop() {
  mfrc522.PCD_Init();  

  atvBotao = digitalRead(BOTAO_PIN);
  if (atvBotao == LOW) {
    Serial.println("  ");
    Serial.print("Botão apertado");
    Serial.println("  ");
    AtivarRele();
  }

  // Procura por cartão RFID
  if (mfrc522.PICC_IsNewCardPresent()) {
    // Seleciona o cartão RFID
    if (mfrc522.PICC_ReadCardSerial()) {
      Serial.println("  ");

      String uidcard = lerUID();
      bool acesso = UIDautorizada(uidcard);

      mfrc522.PICC_HaltA();
      mfrc522.PCD_StopCrypto1();   

      if (acesso) {
        Serial.print("Bem-vindo, usuário autorizado!");
        Serial.println("  ");
        AtivarRele();
      } else {
        Serial.print("Cartão não autorizado");
      }
      mfrc522.PCD_Init();  
    }
  }
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

// Verifica se a UID está na lista de UIDs autorizadas
bool UIDautorizada(String uid) {
  for (String autorizada : AUT_UID) {
    if (uid == autorizada) {
      return true;
    }
  }
  return false;
}

// Lê a UID e transforma em string
String lerUID() {
  String uidcard = "";
  Serial.print("UID da tag : ");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    uidcard.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    uidcard.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  uidcard.toUpperCase();
  uidcard.trim();

  Serial.print(uidcard);
  Serial.println(" ");

  return uidc;
}

//Iago rodrigues Munoz
//CASIN - UFSC - 06/2024