#include <LGSM.h>
char phonenum[20] = "2936";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  while(!LSMS.ready())
    delay(1000);
  
  Serial.println("SIM ready for work!");
  
  LSMS.beginSMS(phonenum);
  LSMS.print("hi from LinkItOne");
  if(LSMS.endSMS()){
    Serial.println("SMS is sent");
  }else{
    Serial.println("SMS is not sent");
  }
  delay(2000);
}

void loop() {
  char buf[20];
  int v;
  if (LSMS.available()) // Check if there is new SMS
  {
    Serial.println("There is new message.");

    LSMS.remoteNumber(buf, 20); // display Number part
    Serial.print("Number:");
    Serial.println(buf);

    Serial.print("Content:"); // display Content part
    while (true)
    {
      v = LSMS.read();
      if (v < 0)
        break;
      Serial.print((char)v);
    }
    Serial.println();

    LSMS.flush(); // delete message
  }
  delay(1000);
}
