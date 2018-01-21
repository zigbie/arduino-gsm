#include <SoftwareSerial.h>

SoftwareSerial gsm(10, 12); // RX, TX
int Relay = 8; // Relay on pin 8
const int buttonPin = 13; // button
int state = HIGH; // launched

int buttonState = 0; // button state

unsigned long workTime = 600000; // work time (20 min = 1200000)

unsigned long startTime;  // button ON or Ring
unsigned long currentTime; // millis();

String val = "";
int ch = 0;

void setup() {

  pinMode(Relay, OUTPUT);
  digitalWrite(Relay, state);
  pinMode(buttonPin, INPUT_PULLUP);
  gsm.begin(57600);
  Serial.begin(57600);
  gsm.println("AT+CLIP=1");
}

void loop() {

  if (gsm.available()) {
    while (gsm.available()) {
      ch = gsm.read();
      val += char(ch);
      delay(10);
    }

    if (val.indexOf("RING") > -1) {
      //Serial.println("RING");
      //Serial.println(val);
      if ((val.indexOf("375291234567") > -1) || (val.indexOf("375291234567") > -1)) {
        //  Serial.println("--- MASTER RING DETECTED ---");
        startTime = millis();
        gsm.println("ATH0");
        delay(500);
      } else {
        gsm.println("ATH0");
      }
    }
    val = "";
  }


  buttonState = readButton(buttonPin);

  if (buttonState == LOW) {
    if (state == LOW) {  // button ON, relay ON
      startTime = 0;
    }

    if (state == HIGH) {  // button ON, relay OFF
      startTime = millis();
    }
    delay(500);
  }
  state = runWebasto (startTime);
  digitalWrite(Relay, state);

}

int readButton(int pin)
{
  if (digitalRead(pin) == LOW) {
    delay(100);
    if (digitalRead(pin) == LOW) {
      return LOW;
    }
  }
  return HIGH;
}

int runWebasto (unsigned long startTime) {
  currentTime = millis();
  if (startTime == 0) {
    state  = HIGH;
    return  state;
  }

  if ((currentTime - startTime) > workTime) {      // work: yes or no
    state  = HIGH;
  } else {
    if (startTime > 2000) {
      state  = LOW;
    }
  }

  return  state;
}