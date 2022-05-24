//Mohammad MUTTAQI - 17050241030
//Summer project-1
//Proximity alert

#define echoPIN 6
#define trigPIN 7
#define buzzerPIN 8

int RED = 2;
int BLUE = 3;

int max_range = 20;
int min_range = 0;

void setup() {
  pinMode(trigPIN, OUTPUT);
  pinMode(echoPIN, INPUT);
  pinMode(buzzerPIN, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(BLUE, OUTPUT);
}

void loop() {

  int measured = measurement(max_range, min_range);
  melody(measured*10);
}

//the necessery measurements will be done in this function
int measurement(int maxrange, int minrange)
{
  long duration, distance;
  
  digitalWrite(trigPIN,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPIN, LOW);

  duration = pulseIn(echoPIN, HIGH);
  distance = duration / 58.2;
  delay(50);
  if (distance >= maxrange){
    digitalWrite(3, HIGH);
  }
  else{
    digitalWrite(3, LOW);
  }
  if(distance >= maxrange || distance <= minrange)
  return 0;
  return distance;
}

int melody(int dly)
{
  tone(buzzerPIN, 440);
  digitalWrite(2, HIGH);
  delay(dly);
  noTone(buzzerPIN);
  digitalWrite(2, LOW);
  delay(dly);
  
}
