#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>

// ======================
// Configuração
// ======================
#define AP_SSID            "ESP-Setup"
#define AP_PASS            "12345678"
#define LED_PIN            2      // LED onboard da maioria das devkits ESP32
#define CONNECT_TIMEOUT_S  20

// ======================
// Objetos
// ======================
WebServer   server(80);
Preferences prefs;

// ======================
// Estado
// ======================
String wifiSSID, wifiPass;
bool   ledState = false;

// ======================
// LED
// ======================
void setLed(bool state) {
  ledState = state;
  digitalWrite(LED_PIN, ledState ? HIGH : LOW);
  Serial.printf("[LED] %s\n", ledState ? "LIGADO" : "DESLIGADO");
}

// ======================
// Portal de configuração (Access Point)
// ======================
void handleRoot() {
  String html = R"rawliteral(
<!DOCTYPE html><html>
<body style='font-family:Arial;padding:20px'>
  <h2>Configuração WiFi</h2>
  <form action="/save" method="POST">
    SSID:<br><input name="ssid" style='width:100%;padding:6px;margin:4px 0 12px'><br>
    Senha:<br><input name="senha" type="password" style='width:100%;padding:6px;margin:4px 0 12px'><br>
    <input type="submit" value="Conectar" style='background:#2a7;color:#fff;padding:10px 24px;border:none;border-radius:4px;cursor:pointer'>
  </form>
</body></html>
)rawliteral";
  server.send(200, "text/html", html);
}

void handleSave() {
  prefs.begin("wifi", false);
  prefs.putString("ssid",  server.arg("ssid"));
  prefs.putString("senha", server.arg("senha"));
  prefs.end();
  server.send(200, "text/html", "<h2>Credenciais salvas! Reiniciando...</h2>");
  delay(2000);
  ESP.restart();
}

void startConfigPortal() {
  Serial.println("\n[Portal] Iniciando ponto de acesso...");
  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID, AP_PASS);
  Serial.printf("[Portal] Conecte ao WiFi '%s' e acesse http://%s\n",
    AP_SSID, WiFi.softAPIP().toString().c_str());

  server.on("/",     HTTP_GET,  handleRoot);
  server.on("/save", HTTP_POST, handleSave);
  server.begin();

  while (true) {
    server.handleClient();
    delay(10);
  }
}

// ======================
// Conexão com a rede do usuário (STA)
// ======================
bool connectWiFi() {
  Serial.printf("[WiFi] Conectando a '%s'...\n", wifiSSID.c_str());
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifiSSID.c_str(), wifiPass.c_str());

  for (int i = 0; i < CONNECT_TIMEOUT_S; i++) {
    if (WiFi.status() == WL_CONNECTED) break;
    Serial.print(".");
    delay(1000);
  }
  Serial.println();

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[WiFi] Falha na conexão.");
    return false;
  }

  Serial.printf("[WiFi] Conectado! IP: %s\n", WiFi.localIP().toString().c_str());
  return true;
}

// ======================
// Página de controle do LED (troca de dados via rede)
// ======================
void handleControlPage() {
  String html = "<html><body style='font-family:Arial;padding:20px'>"
    "<h2>Controle do LED</h2>"
    "<p>Estado atual: <b>" + String(ledState ? "LIGADO" : "DESLIGADO") + "</b></p>"
    "<form action='/led' method='POST'>"
    "Digite 'on' ou 'off':<br>"
    "<input name='cmd' style='width:200px;padding:6px;margin:6px 0' autofocus><br>"
    "<input type='submit' value='Enviar' style='background:#2a7;color:#fff;padding:8px 20px;border:none;border-radius:4px;cursor:pointer'>"
    "</form></body></html>";
  server.send(200, "text/html", html);
}

void handleLedCommand() {
  String cmd = server.arg("cmd");
  cmd.trim();
  cmd.toLowerCase();

  if (cmd == "on" || cmd == "1" || cmd == "ligar") {
    setLed(true);
  } else if (cmd == "off" || cmd == "0" || cmd == "desligar") {
    setLed(false);
  }

  server.sendHeader("Location", "/", true);
  server.send(302, "text/plain", "");
}

void startControlServer() {
  server.on("/",    HTTP_GET,  handleControlPage);
  server.on("/led", HTTP_POST, handleLedCommand);
  server.begin();
  Serial.printf("[Servidor] Acesse http://%s/ para controlar o LED\n",
    WiFi.localIP().toString().c_str());
}

// ======================
// Comando via Monitor Serial (teclado do PC)
// ======================
void checkSerialCommand() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    cmd.toLowerCase();

    if (cmd == "on" || cmd == "1") {
      setLed(true);
    } else if (cmd == "off" || cmd == "0") {
      setLed(false);
    }
  }
}

// ======================
// Setup
// ======================
void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  prefs.begin("wifi", true);
  wifiSSID = prefs.getString("ssid",  "");
  wifiPass = prefs.getString("senha", "");
  prefs.end();

  if (wifiSSID.isEmpty()) {
    startConfigPortal();  // não retorna
  }

  if (!connectWiFi()) {
    Serial.println("[Setup] Reiniciando em 5s para nova tentativa...");
    delay(5000);
    ESP.restart();
  }

  startControlServer();
  Serial.println("[Setup] Tambem e possivel digitar 'on'/'off' no Monitor Serial.");
}

// ======================
// Loop
// ======================
void loop() {
  server.handleClient();
  checkSerialCommand();

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[WiFi] Conexao perdida. Reiniciando...");
    delay(3000);
    ESP.restart();
  }

  delay(10);
}
