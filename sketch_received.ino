#include <WiFiNINA.h>
#include <BH1750.h>
#include <Wire.h>

char ssid[] = "Diya's iPhone";      // Replace with your Wi-Fi SSID
char pass[] = "12345678";  // Replace with your Wi-Fi password

WiFiClient client; 
BH1750 lightMeter;

char HOST_NAME[] = "maker.ifttt.com";
String PATH_NAME = "/trigger/light/json/with/key/ofHfOy24U8Z572xy8SHdtZm97CE8nfjBDo4YKZq9o4J";

void setup() {
  Serial.begin(9600);
  while (!Serial);

  WiFi.begin(ssid, pass);
  Wire.begin();

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi"); 
  lightMeter.begin();
}

void loop() {
  float lux = lightMeter.readLightLevel();

  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");

  if (lux > 100 ) {
    String queryString = "?value1=" + String(lux);
   
    //Make an HTTP request to the IFTTT webhook:
    if (client.connect(HOST_NAME, 80)) {
      // Send HTTP header
      client.println("GET " + PATH_NAME + queryString + " HTTP/1.1");
      client.println("Host: " + String(HOST_NAME));
      client.println("Connection: close");
      client.println(); // End HTTP header

      while (client.connected()) {
        if (client.available()) {
          // Read an incoming byte from the server and print it to the Serial Monitor:
          char c = client.read();
          Serial.print(c);
        }
      }
      // The server's disconnected, stop the client:
      client.stop();
      Serial.println();
      Serial.println("Disconnected");
    }

  }
  delay(3000);
}
