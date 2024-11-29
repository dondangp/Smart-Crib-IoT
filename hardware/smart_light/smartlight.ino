#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>


//Define your Wi-Fi credentials
const char* ssid = "SpectrumSetup-3928";
const char* password = "motorsecurity673";
//const char* ssid = "LuisiPhone";
//const char* password = "delrioluis";

// Define the server URL
#define SERVER_URL "http://13.88.157.6:65432"

// Define pin and number of NeoPixels
#define PIN 15
#define NUMPIXELS 16

// Create a NeoPixel object
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// Variable to hold the total sum from the server
int total = 0;
int id = 1; // means we are a device 
int device = 1; //means we are the smartLight device

// Fixed number to send
const int fixedNumber = 0; // Change this to the value you want to send



void setup() {
  Serial.begin(115200);
  strip.begin();
  strip.show(); // Initialize all pixels to off

  // Connect to Wi-Fi
  //WiFi.begin(ssid, password);
  Serial.print("Ethan ");
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  // Prepare JSON data to send to the server
  //add device data here 
  //String data = "{\"id\": " + String(id) + "}";
  String data = "{\"id\": " + String(id) + ", \"device\": " + String(device) + "}";

  HTTPClient http;
  http.begin(SERVER_URL); // Specify the URL
  http.addHeader("Content-Type", "application/json"); // Specify content-type

  int httpResponseCode = http.POST(data); // Make the POST request

  if (httpResponseCode > 0) {
    String response = http.getString(); // Get the response payload

    // Parse JSON response
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, response);
    JsonObject obj = doc.as<JsonObject>();

    if (obj.containsKey("brightness") && obj["brightness"].is<int>()) {
      total = obj["brightness"].as<int>();
      Serial.print("Current light level from server: ");
      Serial.println(total);

      //brightness must be between 0 and 255
      if (total > 255){
        total = 255;
      }
      else if (total < 0){
        total = 0;
      }
      for (int i = 0; i < strip.numPixels(); i++){
        strip.setPixelColor(i, strip.Color(total, total, total));
      }
      strip.show();
    } else {
      Serial.println("Total sum not found or not a number");
    }
  } else {
    Serial.printf("Error code: %d\n", httpResponseCode);
  }

  http.end(); // Free resources
  delay(1000); // Wait for 10 seconds before sending the next request
}
