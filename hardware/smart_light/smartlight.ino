#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>

// WiFi credentials
const char* ssid = "iPhoneWifi";
const char* password = "password";

#define SERVER_URL "http://13.88.157.6:65432"

//LED ring light
#define PIN 15
#define NUMPIXELS 16
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int total = 0;
int id = 1; // means we are a device 
int device = 1; //means we are the smartLight device
SemaphoreHandle_t stripMutex;

// Task handles
TaskHandle_t TaskSensorHandle = NULL;
TaskHandle_t TaskServerHandle = NULL;

void setup() {
    pinMode(16, INPUT);
    Serial.begin(9600);
    strip.begin();
    strip.show();

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("WiFi connected");

    stripMutex = xSemaphoreCreateMutex();
    if (stripMutex == NULL) {
        Serial.println("Failed to create mutex");
        while (true);
    }

    // Create tasks
    xTaskCreatePinnedToCore(
        TaskSensor,
        "Sensor Task",
        2048,
        NULL,
        1,
        &TaskSensorHandle,
        0
    );

    xTaskCreatePinnedToCore(
        TaskServer,
        "Server Task",
        4096,
        NULL,
        1,
        &TaskServerHandle,
        1
    );
}

void loop() {
}

// Sensor Task
void TaskSensor(void *pvParameters) {
    while (true) {
        l2();
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

// Server Task
void TaskServer(void *pvParameters) {
    while (true) {
        l1();
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

// server update fetching
void l1() {
    String data = "{\"id\": " + String(id) + ", \"device\": " + String(device) + "}";

    HTTPClient http;
    http.begin(SERVER_URL);
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(data);

    //break down json
    if (httpResponseCode > 0) {
        String response = http.getString();

        DynamicJsonDocument doc(1024);
        deserializeJson(doc, response);
        JsonObject obj = doc.as<JsonObject>();

        if (obj.containsKey("brightness") && obj["brightness"].is<int>()) {
            total = obj["brightness"].as<int>();
            total = constrain(total, 0, 255); // Constrain brightness to 0-255

            if (xSemaphoreTake(stripMutex, portMAX_DELAY)) { // Wait for the mutex
                for (int i = 0; i < strip.numPixels(); i++) {
                    strip.setPixelColor(i, strip.Color(total, total, total));
                }
                strip.show();
                xSemaphoreGive(stripMutex); // Release the mutex
            }
        } else {
            Serial.println("Brightness not found or invalid.");
        }
    } else {
        Serial.printf("HTTP error: %d\n", httpResponseCode);
    }

    http.end();
    delay(500); // delay between server requests
}

// Sensor function
void l2() {
    byte state = digitalRead(16);

    if (state == 1) { // Motion detected
        Serial.println("Motion detected. Turning on lights...");
        if (xSemaphoreTake(stripMutex, portMAX_DELAY)) { // Wait for the mutex
            // Turn on the light strip
            for (int i = 0; i < strip.numPixels(); i++) {
                strip.setPixelColor(i, strip.Color(100, 100, 100));
            }
            strip.show();

            // Keep lights on for 5 seconds
            vTaskDelay(5000 / portTICK_PERIOD_MS);

            // Turn off the light strip
            for (int i = 0; i < strip.numPixels(); i++) {
                strip.setPixelColor(i, strip.Color(0, 0, 0));
            }
            strip.show();

            xSemaphoreGive(stripMutex); // Release the mutex
            Serial.println("Lights turned off.");
        }
    } else if (state == 0) {
        Serial.println("No motion detected.");
    }
}
