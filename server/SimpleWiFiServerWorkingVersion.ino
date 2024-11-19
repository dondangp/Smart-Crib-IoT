/*
 WiFi Web Server LED Blink

 A simple web server that lets you blink an LED via the web.
 This sketch will print the IP address of your WiFi Shield (once connected)
 to the Serial monitor. From there, you can open that address in a web browser
 to turn on and off the LED on pin 5.

 If the IP address of your shield is yourAddress:
 http://yourAddress/H turns the LED on
 http://yourAddress/L turns it off

 This example is written for a network using WPA2 encryption. For insecure
 WEP or WPA, change the Wifi.begin() call and use Wifi.setMinSecurity() accordingly.

 Circuit:
 * WiFi shield attached
 * LED attached to pin 5

 created for arduino 25 Nov 2012
 by Tom Igoe

ported for sparkfun esp32
31.01.2017 by Jan Hendrik Berlin

 */

#include <WiFi.h>
#include <Adafruit_NeoPixel.h>

#define PIN 15
#define NUMPIXELS 16

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

const char *ssid = "SpectrumSetup-3928";
const char *password = "motorsecurity673";

NetworkServer server(80);
int b = 25;
int color = 0; // 0 = off, 1 = white, 2 = green, 3 = blue, 4 = red

void setup() {


  Serial.begin(115200);
  //pinMode(16, OUTPUT);  // set the LED pin mode


  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();


  //setuo light to start as white brightness of 25
  strip.begin();
  strip.setBrightness(25);
  strip.setPixelColor(2, 25, 0, 0);
  strip.setPixelColor(6, 25, 0, 0);
  strip.setPixelColor(10, 25, 0, 0);
  strip.setPixelColor(14, 25, 0, 0);
  strip.show();
  strip.show();

}

void loop() {
  NetworkClient client = server.accept();  // listen for incoming clients

  if (client) {                     // if you get a client,
    Serial.println("New Client.");  // print a message out the serial port
    String currentLine = "";        // make a String to hold incoming data from the client
    while (client.connected()) {    // loop while the client's connected
      if (client.available()) {     // if there's bytes to read from the client,
        char c = client.read();     // read a byte, then
        Serial.write(c);            // print it out the serial monitor
        if (c == '\n') {            // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("Click <a href=\"/O\">here</a> to turn the LED off pin 15 off.<br>");
            client.print("Click <a href=\"/W\">here</a> to turn the LED white pin 15 on.<br>");
            client.print("Click <a href=\"/G\">here</a> to turn the LED green pin 15 on.<br>");
            client.print("Click <a href=\"/B\">here</a> to turn the LED blue pin 15 on.<br>");
            client.print("Click <a href=\"/R\">here</a> to turn the LED red pin 15 on.<br>");
            client.print("Click <a href=\"/U\">here</a> to turn the LED brightness UP pin 15 on.<br>");
            client.print("Click <a href=\"/D\">here</a> to turn the LED brightness DOWN 15 on.<br>");


            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {  // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":

   
        if (currentLine.endsWith("GET /O")) {   //turn all LED's off
          turnOff();
          color = 0;
        }
        if (currentLine.endsWith("GET /W")) {   //turn on WHITE LED
          turnOff();
          turnWhite();
          color = 1;
        }
        if (currentLine.endsWith("GET /G")) {   //turn on GREEN LED
          turnOff();
          turnGreen();
          color = 2;
        }
        if (currentLine.endsWith("GET /B")) {   //turn on BLUE LED
          turnOff();
          turnBlue();
          color = 3;
        }
        if (currentLine.endsWith("GET /R")) {   //turn on RED LED
          turnOff();
          turnRed();
          color = 4;
       
        }
        if (currentLine.endsWith("GET /U")) {   // turn UP BRIGHTNESS
          //turns brightness up by 25
          b += 25;

          //cycle the lights with updated brightness
          if(color == 0){
            //do nothing
          }
          if(color == 1){
            turnWhite();
          }
          else if(color == 2){
            turnGreen();
          }
          else if(color == 3){
            turnBlue();
          }
          else if(color == 4){
            turnRed();
          }
        }
        if (currentLine.endsWith("GET /D")) {   //turns DOWN BRIGHTNESS
          //turns down brightness
          b -= 25;


          //cycle the lights with updated brightness
          if(color == 0){
            //do nothing
          }
          else if(color == 1){
            turnWhite();
          }
          else if(color == 2){
            turnGreen();
          }
          else if(color == 3){
            turnBlue();
          }
          else if(color == 4){
            turnRed();
          }
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}

void turnOff(){   //LED sequence
  for (int k = 0; k < NUMPIXELS; k ++){
    strip.setPixelColor(k, 0, 0, 0);
  }
  strip.show();
}
void turnWhite(){ //WHITE LED sequence
  strip.setPixelColor(2, b, 0, 0);
  strip.setPixelColor(6, b, 0, 0);
  strip.setPixelColor(10, b, 0, 0);
  strip.setPixelColor(14, b, 0, 0);
  strip.show();
}
void turnGreen(){ //GREEN LED sequence
  strip.setPixelColor(1, b, 0, 0);
  strip.setPixelColor(5, b, 0, 0);
  strip.setPixelColor(9, b, 0, 0);
  strip.setPixelColor(13, b, 0, 0);
  strip.show();
}
void turnBlue(){  //BLUE LED sequence
  strip.setPixelColor(3, b, 0, 0);
  strip.setPixelColor(7, b, 0, 0);
  strip.setPixelColor(11, b, 0, 0);
  strip.setPixelColor(15, b, 0, 0);
  strip.show();
}
void turnRed(){   //RED LED sequence
  strip.setPixelColor(0, b, 0, 0);
  strip.setPixelColor(4, b, 0, 0);
  strip.setPixelColor(8, b, 0, 0);
  strip.setPixelColor(12, b, 0, 0);
  strip.show();
}
