#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include "secrets.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"
#include <Arduino.h>

#define REPORTING_PERIOD_MS     1000

#define LM35_PIN 34

 
#define AWS_IOT_PUBLISH_TOPIC   "esp32/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp32/sub"

PulseOximeter pox;
unsigned char spo2;
float heartrate;
int sensorValue;
float temperature;
uint32_t tsLastReport = 0;

WiFiClientSecure net = WiFiClientSecure();
PubSubClient client(net);

void connectAWS()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
 
  Serial.println("Connecting to Wi-Fi");
 
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.print("wifi connected .........................");
  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);
 
  // Connect to the MQTT broker on the AWS endpoint we defined earlier
  client.setServer(AWS_IOT_ENDPOINT, 8883);
 
  // Create a message handler
  client.setCallback(messageHandler);
 
  Serial.println("Connecting to AWS IOT");
 
  while (!client.connect(THINGNAME))
  {
    Serial.print(".");
    delay(100);
  }
 
  if (!client.connected())
  {
    Serial.println("AWS IoT Timeout!");
    return;
  }
 
  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
 
  Serial.println("AWS IoT Connected!");
}
 
void publishMessage()
{
  StaticJsonDocument<200> doc;
  doc["temperature"] = temperature;
  doc["heartrate"] = heartrate;
  doc["spO2"] = spo2;
  

  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to client

  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}

void messageHandler(char* topic, byte* payload, unsigned int length)
{
  Serial.print("incoming: ");
  Serial.println(topic);
 
  StaticJsonDocument<200> doc;
  deserializeJson(doc, payload);
  const char* message = doc["message"];
  Serial.println(message);
}
 


// Callback (registered below) fired when a pulse is detected
// calls this function on beat detectect event/ intrupt if enabled in setup
void onBeatDetected()
{
    Serial.println("Beat!");
}



void setup()
{
  
  Serial.begin(115200);
  connectAWS();
  Serial.print("Initializing pulse oximeter..");

    // Initialize the PulseOximeter instance
    if (!pox.begin()) {
        Serial.println("FAILED");
       // while(1);
    } else {
        Serial.println("SUCCESS");
    }

    // The default current for the IR LED is 50mA and it could be changed
    //  by uncommenting the following line. Check MAX30100_Registers.h for all the
    //  available options.
    // pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);

    // Register a callback for the beat detection
  // pox.setOnBeatDetectedCallback(onBeatDetected);


  
}

void loop()
{ 
    pox.update();
    // Make sure to call update as fast as possible
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    // reading heatrate and SpO2

      heartrate = pox.getHeartRate();
      spo2 = pox.getSpO2();
      tsLastReport = millis();


//      reading temprature values   
      sensorValue = analogRead(LM35_PIN);
      temperature = (sensorValue / 4095.0) * 3300.0; // Convert to mV
      temperature = -(temperature - 500.0) / 10.0; // LM35 linear temperature formula


//     print data to serial monitor
      Serial.print(F("\n Temperature: "));
      Serial.print(temperature);

      Serial.print(F("\n spo2: "));
      Serial.print(spo2);

      Serial.print(F("\n heartrate: "));
      Serial.print(heartrate);
//      Publish the sensor data to AWS IoT Core

     publishMessage();

    // client.loop();

    }
}


