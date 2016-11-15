#include <SoftwareSerial.h>

SoftwareSerial gsm(10, 12); // RX, TX
int Relay = 8;
const int buttonPin = 13;
int state = HIGH;

int buttonState = 0;
unsigned long workTime = 720000; //work time (20 min = 1200000)

unsigned long startTime;  // button ON or Ring
unsigned long currentTime; //millis();

void setup(){

  pinMode(Relay, OUTPUT);
  digitalWrite(Relay, state);
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(57600);
  Serial.println("Enter AT command:");
  gsm.begin(57600);
  gsm.println("AT+CLIP=1");
}

void loop() {

  if(gsm.find("RING")){                      // if RING
    Serial.println("RING!");
    gsm.println("AT+CLIP=1");              // включаем АОН,

    while(1){
      if (gsm.find("37529XXXXXXX")){         // if phone number = TRUE
        Serial.println("!!!");
        startTime = millis();
        gsm.println("ATH");           // off Ring
        break;
      } else if (gsm.find("37529XXXXXXX")){
        Serial.println("!!!");
        startTime = millis();
        gsm.println("ATH");
        break;
      } else if (gsm.find("37529XXXXXXX")){
        Serial.println("!!!");
        startTime = millis();
        gsm.println("ATH");
        break;
      } else {
        gsm.println("AT+CPAS");             // modem state
        delay(100);
        if (gsm.find("+CPAS: 0")) break;    // end cycle
     }
    }
  }


  buttonState = digitalRead(buttonPin);

  if (buttonState == LOW && state == LOW) {  // button ON, relay ON
    delay(100);
    startTime = 0;
  }

  if (buttonState == LOW && state == HIGH) {  // button ON, relay OFF
    delay(100);
    startTime = millis();
  }

  state = runWebasto (startTime);
  digitalWrite(Relay, state);

  if (gsm.available())
    Serial.write(gsm.read());
  if (Serial.available())
    gsm.write(Serial.read());
}


int runWebasto (unsigned long startTime) {
  currentTime = millis();
  if(startTime == 0){
  state  = HIGH;
  return  state;
  }

  if((currentTime - startTime) > workTime){        // work: yes or no
    state  = HIGH;
  } else {
    if(startTime > 2000){
    state  = LOW;
    }
  }
//  delay (1000);
//  Serial.println("______");
//  Serial.println(currentTime);
//  Serial.println(startTime);
return  state;
}
