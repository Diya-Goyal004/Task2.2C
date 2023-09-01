#include <WiFiNINA.h>
#include <Wire.h>
#include <secrets.h>  // Include Wi-Fi credentials in this file

#define THRESHOLD_LUX 1000  // Set lux level threshold for sunlight detection

// Define Wi-Fi credentials
char ssid[] = SECRET_SSID;      // Replace with your Wi-Fi network SSID
char pass[] = SECRET_PASSWORD;  // Replace with your Wi-Fi network password

WiFiClient client;

char   HOST_NAME[] = "maker.ifttt.com"; // IFTTT Maker Webhooks host
String PATH_NAME   = "sketch_received";  // Define the event name to trigger in IFTTT (CHANGE as needed)
String queryString = "?value1=57&value2=25"; // Include additional data to send to IFTTT

BH1750 lightMeter; // Initialize the BH1750 light sensor

bool isSunlightPresent = false;
int sunlightEnterEventSent = false;
int sunlightLeaveEventSent = false;

void setup() {
  // Initialize Wi-Fi connection
  WiFi.begin(ssid, pass);
  Serial.begin(9600);
  while (!Serial);

  // Connect to the IFTTT server on port 80
  if (client.connect(HOST_NAME, 80)) {
    Serial.println("Connected to the IFTTT server");
  } else {
    Serial.println("Connection to the IFTTT server failed");
  }

  // Initialize the BH1750 light sensor
  Wire.begin();
  lightMeter.begin();
}

void loop() {
  if (Serial.read() == 's') {
    // Read the current sunlight intensity
    float lux = lightMeter.readLightLevel();

    if (lux > THRESHOLD_LUX && !isSunlightPresent) {
      if (!sunlightEnterEventSent) {
        // Send an IFTTT notification indicating sunlight entering the terrarium
        // (Call the sendIFTTTNotification function here)
        sunlightEnterEventSent = true;
        sunlightLeaveEventSent = false;
      }
      isSunlightPresent = true;
    } else if (lux <= THRESHOLD_LUX && isSunlightPresent) {
      if (!sunlightLeaveEventSent) {
        // Send an IFTTT notification indicating sunlight leaving the terrarium
        // (Call the sendIFTTTNotification function here)
        sunlightLeaveEventSent = true;
        sunlightEnterEventSent = false;
      }
      isSunlightPresent = false;
    }

    // Make an HTTP request to IFTTT for notification (Code for sending data to IFTTT)
    
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.print(c);
      }
    }

    // Disconnect from the IFTTT server
    client.stop();
  }
}
