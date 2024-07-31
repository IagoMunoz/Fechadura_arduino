//atualizar isso aqui pra otimizaçao de memoria nos chars e nas verficaçoes
//tentar reduzir a importaçao de funçao das bibliotecaas



#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>

const char* host = "esp32";
const char* ssid = "";
const char* password = "";

WebServer server(80);

const char* loginUsername = "";
const char* loginPassword = "";
bool isAuthenticated = false;

/*
 * Login page
 */
const char* loginIndex = 
 "<form name='loginForm' action='/login' method='POST'>"
    "<tr>"
        "<font size='4'><b>CasinDoorUpdater</b></font>"
    "</tr>"
    "<tr>"
        "<td><input type='text' name='userid'></td>"
    "</tr>"
    "<tr>"
        "<input type='password' name='pwd'>"
    "</tr>"
    "<tr>"
        "<input type='submit' value='Login'>"
    "</tr>"
"</form>"
;

/*
 * Server Index Page
 */
const char* serverIndex = 
"<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
"<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
   "<input type='file' name='update'>"
        "<input type='submit' value='Update'>"
    "</form>"
 "<div id='prg'>progress: 0%</div>"
 "<script>"
  "$('form').submit(function(e){"
  "e.preventDefault();"
  "var form = $('#upload_form')[0];"
  "var data = new FormData(form);"
  " $.ajax({"
  "url: '/update',"
  "type: 'POST',"
  "data: data,"
  "contentType: false,"
  "processData:false,"
  "xhr: function() {"
  "var xhr = new window.XMLHttpRequest();"
  "xhr.upload.addEventListener('progress', function(evt) {"
  "if (evt.lengthComputable) {"
  "var per = evt.loaded / evt.total;"
  "$('#prg').html('progress: ' + Math.round(per*100) + '%');"
  "}"
  "}, false);"
  "return xhr;"
  "},"
  "success:function(d, s) {"
  "console.log('success!')" 
 "},"
 "error: function (a, b, c) {"
 "}"
 "});"
 "});"
 "</script>";

void handleLogin() {
  if (server.hasArg("userid") && server.hasArg("pwd")) {
    if (server.arg("userid") == loginUsername && server.arg("pwd") == loginPassword) {
      isAuthenticated = true;
      server.sendHeader("Location", "/serverIndex");
      server.send(303);
      return;
    }
  }
  server.send(200, "text/html", loginIndex);
}

void handleServerIndex() {
  if (!isAuthenticated) {
    server.sendHeader("Location", "/");
    server.send(303);
    return;
  }
  server.send(200, "text/html", serverIndex);
}

/*
 * setup function
 */
void setup(void) {
  Serial.begin(115200);

  // Connect to WiFi network
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Use mDNS for host name resolution
  if (!MDNS.begin(host)) {
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");

  // Routes
  server.on("/", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", loginIndex);
  });
  server.on("/login", HTTP_POST, handleLogin);
  server.on("/serverIndex", HTTP_GET, handleServerIndex);
  
  // Handling firmware upload
  server.on("/update", HTTP_POST, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
  }, []() {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
      Serial.printf("Update: %s\n", upload.filename.c_str());
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) {
        Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
      } else {
        Update.printError(Serial);
      }
    }
  });

  server.begin();
}

void loop(void) {
  server.handleClient();
  delay(1);
}
