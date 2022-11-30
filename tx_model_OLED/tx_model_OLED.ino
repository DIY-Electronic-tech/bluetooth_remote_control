#include <SoftwareSerial.h>
#include <GyverOLED.h>

#define BUT 4
#define holdTime 500 // ms 
#define cancelTime 3000

//#define ss Serial

GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;

SoftwareSerial s(10, 11); // RX, TX

bool butFlag = true, turnOn = false, panicMode = false, pressButF = true;
unsigned long int ts, tss, tsss, disSecTimer, disSecTimerLast;  //   uint32_t
uint8_t disMode = 0, minutesToStop = 0;


void setup() {
  //ss.begin(9600);
  s.begin(9600);
  s.setTimeout(40);
  pinMode(BUT, INPUT_PULLUP);
  Wire.setClock(800000L);
  oled.init();
  oled.clear();
  oled.home();
  oled.setScale(3);
  oled.print("Hello");
  oled.update();
  delay(1000);

  oled.clear();
  oled.setScale(2);
  oled.setCursorXY(4, 24);// (128/2)-(72/2)    72 - 6*3*4  ...
  oled.print("Connecting");

  String command = "";
  while (command != "on" && command != "off") {

    s.println("getStatus");
    delay(100);
    if (s.available()) {
      command = s.readStringUntil('\n');
      command.trim();
    }

  }
  if (command == "on") {
    turnOn = true;

  }
  disMode0();


}

void loop() {
  if (!digitalRead(BUT) && butFlag) {
    startTimer();
    butFlag = false;
    pressButF = true;

  }

  if (digitalRead(BUT) && !butFlag) {
    butFlag = true;
    if (getPastTime() > holdTime && getPastTime() < cancelTime) {
      if (disMode == 0) {

        switchRel();

      } else if (disMode == 1) {
        minutesToStop++;
        disMode1();


      }


    } else if (getPastTime() < holdTime) {

      disMode++;
      if (disMode > 1) {
        disMode = 0;

      }

      updateDis();

    } else {

      if (disMode == 0) {

        if (turnOn) {
          switchRel();

        } else {
          oled.clear();
          oled.setScale(3);
          oled.setCursorXY(10, 20);
          oled.print("Cancel");
          delay(1000);
          updateDis();

        }


      } else {

        minutesToStop = 0;
        updateDis();

      }
    }

  }



  if (minutesToStop > 0 && !panicMode && turnOn) {

    if (getPastTime2() / 60 >= minutesToStop) {

      switchRel();

    }

    disSecTimer = minutesToStop - getPastTime2() / 60;

    if (disMode == 0 && (disSecTimerLast != disSecTimer)) {
      disSecTimerLast = disSecTimer;
      disMode0();
    }

  }




  if (getPastTime1() > 1000 && panicMode) {
    if (!turnOn) {
      sendAction();
      startTimer1();

    } else {
      turnOn = !turnOn;
      updateDis();
    }

  }

  if (s.available()) {
    String command = s.readStringUntil('\n');
    command.trim();
    if (command == "0") {
      panicMode = false;
      disMode0();
      startTimer2();


    } else {
      sendAction();

    }

  }

}


void updateDis() {
  if (disMode == 0) {
    disMode0();

  } else if (disMode == 1) {
    disMode1();

  }

}


void switchRel() {

  turnOn = !turnOn;

  oled.clear();
  oled.setScale(3);
  oled.setCursorXY(28, 20);
  oled.print("Send");


  sendAction();
  startTimer1();

  panicMode = true;
  pressButF = false;

}

void startTimer() {
  ts = millis();
}

uint32_t getPastTime() {
  return millis() - ts;
}


void startTimer1() {
  tss = millis();
}

uint32_t getPastTime1() {
  return millis() - tss;
}


void startTimer2() {
  tsss = millis() / 1000;
}

uint32_t getPastTime2() {
  return millis() / 1000 - tsss;
}



void sendAction() {

  if (turnOn) {
    s.println("ledOn");
  } else {
    s.println("ledOff");
  }
}

void disMode0() {
  if  (turnOn) {
    oled.clear();
    oled.setScale(3);
    oled.setCursorXY(46, 20);
    oled.print("ON");
    if (minutesToStop > 0) {
      oled.setScale(1);
      oled.setCursorXY(49, 55);
      String disOutText = "Min ";
      disOutText += disSecTimer;
      oled.print(disOutText);
    }

  } else {
    oled.clear();
    oled.setScale(3);
    oled.setCursorXY(37, 20);
    oled.print("OFF");

  }
}

void disMode1() {

  oled.clear();
  oled.setScale(3);
  if (minutesToStop != 0) {
    if (minutesToStop > 9) {
      oled.setCursorXY(10, 20);
    } else {
      oled.setCursorXY(19, 20);
    }
    String disOutText = "";
    disOutText += minutesToStop;
    disOutText += " min";

    oled.print(disOutText);
  } else {
    oled.setScale(2);
    oled.setCursorXY(16, 24);
    oled.print("Mode Off");
  }

}
