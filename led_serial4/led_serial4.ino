unsigned red = 0;
unsigned green = 0;
unsigned blue = 0;

int RedPin   = 9;
int GreenPin = 10;
int BluePin  = 11;

void setup() {
	
	int divisor = 256;
	setPwmFrequency(RedPin, divisor);
	setPwmFrequency(GreenPin, divisor);
	setPwmFrequency(BluePin, divisor);

	Serial.begin(9600);
	
}

int reading_state = 0;
String command;

void loop() {
	
	char byteIn;
	while (Serial.available()) {
		byteIn = Serial.read();

		if (byteIn == '#') {
			command = "";
		} else if (byteIn == '!') {
			doCommand(command);
		} else {
			command += byteIn;
		}

	}

	analogWrite(RedPin, red);
	analogWrite(GreenPin, green);
	analogWrite(BluePin, blue);

	T_tick();
	
}

void doCommand(String command) {
	// FORMAT: #RRGGBBTTTTT!
	// GIVEN AS: RGGBBTTTTT

	if (command.length() < 7) { gia(); return; }

	String rStr = command.substring(0,2);
	String gStr = command.substring(2,4);
	String bStr = command.substring(4,6);
	String tStr = command.substring(6);

	Serial.println(rStr);
	Serial.println(gStr);
	Serial.println(bStr);

	int rVal = toInt(rStr,16);
	int gVal = toInt(gStr,16);
	int bVal = toInt(bStr,16);
	int tVal = toInt(tStr,10);

	Serial.println(rVal);
	Serial.println(gVal);
	Serial.println(bVal);
	Serial.println(tVal);

	T_start(tVal, rVal, gVal, bVal);

}

void gia() {
	Serial.println("NO GIA BAD NUMBER");
}

struct TweenData {
	int startTime;
	int endTime;

	int rStartVal;
	int rEndVal;
	int rDiff;

	int gStartVal;
	int gEndVal;
	int gDiff;

	int bStartVal;
	int bEndVal;
	int bDiff;
};
TweenData tween;

void T_start(int duration, int rTarget, int gTarget, int bTarget) {
	
	tween.startTime = millis();
	tween.endTime = millis() + (unsigned long)duration;
	
	tween.rStartVal = red;
	tween.gStartVal = green;
	tween.bStartVal = blue;
	
	tween.rEndVal = rTarget;
	tween.gEndVal = gTarget;
	tween.bEndVal = bTarget;
	
	tween.rDiff = (tween.rEndVal - tween.rStartVal);
	tween.gDiff = (tween.gEndVal - tween.gStartVal);
	tween.bDiff = (tween.bEndVal - tween.bStartVal);
	
	Serial.println(tween.rDiff);
	Serial.println(tween.gDiff);
	Serial.println(tween.bDiff);
	
}

void T_tick() {
  
  if (T_isDone()) { return; }
 
  unsigned long tRel = millis() - tween.startTime; 
  unsigned long tDiff = (tween.endTime - tween.startTime);
  
  red = tween.rStartVal + (tRel/(float)tDiff)*tween.rDiff;
  green = tween.gStartVal + (tRel/(float)tDiff)*tween.gDiff;
  blue = tween.bStartVal + (tRel/(float)tDiff)*tween.bDiff;
  
}

boolean T_isDone() {
  return (tween.endTime < millis());
}


int toInt(String input, int base) {
	int total = 0;
	int digit;

	for (int i = 0; i < input.length(); i++) {
		if (input[i] >= '0' && input[i] <= '9') {
		  digit = input[i]-'0';
		} else {
		  digit = input[i]-'A'+10;
		}
		total += digit*pow(base, input.length()-i-1);
		//Serial.println(input);
		//Serial.println(input[i]);
		//Serial.println(digit);
		//Serial.println(total);
	}
	return total;
}






//////////////////////////////////
// PWM Frequency Adjust
//////////////////////////////////

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

