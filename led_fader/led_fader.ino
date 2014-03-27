unsigned red = 0;
unsigned green = 0;
unsigned blue = 0;

int RedPin = 9;
int GreenPin = 10;
int BluePin = 11;

void setup() {
  
  T_start(5000, 255, 128, 0);
  
  int divisor = 256;
  setPwmFrequency(RedPin, divisor);
  setPwmFrequency(GreenPin, divisor);
  setPwmFrequency(BluePin, divisor);
  
}

void loop() {
  
  T_tick();
  
  analogWrite(RedPin, red);
  analogWrite(GreenPin, green);
  analogWrite(BluePin, blue);
  
  if (T_isDone()) {
    int randR = random(0,255);
    int randG = random(0,255);
    int randB = random(0,255);
    T_start(2000, randR, randG, randB);
  }
  
}

unsigned long T_startTime;
unsigned long T_endTime;
int T_rStartVal;
int T_rEndVal;
int T_rDiff;
int T_gStartVal;
int T_gEndVal;
int T_gDiff;
int T_bStartVal;
int T_bEndVal;
int T_bDiff;

void T_start(unsigned long duration, int rTarget, int gTarget, int bTarget) {
  
  T_startTime = millis();
  T_endTime = millis() + duration;
  
  T_rStartVal = red;
  T_gStartVal = green;
  T_bStartVal = blue;
  
  T_rEndVal = rTarget;
  T_gEndVal = gTarget;
  T_bEndVal = bTarget;
  
  T_rDiff = (T_rEndVal - T_rStartVal);
  T_gDiff = (T_gEndVal - T_gStartVal);
  T_bDiff = (T_bEndVal - T_bStartVal);
  
  Serial.println(T_rDiff);
  Serial.println(T_gDiff);
  Serial.println(T_bDiff);
  
}

void T_tick() {
  
  if (T_isDone()) { return; }
 
  unsigned long tRel = millis() - T_startTime; 
  unsigned long tDiff = (T_endTime - T_startTime);
  
  red = T_rStartVal + (tRel/(float)tDiff)*T_rDiff;
  green = T_gStartVal + (tRel/(float)tDiff)*T_gDiff;
  blue = T_bStartVal + (tRel/(float)tDiff)*T_bDiff;
  
}

boolean T_isDone() {
  return (T_endTime < millis());
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
