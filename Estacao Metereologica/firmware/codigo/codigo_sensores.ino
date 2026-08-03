#include "DHT.h"

// --- DHT11 ---
#define DHTPIN 15
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
unsigned long ultimoTempoDHT = 0;
const unsigned long intervaloDHT = 60000; // DHT11 precisa de ~2s entre leituras

// --- Anemômetro ---
#define ANEMOMETRO_PIN 2
volatile unsigned long contadorPulsos = 0;
unsigned long ultimoTempoAnemometro = 0;
const unsigned long intervaloAnemometro = 60000;

void IRAM_ATTR contaPulso() {
  contadorPulsos++;
}

void setup() {
  Serial.begin(9600);
  Serial.println("Iniciando DHT11 + Anemometro...");

  dht.begin();

  pinMode(ANEMOMETRO_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ANEMOMETRO_PIN), contaPulso, FALLING);

  ultimoTempoDHT = millis();
  ultimoTempoAnemometro = millis();
}

void loop() {
  unsigned long agora = millis();

  // --- Leitura do DHT11 a cada 2 segundos ---
  if (agora - ultimoTempoDHT >= intervaloDHT) {
    ultimoTempoDHT = agora;

    float umidade = dht.readHumidity();
    float temperatura = dht.readTemperature();

    if (isnan(umidade) || isnan(temperatura)) {
      Serial.println("Falha ao ler o sensor DHT11!");
    } else {
      Serial.print("Umidade: ");
      Serial.print(umidade);
      Serial.print("%  Temperatura: ");
      Serial.print(temperatura);
      Serial.println("°C");
    }
  }

  // --- Leitura do anemômetro a cada 1 segundo ---
  if (agora - ultimoTempoAnemometro >= intervaloAnemometro) {
    ultimoTempoAnemometro = agora;

    noInterrupts();
    unsigned long pulsos = contadorPulsos;
    contadorPulsos = 0;
    interrupts();

    float segundosDecorridos = intervaloAnemometro / 1000.0;
    float pulsosPorSegundo = pulsos / segundosDecorridos;
    float velocidadeKmh = pulsosPorSegundo * 2.4;

    Serial.print("Vento: ");
    Serial.print(pulsos);
    Serial.print(" pulsos/seg | ");
    Serial.print(velocidadeKmh);
    Serial.println(" km/h");
  }
}