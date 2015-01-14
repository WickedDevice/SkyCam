#include <Wicked_DCMotor.h>

int num_motors = 2;
Wicked_DCMotor motor1(M1);
Wicked_DCMotor motor2(M2);
uint16_t m1Current =0;
uint16_t m2Current =0;
Wicked_DCMotor *m[] = {&motor1, &motor2};

void setup(void){
  Serial.begin(115200);
}

void loop(void){
  
  Serial.println("Hello!");
  
  motor1.setDirection(DIR_CW);
  motor1.setBrake(BRAKE_OFF);
  motor2.setDirection(DIR_CCW);
  motor2.setBrake(BRAKE_OFF);
  motor1.setSpeed(255);
  motor2.setSpeed(255);
  for(uint8_t ii=0; ii<50; ii++){
    Serial.println("Current:");
    Serial.println("motor 1: ");
    m1Current=getMaxCurrent(&motor1, 50);
    Serial.println(m1Current);
    Serial.println("motor 2: ");
    m2Current=getMaxCurrent(&motor2, 50);
    Serial.println(m2Current);
    if(m1Current<90||m2Current<90){
      takeUpSlack(m, 2, 90, 50);
      motor1.setDirection(DIR_CW);
      motor2.setDirection(DIR_CCW);
      motor1.setSpeed(255);
      motor2.setSpeed(255);
    }
    if(m1Current>200){
      break;
    }
  }
  
    motor1.setDirection(DIR_CCW);
  motor1.setBrake(BRAKE_OFF);
  motor2.setDirection(DIR_CW);
  motor2.setBrake(BRAKE_OFF);
  motor1.setSpeed(255);
  motor2.setSpeed(255);
  for(uint8_t ii=0; ii<50; ii++){
    Serial.println("Current:");
    Serial.println("motor 1: ");
    m1Current=getMaxCurrent(&motor1, 50);
    Serial.println(m1Current);
    Serial.println("motor 2: ");
    m2Current=getMaxCurrent(&motor2, 50);
    Serial.println(m2Current);
    if(m1Current<90||m2Current<90){
      takeUpSlack(m, 2, 90, 50);
      motor1.setDirection(DIR_CCW);
      motor2.setDirection(DIR_CW);
      motor1.setSpeed(255);
      motor2.setSpeed(255);
    }
    if(m2Current>200){
      break;
    }
  }
}
  
  
  
  
uint16_t getMaxCurrent(Wicked_DCMotor * motor, const long interval){
  unsigned long previousMillis = millis();
  unsigned long currentMillis = 0;
  uint16_t maxValue = 0;
  for(;;){
    currentMillis = millis();
    if(currentMillis - previousMillis >= interval) {
      break;
    }
    
    uint16_t currentValue = motor->currentSense();
    if(currentValue > maxValue){
      maxValue = currentValue; 
    }
  }
  
  return maxValue;
}

void takeUpSlack(Wicked_DCMotor *motor[], int motors, uint16_t tension, const long interval){
  uint16_t current[motors];
  for(int ii=0; ii < motors; ii++){ 
    current[ii]=getMaxCurrent(motor[ii], interval);
  }
  for(int ii=0; ii < motors; ii++){
    motor[ii]->setSpeed(0);
  }
  for(int ii=0; ii< motors; ii++){
    motor[ii]->setDirection(DIR_CW);
    while(current[ii]<tension){
      motor[ii]->setSpeed(255);
      current[ii]=getMaxCurrent(motor[ii], interval);
    }
  }
}
    
  
