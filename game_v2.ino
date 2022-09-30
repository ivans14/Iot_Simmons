//============================================variables==========================================

int gameModeChoice = 1; 
bool compareSentence = true;
int LEDRed;
int LEDBlue;
int LEDGreen;
int LEDYellow;
int buttonYellow = 1;
int buttonGreen = 1;
int buttonRed = 1;
int buttonBlue = 1;
int j=0;

const long lightUpTime = 1000;
int count = 0;
int currentSentence[32]= {0};
bool newWord;


//=============================================functions==========================================

int gameMode() {
  Serial.println("Welcome to Simons Game. Please chose a game mode number.");
  Serial.println("    1. Single Player");
  Serial.println("    2. Multi Player - adding colors alternately");
  Serial.println("    1. Multi Player - responsible for a color each");

  while (Serial.available() == 0) {
  }

  int gameModeChoice = Serial.parseInt();

  return gameModeChoice;
  
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


void lightLED(int LED,int previousMillis){
  digitalWrite(HIGH,LED);
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= lightUpTime){
    digitalWrite(LOW,LED);
    newWord = true;   
    }
}

//user input function
//void userSentenceInput () {
//  if (digitalRead(buttonYellow) == LOW){
//    j++;
//    currentSentence[j]=1; //1 represents yellow
//
//    //light led
//    lightLED
//
//  }
//  
//  else if (digitalRead(buttonGreen) == LOW){
//    j++;
//    currentSentence[j]=2; //2 represents green
//    analogWrite(ledRed, 0);
//    analogWrite(ledGreen, 255);
//    analogWrite(ledBlue, 0);
//  }
//  
//  else if (digitalRead(buttonRed) == LOW){
//    j++;
//    currentSentence[j]=3; //3 represents red
//    analogWrite(ledRed, 255);
//    analogWrite(ledGreen, 0);
//    analogWrite(ledBlue, 0);   
//  }
//  
//  else if (digitalRead(buttonBlue) == LOW){
//    j++;
//    currentSentence[j]=4; //4 represents blue
//    analogWrite(ledRed, 0);
//    analogWrite(ledGreen, 0);
//    analogWrite(ledBlue, 255);
//  }
//  return currentSentence;
//}


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
          lightLED(LEDBlue,previousMillis);
          }
        }
        break;
        
      case 2: {
        while(newWord = false){
          lightLED(LEDGreen,previousMillis);
        }}
        break;
        
      case 3: {
        while(newWord = false){
          lightLED(LEDYellow,previousMillis);
          }
        }
        break;
      case 4: {
        while(newWord = false){
          lightLED(LEDRed,previousMillis);
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
        Serial.println("You have chosen gamemode 1. Please choose skill level from 1-4");
        Serial.println("    1. Repeat a sequence of 8 colours");
        Serial.println("    2. Repeat a sequence of 14 colours");
        Serial.println("    3. Repeat a sequence of 20 colours");
        Serial.println("    4. Repeat a sequence of 31 colours");

        while (Serial.available() == 0) {
        }
        int skillLevel = Serial.parseInt();
        switch (skillLevel) {
          case 1: {
            int sentenceLength = 8; 
            SentenceAdd(currentSentence);
            int index = 0;
            simonSpeaks(currentSentence,sentenceLength);

            //win or loose round
            while (currentSentence[j]!=0){ //while loop and lose ==false
              //input a number (rewrite userSentenceInput to return int and light up LED)
              int input = userSentenceInput(); 
              if (compare_sentence(currentSentence, input, j) = true) {
                j++;
              }
              else {
                return = false;
                break;
              }
            }
            
            //win level
            if (j==sentenceLength){
              break;
              }
            }

          case 2: {
              //2. Repeat a sequence of 14 colours
              if (compareSentence == true){
                return true;
              }

              else {
                return false;
              }
            }
            break;

          case 3: {
              //3. Repeat a sequence of 20 colours
              if (compareSentence == true){
                return true;
              }

              else {
                return false;
              }
            }
            break;

          case 4: {
              //4. Repeat a sequence of 31 colours
              if (compareSentence == true){
                return true;
              }

              else {
                return false;
              }
            }
            break;

          default: {
              Serial.println("Please choose a valid option. Valid options are 1, 2, 3 or 4.");
            }
        }

      } break;

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
        Serial.println("You have chosen gamemode 2.");
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
  // put your setup code here, to run once:

}

void loop() {

  game();

  // put your main code here, to run repeatedly:

}
