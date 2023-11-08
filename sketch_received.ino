#include <WiFiNINA.h>
#include <Wire.h>
#include <BH1750.h>

// Define Wi-Fi credentials
char ssid[] = "Diya's iPhone";      // Wi-Fi SSID
char pass[] = "123456";  // Wi-Fi password

WiFiClient client; 
BH1750 sensor;

// IFTTT parameters
char HOST_NAME[] = "maker.ifttt.com";
String PATH_NAME = "/trigger/SunlightStatus/json/with/key/gyMMgpAyCGjgoohKz54SpA4VnJT3eZTQS0U2PEzYM8b";

void setup() {
  // Initialize the serial communication for debugging
  Serial.begin(9600);
  while (!Serial);  // Wait for Serial to be ready

  // Connect to Wi-Fi network
  WiFi.begin(ssid, pass);
  Wire.begin();

  // Wait for a Wi-Fi connection to be established
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi"); 
  sensor.begin();  // Initialize the BH1750 sensor
}

void loop() {
  // Read the light level from the sensor
  float lux = sensor.readLightLevel();

  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");

  if (lux > 100 ) {
    // Prepare the query string for the IFTTT webhook
    String queryString = "?value1=" + String(lux);

    // Making an HTTP request to the IFTTT webhook
    if (client.connect(HOST_NAME, 80)) {
      // Send HTTP header
      client.println("GET " + PATH_NAME + queryString + " HTTP/1.1");
      client.println("Host: " + String(HOST_NAME));
      client.println("Connection: close");
      client.println(); 
      // End HTTP header

      // Read and print the response from the server
      while (client.connected()) {
        if (client.available()) {
          // Read an incoming byte from the server and print it to the Serial Monitor
          char c = client.read();
          Serial.print(c);
        }
      }
      // The server is disconnected, therefore, we stop the client
      client.stop();
      Serial.println();
      Serial.println("Disconnected");
    }
  }

  delay(4000);  // Delay for 4 seconds before the next loop iteration
}
