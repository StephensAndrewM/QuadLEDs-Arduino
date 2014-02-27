 //Developed by Rajarshi Roy
int red, green, blue; //red, green and blue values
int RedPin = 9; //Red: PWM pin 10
int GreenPin = 10; //Green: PWM pin 11
int BluePin = 11; //Blue: PWM pin 9



void setup()
{

//initial values (no significance)
red   = 128;
green = 128;
blue  = 0;

int divisor = 256;

setPwmFrequency(RedPin, divisor);
setPwmFrequency(GreenPin, divisor);
setPwmFrequency(BluePin, divisor);

//pinMode(RedPin, OUTPUT);
//pinMode(GreenPin, OUTPUT);
//pinMode(BluePin, OUTPUT);

}

void loop()
{

//finally control led brightness through pulse-width modulation
analogWrite (RedPin, red);
analogWrite (GreenPin, green);
analogWrite (BluePin, blue);
delay(10); //just to be safe

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
