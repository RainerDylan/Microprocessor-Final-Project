#include <Arduino.h>

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define right 23
#define rightL 29
#define left 22
#define leftL 27
#define relay1 25 //backward
#define relay2 26 //boost
#define mosfet 44 //speed
#define relay3 24 //brake
#define beeper 28
#define trigPin 21
#define echoPin 20

//#define relay3 4

RF24 radio(48, 49); // CE, CSN
boolean buttonState = 0;
int joyx = 0;
int joyy = 0;
int tjoyx = 0;
int tjoyy = 0;
int battery = 0;

//Servo myServo;

const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);
  pinMode(left, OUTPUT);
  pinMode(right, OUTPUT);
  pinMode(leftL, OUTPUT);
  pinMode(rightL, OUTPUT);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(mosfet, OUTPUT);
  pinMode(beeper, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  //tjoyx = 510;
  //myServo.attach(5);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();
  radio.enableDynamicPayloads();
  radio.enableAckPayload();

}

long readUltrasonic() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 20000); // timeout after 20ms
  if (duration == 0) {
    return 999; // no echo detected
  }
  long distanceCm = duration * 0.034 / 2;
  return distanceCm;
}

void loop() {
  while (!radio.available());
  int angleV = 0;
  int message[7];

  radio.read(&message, sizeof(message));

  // map it to the range of the analog out:
  tjoyx = map(message[0], 0, 1023, 0, 255);
  long distance = readUltrasonic();

  digitalWrite(right, message[2]);
  digitalWrite(rightL, message[2]);
  digitalWrite(left, message[3]);
  digitalWrite(leftL, message[3]);
  digitalWrite(relay1, message[4]);
  digitalWrite(relay2, message[5]);
  if(distance < 20 && message[4] == 0){
    analogWrite(mosfet, 0);
    digitalWrite(relay3, 0);
    digitalWrite(rightL, 1);
    digitalWrite(leftL, 1);

  }else{
    digitalWrite(relay3, message[6]);
    analogWrite(mosfet, tjoyx);

  }
  
  if((message[2]==1 && message[3]==1)||(distance < 20 && message[4] == 0)){
    digitalWrite(beeper, 1);
  }else{
    digitalWrite(beeper, 0);
  }

  // long distance = readUltrasonic();
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Now prepare and attach the reply
  int replyData[1];
  replyData[0] = distance;
  // Attach the reply to the ACK
  radio.writeAckPayload(0, &replyData, sizeof(replyData));

  // delay(5); 
}
