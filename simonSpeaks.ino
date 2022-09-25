int LEDRed
int LEDBlue
int LEDGreen
int LEDYellow

const long lightUpTime = 1000;


void simonSpeaks(sentence){
  for (int pos = 0; pos < sentence.length(); pos++){
    int element = sentence[pos]
    newWord = false;
    switch(element){
      unsigned long previousMillis = millis();
      case 0:
        while(newWord = false){
          lightLED(LEDRed,previousMillis)
        }
        break;
        
      case 1:
        while(newWord = false){
          lightLED(LEDBlue,previousMillis)
        }
        break;
        
      case 2:
        while(newWord = false){
          lightLED(LEDGreen,previousMillis)
        }
        break;
        
      case 3:
        while(newWord = false){
          lightLED(LEDYellow,previousMillis)
        }
        break;
    }
  }
  return simonDone = true;
}

void lightLED(LED,previousMillis){
  digitalWrite(HIGH,LED)
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= lightUpTime){
    digitalWrite(LOW,LED)
    newWord = true;   
    }
}
