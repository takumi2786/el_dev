// ESP32/HelloServer/HelloServer.ino
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

const char* ssid = "303ZTa-C2D794";
const char* password = "4541857a";

WebServer server(80);

const int led = 13;

void handleIndex() {
  server.send(200, "text/html", "<b>hello from esp32!</b>");
}

void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/plain", "hello from esp32!");
  digitalWrite(led, 0);
}

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
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

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  // ルーティングを登録する．
  server.on("/", handleIndex);
  server.on("/inline", []() {//インラインでの書き方．
    server.send(200, "text/plain", "this works as well");
  });
  // NotFoundエラーの場合の処理．
  server.onNotFound(handleNotFound);

  // ウェブサーバの開始
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
}

/*
WebServerクラスのメモ

WebServer::handleClient()
クライアントからのリクエストを処理する．

// ファイルで作ったhtmlを公開するには？



*/