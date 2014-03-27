unsigned red = 0;
unsigned green = 0;
unsigned blue = 0;

int RedPin = 9;
int GreenPin = 10;
int BluePin = 11;

void setup() {
  
  int divisor = 256;
  setPwmFrequency(RedPin, divisor);
  setPwmFrequency(GreenPin, divisor);
  setPwmFrequency(BluePin, divisor);
  
}

int last = 0;

void loop() {
  
//  if (last+10000 < millis()) { return; }
//  last = millis();
  
  delay(1000);
  
  red = random(0, 255);
  green = random(0, 255);
  blue = random(0, 255);
  
  green = 200;
  red = 250;
  blue = 70;
    
  analogWrite (RedPin, red);
  analogWrite (GreenPin, green);
  analogWrite (BluePin, blue);

//digitalWrite(RedPin, HIGH);
//digitalWrite(BluePin, HIGH);
//digitalWrite(GreenPin, HIGH);
  
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
