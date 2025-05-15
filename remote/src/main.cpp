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
#define speed A0 
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

RF24 radio(48, 49); // CE, CSN
const byte address[6] = "ABCD1";

unsigned long lastSendTime = 0;
unsigned long lastScreenUpTime = 0;
const unsigned long sendInterval = 30; // Send every 30 millisecond
const unsigned long ScreenUpInterval = 100; // refresh every 100 millisecond
int myValue = 0;
int receivedValue[3];
int message[7];

void setup() {
  Serial.begin(9600);
  pinMode(22, INPUT_PULLUP);
  pinMode(23, INPUT_PULLUP);
  pinMode(24, INPUT_PULLUP);
  pinMode(25, INPUT_PULLUP);
  pinMode(26, INPUT_PULLUP);
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_250KBPS); // Optional: better range
  radio.openWritingPipe(address);
  radio.openReadingPipe(0, address);
  radio.startListening();
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
}

void loop() {
  // Non-blocking send every 'sendInterval' ms
  if (millis() - lastScreenUpTime > sendInterval) {
    lastScreenUpTime = millis();
    sendMyData();
  }

  // Non-blocking refresh screen
  if (millis() - lastSendTime > ScreenUpInterval) {
    lastSendTime = millis();
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);

    display.print(F("Speed:"));
    display.println(receivedValue[1]);
    display.print(F("Distance:"));
    display.print(receivedValue[0]);
    display.println(F(" cm"));
    display.print(F("max Throtel:"));
    display.print(receivedValue[2]);

    display.display();
  }

  // "Interrupt-style" receive check
  if (radio.available()) {
    while (radio.available()) {
      radio.read(&receivedValue, sizeof(receivedValue));
      Serial.print("Received: ");
      Serial.println(receivedValue[0]);
    }
  }
}

void sendMyData() {
  radio.stopListening(); // Must stop listening before writing
  message[0] = analogRead(speed);
  message[2] = !digitalRead(button1);
  message[3] = !digitalRead(button2);
  message[4] = !digitalRead(button3);
  message[5] = !digitalRead(button4);
  message[6] = !digitalRead(button5);

  bool success = radio.write(&message, sizeof(message));
  if (success) {
    Serial.print("Sent: ");
    Serial.println(message[0]);
  } else {
    Serial.println("Send failed");
  }

  radio.startListening(); // Resume listening immediately
}
