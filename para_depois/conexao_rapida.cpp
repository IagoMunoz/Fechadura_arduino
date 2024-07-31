//tentar usar mais a funçao cheinternetconnection plea velocidade e efetividade


#include <WiFi.h>
#include <Preferences.h>
#include <HTTPClient.h>

// Defina o nome da rede Wi-Fi e a senha
const char* ssid = "";
const char* password = "";

// Variável para armazenar o status da conexão Wi-Fi
bool wifiCon = WL_DISCONNECTED;
bool onlineOps = false;

void setup() {
  // Inicializa a comunicação serial
  Serial.begin(115200);

  // Conecta-se à rede Wi-Fi
  Serial.println();
  Serial.println();
  Serial.print("Conectando-se a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
}

void loop() {
  // Atualiza o status da conexão Wi-Fi
  wifiCon = checkInternetConnection();

  // Verifica o status da conexão
  if (wifiCon == false) {
    Serial.println("\nDesconectado do Wi-Fi");
    WiFi.begin(ssid, password);
    wifiCon = checkInternetConnection();
    delay(128);
    return;
  }
  Serial.println("aaaaaaa");

  delay(512);
}

bool checkInternetConnection() {
  HTTPClient http;
  http.begin("http://www.google.com"); // Pode ser qualquer URL conhecida que responda a requisições HTTP
  int httpCode = http.GET();
  http.end();

  // Retorna true se a resposta for 200 (HTTP OK)
  return (httpCode > 0 && httpCode == HTTP_CODE_OK);
}

