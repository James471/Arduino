int pinSensorLeft=8;
int pinSensorRight=9;
int pinSensorFront=10;
int sensorLeftValue=0;
int sensorRightValue=0;
int sensorFrontValue=0;
int pinLeftWheel2=4;
int pinLeftWheel1=5;
int pinRightWheel2=6;
int pinRightWheel1=7;
int duration=1;//Needs calibration for turning by 90 degree
int decisonIndex=-1;
int stepsIndex=-1;
int decisionNotTaken[10][2];
int steps[100];
int lastAction=0;
boolean isDecisionNeeded=false;
const int constForwardLeftTurn=10;
const int constForwardRightTurn=20;
const int constForwardMotion=1;
const int leftPathUnexplored=-1;
const int rightPathUnexplored=1;

void setup() {
  //IR returns high when there is no obstacle
  Serial.begin(9600);
  pinMode(pinSensorLeft,INPUT);
  pinMode(pinSensorRight,INPUT);
  pinMode(pinSensorFront,INPUT);
}

void loop() {
  sensorLeftValue=digitalRead(pinSensorLeft);
  sensorRightValue=digitalRead(pinSensorRight);
  sensorFrontValue=digitalRead(pinSensorFront);
  if(!isDecisionNeeded){
    if(sensorLeftValue==LOW && sensorRightValue==LOW && sensorFrontValue=HIGH){
      moveForward();
    }
    else if(sensorLeftVaue==LOW && sensorRightValue==HIGH && sensorFrontValue==LOW){
      forwardRightTurn();
    }
    else if(sensorLeftValue==HIGH && sensorRightValue==LOW && sensorFrontValue==LOW){
      forwardLeftTurn();
    }
    else if(sensorLeftValue==HIGH && sensorRightValue==HIGH && sensorFrontValue==HIGH){
      passDecisionPoint();
      decisionNotTaken[++decisionIndex]={leftPathUnexplored,rightPathUnexplored};
    }
    else if(sensorLeftValue==LOW && sensorRightValue==LOW && sensorFrontValue==LOW){  
      retrace();
    }
    else if(sensorLeftValue=LOW && sensorRightValue==HIGH && sensorFrontValue==HIGH){
      passDecisionPoint();
      decisionNotTaken[++decisionIndex]={0,rightPathUnexplored}//0 stands for a decision which was not needed.
    }
    else if(sensorLeftValue==HIGH && sensorRightValue==LOW && sensorFrontValue==HIGH){
      passDecisionPoint();
      decisionNotTaken[++decisionIndex]={leftPathUnexplored,0};
    }
    else if(sensorLeftValue==HIGH && sensorRightValue==HIGH && sensorFrontValue==LOW){    
      forwardLeftTurn();
      decisionNotTaken[++decisionIndex]{0,rightPathUnexplored};
    }
  }
  else{
    if(decisionNotTaken[decisionIndex][0]==0 && decisionNotTaken[decisionIndex][1]=rightPathUnexplored){
      forwardRightTurn();
      decisionNotTaken[decisionIndex][1]=0;
      decisionIndex--;
    }
    else if(decisionNotTaken[decisionIndex][0]==leftPathUnexplored && decisionNotTaken[decisionIndex][1]=0){
      forwardLeftTurn();
      decisionNotTaken[decisionIndex][0]=0;
      decisionIndex--;
    }
    else{
      forwardLeftTurn();
      decisionNotTaken[decisionIndex][0]=0;
    }
    isDecisionNeeded=false;
  }
}

void forwardLeftTurn(){
  Serial.print("Moving Left");
  Serial.print("\n");
  steps[++stepsIndex]+=constForwardLeftTurn;
  lastAction=constForwardLeftTurn;
  for(int i=0;i<duration;i++){
    digitalWrite(pinLeftWheel1,LOW);
    digitalWrite(pinLeftWheel2,HIGH);
    digitalWrite(pinRightWheel1,HIGH);
    digitalWrite(pinRightWheel2,LOW);
  }  
}

void forwardRightTurn(){
  Serial.print("Moving Right");
  Serial.print("\n");
  steps[++stepsIndex]+=constForwardRightTurn;
  lastAction=constForwardRightTurn;
  for(int i=0;i<duration;i++){
    digitalWrite(pinLeftWheel1,HIGH);
    digitalWrite(pinLeftWheel2,LOW);
    digitalWrite(pinRightWheel1,LOW);
    digitalWrite(pinRightWheel2,HIGH);
  }
}

void moveForward(){
  Serial.print("Moving Forward");
  Serial.print("\n");
  if(lastAction==constForwardMotion){
    steps[stepsIndex]+=moveForward(); 
  }
  else{
    steps[++stepsIndex]+=moveForward();
    lastAction==constForwardMotion;
  }
  digitalWrite(pinLeftWheel1,HIGH);
  digitalWrite(pinLeftWheel2,LOW);
  digitalWrite(pinRightWheel1,HIGH);
  digitalWrite(pinRightWheel2,LOW);
}

void moveBackward(){
  Serial.print("Moving Forward");
  Serial.print("\n");
  steps[stepsIndex]-=moveForward(); 
  digitalWrite(pinLeftWheel1,LOW);
  digitalWrite(pinLeftWheel2,HIGH);
  digitalWrite(pinRightWheel1,LOW);
  digitalWrite(pinRightWheel2,HIGH);
}

void backwardLeftTurn(){
  
}

void backwardRightTurn(){
  
}

void retrace(){
  //Retrace your steps till the last decision point
  if(steps[stepsIndex]!=constForwardLeftTurn && steps[stepsIndex]!=constForwardRightTurn){
    while(steps[stepsIndex]!=0){
      moveBackward();
    }
    stepsIndex--;
    isDecisionNeeded=true;
    if(steps[stepsIndex]==constForwardLeftTurn){
      backwardRightTurn();
      steps[stepsIndex]-=constForwardLeftTurn;
    }
    else{
      backwardLeftTurn();
      steps[stepsIndex]-=constForwardRightTurn;
    }
    stepsindex--;
  }
  else{
    Serial.println("Wierd");
  }
}

void passDecisionPoint(){  
  while(sensorLeftValue==HIGH || sensorRightValue==HIGH){
    sensorLeftValue=digitalRead(pinSensorLeft);
    sensorRightValue=digitalRead(pinSensorRight);
    sensorFrontValue=digitalRead(pinSensorFront);
    moveForward();
  }
  for(int i=0;i<10;i++){
    moveForward();
  }
}
