#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define LED_PIN D7
 
char ssid[] = "/Your SSID\";
char pass[] = "/Your Password\";

char mqtt_server[] = "iot.eclipse.org";

WiFiClient espClient;
PubSubClient client(espClient);


void mqttCallback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) 
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if ((char)payload[0] == '1') 
  {
    Serial.println("Turning on the light");
    digitalWrite(LED_PIN,HIGH);
  } else 
  {
    Serial.println("Turning off the light");
    digitalWrite(LED_PIN,LOW);
  }
}


void mqttConnect() 
{
  while (!client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";
   
    clientId += String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str())) 
    {
      Serial.println("connected");
      client.subscribe("mqtteclipse/cmd");
    } else 
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() 
{
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(115200);
  WiFi.begin(ssid,pass);

  client.setServer(mqtt_server, 1883);
  client.setCallback(mqttCallback);
}

void loop() 
{
  if (!client.connected()) 
  {
    mqttConnect();
  }

  client.loop();
}
