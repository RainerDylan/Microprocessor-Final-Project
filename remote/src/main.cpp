#include <Arduino.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <nRF24L01.h>
#include <RF24.h>

#define button1 23 //right
#define button2 22 //left
#define button3 25 //backward
#define button4 26 //boost
#define button5 24 //brake
#define battery A1
#define speed A0 
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

RF24 radio(48, 49); // CE, CSN

const byte address[7] = "00001";

void setup() {
  pinMode(22, INPUT_PULLUP);
  pinMode(23, INPUT_PULLUP);
  pinMode(24, INPUT_PULLUP);
  pinMode(25, INPUT_PULLUP);
  pinMode(26, INPUT_PULLUP);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();
  radio.enableDynamicPayloads();
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  
}

void loop() {

  int message[7];
  message[0] = analogRead(speed);
  message[1] = analogRead(battery);
  message[2] = !digitalRead(button1);
  message[3] = !digitalRead(button2);
  message[4] = !digitalRead(button3);
  message[5] = !digitalRead(button4);
  message[6] = !digitalRead(button5);
  
  int replyData[1];
  // radio.write(&buttonState, sizeof(buttonState));
  //radio.write(&joyx, sizeof(joyx));
  // radio.write(&message, sizeof(message));
  radio.stopListening();
  bool ok = radio.write(&message, sizeof(message)); // Send control data
  delayMicroseconds(150);


  if (ok) { // If send was successful
    if (radio.isAckPayloadAvailable()) {
      radio.read(&replyData, sizeof(replyData)); // Read the reply data
      // Now replyData[0] = distance
    }
  }

  radio.startListening(); // Ready for next cycle

  // Then draw on OLED like before:
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);

  display.print(F("Speed:"));
  display.println(analogRead(speed));
  display.print(F("Distance:"));
  display.print(replyData[0]);
  display.println(F(" cm"));

  display.display();

  delay(5);
}
