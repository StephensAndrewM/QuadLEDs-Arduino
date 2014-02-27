int red = 0;
int green = 0;
int blue = 0;

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


  boolean entering_val = false;
  char enter_color;
  int color_val;
 
  String content = "";
  char character;

void loop() {
  
  
  
  /*String input = getStringInput();
  if (input != "") {
     String commands[] = myString.split("\\s+");
     for(int i = 0; i < commands.length; i++) {
       String command = commands[i];
       int power = atoi(command.substring(1, 4))
       switch (command[0]) {
         case 'r': red = power; break;
         case 'g': green = power; break;
         case 'b': blue = power; break;
         default: badInput();
       }
     }
  }*/
  
  boolean new_val = false;
  
  while(Serial.available()) {
    new_val = true;
    character = Serial.read();
    
    // Whitespace Entered
    if (character == ' ') {
      Serial.write("WHITESPACE\n");
      
      // Reset Everything
      content = "";
      color_val = 0;
      entering_val = false;
    
    // Letter Command
    } else if (charInRange(character, 'A', 'Z')) {
      Serial.write("LETTER\n");
      
      // No Letters Should be Present While Entering Values
      if (entering_val) {
        badInput(); 
        break;
      } else {
        enter_color = character;
        entering_val = true;
      }
    
    // Part of Multi-Digit Number
    } else if (charInRange(character, '0', '9')) {
      content.concat(character);
      Serial.write("NUMBER\n");
      
      // Get Color Value
      color_val = atoi(content.c_str());
      Serial.print(String(color_val));

      // Make Sure Valid Input
      if (color_val < 0 || color_val > 255) {
        badInput();
      }
      
      // Assign to Proper Variable
      switch(enter_color) {
        case 'R': red = color_val; break;
        case 'G': green = color_val; break;
        case 'B': blue = color_val; break;
        default: badInput(); break;
      }
    
    // Non-Numeric, Non-Letter, and Non-Whitespace
    } else {
      Serial.write("ELSE\n");
      badInput();
      break;
    }
  }
  
  analogWrite (RedPin, red);
  analogWrite (GreenPin, green);
  analogWrite (BluePin, blue);
  
  // Debug
  if (new_val) {
    Serial.print("CYCLE\n");
    char debug[128];
    sprintf(debug, "R %d  G %d  B %d\n", red, green, blue);
    Serial.write(debug);
  }
  
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
  Serial.write("NO GIA BAD NUMBER\n");
  content = "";
}
