#include <DHT.h>
#include <WiFi.h>
#include <PubSubClient.h>

#define DHTPIN 4
#define DHTTYPE DHT22
#define RELAY_PIN 5
#define TEMP_CRITICA 30.0

const char* ssid = "Wokwi-GUEST";
const char* password = "";

const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;

const char* topic_temp = "mackenzie/ilhas-calor/temperatura";
const char* topic_umid = "mackenzie/ilhas-calor/umidade";
const char* topic_bomba = "mackenzie/ilhas-calor/bomba";
const char* topic_cmd = "mackenzie/ilhas-calor/comando";

DHT dht(DHTPIN, DHTTYPE);

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
  String msg = "";

  for (unsigned int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }

  Serial.print("Comando recebido: ");
  Serial.println(msg);

  unsigned long inicio = millis();

  if (msg == "LIGAR") {
    digitalWrite(RELAY_PIN, HIGH);
    Serial.println("BOMBA LIGADA REMOTAMENTE");
  }

  if (msg == "DESLIGAR") {
    digitalWrite(RELAY_PIN, LOW);
    Serial.println("BOMBA DESLIGADA REMOTAMENTE");
  }

  unsigned long fim = millis();

  Serial.print("Tempo comando->atuador: ");
  Serial.print(fim - inicio);
  Serial.println(" ms");
}

void conectarWiFi() {
  Serial.print("Conectando WiFi");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi conectado");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void conectarMQTT() {

  while (!client.connected()) {

    Serial.print("Conectando MQTT...");

    if (client.connect("ESP32_IlhasCalor")) {

      Serial.println(" conectado");

      client.subscribe(topic_cmd);

    } else {

      Serial.print(" erro=");
      Serial.println(client.state());

      delay(2000);
    }
  }
}

void setup() {

  Serial.begin(115200);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  dht.begin();

  conectarWiFi();

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  Serial.println("Sistema iniciado");
}

void loop() {

  if (!client.connected()) {
    conectarMQTT();
  }

  client.loop();

  float temp = dht.readTemperature();
  float umid = dht.readHumidity();

  if (isnan(temp) || isnan(umid)) {
    Serial.println("Erro ao ler DHT22");
    delay(2000);
    return;
  }

  Serial.print("Temperatura: ");
  Serial.print(temp);
  Serial.print(" °C | Umidade: ");
  Serial.print(umid);
  Serial.println(" %");

  unsigned long inicioSensor = millis();

  client.publish(topic_temp, String(temp).c_str());
  client.publish(topic_umid, String(umid).c_str());

  unsigned long fimSensor = millis();

  Serial.print("Tempo sensor->MQTT: ");
  Serial.print(fimSensor - inicioSensor);
  Serial.println(" ms");

  if (temp > TEMP_CRITICA) {

    digitalWrite(RELAY_PIN, HIGH);

    client.publish(topic_bomba, "LIGADA");

    Serial.println("BOMBA LIGADA");

  } else {

    digitalWrite(RELAY_PIN, LOW);

    client.publish(topic_bomba, "DESLIGADA");

    Serial.println("BOMBA DESLIGADA");
  }

  delay(2000);
}