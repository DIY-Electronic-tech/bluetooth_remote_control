#include <SoftwareSerial.h>
#define BUT 4
#define LED 3
#define holdTime 500 // ms 

//#define s Serial

SoftwareSerial s(10, 11); // RX, TX

bool butFlag = true, turnOn = false, panicMode = false, pressButF = true;
long int ts, tss;

void setup() {
  s.begin(9600);
  s.setTimeout(40);
  pinMode(BUT, INPUT_PULLUP);
  pinMode(LED, OUTPUT);

}

void loop() {
  if (!digitalRead(BUT) && butFlag) {
    startTimer();
    butFlag = false;
    pressButF = true;
    
  }

  if (digitalRead(BUT) && !butFlag) {
    butFlag = true;
    
  }

  if (!digitalRead(BUT) && pressButF) {
    if (getPastTime() > holdTime) {
      turnOn = !turnOn;
      digitalWrite(LED, !digitalRead(LED));
      delay(10);
      digitalWrite(LED, !digitalRead(LED));


      sendAction();
      startTimer1();
      
      panicMode = true;
      pressButF = false;

    }

  }


  if (getPastTime1() > 1000 && panicMode) {
    if (!turnOn) {
      sendAction();
      startTimer1();
      
    } else {
      turnOn = !turnOn;
    }

  }

  if (s.available()) {
    String command = s.readStringUntil('\n');
    command.trim();
    if (command == "0") {
      panicMode = false;
      digitalWrite(LED, turnOn);
      
    } else {
      sendAction();
      
    }

  }

}


void startTimer() {
  ts = millis();
}

int getPastTime() {
  return millis() - ts;
}


void startTimer1() {
  tss = millis();
}

int getPastTime1() {
  return millis() - tss;
}

void sendAction() {

  if (turnOn) {
    s.println("ledOn");
  } else {
    s.println("ledOff");
  }
}
