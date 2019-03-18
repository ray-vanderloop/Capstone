/* This code implements the ammeter functionality through the Arduino
 * Using the Arduino's ammeter, the current value is read and outputted to a seven segment display
 * When the current becomes positive it indicates that the launch button has been pressed and this will trigger the launch light LED to turn on
 * This light must stay on for the duration of use.
 * After this event has taken place, the current will drop back down to 0. This indicates that there has been successful ignition and this will trigger the ignition light LED to turn on.
 * This light must also stay on for the duration of use.
 */

// pins, as attached to the arduino
int digitPins[] = {
  5,4,3,2};
int segPins[] = {
  6,7,8,9,10,11,12};
int launchLED = 16;
int ignitionLED = 15;

int wait = 0;
int blech = 100;

// digits to display, updated from serial moniter
int num0 = 0;
int num1 = 0;
int num2 = 0;
int num3 = 0;

//state variable to keep track of what state the launch is in. This is used to know when to turn the ignition light on
int state1 = 0;
int state2 = 0;

// how long to linger on a segment
int dtimer = 0;

void DisplayHex(int digit, int num){

  //setting everything OFF
  for(int i = 0; i < 4; i++){
    pinMode(digitPins[i],OUTPUT);
    digitalWrite(digitPins[i],HIGH);
  }

  for(int i = 0; i < 7; i++){
    pinMode(segPins[i],OUTPUT);
    digitalWrite(segPins[i],LOW);
  }


  //Turning ON what you need
  switch(num){
  case -1:
    {
      digitalWrite(digitPins[digit],LOW);
      delay(dtimer);
      digitalWrite(segPins[0],LOW);
      delay(dtimer);
      digitalWrite(segPins[1],LOW);
      delay(dtimer);
      digitalWrite(segPins[2],LOW);
      delay(dtimer);
      digitalWrite(segPins[3],LOW);
      delay(dtimer);
      digitalWrite(segPins[4],LOW);
      delay(dtimer);
      digitalWrite(segPins[5],LOW);
      delay(dtimer);
      digitalWrite(segPins[6],LOW);
      delay(dtimer);
      break;
    }
  case 0:
    {
      digitalWrite(digitPins[digit],LOW);
      delay(dtimer);
      digitalWrite(segPins[0],HIGH);
      delay(dtimer);
      digitalWrite(segPins[1],HIGH);
      delay(dtimer);
      digitalWrite(segPins[2],HIGH);
      delay(dtimer);
      digitalWrite(segPins[3],HIGH);
      delay(dtimer);
      digitalWrite(segPins[4],HIGH);
      delay(dtimer);
      digitalWrite(segPins[5],HIGH);
      delay(dtimer);
      digitalWrite(segPins[6],LOW);
      delay(dtimer);
      break;
    }
  case 1:
    {
      digitalWrite(digitPins[digit],LOW);
      delay(dtimer);
      digitalWrite(segPins[0],LOW);
      delay(dtimer);
      digitalWrite(segPins[1],HIGH);
      delay(dtimer);
      digitalWrite(segPins[2],HIGH);
      delay(dtimer);
      digitalWrite(segPins[3],LOW);
      delay(dtimer);
      digitalWrite(segPins[4],LOW);
      delay(dtimer);
      digitalWrite(segPins[5],LOW);
      delay(dtimer);
      digitalWrite(segPins[6],LOW);
      delay(dtimer);
      break;
    }
  case 2:
    {
      digitalWrite(digitPins[digit],LOW);
      delay(dtimer);
      digitalWrite(segPins[0],HIGH);
      delay(dtimer);
      digitalWrite(segPins[1],HIGH);
      delay(dtimer);
      digitalWrite(segPins[2],LOW);
      delay(dtimer);
      digitalWrite(segPins[3],HIGH);
      delay(dtimer);
      digitalWrite(segPins[4],HIGH);
      delay(dtimer);
      digitalWrite(segPins[5],LOW);
      delay(dtimer);
      digitalWrite(segPins[6],HIGH);
      delay(dtimer);
      break;
    }
  case 3:
    {
      digitalWrite(digitPins[digit],LOW);
      delay(dtimer);
      digitalWrite(segPins[0],HIGH);
      delay(dtimer);
      digitalWrite(segPins[1],HIGH);
      delay(dtimer);
      digitalWrite(segPins[2],HIGH);
      delay(dtimer);
      digitalWrite(segPins[3],HIGH);
      delay(dtimer);
      digitalWrite(segPins[4],LOW);
      delay(dtimer);
      digitalWrite(segPins[5],LOW);
      delay(dtimer);
      digitalWrite(segPins[6],HIGH);
      delay(dtimer);
      break;
    }
  case 4:
    {
      digitalWrite(digitPins[digit],LOW);
      delay(dtimer);
      digitalWrite(segPins[0],LOW);
      delay(dtimer);
      digitalWrite(segPins[1],HIGH);
      delay(dtimer);
      digitalWrite(segPins[2],HIGH);
      delay(dtimer);
      digitalWrite(segPins[3],LOW);
      delay(dtimer);
      digitalWrite(segPins[4],LOW);
      delay(dtimer);
      digitalWrite(segPins[5],HIGH);
      delay(dtimer);
      digitalWrite(segPins[6],HIGH);
      delay(dtimer);
      break;
    }
  case 5:
    {
      digitalWrite(digitPins[digit],LOW);
      delay(dtimer);
      digitalWrite(segPins[0],HIGH);
      delay(dtimer);
      digitalWrite(segPins[1],LOW);
      delay(dtimer);
      digitalWrite(segPins[2],HIGH);
      delay(dtimer);
      digitalWrite(segPins[3],HIGH);
      delay(dtimer);
      digitalWrite(segPins[4],LOW);
      delay(dtimer);
      digitalWrite(segPins[5],HIGH);
      delay(dtimer);
      digitalWrite(segPins[6],HIGH);
      delay(dtimer);
      break;
    }
  case 6:
    {
      digitalWrite(digitPins[digit],LOW);
      delay(dtimer);
      digitalWrite(segPins[0],HIGH);
      delay(dtimer);
      digitalWrite(segPins[1],LOW);
      delay(dtimer);
      digitalWrite(segPins[2],HIGH);
      delay(dtimer);
      digitalWrite(segPins[3],HIGH);
      delay(dtimer);
      digitalWrite(segPins[4],HIGH);
      delay(dtimer);
      digitalWrite(segPins[5],HIGH);
      delay(dtimer);
      digitalWrite(segPins[6],HIGH);
      delay(dtimer);
      break;
    }
  case 7:
    {
      digitalWrite(digitPins[digit],LOW);
      delay(dtimer);
      digitalWrite(segPins[0],HIGH);
      delay(dtimer);
      digitalWrite(segPins[1],HIGH);
      delay(dtimer);
      digitalWrite(segPins[2],HIGH);
      delay(dtimer);
      digitalWrite(segPins[3],LOW);
      delay(dtimer);
      digitalWrite(segPins[4],LOW);
      delay(dtimer);
      digitalWrite(segPins[5],LOW);
      delay(dtimer);
      digitalWrite(segPins[6],LOW);
      delay(dtimer);
      break;
    }
  case 8:
    {
      digitalWrite(digitPins[digit],LOW);
      delay(dtimer);
      digitalWrite(segPins[0],HIGH);
      delay(dtimer);
      digitalWrite(segPins[1],HIGH);
      delay(dtimer);
      digitalWrite(segPins[2],HIGH);
      delay(dtimer);
      digitalWrite(segPins[3],HIGH);
      delay(dtimer);
      digitalWrite(segPins[4],HIGH);
      delay(dtimer);
      digitalWrite(segPins[5],HIGH);
      delay(dtimer);
      digitalWrite(segPins[6],HIGH);
      delay(dtimer);
      break;
    }
  case 9:
    {
      digitalWrite(digitPins[digit],LOW);
      delay(dtimer);
      digitalWrite(segPins[0],HIGH);
      delay(dtimer);
      digitalWrite(segPins[1],HIGH);
      delay(dtimer);
      digitalWrite(segPins[2],HIGH);
      delay(dtimer);
      digitalWrite(segPins[3],LOW);
      delay(dtimer);
      digitalWrite(segPins[4],LOW);
      delay(dtimer);
      digitalWrite(segPins[5],HIGH);
      delay(dtimer);
      digitalWrite(segPins[6],HIGH);
      delay(dtimer);
      break;
    }
  }
}




void setup() {
  //Starting serial communication
  Serial.begin(9600);
  //starting LED because, why not? 
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);

  pinMode(launchLED,OUTPUT);
  digitalWrite(launchLED,LOW);

  pinMode(ignitionLED,OUTPUT);
  digitalWrite(ignitionLED,LOW);

  //setting up pinModes for segPins
  for(int i = 0; i < 7;i++){
    pinMode(segPins[i],OUTPUT);
    digitalWrite(segPins[i],LOW);
  }
  //setting up pinModes for digit pins
  for(int i = 0; i < 4;i++){
    pinMode(digitPins[i],OUTPUT);
    digitalWrite(digitPins[i],LOW);
  }
// Ammeter pin
//pinMode()
  
}

// waiting for all 4 digits, before reading them one after another
void loop() {
  // this code is for reading values FROM the pi. not needed
  /*while (Serial.available() >=4){
    num0 = Serial.read(); //MSB
    num1 = Serial.read();
    num2 = Serial.read();
    num3 = Serial.read(); //LSB
  }
  */
// converting analog read value into amps, then seperating into digits
wait++;
if(wait > blech){
  double a = analogRead(A0) *(5/(1.024*4.7));
  int amps = (int)a;
  Serial.println(amps);
   num3 = (int)(floor(amps/1000));
   amps-= floor(amps/1000)*1000;
   
   num2 = (int)floor(amps/100);
   amps-= floor(amps/100)*100;
   
      num1 = (int)floor(amps/10);
   amps-= floor(amps/10)*10;

   num0 = amps;
  //Serial.println(num3);
  //Serial.println(num2);
  //Serial.println(num1);
  //Serial.println(num0);
  //delay(700);
wait = 0;
}

//if ammeter value is not 0, turn the launch light on - this light should be kept on for the duration of use
  if((num0 > 0)||(num1 > 0)||(num2 > 0)||(num3 > 0)) {
    digitalWrite(launchLED,HIGH);
    state1++;
  }
  //if ammeter value is 0, and the state variable is not 0 (indicating the launch light has been turned on), then the ignition light will be turned on - this light should be kept on for the duration of use
  else if((num0 == 0)&&(num1 == 0)&&(num2 == 0)&&(num3 == 0)&&(state1 != 0)) {
    state2++;
    if(state2 == 10) {
    digitalWrite(ignitionLED,HIGH);
    }
  }

  // correcting for numbers that dont use all the digits
  if(num0 == 0){
    num0 = -1;
  }

  if((num1 == 0)&&(num0 == -1)){
    num1 = -1;
  }

  if((num2 == 0)&&(num1 == -1)){
    num2 = -1;
  }

  // displaying numbers
  DisplayHex(0,num0);
  DisplayHex(1,num1);
  DisplayHex(2,num2);
  DisplayHex(3,num3);

}
