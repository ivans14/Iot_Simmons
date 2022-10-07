//Missing LCD Screen, Highscore Function /Able to save highscore.

//============================================variables==========================================

int gameModeChoice = 1; 

bool compareSentence = true;

int LEDRed = 8;
int LEDBlue = 11;
int LEDGreen = 10;
int LEDYellow = 9;
int buttonYellow = 3;
int buttonGreen = 4;
int buttonRed = 2;
int buttonBlue = 1;

int buzzerPin = 6; 

int j=0;

const long lightUpTime = 1000;
const long inputTime = 5000;

int skill;
int count = 0;
int currentSentence[32]= {0};
bool newWord;
bool chosen = false;
bool skillChosen = false;


//=============================================functions==========================================

int gameMode() {
  Serial.println("Welcome to Simons Game. Please chose a game mode number.");
  Serial.println("    1. Single Player");
  Serial.println("    2. Multi Player - adding colors alternately");
  Serial.println("    1. Multi Player - responsible for a color each");

  /*while (Serial.available() == 0) {
  }*/

  while(chosen == false){
    if(buttonRed == HIGH){
      int gameModeChoice = 1;
      chosen = true;
    }
    else if(buttonYellow == HIGH){
      int gameModeChoice = 2;
      chosen = true;
    }
    else if(buttonGreen == HIGH){
      int gameModeChoice = 3;
      chosen = true;
    }
  }

  //int gameModeChoice = Serial.parseInt();

  return gameModeChoice;  
}

int skillLevel() {
  Serial.println("You have chosen gamemode 1. Please choose skill level from 1-4");
  Serial.println("    1. Repeat a sequence of 8 colours");
  Serial.println("    2. Repeat a sequence of 14 colours");
  Serial.println("    3. Repeat a sequence of 20 colours");
  Serial.println("    4. Repeat a sequence of 31 colours");

  while(skillChosen == false){
    if(buttonRed == HIGH){
      int skill = 8;
      skillChosen = true;
    }
    else if(buttonYellow == HIGH){
      int skill = 16;
      skillChosen = true;
    }
    else if(buttonGreen == HIGH){
      int skill = 24;
      skillChosen = true;
    }
    else if(buttonBlue == HIGH){
      int skill = 32;
      skillChosen = true;
    }
  }
  return skill;
}

int * SentenceAdd(int *  current_sentence) {
  int ran_number = rand() % 4 +1;
  current_sentence[count] = ran_number;
  count +=1;
  return current_sentence;
}

bool compare_sentence(int * sentence, int input, int index){
  if (sentence[index] == input) {
    return true;
  }
  else{
    return false;
  }
}

//user input function
int userSentenceInput(){
  long previousInputMillis = millis();
  long inputMillis = millis();

  while (inputMillis - previousInputMillis >= inputTime){
    inputMillis = millis();
    if (digitalRead(buttonYellow) == HIGH){
      while(digitalRead(buttonYellow) == HIGH){
        digitalWrite(LEDYellow,HIGH);
        tone(buzzerPin,1000);
      }
      digitalWrite(LEDYellow,LOW);
      noTone(buzzerPin);
      return 1;
    }
    else if (digitalRead(buttonGreen) == HIGH){
      while(digitalRead(buttonGreen) == HIGH){
        digitalWrite(LEDGreen,HIGH);
        tone(buzzerPin,2000);
      }
      digitalWrite(LEDGreen,LOW);
      noTone(buzzerPin);
      return 2;
    }
    else if (digitalRead(buttonRed) == HIGH){
      while(digitalRead(buttonRed) == HIGH){
        digitalWrite(LEDRed,HIGH);
        tone(buzzerPin,3000);
      }
      digitalWrite(LEDRed,LOW);
      noTone(buzzerPin);
      return 3;
    }
    else if (digitalRead(buttonBlue) == HIGH){
      while(digitalRead(buttonBlue) == HIGH){
        digitalWrite(LEDBlue,HIGH);
        tone(buzzerPin,4000);
      }
      digitalWrite(LEDBlue,LOW);
      noTone(buzzerPin);
      return 4;
    }
  }
  return 5; 
}

void lightLED(int LED,int previousMillis, int freq){
  digitalWrite(HIGH,LED);
  tone(buzzerPin,freq);
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= lightUpTime){
    digitalWrite(LOW,LED);
    noTone(buzzerPin);
    newWord = true;   
    }
}

boolean simonSpeaks(int * sentence, int length){
  for (int pos = 0; pos < length; pos++){
    int element = sentence[pos];
    boolean newWord = false;
    switch(element){
      unsigned long previousMillis = millis();
      case 0: {
        return false;
        }
        
      case 1: {
        while(newWord = false){
          lightLED(LEDBlue,previousMillis,1000);
          }
        }
        break;
        
      case 2: {
        while(newWord = false){
          lightLED(LEDGreen,previousMillis,2000);
        }}
        break;
        
      case 3: {
        while(newWord = false){
          lightLED(LEDYellow,previousMillis,3000);
          }
        }
        break;
      case 4: {
        while(newWord = false){
          lightLED(LEDRed,previousMillis,4000);
          }
        }
        break;
    }
  }
}


//=============================run the game with all functions=========================================
boolean game() {
  
  switch (gameMode()) {
    case 1: {
      //Could insert a while loop of which is entered when the case is first fulfill
      int sentenceLength = skillLevel();
      int * currentSentence = SentenceAdd(currentSentence);

      simonSpeaks(currentSentence,sentenceLength);

      while (currentSentence[j]!=0)
      {
        int input = userSentenceInput();
        if(compare_sentence(currentSentence,input,j)==true){
          j++;
        }
        else{
          return false;
        }

      }

      if (j >= sentenceLength)
      {
        return true;
      }

    } 
      break;

    case 2: {
      Serial.println("You have chosen gamemode 2.");
      //run function of gamemode 2
      if (compareSentence == true){
        return true;
      }

      else {
        return false;
      }
    }
    break;

    case 3: {
      Serial.println("You have chosen gamemode 3.");
      //run function of gamemode 2'
      if (compareSentence == true){
        return true;
      }

      else {
        return false;
      }
    }
    break;

    default: {
        Serial.println("Please choose a valid option. Valid options are 1, 2 or 3.");
      }
  }
}


void setup() {

  Serial.begin(9600);

  pinMode(LEDRed, OUTPUT);
  pinMode(LEDBlue, OUTPUT);
  pinMode(LEDGreen, OUTPUT);
  pinMode(LEDYellow, OUTPUT);

  pinMode(buttonRed, INPUT_PULLUP);
  pinMode(buttonBlue, INPUT_PULLUP);
  pinMode(buttonGreen, INPUT_PULLUP);
  pinMode(buttonYellow, INPUT_PULLUP);

}

void loop() {

  bool ending = game();

  if (ending == true){
    //Player won
  }
  else if(ending == false){
    //Player Lost
  }

  // put your main code here, to run repeatedly:

}