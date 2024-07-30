#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN   5
#define RST_PIN  17
#define RELE_PIN 16
#define BOTAO_PIN 25

bool atvBotao = false;

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Cria instância MFRC522.

// Lista com as UIDs autorizadas
char AUT_UID[][11] = {
  // Adicione aqui as UIDs autorizadas, por exemplo: "0A0B0C0D0E"
};

void setup() {
  Serial.begin(115200);   // Inicia a serial
  SPI.begin();      // Inicia o bus SPI
  mfrc522.PCD_Init();   // Inicia o MFRC522
  pinMode(RELE_PIN, OUTPUT); // Configura o pino do relé como saída
  pinMode(BOTAO_PIN, INPUT_PULLUP); // Configura o pino do botão como entrada com pull-up
}

void loop() {
  mfrc522.PCD_Init();  

  atvBotao = digitalRead(BOTAO_PIN);
  if (atvBotao == LOW) {
    Serial.println("Botão apertado");
    AtivarRele();
  }

  // Procura por cartão RFID
  if (mfrc522.PICC_IsNewCardPresent()) {
    // Seleciona o cartão RFID
    if (mfrc522.PICC_ReadCardSerial()) {
      char uidcard[11];
      lerUID(uidcard);
      bool acesso = UIDautorizada(uidcard);

      mfrc522.PICC_HaltA();
      mfrc522.PCD_StopCrypto1();

      if (acesso) {
        Serial.println("Bem-vindo, usuário autorizado!");
        AtivarRele();
      } else {
        Serial.println("Cartão não autorizado");
      }
      mfrc522.PCD_Init();
    }
  }
}

// Função de ativação do relé
void AtivarRele() {
  Serial.println("Abrindo porta");

  for (int i = 0; i < 8; i++) {  // Loop que repete 8 vezes
    digitalWrite(RELE_PIN, HIGH);  // Ativa o relé
    delay(8);
    digitalWrite(RELE_PIN, LOW); // Desativa o relé
    delay(8); 
  }
  return;
}

// Verifica se a UID está na lista de UIDs autorizadas
bool UIDautorizada(char* uid) {
  for (int i = 0; i < sizeof(AUT_UID) / sizeof(AUT_UID[0]); i++) {
    if (strcmp(uid, AUT_UID[i]) == 0) {
      return true;
    }
  }
  return false;
}

// Lê a UID e transforma em string
void lerUID(char* uidcard) {
  Serial.print("UID da tag : ");
  for (uint8_t i = 0; i < mfrc522.uid.size; i++) {
    sprintf(&uidcard[i * 2], "%02X", mfrc522.uid.uidByte[i]);
  }
  uidcard[2 * mfrc522.uid.size] = '\0'; // Adiciona o terminador nulo ao final da string

  Serial.println(uidcard);
}
