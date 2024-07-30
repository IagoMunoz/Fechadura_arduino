
#include <SPI.h>
#include <MFRC522.h>
  
#define SS_PIN   5
#define RST_PIN  17
#define RELE_PIN 16
#define BOTAO_PIN 25

bool atvBotao = false;

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Cria instância MFRC522.

// Lista com as UIDs autorizadas
String AUT_UID[] = {

};


void setup() {
  Serial.begin(115200);   // Inicia a serial
  SPI.begin();      // Inicia o bus SPI
  mfrc522.PCD_Init();   // Inicia o MFRC522      // Configura o pino do relé como saída
  pinMode(BOTAO_PIN, INPUT_PULLUP);
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
    pinMode(RELE_PIN, OUTPUT);  // Ativa o relé
    delay(8);
    pinMode(RELE_PIN, INPUT); // Desativa o relé
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

  return uidcard;
}
