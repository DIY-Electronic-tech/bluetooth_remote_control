#include <SoftwareSerial.h>
#define REL 8
#define BUZ 6
//#define s Serial

SoftwareSerial s(10, 11); // RX, TX

void setup() {
  s.begin(9600);
  s.setTimeout(40);
  pinMode(REL, OUTPUT);
  pinMode(BUZ, OUTPUT);
  digitalWrite(REL, HIGH);


}
void loop() {
  if (s.available()) {
    String command = s.readStringUntil('\n');
    command.trim();
    if (command == "ledOn") {
      s.println(0);
      digitalWrite(REL, LOW);
    } else if (command == "ledOff") {
      s.println(0);
      digitalWrite(REL, HIGH);
    } else if (command == "getStatus") {
      if (!digitalRead(REL)) {
        s.println("on");
      } else {
        s.println("off");
      }
    } else {
      s.println(1);

    }
  }
}
