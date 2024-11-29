// Use this example with the Adafruit Keypad products.
// You'll need to know the Product ID for your keypad.
// Here's a summary:
//   * PID3844 4x4 Matrix Keypad
//   * PID3845 3x4 Matrix Keypad
//   * PID1824 3x4 Phone-style Matrix Keypad
//   * PID1332 Membrane 1x4 Keypad
//   * PID419  Membrane 3x4 Matrix Keypad
//new
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
const char* ssid = "SpectrumSetup-3928";
const char* password = "motorsecurity673";
#define SERVER_URL "http://13.88.157.6:65432"
int id = 1;
int device = 3;
int doorlock = 0;
//new


#include "Adafruit_Keypad.h"
#include <stdlib.h>
#include <string.h>
// define your specific keypad here via PID
#define KEYPAD_PID1824
// define your pins here
// can ignore ones that don't apply
#define R1    6
#define R2    9
#define R3    10
#define R4    11
#define C1    3
#define C2    4
#define C3    5
// leave this import after the above configuration
#include "keypad_config.h"

//initialize an instance of class NewKeypad
Adafruit_Keypad customKeypad = Adafruit_Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);

#define relay_1 13
#define relay_2 12


char Luis_password[5] = "1234";
char input_password[5] = "____"; //represent a blank password
int current_index = 0;


void setup() {
  //new
 

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
  //new

  pinMode(relay_1, OUTPUT);
  pinMode(relay_2, OUTPUT);
  Serial.begin(9600);
  customKeypad.begin();

}
/*
void loop() {
  // put your main code here, to run repeatedly:
  customKeypad.tick();

  while(customKeypad.available()){
    keypadEvent e = customKeypad.read();
    //Serial.print((char)e.bit.KEY);

    if(e.bit.EVENT == KEY_JUST_PRESSED){  //when button is pressed
      //Serial.println((char)e.bit.KEY);    //print the button pressed
      if((char)e.bit.KEY != '*' && (char)e.bit.KEY != '#'){ //if input is a number
        if(current_index >= 4){
          Serial.println("Password Too Long");
          current_index = 0;
          input_password[0] = '_';
          input_password[1] = '_';
          input_password[2] = '_';
          input_password[3] = '_';
          Serial.println("Password Cleared");
          Serial.println(input_password);
        }
        else{ // otherwise if theres enough space append to input_password
          input_password[current_index] = ((char)e.bit.KEY);  //add number to input_password
          current_index += 1; //traverse index
          Serial.println(input_password);
        }
      }
      else if((char)e.bit.KEY == '*'){ // if '*' is entered clear password
        input_password[0] = '_';
        input_password[1] = '_';
        input_password[2] = '_';
        input_password[3] = '_';
        current_index = 0;
        Serial.println("Password Cleared");
        Serial.println(input_password);
      }
      else if((char)e.bit.KEY == '#'){ // enter/check input_password
        Serial.print("Password Entered: ");
        Serial.println(input_password);
        if (input_password[0] == Luis_password[0]){
          if (input_password[1] == Luis_password[1]){
            if (input_password[2] == Luis_password[2]){
              if (input_password[3] == Luis_password[3]){
                Serial.println("Correct Password");
                current_index = 10; //use this as a flag 

                lock_door();
                unlock_door();
              }
            }
          }
        }
        if (current_index != 10){
          Serial.println("Incorrect Password");
        }


        //Serial.println(input_password);
        //clear password
        input_password[0] = '_';
        input_password[1] = '_';
        input_password[2] = '_';
        input_password[3] = '_';
        Serial.println("Password Cleared");
        Serial.println(input_password);
        current_index = 0;
      }

    }
    else if(e.bit.EVENT == KEY_JUST_RELEASED){  //when button releases print current input password state
      //Serial.print("released : ");
      //Serial.println(input_password);
    }
  }

  delay(10);
}
*/

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

    if (obj.containsKey("doorlock") && obj["doorlock"].is<int>()) {
      doorlock = obj["doorlock"].as<int>();
      Serial.print("Current doorlock from server: ");
      Serial.println(doorlock);

      if (doorlock == 1){
        //lock door
        lock_door();
      }
      else if (doorlock == 0){
        unlock_door();
      }

      // Control NeoPixels based on the total sum
      // if (total % 2 == 0) {
      //   // Even number -> Turn on light
      //   for (int i = 0; i < strip.numPixels(); i++) {
      //     //maxbrightness = 255
      //     strip.setPixelColor(i, strip.Color(100, 100, 100)); // White color
      //   }
      // } else {
      //   // Odd number -> Turn off light
      //   for (int i = 0; i < strip.numPixels(); i++) {
      //     strip.setPixelColor(i, strip.Color(0, 0, 0)); // Turn off
      //   }
      // }

      //brightness must be between 0 and 255
      
    } else {
      Serial.println("Doorlock not found or not a number");
    }
  } else {
    Serial.printf("Error code: %d\n", httpResponseCode);
  }

  http.end(); // Free resources
  //delay(1000); // Wait for 10 seconds before sending the next request
}

void lock_door(){
  digitalWrite(relay_1, HIGH);
  delay(10000);
  digitalWrite(relay_1, LOW);
}

void unlock_door(){
  digitalWrite(relay_2, HIGH);
  delay(10000);
  digitalWrite(relay_2, LOW);
}
