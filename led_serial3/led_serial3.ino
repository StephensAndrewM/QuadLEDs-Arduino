#include <QueueList.h>

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

	TweenTick();
	
}

void doCommand(String command) {
	// FORMAT: #RRGGBBTTTTT!
	// GIVEN AS: RGGBBTTTTT

	if (command.length() < 7) { gia(); return; }

	String rStr = command.substring(0,2);
	String gStr = command.substring(2,4);
	String bStr = command.substring(4,6);
	String tStr = command.substring(6);

//        Serial.println(rStr);
//        Serial.println(gStr);
//        Serial.println(bStr);

	int rVal = toInt(rStr,16);
	int gVal = toInt(gStr,16);
	int bVal = toInt(bStr,16);
	int tVal = toInt(tStr,10);

        Serial.println(rVal);
        Serial.println(gVal);
        Serial.println(bVal);
        Serial.println(tVal);

	TweenPush(rVal, gVal, bVal, tVal);
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

void gia() {
	Serial.println("NO GIA BAD NUMBER");
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

	Tween* tween = (Tween*)malloc(sizeof(tween));

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
	Tween* tween = tweenQueue.peek();

	// Save Timing Data
	liveTween.startTime = millis();
	liveTween.endTime = millis() + tween->duration;

	// Save Starting Values
	liveTween.rStartVal = red;
	liveTween.gStartVal = green;
	liveTween.bStartVal = blue;

	// Save Ending Values
	liveTween.rEndVal = tween->red;
	liveTween.gEndVal = tween->green;
	liveTween.bEndVal = tween->blue;

}

void TweenTick() {

	if (tweenQueue.isEmpty()) {
		return; 		
	}
	Tween* current = tweenQueue.peek();

	if (liveTween.endTime <= millis()) { 
		Tween* done = tweenQueue.pop();
		free(done);
		return;
	}
 
	int tRel = millis() - liveTween.startTime; 
	int tDiff = (liveTween.endTime - liveTween.startTime);

	int rDiff = (liveTween.rEndVal - liveTween.rStartVal);
	red = liveTween.rStartVal + (tRel/tDiff)*rDiff;

	int gDiff = (liveTween.gEndVal - liveTween.gStartVal);
	green = liveTween.gStartVal + (tRel/tDiff)*gDiff;

	int bDiff = (liveTween.bEndVal - liveTween.bStartVal);
	blue = liveTween.bStartVal + (tRel/tDiff)*bDiff;

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
