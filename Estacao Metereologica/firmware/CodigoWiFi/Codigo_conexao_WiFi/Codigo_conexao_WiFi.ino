#include <WiFi.h>

const char* ssid = "";
const char* password = "";
int led = 2;
WiFiServer server(80);

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  pinMode(led, OUTPUT);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Conecting to WiFi..");
  }
  Serial.println("Conected to the WiFi network");
  Serial.println("IP addres: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.print("Click <a href=\"/H\">here</a> to turn the LED on pin 2 on.<br>");
            client.print("Click <a href=\"/L\">here</a> to turn the LED on pin 2 off.<br>");
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
        if (currentLine.endsWith("GET /H")) {
          digitalWrite(led, HIGH);
        }
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(led, LOW);
        }
      }
    }
    client.stop();
    Serial.println("Client Disconnected.");
  }
}