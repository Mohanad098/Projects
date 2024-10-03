//Included libraries:
#include <Servo.h>
#include <Arduino_FreeRTOS.h>
#include "semphr.h"
#include <FreeRTOSConfig.h>

//The connected pins:
#define IR_feeding 26
#define IR_gripper 48
#define Proximity 46
#define Limitswitch_feeding 44
#define Limitswitch_conveyer 42
#define Limitswitch_vertical 40
#define Limitswitch_left 38
#define Limitswitch_right 32
#define DCV_feeding 24
#define DCV_robotic 28

//First stepper (x)
#define dir_rotation 5
#define step_rotation 2

//Second stepper (y)
#define dir_conv 6
#define step_conv 3

//Third stepper (z)
#define dir_leftpack 7
#define step_leftpack 4

//FOURTH stepper
#define dir_rightpack 13
#define step_rightpack 12

//Fifth stepper (Nema23)
#define dir_vertical 31
#define step_vertical 30

//Enable pin for CNC shield v3
#define en 8

//stepper motor required distances
#define STEPS 200
#define vertical_distance 170


//Semaphore initialization
SemaphoreHandle_t Homing;
SemaphoreHandle_t Conveyer_start;
SemaphoreHandle_t Metal;
SemaphoreHandle_t Non_metal;
SemaphoreHandle_t Leftpackaging;
SemaphoreHandle_t Rightpackaging;

//creating a name to use for the servo
Servo gripper_servo;

//global variables
int Servo_pos = 0;
int Servo_open = 150;
int Servo_close = 85;

//functions initializations
void TaskHoming(void);
void Taskfeeding(void);
void TaskRobotic1(void);
void TaskRobotic2 (void);
void TaskPackagingleft(void);
void TaskPackagingright(void);

void setup() {
  Serial.begin(1200);
  //semaphore creation
  Homing = xSemaphoreCreateBinary();
  Conveyer_start = xSemaphoreCreateBinary();
  Metal = xSemaphoreCreateBinary();
  Non_metal = xSemaphoreCreateBinary();
  Leftpackaging = xSemaphoreCreateBinary();
  Rightpackaging = xSemaphoreCreateBinary();

  //Tasks creation
  xTaskCreate(TaskHoming, "Homing", 400, NULL, 0, NULL);
  xTaskCreate(Taskfeeding, "feeding", 800, NULL, 1, NULL);
  xTaskCreate(TaskRobotic1, "RoboticArm1", 500, NULL, 2, NULL);
  xTaskCreate(TaskRobotic2, "RoboticArm2", 500, NULL, 2, NULL);
  xTaskCreate(TaskPackagingleft, "PackagingLeft", 128, NULL, 3, NULL);
  xTaskCreate(TaskPackagingright, "PackagingRight", 128, NULL, 3, NULL);

  //Pin directions
  pinMode(IR_feeding, INPUT);
  pinMode(IR_gripper, INPUT);
  pinMode(Proximity, INPUT);
  pinMode(Limitswitch_feeding, INPUT_PULLUP);
  pinMode(Limitswitch_conveyer, INPUT_PULLUP);
  pinMode(Limitswitch_vertical, INPUT_PULLUP);
  pinMode(Limitswitch_left, INPUT_PULLUP);
  pinMode(Limitswitch_right, INPUT_PULLUP);
  pinMode(DCV_feeding, OUTPUT);
  pinMode(DCV_robotic, OUTPUT);

  gripper_servo.attach(34);

  //Motor pin directions
  pinMode(en, OUTPUT);
  digitalWrite(en, LOW);

  pinMode(dir_rotation, OUTPUT);
  pinMode(dir_conv, OUTPUT);
  pinMode(dir_leftpack, OUTPUT);
  pinMode(dir_rightpack, OUTPUT);
  pinMode(dir_vertical, OUTPUT);

  pinMode(step_rotation, OUTPUT);
  pinMode(step_conv, OUTPUT);
  pinMode(step_leftpack, OUTPUT);
  pinMode(step_rightpack, OUTPUT);
  pinMode(step_vertical, OUTPUT);

  digitalWrite(DCV_feeding, HIGH);
  digitalWrite(DCV_robotic, HIGH);

  xSemaphoreGive(Homing);

}
void loop() {
  // Empty
}

/************************************************************HOMING***************************************************/
void TaskHoming(void) {

  for (;;) {
    xSemaphoreTake(Homing, portMAX_DELAY);
    //arm reaches top
    digitalWrite(DCV_feeding, HIGH);
    digitalWrite(DCV_robotic, HIGH);
    Serial.println("Homing");
    gripper_servo.write(0);
    digitalWrite(dir_vertical, LOW);
    while (digitalRead(Limitswitch_vertical) == 1) {
      digitalWrite(step_vertical, HIGH);
      delayMicroseconds(1500);
      digitalWrite(step_vertical, LOW);
      delayMicroseconds(1500);
    }
    //arm goes up 15 cm = (150*200)/8= 3750 steps
    digitalWrite(dir_vertical, HIGH);
    for (int x = 0; x < 3600; x++) {
      digitalWrite(step_vertical, HIGH);
      delayMicroseconds(800);
      digitalWrite(step_vertical, LOW);
      delayMicroseconds(800);
    }
    //arm reaches the left packaging and press the limit switch there
    digitalWrite(dir_rotation, LOW);
    while (digitalRead(Limitswitch_left) == 1) {
      digitalWrite(step_rotation, HIGH);
      delayMicroseconds(3000);
      digitalWrite(step_rotation, LOW);
      delayMicroseconds(3000);
    }
    //arm goes then to right packaging and counts number of steps needed
    digitalWrite(dir_rotation, HIGH);
    while (digitalRead(Limitswitch_right) == 1) {
      digitalWrite(step_rotation, HIGH);
      delayMicroseconds(3000);
      digitalWrite(step_rotation, LOW);
      delayMicroseconds(3000);
    }

    digitalWrite(dir_rotation, LOW);
    for (int x = 0; x < 70; x++) {
      digitalWrite(step_rotation, HIGH);
      delayMicroseconds(3000);
      digitalWrite(step_rotation, LOW);
      delayMicroseconds(3000);
    }

    digitalWrite(dir_vertical, LOW);
    while (digitalRead(Limitswitch_vertical) == 1) {
      digitalWrite(step_vertical, HIGH);
      delayMicroseconds(1500);
      digitalWrite(step_vertical, LOW);
      delayMicroseconds(1500);
    }

    // arm goes up 7 cm = (90*200)/8 = 2250
    digitalWrite(dir_vertical, HIGH);
    for (int x = 0; x < 1550; x++) {
      digitalWrite(step_vertical, HIGH);
      delayMicroseconds(1500);
      digitalWrite(step_vertical, LOW);
      delayMicroseconds(1500);
    }
    digitalWrite(dir_rotation, LOW);
    while (digitalRead(Limitswitch_conveyer) == 0) {
      digitalWrite(step_rotation, HIGH);
      delayMicroseconds(3000);
      digitalWrite(step_rotation, LOW);
      delayMicroseconds(3000);
    }

    digitalWrite(dir_rotation, HIGH);
    for (int x = 0; x < 6; x++) {
      digitalWrite(step_rotation, HIGH);
      delayMicroseconds(3000);
      digitalWrite(step_rotation, LOW);
      delayMicroseconds(3000);
    }

    xSemaphoreGive(Conveyer_start);
  }
}

/************************************************************FEEDING TASK****************************************************/
void Taskfeeding(void) {
  for (;;) {
    xSemaphoreTake(Conveyer_start, portMAX_DELAY);
    Serial.println("Feeding task");
    digitalWrite(dir_conv, LOW);
    for (int x = 0; digitalRead(IR_feeding) == 1; x++) {
      digitalWrite(step_conv, HIGH);
      delayMicroseconds(3000);
      digitalWrite(step_conv, LOW);
      delayMicroseconds(3000);
    }
    Serial.println("CONVYER stopped");
    digitalWrite(DCV_feeding, LOW);
    //If limit switch is pressed, AKA piston reached required distance, DCV gets signal 0 to return back
    //conveyer motor moves till IR reads

    while (digitalRead(Limitswitch_feeding) == 1) {}
    if (digitalRead(Limitswitch_feeding) == 0) {
      Serial.println("Limit switch pressed");
      digitalWrite(DCV_feeding, HIGH);
      delay(1000);
      for (int x = 0; (digitalRead(IR_gripper) == 1); x++) {
        digitalWrite(step_conv, HIGH);
        delayMicroseconds(1500);
        digitalWrite(step_conv, LOW);
        delayMicroseconds(1500);
      }
    }
    Serial.println("conveyer end");
    if (digitalRead(IR_gripper) == 0) {
      Serial.println("IR gripper = 0 ");
      digitalWrite(dir_conv, LOW);
      for (int x = 0; x <= STEPS * 1.5 ; x++) {
        digitalWrite(step_conv, HIGH);
        delayMicroseconds(1500);
        digitalWrite(step_conv, LOW);
        delayMicroseconds(1500);
      }
      delay(1000);
      if (digitalRead(Proximity) == LOW) {
        xSemaphoreGive(Metal);
      }
      else if (digitalRead(Proximity) == HIGH) {
        Serial.println("Semaphore non metal sent");
        xSemaphoreGive(Non_metal);
      }
    }
  }
}

/*************************************************************ROBOTIC ARM TASK***********************************************/

void TaskRobotic1(void) {
  for (;;) {
    xSemaphoreTake( Metal, portMAX_DELAY);
    Serial.println("Robotic metal");
    digitalWrite(DCV_robotic, LOW);
    delay(500);
    while (Servo_pos != Servo_open) {
      Servo_pos++;
      gripper_servo.write(Servo_pos);
      delay(15);
    }
    digitalWrite(dir_vertical, LOW);
    while (digitalRead(Limitswitch_vertical) == 1) {
      digitalWrite(step_vertical, HIGH);
      delayMicroseconds(1500);
      digitalWrite(step_vertical, LOW);
      delayMicroseconds(1500);
    }
    delay(2000);
    while (Servo_pos > Servo_close) {
      Servo_pos--;
      gripper_servo.write(Servo_pos);
      delay(15);
    }
    delay(2000);
    digitalWrite(dir_vertical, HIGH);
    //vertical_for_packaging 16 cm
    for (int x = 0; x < 3600; x++) {
      digitalWrite(step_vertical, HIGH);
      delayMicroseconds(1500);
      digitalWrite(step_vertical, LOW);
      delayMicroseconds(1500);
    }

    digitalWrite(dir_rotation, LOW);
    while (digitalRead(Limitswitch_left) == 1) {
      digitalWrite(step_rotation, HIGH);
      delayMicroseconds(3000);
      digitalWrite(step_rotation, LOW);
      delayMicroseconds(3000);
    }
    delay(2000);
    while (Servo_pos != Servo_open) {
      Servo_pos++;
      gripper_servo.write(Servo_pos);
      delay(15);
    }
    digitalWrite(dir_rotation, HIGH);
    for (int x = 0; x < 130; x++) {
      digitalWrite(step_rotation, HIGH);
      delayMicroseconds(3000);
      digitalWrite(step_rotation, LOW);
      delayMicroseconds(3000);
    }
    digitalWrite(dir_vertical, LOW);
    while (digitalRead(Limitswitch_vertical) == 1) {
      digitalWrite(step_vertical, HIGH);
      delayMicroseconds(1500);
      digitalWrite(step_vertical, LOW);
      delayMicroseconds(1500);
    }

    // arm goes up 7 cm = (90*200)/8 = 2250
    digitalWrite(dir_vertical, HIGH);
    for (int x = 0; x < 1650; x++) {
      digitalWrite(step_vertical, HIGH);
      delayMicroseconds(1500);
      digitalWrite(step_vertical, LOW);
      delayMicroseconds(1500);
    }
    digitalWrite(dir_rotation, LOW);
    while (digitalRead(Limitswitch_conveyer) == 0) {
      digitalWrite(step_rotation, HIGH);
      delayMicroseconds(3000);
      digitalWrite(step_rotation, LOW);
      delayMicroseconds(3000);
    }

    digitalWrite(dir_rotation, HIGH);
    for (int x = 0; x < 2; x++) {
      digitalWrite(step_rotation, HIGH);
      delayMicroseconds(3000);
      digitalWrite(step_rotation, LOW);
      delayMicroseconds(3000);
    }

    digitalWrite(DCV_robotic, HIGH);
    xSemaphoreGive(Leftpackaging);
  }
}
/*******************************************************If NON METAL************************************************/
void TaskRobotic2 (void) {
  for (;;) {
    xSemaphoreTake( Non_metal, portMAX_DELAY);
    Serial.println("Robot non metal");
    digitalWrite(DCV_robotic, LOW);
    delay(500);
    while (Servo_pos != Servo_open) {
      Servo_pos++;
      gripper_servo.write(Servo_pos);
      delay(15);
    }
    digitalWrite(dir_vertical, LOW);
    while (digitalRead(Limitswitch_vertical) == 1) {
      digitalWrite(step_vertical, HIGH);
      delayMicroseconds(1500);
      digitalWrite(step_vertical, LOW);
      delayMicroseconds(1500);
    }
    delay(2000);
    while (Servo_pos > Servo_close) {
      Servo_pos--;
      gripper_servo.write(Servo_pos);
      delay(15);
    }
    delay(2000);
    digitalWrite(dir_vertical, HIGH);
    //vertical_for_packaging 16 cm
    for (int x = 0; x < 3600; x++) {
      digitalWrite(step_vertical, HIGH);
      delayMicroseconds(1500);
      digitalWrite(step_vertical, LOW);
      delayMicroseconds(1500);
    }
    digitalWrite(dir_rotation, HIGH);
    while (digitalRead(Limitswitch_right) == 1) {
      digitalWrite(step_rotation, HIGH);
      delayMicroseconds(3000);
      digitalWrite(step_rotation, LOW);
      delayMicroseconds(3000);
    }
    delay(2000);
    while (Servo_pos != Servo_open) {
      Servo_pos++;
      gripper_servo.write(Servo_pos);
      delay(15);
    }
    digitalWrite(dir_rotation, LOW);
    for (int x = 0; x < 70; x++) {
      digitalWrite(step_rotation, HIGH);
      delayMicroseconds(3000);
      digitalWrite(step_rotation, LOW);
      delayMicroseconds(3000);
    }

    digitalWrite(dir_vertical, LOW);
    while (digitalRead(Limitswitch_vertical) == 1) {
      digitalWrite(step_vertical, HIGH);
      delayMicroseconds(1500);
      digitalWrite(step_vertical, LOW);
      delayMicroseconds(1500);
    }

    // arm goes up 7 cm = (90*200)/8 = 2250
    digitalWrite(dir_vertical, HIGH);
    for (int x = 0; x < 1650; x++) {
      digitalWrite(step_vertical, HIGH);
      delayMicroseconds(1500);
      digitalWrite(step_vertical, LOW);
      delayMicroseconds(1500);
    }
    digitalWrite(dir_rotation, LOW);
    while (digitalRead(Limitswitch_conveyer) == 0) {
      digitalWrite(step_rotation, HIGH);
      delayMicroseconds(3000);
      digitalWrite(step_rotation, LOW);
      delayMicroseconds(3000);
    }

    digitalWrite(dir_rotation, HIGH);
    for (int x = 0; x < 2; x++) {
      digitalWrite(step_rotation, HIGH);
      delayMicroseconds(3000);
      digitalWrite(step_rotation, LOW);
      delayMicroseconds(3000);
    }
    digitalWrite(DCV_robotic, HIGH);
    xSemaphoreGive(Rightpackaging);
  }
}

void TaskPackagingleft(void) {
  for (;;) {
    xSemaphoreTake(Leftpackaging, portMAX_DELAY);
    digitalWrite(dir_leftpack, HIGH);
    for (int x = 0; x < 50; x++) {
      digitalWrite(step_leftpack, HIGH);
      delayMicroseconds(3000);
      digitalWrite(step_leftpack, LOW);
      delayMicroseconds(3000);
    }
    xSemaphoreGive(Conveyer_start);
  }
}
void TaskPackagingright(void) {
  for (;;) {
    xSemaphoreTake(Rightpackaging, portMAX_DELAY);
    digitalWrite(dir_rightpack, HIGH);
    for (int x = 0; x < 50; x++) {
      digitalWrite(step_rightpack, HIGH);
      delayMicroseconds(3000);
      digitalWrite(step_rightpack, LOW);
      delayMicroseconds(3000);
    }
    xSemaphoreGive(Conveyer_start);
  }
}
