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

	TweenInit();
}

void gia() {
	Serial.print("X");
}




typedef struct Tween {
	int startTime;
	int endTime;
	int rStartVal;
	int rEndVal;
	int gStartVal;
	int gEndVal;
	int bStartVal;
	int bEndVal;
} Tween*;

void TweenInit(int duration, int rTarget, int gTarget, int bTarget) {

	Tween* tween = malloc(sizeof(tween));

	tween->startTime = millis();
	tween->endTime = millis() + duration;

	tween->rStartVal = red;
	tween->gStartVal = green;
	tween->bStartVal = blue;

	tween->rEndVal = rTarget;
	tween->gEndVal = gTarget;
	tween->bEndVal = bTarget;

	// APPEND TO LIST

}

void TweenTick(tweens) {

	if (/* TWEENS LENGTH 0*/) {
		return; 		
	}
	Tween current = tweens[0];

	if (current->endTime <= millis()) { 
		// REMOVE FROM LIST
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
