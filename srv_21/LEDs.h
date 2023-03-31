#ifndef LEDs_h
#define LEDs_h

class LEDs {
    int redPin; int bluePin; int greenPin;
    int frontLight; int backLight;

    int BLACK[3] = {0, 0, 0};
    int RED[3] = {255, 0, 0};
    int GREEN[3] = {0, 255, 0};
    int BLUE[3] = {0, 0, 255};
    int ORANGE[3] = {255, 165, 0};
    int YELLOW[3] = {255, 255, 0};
    int CYAN[3] = {0, 255, 255};
    int MAGENTA[3] = {255, 0 , 255};
    int WHITE[3] = {255, 255, 255};
    int SKY[3] = {135, 206, 235};

  public:
    bool frontLightState = false;

    // Function to setup LEDs
    void SetupLEDs(int _frontLight, int _backLight, int _redPin, int _greenPin, int _bluePin) {
      frontLight = _frontLight;
      backLight = _backLight;
      redPin = _redPin; greenPin = _greenPin; bluePin = _bluePin;
      pinMode(frontLight, OUTPUT);
      pinMode(backLight, OUTPUT);
      pinMode(redPin, OUTPUT);
      pinMode(greenPin, OUTPUT);
      pinMode(bluePin, OUTPUT);
    }

    // Function to toggle front lights depending on the defined state
    void ToggleFrontLights(bool state) {
      if (state == true) {
        digitalWrite(frontLight, HIGH);
      }
      else {
        digitalWrite(frontLight, LOW);
      }
      frontLightState = state;
    }

    // Function to toggle back lights depending on the defined state
    void ToggleBackLights(bool state){
      if(state == true){
        digitalWrite(backLight, HIGH);
      }else{
        digitalWrite(backLight, LOW);
      }
    }

    // Function to set the values of RGB LED
    void SetStateLight(String color) {
      if (color == "BLACK") {
        analogWrite(redPin, BLACK[0]);
        analogWrite(greenPin, BLACK[1]);
        analogWrite(bluePin, BLACK[2]);
      }
      if (color == "WHITE") {
        analogWrite(redPin, WHITE[0]);
        analogWrite(greenPin, WHITE[1]);
        analogWrite(bluePin, WHITE[2]);
      }
      if (color == "RED") {
        analogWrite(redPin, RED[0]);
        analogWrite(greenPin, RED[1]);
        analogWrite(bluePin, RED[2]);
      }
      if (color == "GREEN") {
        analogWrite(redPin, GREEN[0]);
        analogWrite(greenPin, GREEN[1]);
        analogWrite(bluePin, GREEN[2]);
      }
      if (color == "BLUE") {
        analogWrite(redPin, BLUE[0]);
        analogWrite(greenPin, BLUE[1]);
        analogWrite(bluePin, BLUE[2]);
      }
      if (color == "YELLOW") {
        analogWrite(redPin, YELLOW[0]);
        analogWrite(greenPin, YELLOW[1]);
        analogWrite(bluePin, YELLOW[2]);
      }
      if (color == "ORANGE") {
        analogWrite(redPin, ORANGE[0]);
        analogWrite(greenPin, ORANGE[1]);
        analogWrite(bluePin, ORANGE[2]);
      }
      if (color == "CYAN") {
        analogWrite(redPin, CYAN[0]);
        analogWrite(greenPin, CYAN[1]);
        analogWrite(bluePin, CYAN[2]);
      }
      if (color == "MAGENTA") {
        analogWrite(redPin, MAGENTA[0]);
        analogWrite(greenPin, MAGENTA[1]);
        analogWrite(bluePin, MAGENTA[2]);
      }
      if (color == "SKY BLUE" || color == "SKY") {
        analogWrite(redPin, SKY[0]);
        analogWrite(greenPin, SKY[1]);
        analogWrite(bluePin, SKY[2]);
      }
    }

};

#endif
