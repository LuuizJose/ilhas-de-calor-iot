#include <DHT.h>
#include <WiFi.h>
#include <PubSubClient.h>

#define DHTPIN 4
#define DHTTYPE DHT22
#define LED_PIN 5
#define TEMP_CRITICA 30.0

const char* ssid = "Wokwi-GUEST";
const char* password = "";

const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;

const char* topic_temp = "mackenzie/ilhas-calor/temperatura";
const char* topic_umid = "mackenzie/ilhas-calor/umidade";
const char* topic_luz = "mackenzie/ilhas-calor/bomba";
const char* topic_cmd = "mackenzie/ilhas-calor/comando";

DHT dht(DHTPIN, DHTTYPE);
WiFiClient espClient;
PubSubClient client(espClient);

bool modoManual = false;
bool luzLigada = false;

void ligarLuz() {
  digitalWrite(LED_PIN, HIGH);
  luzLigada = true;
}

void desligarLuz() {
  digitalWrite(LED_PIN, LOW);
  luzLigada = false;
}

void callback(char* topic, byte* payload, unsigned int length) {
  String msg = "";

  for (unsigned int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }

  msg.trim();
  msg.toUpperCase();

  Serial.print("Comando recebido: ");
  Serial.println(msg);

  if (msg == "LIGAR") {
    modoManual = true;
    ligarLuz();
    client.publish(topic_luz, "LIGADA");
  } 
  else if (msg == "DESLIGAR") {
    modoManual = true;
    desligarLuz();
    client.publish(topic_luz, "DESLIGADA");
  } 
  else if (msg == "AUTO") {
    modoManual = false;
    Serial.println("Modo automatico ativado");
  }
}

void conectarWiFi() {
  Serial.print("Conectando WiFi");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi conectado");
}

void conectarMQTT() {
  while (!client.connected()) {
    Serial.print("Conectando MQTT...");

    String clientId = "ESP32_IlhasCalor_";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println(" conectado");
      client.subscribe(topic_cmd);
      Serial.println("Inscrito no topico de comando");
    } else {
      Serial.print(" erro=");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  desligarLuz();

  dht.begin();

  conectarWiFi();

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  conectarMQTT();

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
  Serial.print(" C | Umidade: ");
  Serial.print(umid);
  Serial.println(" %");

  client.publish(topic_temp, String(temp).c_str());
  client.publish(topic_umid, String(umid).c_str());

  if (!modoManual) {
    if (temp > TEMP_CRITICA) {
      ligarLuz();
      client.publish(topic_luz, "LIGADA");
      Serial.println("LUZ LIGADA AUTOMATICAMENTE");
    } else {
      desligarLuz();
      client.publish(topic_luz, "DESLIGADA");
      Serial.println("LUZ DESLIGADA AUTOMATICAMENTE");
    }
  } else {
    Serial.println("MODO MANUAL ATIVO");
  }

  delay(2000);
}
