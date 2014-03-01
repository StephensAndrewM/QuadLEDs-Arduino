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
	
	Serial.begin(9600);
	
}

int reading_state = 0;

void loop() {
	
	char byteIn;
	String command;
	while (Serial.available()) {
		byteIn = Serial.read();
		command += byteIn;

		if (byteIn == '#') {
			command = "";
		} else if (byteIn == '!') {
			doCommand(command);
		}

	}

	TweenTick();
	
}

void doCommand(String command) {
	// FORMAT: #RRGGBBTTTTT!
	// GIVEN AS: RGGBBTTTTT

	if (command.length < 7) { gia(); return; }

	String rStr = command.substr(0,2);
	String gStr = command.substr(2,4);
	String bStr = command.substr(4,6);
	String tStr = command.substr(6);

	int rVal = toInt(rStr,16);
	int gVal = toInt(gStr,16);
	int bVal = toInt(bStr,16);
	int tVal = toInt(tStr);

	TweenPush(rVal, gVal, bVal, tVal);
}

int toInt(String input, int base) {
	int total = 0;
	for (int i = 0; i < base.length; i++) {
		total += (input[i]-'0')*pow(base, base.length-i-1);
	}
	return total;
}

void gia() {
	Serial.print("X");
}


struct Tween {
	int duration;
	int red;
	int green;
	int blue;
};

struct TweenData {
	int startTime;
	int endTime;
	int rStartVal;
	int rEndVal;
	int gStartVal;
	int gEndVal;
	int bStartVal;
	int bEndVal;
};

QueueList<Tween*> tweenQueue;
TweenData liveTween;

void TweenPush(int duration, int rTarget, int gTarget, int bTarget) {

	Tween* tween = malloc(sizeof(tween));

	tween->duration = duration;
	tween->red = rTarget;
	tween->green = gTarget;
	tween->blue = bTarget;

	tweenQueue.push(tween);

}

void TweenInit() {

	// Don't Do Anything if No Tween to Initialize
	if (tweenQueue.isEmpty()) {
		return; 		
	}

	// Get the Tween Input Data
	tween = tweenQueue.peek();

	// Save Timing Data
	liveTween.startTime = millis();
	liveTween.endTime = millis() + duration;

	// Save Starting Values
	liveTween.rStartVal = red;
	liveTween.gStartVal = green;
	liveTween.bStartVal = blue;

	// Save Ending Values
	liveTween.rEndVal = tween->red;
	liveTween.gEndVal = tween->green;
	liveTween.bEndVal = tween->blue;

}

void TweenTick(tweens) {

	if (tweenQueue.isEmpty()) {
		return; 		
	}
	Tween current = tweens[0];

	if (current->endTime <= millis()) { 
		tweenQueue.pop()
		return;
	}
 
	int tRel = millis() - current->startTime; 
	int tDiff = (current->endTime - current->startTime);

	int rDiff = (current->rEndVal - current->rStartVal);
	red = current->rStartVal + (tRel/tDiff)*rDiff;

	int gDiff = (current->gEndVal - current->gStartVal);
	green = current->gStartVal + (tRel/tDiff)*gDiff;

	int bDiff = (current->bEndVal - current->bStartVal);
	blue = current->bStartVal + (tRel/tDiff)*bDiff;

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

/*String getStringInput() {
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
*/