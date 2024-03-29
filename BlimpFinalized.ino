  #include <Servo.h>

float modifiedMap(float x, float in_min, float in_max, float out_min, float out_max)
{
 float temp = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
 temp = (int) (100*temp + .1);
 return (float) temp/100;
}
// ---- Input Variable ----
int throttlePin = 13;
int rudderPin = 10;
int aileronPin = 12;
int elevatorPin = 11;
int dropPin = 9;

// ---- Vars ----
int throttle;
int rudder;
int aileron;
int elevator;
int drop;
int Relevator;
int Lelevator;
int Lmotor;
int Rmotor;
float ailVal;
int Droppos;

// ---- servo initialization ----
Servo Rservo;
Servo Lservo;
Servo Dropservo;

// ---- motor initialization ----
int lMotorPin = 7;
int rMotorPin = 8;


int LthrottleControl(int val)
{
  val = constrain(val, 0, 255);
  analogWrite(lMotorPin, val);
  return val;
}
int RthrottleControl(int val)
{
  val = constrain(val, 0, 255);
  analogWrite(rMotorPin, val);
  return val;
}
  

void writeServo(int val, Servo &s)
{
  val /= 10;
  val *= 10;
  // Serial.println(val);
  s.write(val);
}

void setup()
{
  // ---- Reciever ----
  pinMode(throttlePin, INPUT);
  pinMode(rudderPin, INPUT);
  pinMode(aileronPin, INPUT);
  pinMode(elevatorPin, INPUT);
  pinMode(dropPin, INPUT);

  // ---- Servos ----
  Rservo.attach(2);
  Lservo.attach(4);
  Dropservo.attach(6);

  // ---- Motors ----
   pinMode(lMotorPin, OUTPUT);
   pinMode(rMotorPin, OUTPUT);


  Serial.begin(9600);
}

void loop()
{
  // ---- reads all of the inputs ----
  throttle = pulseIn(throttlePin, HIGH);
  rudder = pulseIn(rudderPin, HIGH);
  aileron = pulseIn(aileronPin, HIGH);
  elevator = pulseIn(elevatorPin, HIGH);
  drop = pulseIn(dropPin, HIGH);

    

  // ---- mapping values ----
  if (rudder > 1600 || rudder < 1400)
  {
    rudder = map(rudder, 1485, 1888, 45, 135) + 45;
    // Rservo.write(map(rudder, 1485, 1888, 45, 135) + 45);
    writeServo(rudder, Rservo);
    writeServo(rudder, Lservo);
  }
  else 
  {
    Relevator = map(elevator, 1088, 1888, 45, 135) + 10;
    Lelevator = map(elevator, 1888, 1088, 45, 135) + 8;
    writeServo(Relevator, Rservo);
    writeServo(Lelevator, Lservo);
  }
     
//Serial.println(rudder);
  
    if (aileron > 1650)
    {
    ailVal = modifiedMap(aileron, 1550, 1888, 0, 1.1); 
    ailVal = constrain(ailVal, 0, 1);
    Serial.println(LthrottleControl(map(throttle, 1088, 1888, 0, 125)*ailVal));
    LthrottleControl(map(throttle, 1088, 1888, 0, 255)*ailVal) ;
    RthrottleControl(map(throttle, 1088, 1888, 0, 255)*(ailVal - 0.6)) ;
    }
    else if (aileron < 1300)
  
    {
    ailVal = modifiedMap(aileron, 1088, 1400, 1.1, 0); 
    ailVal = constrain(ailVal, 0, 1);
    Serial.println(RthrottleControl(map(throttle, 1088, 1888, 0, 125)*ailVal));
    LthrottleControl(map(throttle, 1088, 1888, 0, 255)*(ailVal - 0.6)) ;
    RthrottleControl(map(throttle, 1088, 1888, 0, 255)*ailVal) ;
    }
    else
    {
    LthrottleControl(map(throttle, 1088, 1888, 0, 255)) ;
    RthrottleControl(map(throttle, 1088, 1888, 0, 255)) ;
    Serial.println(RthrottleControl(map(throttle, 1088, 1888, 0, 255))) ;
    
    }

    Droppos = map(drop, 1888, 1088, 0, 180);
    Droppos /= 100;
    Droppos *= 100; 
    Dropservo.write(Droppos);



//    Serial.println( RthrottleControl(map(throttle, 1088, 1888, 0, 125))) ;
    
    }


    
