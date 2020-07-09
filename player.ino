#define RECEIVE_PIN 2
#define END_PIN 3

#define RIGHT_BOTTOM 4
#define MID_BOTTOM 5
#define LEFT_BOTTOM 6
#define RIGHT_MIDDLE 7
#define MID_MIDDLE 8
#define LEFT_MIDDLE 9
#define RIGHT_TOP 10
#define MID_TOP 11
#define LEFT_TOP 12

int number = 0;

void setup(){
  
  	Serial.begin(9600);
  
    pinMode(RIGHT_BOTTOM, OUTPUT);
    pinMode(MID_BOTTOM, OUTPUT);
    pinMode(LEFT_BOTTOM, OUTPUT);
    pinMode(RIGHT_MIDDLE, OUTPUT);
    pinMode(MID_MIDDLE, OUTPUT);
    pinMode(LEFT_MIDDLE, OUTPUT);
    pinMode(RIGHT_TOP, OUTPUT);
    pinMode(MID_TOP, OUTPUT);
    pinMode(LEFT_TOP, OUTPUT);

    attachInterrupt(digitalPinToInterrupt(RECEIVE_PIN), receive, RISING); 
    attachInterrupt(digitalPinToInterrupt(END_PIN), end, RISING);
  
}

void receive(){

    number++;

}

void end(){

    digitalWrite(number, HIGH);
    number = 0;
  
}

void loop(){}