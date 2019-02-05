#include <OneWire.h>
#include <delay.h>
// DS18S20 Temperature chip i/o
// on pin 4
OneWire ds(4);
int temp;
unsigned long pre, pr;
unsigned int key, ten, un, a, on, pin, ms;
unsigned int blk = LOW;


boolean getTemperature(){
  byte i;
  byte present = 0;
  byte data[12];
  byte addr[8];
  //find a device
  if(!ds.search(addr)){
    ds.reset_search();
    return false;
  }
  if(OneWire::crc8(addr,7)!=addr[7]){
    return false;
  }
  if(addr[0] != 0x10 && addr[0] != 0x28){
    return false;
  }
  ds.reset();
  ds.select(addr);
  //Start conversion
  ds.write(0x44, 1);
  //Wait some time
  pr = millis();
  while( millis() - pr < 850){
    Temp();
  }
  //delay(850);
  present = ds.reset();
  ds.select(addr);
  //Issue Read scratchpad command
  ds.write(0xBE);
  //Receive 9 bytes
  for (i = 0; i < 9; i++){
    data[i] = ds.read();
  }
  //Calculate temperature value
  temp = ((data[1] << 8) + data[0])*0.0625;
  return true;
}
void setup() {
  // initialize inputs/outputs
  pinMode (0, OUTPUT); //Blue
  pinMode (1, OUTPUT); //Green
  pinMode (2, OUTPUT); //Red
  pinMode (3, INPUT_PULLUP);
  digitalWrite(0, HIGH);
  digitalWrite(1, HIGH);
  digitalWrite(2, HIGH);
  getTemperature();
}

void loop() {
  unsigned long cur = millis();
  if(cur - pre > 4150){ 
    pre = cur;
  getTemperature();
   }
  key = digitalRead(3);
  if(key == LOW){
    Blink();
  }
  else{
    Temp();
  }
}
void Blink(){
  getTemperature();
  digitalWrite(0, HIGH);
  digitalWrite(1, HIGH);
  digitalWrite(2, HIGH);
  delay(2000);
  if(temp < 0){
    digitalWrite(2,0);
    temp = -temp;
  }
  ten = temp/10;
  for( a = 0; a < ten; a++){
    digitalWrite(1,LOW);
    delay(250);
    digitalWrite(1,HIGH);
    delay(250);
  }
  delay(1000);
  un = (temp - (ten*10));
  for( a = 0; a < un; a++){
    digitalWrite(0,LOW);
    delay(250);
    digitalWrite(0,HIGH);
    delay(250);
  }
  delay(1000);
}


void under(){
  digitalWrite(0, blk);
  delay(250);
  blk=!blk;
}

void over(){
  ms = (60-temp)*30;
  digitalWrite(2, blk);
  delay(ms+100);
  blk=!blk;
}

void LED(){
  digitalWrite(pin,0);
  delayMicroseconds(on);
  digitalWrite(pin, 1);
  delayMicroseconds(500-on);

}

void Temp(){
  if(pin == 0 && temp < 20 && temp > 10){
    on = 255 - (temp*5);
    LED();
  }
  else{
    digitalWrite(0,1);
  }
  if(pin == 1){
    if(temp > 10 && temp <= 19){
      on = (temp - 10)*15;
      LED();
    }
    if(temp >= 20 && temp < 30){
      on = (30 - temp)*15;
      LED();
    }
  }
  else{
    digitalWrite(1,1);
  }
  if(pin == 2 && temp > 20 && temp < 30){ 
    on = (temp - 20)*25;
    LED();
  }
  else{
    digitalWrite(2,1);
  }
  if(temp < 11){
    under();
  }
  if(temp > 29){
    over();
  }
  pin++;
  if(pin > 2){
    pin = 0;
  }
}




























