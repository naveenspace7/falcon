#include <Wire.h>
#include <HMC5883L.h>

// ### ADDRESS OF EACH ELEMENT
#define TIMESTAMP 11

const int trigPin1 = 9;
const int trigPin2 = 8;
const int trigPin3 = 7;
const int trigPin4 = 6;
const int echoPin = 10;
const int iteration = 5;
bool lock = true;

long duration;
int distance;
int distArr[5] = {0,0,0,0,0};

int distance[4] = {0,0,0,0};
int angle = 0;

HMC5883L compass;

const int bSize = 3;

String input = "";
char Buffer[bSize];
int ByteCount;

int timestamp = 0;

void setup()
{
  Serial.begin(9600);
  Serial.println("Initialize HMC5883L");  
  while (!compass.begin())
  {
    Serial.println("Could not find a valid HMC5883L sensor, check wiring!");
    delay(500);
  }
  compass.setRange(HMC5883L_RANGE_1_3GA);
  compass.setMeasurementMode(HMC5883L_CONTINOUS);
  compass.setDataRate(HMC5883L_DATARATE_30HZ);  
  compass.setSamples(HMC5883L_SAMPLES_8);  
  compass.setOffset(0, 0);
}

void loop() 
{
  if(lock)
  {
    distance[0] = usr_distance(trigPin1);
    distance[1] = usr_distance(trigPin2);
    distance[2] = usr_distance(trigPin3);
    distance[3] = usr_distance(trigPin4);

    angle = get_angle();
    
    lock = false;
  }
  delay(100);
}

// INFO: SerialEvent occurs whenever a new data comes in the hardware serial RX.
void serialEvent()
{
  // make the lock true and that's it
  lock = true;
}

void read_serial()
{  
  inputString = "";
  Serial.setTimeout(1);
  //when available
  while(Serial.available())
  {
    // Get the request from the master into req
    ByteCount =  Serial.readBytesUntil('\0',Buffer,bSize);
  }      
}

int get_angle()
{
  Vector norm = compass.readNormalize();
  float heading = atan2(norm.YAxis, norm.XAxis);
  float declinationAngle = (4.0 + (26.0 / 60.0)) / (180 / M_PI);
  heading += declinationAngle;
  if (heading < 0)
    heading += 2 * PI;
  if (heading > 2 * PI)
    heading -= 2 * PI;
  float headingDegrees = heading * 180/M_PI; 
  return headingDegrees;
}

void usr_distance(int pin)
{
  int temp_distance = 0, i, buff_distance = 0;

  for(i=0; i<iteration; i++)
  {
    digitalWrite(pin, LOW);
    delayMicroseconds(2);
    digitalWrite(pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(pin, LOW);
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
