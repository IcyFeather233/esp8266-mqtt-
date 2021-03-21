#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

const char* ssid = "";
const char* password = "";
const char* mqttServer = "";
const int mqttPort = 1883;

const char* lightTopic = "lightTopic";

WiFiClient espClient;
PubSubClient client(espClient);

int lightPin = D4;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println(topic);
  String command = "";
  for (int i = 0; i < length; i++) {
    command += (char)payload[i];
  }
  Serial.println(command);
  handlePayload(String(topic), command); 
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    //详细参数说明请查看文档
    if (client.connect("ESP8266 Client"))
    {
      Serial.println("connected");
      client.subscribe(lightTopic, 1);
    }
    else
    {
       Serial.print("failed, rc=");
       Serial.print(client.state());
       Serial.println(" try again in 5 seconds");
       delay(5000);
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

//处理命令
String handlePayload(String topic, String payload) {
  if (String(lightTopic).equals(topic)) {
    //light command
    if (String("lightOn").equals(payload)) {
      digitalWrite(lightPin, HIGH);
    } else if (String("lightOff").equals(payload)) {
      digitalWrite(lightPin, LOW);
    } 
  }
}

