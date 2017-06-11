const int trigPin = 9;
const int echoPin = 10;
const int iteration = 5;

long duration;
int distance;
int distArr[5] = {0,0,0,0,0};

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
	distance = usr(trigPin);
  Serial.print("Distance:");
  Serial.println(distance);
  delay(1000);
}

int usr(int trig_x)
{
  int i = 0, temp_distance = 0, buff_distance = 0;

  for(i = 0; i < iteration; i++)
  {
    digitalWrite(trig_x, LOW);
    delayMicroseconds(2);
    digitalWrite(trig_x, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig_x, LOW);
    duration = pulseIn(echoPin, HIGH);
    buff_distance = duration * 0.034/2;    
    distArr[i] = buff_distance;
  }
  return mean(distArr,iteration);   
}

int mean(int*arr,int n)
{
  int temp = 0, count;
  for(count = 0; count<n; count++)
  {
    temp = temp + arr[count];
  }
  return temp;  
}

