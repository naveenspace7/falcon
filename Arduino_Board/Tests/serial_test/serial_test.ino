String inputString = "";

void setup() {
  Serial.begin(9600);
  
}

void loop() {
  //Serial.println("1234");
  //delay(100);
}

void serialEvent()
{
  inputString = "";
  Serial.setTimeout(1);
  //Serial.println("something came in");
  while(Serial.available())
  {
    //char inChar = (char)Serial.read();
    //inputString += inChar;
    inputString = Serial.readString();
  }
  Serial.println(inputString);
  Serial.flush();
  inputString = "";
}

