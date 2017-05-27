#include<Keyboard.h>
 int k=0; // to keep count of whether the cntrl or alt is pressed
void setup()
 {
 
 
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP); 
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(13, INPUT_PULLUP);
  Keyboard.begin();
}

void loop()
 {
  
  //if the button is pressed
  if(digitalRead(2)==LOW)
 {  
  if(k==0)
  {
   //Send the message
     Keyboard.print("E");
  }
  if(k==1)
  {
   //Send the message
     Keyboard.print("D");
  k=0;//reset the values to 0( alt and cntrl)
  }
  if(k==2)
  {
   //Send the message
     Keyboard.print("V");
  k=0;//reset the values to 0( alt and cntrl)
  }
  }
  if(digitalRead(3)==LOW)
 {
  if(k==0)
  {
   //Send the message
     Keyboard.print("T");
  }
  if(k==1)
  {
   //Send the message
     Keyboard.print("C");
  k=0;//reset the values to 0( alt and cntrl)
  }
  if(k==2)
  {
   //Send the message
     Keyboard.print("K");
  k=0;//reset the values to 0( alt and cntrl)
  }
  }
  if(digitalRead(4)==LOW)
 {
  if(k==0)
  {
   //Send the message
     Keyboard.print("A");
  }
  if(k==1)
  {
   //Send the message
     Keyboard.print("U");
  k=0;//reset the values to 0( alt and cntrl)
  }
  if(k==2)
  {
   //Send the message
     Keyboard.print("X");
  k=0;//reset the values to 0( alt and cntrl)
  }
  }
  if(digitalRead(5)==LOW)
 {
  if(k==0)
  {
   //Send the message
     Keyboard.print("O");
  }
  if(k==1)
  {
   //Send the message
     Keyboard.print("M");
  k=0;//reset the values to 0( alt and cntrl)
  }
  if(k==2)
  {
   //Send the message
     Keyboard.print("J");
  k=0;//reset the values to 0( alt and cntrl)
  }
  }
  if(digitalRead(6)==LOW)
 {
  if(k==0)
  {
   //Send the message
     Keyboard.print("I");
  }
  if(k==1)
  {
   //Send the message
     Keyboard.print("F");
  k=0;//reset the values to 0( alt and cntrl)
  }
  if(k==2)
  {
   //Send the message
     Keyboard.print("Q");
  k=0;//reset the values to 0( alt and cntrl)
  }
  }
  if(digitalRead(7)==LOW)
 {
  if(k==0)
  {
   //Send the message
     Keyboard.print("N");
  }
  if(k==1)
  {
   //Send the message
     Keyboard.print("P");
  k=0;//reset the values to 0( alt and cntrl)
  }
  if(k==2)
  {
   //Send the message
     Keyboard.print("Z");
  k=0;//reset the values to 0( alt and cntrl)
  }
  }
  if(digitalRead(8)==LOW)
 {
    if(k==0)
  {
   //Send the message
     Keyboard.print("S");
  }
  if(k==1)
  {
   //Send the message
     Keyboard.print("G");
  k=0;//reset the values to 0( alt and cntrl)
  }
  if(k==2)
  {
   //Send the message
     Keyboard.print(" ");
  k=0;//reset the values to 0( alt and cntrl)
  }
  }
  if(digitalRead(9)==LOW)
 {
    if(k==0)
  {
   //Send the message
     Keyboard.print("R");
  }
  if(k==1)
  {
   //Send the message
     Keyboard.print("W");
  k=0;//reset the values to 0( alt and cntrl)
  }
  if(k==2)
  {
   //Send the message
     Keyboard.print(8);//Backspace
  k=0;//reset the values to 0( alt and cntrl)
  }
  }
  if(digitalRead(10)==LOW)
 {
    if(k==0)
  {
   //Send the message
     Keyboard.print("H");
  }
  if(k==1)
  {
   //Send the message
     Keyboard.print("Y");
  k=0;//reset the values to 0( alt and cntrl)
  }
  if(k==2)
  {
   //Send the message
     Keyboard.print("I need help, Please Help!");//When in need of assistance
  k=0;//reset the values to 0( alt and cntrl)
  }
  }  
  if(digitalRead(11)==LOW)
 {
    if(k==0)
  {
   //Send the message
     Keyboard.print("L");
  }
  if(k==1)
  {
   //Send the message
     Keyboard.print("B");
  k=0;//reset the values to 0( alt and cntrl)
  }
  if(k==2)
  {
   //Send the message
     Keyboard.print(" Hello, How are you?");//A greeting that’s used very frequently.
  k=0;//reset the values to 0( alt and cntrl)
  }
  }
  if(digitalRead(12)==LOW)
 {
    //Send the message
    k=1;
    //Keyboard.print("Control key executed");
  }
  if(digitalRead(13)==LOW)
 {
    //Send the message
    k=2;
    //Keyboard.print("Alt key executed");
  }
  delay(500);              // wait for half a second
}
 