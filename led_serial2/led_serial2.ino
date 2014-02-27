unsigned red = 0;
unsigned green = 0;
unsigned blue = 0;

int RedPin = 9; //Red: PWM pin 10
int GreenPin = 10; //Green: PWM pin 11
int BluePin = 11; //Blue: PWM pin 9

void setup() {
  
  int divisor = 256;
  setPwmFrequency(RedPin, divisor);
  setPwmFrequency(GreenPin, divisor);
  setPwmFrequency(BluePin, divisor);
  
  Serial.begin(4800);
  
}

int reading_state = 0;

void loop() {
  
  char databyte;
  while (Serial.available()) {
    databyte = Serial.read();
    
    Serial.write(databyte);
    
    if (databyte == 'A') {
      reading_state = 0;
    }
    
    switch(reading_state) {
      case 0: break;
      case 1: red = databyte; break;
      case 2: blue = databyte; break;
      case 3: green = databyte;
        char debug[128];
        sprintf(debug, "R %d  G %d  B %d\n", red, green, blue);
        Serial.write(debug);
        Serial.write("K");
        break;
      default: badInput(); reading_state = -1; break;
    }
    reading_state++;
    //Serial.write(reading_state+'0');
  }
  
  analogWrite (RedPin, red);
  analogWrite (GreenPin, green);
  analogWrite (BluePin, blue);
  
}

boolean charInRange(char query, char lower, char upper) {
  return query >= lower && query <= upper;
}

void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if(pin == 3 || pin == 11) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x7; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}

String getStringInput() {
  String content = "";
  char character;

  while(Serial.available()) {
      character = Serial.read();
      content.concat(character);
  }
  
  return content;

}

void badInput() {
  Serial.write("X");
}
