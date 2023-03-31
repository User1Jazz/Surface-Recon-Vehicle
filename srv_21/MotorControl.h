#ifndef MotorControl_h
#define MotorControl_h

class MotorControl {

    int enA, inA1, inA2;
    int enB, inB1, inB2;


  public:

    MotorControl() {}

    // Function to setup two motors (left and right)
    void SetupMotors(int _enA, int _enB, int _inA1, int _inA2, int _inB1, int _inB2) {
      // Set the pins to variables
      enA = _enA;
      enB = _enB;
      inA1 = _inA1;
      inA2 = _inA2;
      inB1 = _inB1;
      inB2 = _inB2;

      // Set all the motor control pins to outputs
      pinMode(enA, OUTPUT);
      pinMode(inA1, OUTPUT);
      pinMode(inA2, OUTPUT);

      pinMode(enB, OUTPUT);
      pinMode(inB1, OUTPUT);
      pinMode(inB2, OUTPUT);

      // Turn off motors - Initial state
      Stop();
    }

    //  Function to perform a drive operation depending on given speed parameters
    void Drive(float A_motorSpeed, float B_motorSpeed) {

      // Turn on motor A & B depending on speed parameters
      if (A_motorSpeed < 0) {
        digitalWrite(inA1, LOW);
        digitalWrite(inA2, HIGH);
      }
      else {
        digitalWrite(inA1, HIGH);
        digitalWrite(inA2, LOW);
      }

      if (B_motorSpeed < 0) {
        digitalWrite(inB1, LOW);
        digitalWrite(inB2, HIGH);
      }
      else {
        digitalWrite(inB1, HIGH);
        digitalWrite(inB2, LOW);
      }

      analogWrite(enA, abs(A_motorSpeed));
      analogWrite(enB, abs(B_motorSpeed));
    }

    // Function to stop motors
    void Stop() {
      // Turn off motors A & B
      digitalWrite(inA1, LOW);
      digitalWrite(inA2, LOW);
      digitalWrite(inB1, LOW);
      digitalWrite(inB2, LOW);
      analogWrite(enA, 0);
      analogWrite(enB, 0);
    }

};

#endif
