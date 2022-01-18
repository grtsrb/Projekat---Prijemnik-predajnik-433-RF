#include <RCSwitch.h>
RCSwitch mySwitch = RCSwitch();

void setup() {
  Serial.begin(9600);
  
   mySwitch.enableTransmit(10);
   mySwitch.setRepeatTransmit(5);

}

void loop() {
  
  while (Serial.available()==0)
  {
  }
  unsigned int kod=Serial.parseInt();
  Serial.println(kod);
  mySwitch.send(kod, 7);
  delay(1000);
 
}
