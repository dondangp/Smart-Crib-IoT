#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "Adafruit_Keypad.h"

// WiFi credentials
const char* ssid = "LuisiPhone";
const char* password = "delrioluis";
#define SERVER_URL "http://13.88.157.6:65432"
int id = 1;
int device = 3;
int doorlock = 0;

// Keypad pin definitions
#define KEYPAD_PID1824
#define R1    6
#define R2    9
#define R3    10
#define R4    11
#define C1    3
#define C2    4
#define C3    5

#include "keypad_config.h"

// Keypad configuration
char Luis_password[5] = "1234";
char input_password[5] = "____";
int current_index = 0;
Adafruit_Keypad customKeypad = Adafruit_Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Relay pins for door lock
#define relay_1 13
#define relay_2 12

// Task handles
TaskHandle_t TaskKeypadHandle = NULL;
TaskHandle_t TaskServerHandle = NULL;

void setup() {
  Serial.begin(9600);

  // Initialize relays
  pinMode(relay_1, OUTPUT);
  pinMode(relay_2, OUTPUT);

  // Start Wi-Fi
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  

  // Initialize keypad
  customKeypad.begin();

  // Create two tasks: one for keypad input and one for server updates
  xTaskCreatePinnedToCore(
    TaskKeypad,    // Task function
    "Keypad Task", // Name of the task
    2048,          // Stack size (bytes)
    NULL,          // Parameter to pass to task (none in this case)
    1,             // Priority of the task
    &TaskKeypadHandle, // Task handle
    0              // Core to run the task on (0 or 1)
  );


  xTaskCreatePinnedToCore(
    TaskServer,    // Task function
    "Server Task", // Name of the task
    4096,          // Stack size (bytes)
    NULL,          // Parameter to pass to task (none in this case)
    1,             // Priority of the task
    &TaskServerHandle, // Task handle
    1              // Core to run the task on (0 or 1)
  );
}


void loop() {
  // Nothing needed here; the tasks are handling everything.
}

// Task for handling keypad input
void TaskKeypad(void *pvParameters) {
  while (true) {
    customKeypad.tick();  // Update keypad buffer
    if (customKeypad.available()) {
      keypadEvent e = customKeypad.read();
      if (e.bit.EVENT == KEY_JUST_PRESSED) {
        handleKeypadInput((char)e.bit.KEY);  // Process keypress
      }
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);  // Delay to avoid hogging CPU
  }
}

// Task for handling server updates
void TaskServer(void *pvParameters) {
  while (true) {
    // Prepare and send server request
    String data = "{\"id\": " + String(id) + ", \"device\": " + String(device) + "}";
    HTTPClient http;
    http.begin(SERVER_URL);
    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.POST(data);
    if (httpResponseCode > 0) {
      String response = http.getString();
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, response);
      JsonObject obj = doc.as<JsonObject>();

      if (obj.containsKey("doorlock") && obj["doorlock"].is<int>()) {
        doorlock = obj["doorlock"].as<int>();
        if (doorlock == 1) lock_door();
        else if (doorlock == 0) unlock_door();
      }
    }
    http.end();
    vTaskDelay(5000 / portTICK_PERIOD_MS);  // Wait 5 seconds before sending another request
  }
}

// Handle keypad input logic
void handleKeypadInput(char key) {
  if (key != '*' && key != '#') {  // Password key
    if (current_index >= 4) {
      resetPassword();
      Serial.println("Password Too Long");
    } else {
      input_password[current_index] = key;
      current_index++;
      Serial.println(input_password);
    }
  } else if (key == '*') {
    resetPassword();
    Serial.println("Password Cleared");
  } else if (key == '#') {
    checkPassword();
    
  }
}

// Reset password entry
void resetPassword() {
  // memset(input_password, '_', sizeof(input_password));
  // current_index = 0;
  input_password[0] = '_';
  input_password[1] = '_';
  input_password[2] = '_';
  input_password[3] = '_';
  current_index = 0;

  

}

// Check if the entered password matches
void checkPassword() {
  if (strcmp(input_password, Luis_password) == 0) {
    Serial.println("Correct Password");
    unlock_door();
    delay(10000);  // Allow time to open door

    lock_door();
  } else {
    Serial.println("Incorrect Password");
  }
  resetPassword();
  Serial.println("Password Cleared");

}

// Lock and unlock door functions
void lock_door() {
  digitalWrite(relay_1, HIGH);
  delay(5000);
  digitalWrite(relay_1, LOW);
}

void unlock_door() {
  digitalWrite(relay_2, HIGH);
  delay(5000);
  digitalWrite(relay_2, LOW);
}
