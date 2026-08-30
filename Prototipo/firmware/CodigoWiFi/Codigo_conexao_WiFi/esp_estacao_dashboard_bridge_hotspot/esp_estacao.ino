#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>
#include <HTTPClient.h>
#include <DHT.h>
#include "esp_wifi.h"

#if __has_include("esp_eap_client.h")
#include "esp_eap_client.h"
#define ESP32_EAP_API_MODERN 1
#else
#include "esp_wpa2.h"
#define ESP32_EAP_API_MODERN 0
#endif

// ======================
// Configuração básica
// ======================
#define AP_SSID            "ESP-Setup"
#define AP_PASS            "12345678"
#define CONNECT_TIMEOUT_S  20

// Troque pelo IP da máquina onde o dashboard/bridge estiver rodando.
// Exemplo: "192.168.1.20"
const char* DASHBOARD_HOST = "172.20.10.2";
const uint16_t DASHBOARD_PORT = 3001;
const char* DASHBOARD_PATH = "/api/weather";
const unsigned long PUBLISH_INTERVAL_MS = 10000;

// ======================
// Sensores
// ======================
#define DHTPIN 15
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define ANEMOMETRO_PIN 2
volatile unsigned long contadorPulsos = 0;
unsigned long ultimoTempoDHT = 0;
const unsigned long intervaloDHT = 2000;
unsigned long ultimoTempoAnemometro = 0;
const unsigned long intervaloAnemometro = 1000;

float temperaturaC = NAN;
int umidadePercent = -1;
float velocidadeVentoKmh = 0.0;

void IRAM_ATTR contaPulso() {
  contadorPulsos++;
}

void atualizarLeiturasSensores() {
  unsigned long agora = millis();

  if (agora - ultimoTempoDHT >= intervaloDHT) {
    ultimoTempoDHT = agora;

    float umidade = dht.readHumidity();
    float temperatura = dht.readTemperature();

    if (!isnan(umidade) && !isnan(temperatura)) {
      umidadePercent = (int)round(umidade);
      temperaturaC = temperatura;
    } else {
      Serial.println("[Sensores] Falha ao ler o sensor DHT11.");
    }
  }

  if (agora - ultimoTempoAnemometro >= intervaloAnemometro) {
    ultimoTempoAnemometro = agora;

    noInterrupts();
    unsigned long pulsos = contadorPulsos;
    contadorPulsos = 0;
    interrupts();

    float segundosDecorridos = intervaloAnemometro / 1000.0;
    float pulsosPorSegundo = pulsos / segundosDecorridos;
    velocidadeVentoKmh = pulsosPorSegundo * 2.4;

    Serial.printf("[Sensores] Vento: %.1f km/h\n", velocidadeVentoKmh);
  }
}

// ======================
// Objetos
// ======================
WebServer server(80);
Preferences prefs;

// ======================
// Estado
// ======================
String wifiSSID, wifiPass;
String wifiMode;
String wifiIdentity;
String wifiUsername;
unsigned long lastPublishMs = 0;

// ======================
// Leitura dos sensores
// ======================
float readTemperatureC() {
  atualizarLeiturasSensores();
  return temperaturaC;
}

int readHumidityPercent() {
  atualizarLeiturasSensores();
  return umidadePercent;
}

float readWindSpeedKmh() {
  atualizarLeiturasSensores();
  return velocidadeVentoKmh;
}

String readSkyCondition() {
  atualizarLeiturasSensores();

  if (isnan(temperaturaC) || umidadePercent < 0) {
    return "indefinido";
  }

  if (temperaturaC >= 30) {
    return "ensolarado";
  }

  if (umidadePercent >= 80) {
    return "chuvoso";
  }

  if (umidadePercent >= 60) {
    return "nublado";
  }

  return "parcialmente-nublado";
}

String buildReadingPayload() {
  String payload = "{";
  float temperature = readTemperatureC();
  int humidity = readHumidityPercent();
  float windSpeed = readWindSpeedKmh();

  payload += "\"temperature\":";
  payload += isnan(temperature) ? "null," : String(temperature, 1) + ",";
  payload += "\"humidity\":";
  payload += humidity < 0 ? "null," : String(humidity) + ",";
  payload += "\"windSpeed\":";
  payload += isnan(windSpeed) ? "null," : String(windSpeed, 1) + ",";
  payload += "\"condition\":\"" + readSkyCondition() + "\"";
  payload += "}";
  return payload;
}

// ======================
// Envio para o dashboard
// ======================
bool publishReadingToDashboard() {
  String payload = buildReadingPayload();
  Serial.println("[Dashboard] Enviando payload: " + payload);

  HTTPClient http;
  http.begin(DASHBOARD_HOST, DASHBOARD_PORT, DASHBOARD_PATH);
  http.addHeader("Content-Type", "application/json");

  int httpCode = http.POST(payload);
  String response = http.getString();

  if (httpCode >= 200 && httpCode < 300) {
    Serial.println("[Dashboard] Leitura enviada com sucesso.");
    http.end();
    return true;
  }

  Serial.printf("[Dashboard] Falha ao enviar: %d | Resposta: %s\n", httpCode, response.c_str());
  http.end();
  return false;
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
    Tipo de rede:<br>
    <label><input type='radio' name='mode' value='normal' checked> Rede comum ou hotspot do telefone</label><br>
    <label><input type='radio' name='mode' value='enterprise'> WPA2 Enterprise institucional</label><br><br>
    Senha da rede comum (deixe vazia se a rede for aberta):<br>
    <input name="senha" type="password" style='width:100%;padding:6px;margin:4px 0 12px'><br>
    Identidade anônima (WPA2 Enterprise, opcional):<br>
    <input name="identity" style='width:100%;padding:6px;margin:4px 0 12px'><br>
    Usuário institucional (WPA2 Enterprise):<br>
    <input name="username" style='width:100%;padding:6px;margin:4px 0 12px'><br>
    Senha institucional (WPA2 Enterprise):<br>
    <input name="senhaEnterprise" type="password" style='width:100%;padding:6px;margin:4px 0 12px'><br>
    <input type="submit" value="Conectar" style='background:#2a7;color:#fff;padding:10px 24px;border:none;border-radius:4px;cursor:pointer'>
  </form>
</body></html>
)rawliteral";
  server.send(200, "text/html", html);
}

void handleSave() {
  prefs.begin("wifi", false);
  prefs.putString("ssid",  server.arg("ssid"));
  prefs.putString("mode", server.arg("mode"));
  prefs.putString("identity", server.arg("identity"));
  prefs.putString("username", server.arg("username"));
  prefs.putString("senha", server.arg("mode") == "enterprise"
    ? server.arg("senhaEnterprise")
    : server.arg("senha"));
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
  WiFi.disconnect(true);
  delay(100);

  if (wifiMode == "enterprise") {
    Serial.println("[WiFi] Modo WPA2 Enterprise.");
#if ESP32_EAP_API_MODERN
    esp_eap_client_set_identity(
      (uint8_t*)wifiIdentity.c_str(), wifiIdentity.length());
    esp_eap_client_set_username(
      (uint8_t*)wifiUsername.c_str(), wifiUsername.length());
    esp_eap_client_set_password(
      (uint8_t*)wifiPass.c_str(), wifiPass.length());
    esp_wifi_sta_enterprise_enable();
#else
    esp_wifi_sta_wpa2_ent_set_identity(
      (uint8_t*)wifiIdentity.c_str(), wifiIdentity.length());
    esp_wifi_sta_wpa2_ent_set_username(
      (uint8_t*)wifiUsername.c_str(), wifiUsername.length());
    esp_wifi_sta_wpa2_ent_set_password(
      (uint8_t*)wifiPass.c_str(), wifiPass.length());
    esp_wifi_sta_wpa2_ent_enable();
#endif
    WiFi.begin(wifiSSID.c_str());
  } else if (wifiPass.isEmpty()) {
    Serial.println("[WiFi] Modo rede comum/hotspot aberto.");
  #if ESP32_EAP_API_MODERN
    esp_wifi_sta_enterprise_disable();
  #else
    esp_wifi_sta_wpa2_ent_disable();
  #endif
    WiFi.begin(wifiSSID.c_str());
  } else {
    Serial.println("[WiFi] Modo rede comum/hotspot com senha.");
  #if ESP32_EAP_API_MODERN
    esp_wifi_sta_enterprise_disable();
  #else
    esp_wifi_sta_wpa2_ent_disable();
  #endif
    WiFi.begin(wifiSSID.c_str(), wifiPass.c_str());
  }

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
// Setup
// ======================
void setup() {
  Serial.begin(115200);
  delay(1000);

  dht.begin();
  pinMode(ANEMOMETRO_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ANEMOMETRO_PIN), contaPulso, FALLING);

  prefs.begin("wifi", true);
  wifiSSID = prefs.getString("ssid",  "");
  wifiMode = prefs.getString("mode", "normal");
  wifiIdentity = prefs.getString("identity", "");
  wifiUsername = prefs.getString("username", "");
  wifiPass = prefs.getString("senha", "");
  prefs.end();

  if (wifiSSID.isEmpty() || (wifiMode == "enterprise" &&
      (wifiUsername.isEmpty() || wifiPass.isEmpty()))) {
    startConfigPortal();
  }

  if (!connectWiFi()) {
    Serial.println("[Setup] Não foi possível conectar com as credenciais atuais.");
    Serial.println("[Setup] Voltando ao portal de configuração para corrigir a rede...");
    delay(2000);
    startConfigPortal();
  }

}

// ======================
// Loop
// ======================
void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[WiFi] Conexão perdida. Reiniciando...");
    delay(3000);
    ESP.restart();
  }

  if (millis() - lastPublishMs > PUBLISH_INTERVAL_MS) {
    publishReadingToDashboard();
    lastPublishMs = millis();
  }

  delay(10);
}
