#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "Adafruit_Keypad.h"
#include "keypad_config.h"

#define SERVER_URL "http://13.88.157.6:65432"

//keypad config specifics
#define KEYPAD_PID1824
#define R1    6
#define R2    9
#define R3    10
#define R4    11
#define C1    3
#define C2    4
#define C3    5

// actuator cont
#define relay_1 13
#define relay_2 12

// Button pin
#define button_pin 8

// connecting with iphone for demo
const char* ssid = "iPhoneWifi"; 
const char* password = "password";

int id = 1; // id as a device
int device = 3; // we are device 3doorlock
int doorlock = 0; 
int newPassword = 0;  
int appPrevstate = -1;  
int current_index = 0;


char Luis_password[5] = "1234";
char input_password[5] = "____";

Adafruit_Keypad customKeypad = Adafruit_Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);


// task/mutex handles
TaskHandle_t TaskKeypadHandle = NULL;
TaskHandle_t TaskServerHandle = NULL;
TaskHandle_t TaskButtonHandle = NULL;
SemaphoreHandle_t doorLockMutex;

void setup() {
  Serial.begin(9600);

  // relays
  pinMode(relay_1, OUTPUT);
  pinMode(relay_2, OUTPUT);

  // indoor btn
  pinMode(button_pin, INPUT_PULLUP);

  //connect wifi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");

  // Initialize keypad
  customKeypad.begin();

  // Create mutex
  doorLockMutex = xSemaphoreCreateMutex();

  // create/begin tasks
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
    TaskServer,    
    "Server Task", 
    4096,          
    NULL,         
    1,             
    &TaskServerHandle, 
    1              
  );

  xTaskCreatePinnedToCore(
    TaskButton,   
    "Button Task", 
    2048,          
    NULL,          
    1,             
    &TaskButtonHandle, 
    0              
  );
}

void loop() {
  
}

// task for handling keypad input
void TaskKeypad(void *pvParameters) {
  while (true) {
    customKeypad.tick();  // update keypad buffer
    if (customKeypad.available()) {
      keypadEvent e = customKeypad.read();
      if (e.bit.EVENT == KEY_JUST_PRESSED) {
        handleKeypadInput((char)e.bit.KEY);  // process keypress
      }
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);  // slowdown
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

      // break down json
      String response = http.getString();
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, response);
      JsonObject obj = doc.as<JsonObject>();

      if (obj.containsKey("doorlock") && obj["doorlock"].is<int>()) {
        doorlock = obj["doorlock"].as<int>();

        if (doorlock == 1){
          if (doorlock != appPrevstate){
            appPrevstate = 1;
            lock_door();
          }else {
            Serial.println("dont lock");
          }
        }else {
          if (doorlock != appPrevstate){
            appPrevstate = 0;
            unlock_door();
          }else {
            Serial.println("dont unlock");
          }
        }

        
      }

      //update keypad password 
      if (obj.containsKey("newPassword") && obj["newPassword"].is<int>()) {
        newPassword = obj["newPassword"].as<int>();
        sprintf(Luis_password, "%d", newPassword);
        Serial.println(Luis_password);
      }
    }
    http.end();
    vTaskDelay(250 / portTICK_PERIOD_MS);  // Wait before sending another request
  }
}

// Task for button press
void TaskButton(void *pvParameters) {
  while (true) {
    if (digitalRead(button_pin) == LOW) { //button is pressed
      if (xSemaphoreTake(doorLockMutex, portMAX_DELAY) == pdTRUE) {
        Serial.println("Button Pressed: Unlocking door");
        unlock_door();
        delay(5000);  // Keep the door unlocked for 5 seconds
        lock_door();
        xSemaphoreGive(doorLockMutex);
      }
    }
    vTaskDelay(50 / portTICK_PERIOD_MS);  // Debounce 
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
    if (xSemaphoreTake(doorLockMutex, portMAX_DELAY) == pdTRUE) {
      checkPassword();
      xSemaphoreGive(doorLockMutex);
    }
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
  Serial.println("locking");
  digitalWrite(relay_1, HIGH);
  delay(5000);
  digitalWrite(relay_1, LOW);
}

void unlock_door() {
  Serial.println("unlocking");
  digitalWrite(relay_2, HIGH);
  delay(5000);
  digitalWrite(relay_2, LOW);
}

