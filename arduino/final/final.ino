int inByte = 0;         // incoming serial byte
//int newInByte = 0;

// the setup function runs once when you press reset or power the board
void setup() {
   Serial.begin(9600);
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
   if (Serial.available() > 0) {
    // get incoming byte:
    inByte = Serial.read();
    Serial.println(inByte);
   
   
   if(inByte == 1 ){
    for(int i= 0; i < 3; i++){
      digitalWrite(13, HIGH);
      // turn the LED on (HIGH is the voltage level)
      digitalWrite(12, HIGH);
      delay(500);              // wait for a second
      digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
      digitalWrite(12, LOW);
      delay(500);         // wait for a second
    
    }
    // Clear out the serial buffer in case we got more
    //  serial data while we were blinking
    while (Serial.available() > 0) {
    // get incoming bytes just to remove them from the buffer
      Serial.read(); // does nothing with the data, just throws it out
    }
   }
//  
//  digitalWrite(13, HIGH);
//  // turn the LED on (HIGH is the voltage level)
//  digitalWrite(12, HIGH);
//  delay(200);              // wait for a second
//  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
//  digitalWrite(12, LOW);
//  delay(200);              // wait for a second
////  
//    digitalWrite(13, HIGH);
//  // turn the LED on (HIGH is the voltage level)
//  digitalWrite(12, HIGH);
//  delay(200);              // wait for a second
//  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
//  digitalWrite(12, LOW);
//  delay(200);              // wait for a second
//  inByte = 0;
  //Serial.println(inByte);
}
}
