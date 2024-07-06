#include <SPI.h>
#include <MFRC522.h>
#include <avr/wdt.h>

#define RST_PIN   9   // Pinos para leitura
#define SS_PIN    10  // Pinos para leitura
#define RELAY_PIN 2   // Estabelece o pino do rele, possível alterar
#define BUTTON_PIN 7  // Estabelece o pino do botão, possível alterar

MFRC522 mfrc522(SS_PIN, RST_PIN); // Inicia o leitor

// Lista com as UIDs autorizadas
String AUT_UID[] = {
  //UID no formato XX XX XX XX
};

//Estabelecem o estado do botao e usam tecnicas
//de debounce para estabilizar a leitura 
bool buttonState = false;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 200;

//Estabelecem uma quantidade de iteracoes para
//reiniciar o arduino assegurando funcionalidade
//apos longos periodos de inatividade

void setup() {                       //Estabelece o codigo principal
  Serial.begin(9600);                //Estabelece taxa de transmissao
  SPI.begin();                       //Estabelece a ligaçao com leitor rfid
  mfrc522.PCD_Init();                //Faz o leitor começar a operar
  pinMode(RELAY_PIN, OUTPUT);        //Configura o pino do rele como saida
  digitalWrite(RELAY_PIN, HIGH);     //Estabele o estado natural do rele como desligado
  pinMode(BUTTON_PIN, INPUT_PULLUP); //Estabelece o estado natural do botao como nao pressionado
}

void loop() {   
  unsigned long currentMillis = millis();                 //Usado de debounce pelo bota

  if (digitalRead(BUTTON_PIN) == LOW && !buttonState && 
  (currentMillis - lastDebounceTime) > debounceDelay) {   //Faz um if de debouce para verficiar a o repressionamento do botao
    buttonState = true;                                   //Muda a varivavel para indicar que o botao foi pressionado
    lastDebounceTime = currentMillis;                     //Redefine o tempo do ultimo pressionametno para debounce

    Serial.println(
      "Botão pressionado! Forçando acesso...");           //Printa no serial monitor para informar);
    ativarRele();                                         //Ativa o rele    
    mfrc522.PCD_Init();                                   //Reinicia o leitor
  }

  if (digitalRead(BUTTON_PIN) == HIGH && buttonState && 
  (currentMillis - lastDebounceTime) > debounceDelay) {   //Debounce pra verificar se o botao foi pressionado na ultima iteraçao
    buttonState = false;                                  //Atualiza o estado da varivel do botao ter sido pressionado
    lastDebounceTime = currentMillis;                     //Define o tempo para a proxima verificaão de debounce
  }

  Serial.println(
    "Aproxime o cartão RFID para ler a UID...");         //Indica iteraçao do loop
  digitalWrite(RELAY_PIN, HIGH);                         //Garante que o relé está desligado

  if (mfrc522.PICC_IsNewCardPresent()) {          // Verifica se há um novo cartão presente no leitor
  if (mfrc522.PICC_ReadCardSerial()) {           // Retorna uma booleana dependendo da confirmação ou não da leitura 
    String uid = lerUID();                       // Retorna uma string com a UID do cartão lido          

    // Printam a UID do cartão recém lido
    Serial.print("UID lida: ");
    Serial.println(uid);

    if (isUIDAuthorized(uid)) {                  // Chama a função para verificar se a uid está autorizada
      // Caso sim, printa que foi autorizado para debug,
      // ativa o relé e reinicia o leitor 
      Serial.println("Acesso autorizado!");
      ativarRele();
      mfrc522.PICC_HaltA();                      // Faz o cartão momentaneamente parar de responder
      mfrc522.PCD_StopCrypto1();                 // Desativa a criptografia do cartão
      delay(10); 

      mfrc522.PICC_HaltA();                          
      mfrc522.PCD_StopCrypto1();                     
      delay(10); 
      mfrc522.PCD_Init();                            //Reinicia o leitor
      // Garante que o loop rode aproximadamente 8 vezes por segundo
      delay(75);
      mfrc522.PCD_Init();
      delay(10);
      Serial.println("reset ardas...");  
      wdt_enable(WDTO_8S);
    return;
    } else {
      // Exibe que o acesso foi negado para debug
      Serial.println("Acesso negado!");
    }
  } else {
    mfrc522.PICC_HaltA();                  
    mfrc522.PCD_StopCrypto1();                 
    delay(10); 
    mfrc522.PCD_Init();                          
    // Garante que o loop rode aproximadamente 8 vezes por segundo
    delay(75);
    mfrc522.PCD_Init();
    delay(10);
    Serial.println("reset ardas...");  
    wdt_enable(WDTO_8S);
    return;
  }
} else {
  mfrc522.PICC_HaltA();                          
  mfrc522.PCD_StopCrypto1();                     
  delay(10); 
  mfrc522.PCD_Init();                            //Reinicia o leitor
  // Garante que o loop rode aproximadamente 8 vezes por segundo
  delay(75);
  mfrc522.PCD_Init();
  delay(10);
  Serial.println("reset ardas...");  
  wdt_enable(WDTO_8S);
  return;
}




  mfrc522.PICC_HaltA();                          
  mfrc522.PCD_StopCrypto1();                     
  delay(10); 
  mfrc522.PCD_Init();                            //Reinicia o leitor
  // Garante que o loop rode aproximadamente 8 vezes por segundo
  delay(75);
  mfrc522.PCD_Init();
  delay(10);
  Serial.println("reset ardas...");  
  wdt_enable(WDTO_8S);
  return;

  
}

//Formata a uid para printa-la no serial monitor
String lerUID() {                                      
  String uid = "";
  
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    uid.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    uid.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  uid.toUpperCase();
  uid.trim();
  return uid;
}


//Ativa o rele 8 vezes, SIM OITO VEZES, para garantir
//que ele vai destrancar a porta de primeira na ativaçao
void ativarRele() {
  digitalWrite(RELAY_PIN, LOW); 
  delay(8);
  digitalWrite(RELAY_PIN, HIGH);
  delay(8);

  digitalWrite(RELAY_PIN, LOW); 
  delay(8);
  digitalWrite(RELAY_PIN, HIGH);
  delay(8);

  digitalWrite(RELAY_PIN, LOW); 
  delay(8);
  digitalWrite(RELAY_PIN, HIGH);
  delay(8);

  digitalWrite(RELAY_PIN, LOW); 
  delay(8);
  digitalWrite(RELAY_PIN, HIGH);
  delay(8);

  digitalWrite(RELAY_PIN, LOW); 
  delay(8);
  digitalWrite(RELAY_PIN, HIGH);
  delay(8);

  digitalWrite(RELAY_PIN, LOW); 
  delay(8);
  digitalWrite(RELAY_PIN, HIGH);
  delay(8);

  digitalWrite(RELAY_PIN, LOW); 
  delay(8);
  digitalWrite(RELAY_PIN, HIGH);
  delay(8);

  digitalWrite(RELAY_PIN, LOW); 
  delay(8);
  digitalWrite(RELAY_PIN, HIGH);
  delay(8); 
}

//Verifica se a UID esta na lista de uids autorizadas
bool isUIDAuthorized(String uid) {
  for (String autorizada : AUT_UID) {
    if (uid == autorizada) {
      return true;
    }
  }
  return false;
}

//Iago rodrigues Munoz
//CASIN - UFSC - 06/2024
