unsigned red = 0;
unsigned green = 0;
unsigned blue = 0;

int RedPin = 9;
int GreenPin = 10;
int BluePin = 11;

void setup() {
  
  T_start(10000, 255, 128, 0);
  
}

void loop() {
  
  T_tick();
  
}

int T_startTime;
int T_endTime;
int T_rStartVal;
int T_rEndVal;
int T_gStartVal;
int T_gEndVal;
int T_bStartVal;
int T_bEndVal;

void T_start(int duration, int rTarget, int gTarget, int bTarget) {
  
  T_startTime = millis();
  T_endTime = millis() + duration;
  
  T_rStartVal = red;
  T_gStartVal = green;
  T_bStartVal = blue;
  
  T_rEndVal = rTarget;
  T_gEndVal = gTarget;
  T_bEndVal = bTarget;
  
}

void T_tick() {
  
  if (T_endTime < millis()) { return; }
 
  int tRel = millis() - T_startTime; 
  int tDiff = (T_endTime - T_startTime);
  
  int rDiff = (T_rEndVal - T_rStartVal);
  red = T_rStartVal + (tRel/tDiff)*rDiff;
  
  int gDiff = (T_gEndVal - T_gStartVal);
  green = T_gStartVal + (tRel/tDiff)*gDiff;

  int bDiff = (T_bEndVal - T_bStartVal);
  blue = T_bStartVal + ((float)tRel/tDiff)*bDiff;
  
}
