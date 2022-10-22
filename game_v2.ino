// Highscore Function /Able to save highscore.

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int gameModeChoice = 1;

bool compareSentence = true;

int LEDRed = 0;
int LEDBlue = 15;
int LEDGreen = 16;
int LEDYellow = 2;
int buttonYellow = 3;
int buttonGreen = 12;
int buttonRed = A0;
int buttonBlue = 14;
//int buttonReset = D0;

int buzzerPin = 13;

int j = 0;

const long lightUpTime = 500;
const long inputTime = 5000;

int skill;
int count = 0;
int currentSentence[32] = {0};
int ind = 0;

bool chosen = false;
bool skillChosen;
bool resett = false;

int skillLevel() {
  delay(300);

  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("CHOOSE SKILL:");
  lcd.setCursor(0, 1);
  lcd.print("8, 14, 20 or 31");

  Serial.println("Please choose skill level from 1-4");
  Serial.println("    1. Repeat a sequence of 8 colours");
  Serial.println("    2. Repeat a sequence of 14 colours");
  Serial.println("    3. Repeat a sequence of 20 colours");
  Serial.println("    4. Repeat a sequence of 31 colours");
  skillChosen = false;

  while (skillChosen == false) {
    delay(10);
    //Serial.println(analogRead(buttonRed));

    if (analogRead(buttonRed) == 1024) {
      skill = 3;
      skillChosen = true;
    }
    else if (digitalRead(buttonYellow) == LOW) {
      skill = 16;
      skillChosen = true;
    }
    else if (digitalRead(buttonGreen) == LOW) {
      skill = 24;
      skillChosen = true;
    }
    else if (digitalRead(buttonBlue) == LOW) {
      skill = 32;
      skillChosen = true;
    }
  }
  for (int i = 0; i < skill; i++) {
    currentSentence[i] = 0;
    //Serial.println(currentSentence[i]);
  }
  return skill;
}

int * SentenceAdd(int *  current_sentence) {
  int randNumber = random(1, 5);
  current_sentence[count] = randNumber;
  count += 1;
  return current_sentence;
}

void lightLED(int LED, int previousMillis, int freq) {
  //Serial.println("light");
  digitalWrite(LED, HIGH);
  tone(buzzerPin, freq);
  unsigned long currentMillis = millis();
  while (currentMillis - previousMillis <= lightUpTime) {
    currentMillis = millis();
  }
  digitalWrite(LED, LOW);
  noTone(buzzerPin);
  delay(500);
  //newWord=true;
}

boolean simonSpeaks(int * sentence, int length) {

  for (int pos = 0; pos < length; pos++) {
    int element = sentence[pos];
    unsigned long previousMillis = millis();

    if (element == 0) {
      return element;
    }

    else if (element == 1) {
      lightLED(LEDRed, previousMillis, 4000);
    }

    else if (element == 2) {
      lightLED(LEDYellow, previousMillis, 2000);
    }

    else if (element == 3) {
      lightLED(LEDBlue, previousMillis, 1000);
    }

    else if (element == 4) {
      lightLED(LEDGreen, previousMillis, 3000);
    }
  }
}

int userSentenceInput() {
  long previousInputMillis = millis();
  long inputMillis = millis();

  while (inputMillis - previousInputMillis <= inputTime) {
    inputMillis = millis();
    delay(200);
    if (digitalRead(buttonYellow) == LOW) {
      while (digitalRead(buttonYellow) == LOW) {
        digitalWrite(LEDYellow, HIGH);
        tone(buzzerPin, 2000);
      }
      delay(200);
      digitalWrite(LEDYellow, LOW);
      noTone(buzzerPin);
      return 2;
    }
    else if (digitalRead(buttonGreen) == LOW) {
      while (digitalRead(buttonGreen) == LOW) {
        digitalWrite(LEDGreen, HIGH);
        tone(buzzerPin, 3000);
      }
      delay(200);
      digitalWrite(LEDGreen, LOW);
      noTone(buzzerPin);
      return 4;
    }
    else if (analogRead(buttonRed) == 1024) {
      while (analogRead(buttonRed) == 1024) {
        digitalWrite(LEDRed, HIGH);
        tone(buzzerPin, 4000);
      }
      digitalWrite(LEDRed, LOW);
      noTone(buzzerPin);
      delay(200);
      return 1;
    }
    else if (digitalRead(buttonBlue) == LOW) {
      while (digitalRead(buttonBlue) == LOW) {
        digitalWrite(LEDBlue, HIGH);
        tone(buzzerPin, 1000);
      }
      digitalWrite(LEDBlue, LOW);
      noTone(buzzerPin);
      delay(200);
      return 3;
    }
  }
  delay(200);
  //Serial.print("5");
  return 5;
}

bool compare_sentence(int * sentence, int input, int ind) {
  if (sentence[ind] == input) {
    return true;
  }
  else {
    return false;
  }
}


boolean game() {
  int sentenceLength = skillLevel();
  ind = 0;
  delay(2000);
  Serial.print(sentenceLength);
  Serial.print(ind);

  while (ind < sentenceLength) {

    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("Skill");
    lcd.setCursor(11, 0);
    lcd.print(sentenceLength);
    lcd.setCursor(4, 1);
    lcd.print("Score:");
    lcd.setCursor(11, 1);
    lcd.print(ind);

    Serial.print(ind);
    j = 0;
    SentenceAdd(currentSentence);
    delay(50);
    /*for (int i = 0; i < sentenceLength; i++) {
      //Serial.println(currentSentence[i]);
      }*/


    simonSpeaks(currentSentence, sentenceLength);

    while (currentSentence[j] != 0) {
      int input = userSentenceInput();
      delay(200);
      if (compare_sentence(currentSentence, input, j) == true) {
        //Serial.print("correct");
        j++;
      }
      else {
        //Serial.println("you lose");
        return false;
      }
    }

    delay(300);
    ind++;
  }

  if (j >= sentenceLength) {
    lcd.setCursor(11, 1);
    lcd.print(ind);
    //Serial.println("congrats");
    return true;
  }
}

void res() {
  resett = true
  while (resett = true) {
    j = 0;
    skill;
    count = 0;
    currentSentence[32] = {0};
    ind = 0;

    chosen = false;
    skillChosen = false;

    if (analogRead(buttonRed) == 1024) {
      resett = false
    }
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(LEDRed, OUTPUT);
  pinMode(LEDBlue, OUTPUT);
  pinMode(LEDGreen, OUTPUT);
  pinMode(LEDYellow, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  pinMode(buttonRed, INPUT);
  pinMode(buttonBlue, INPUT_PULLUP);
  pinMode(buttonGreen, INPUT_PULLUP);
  pinMode(buttonYellow, INPUT_PULLUP);
  //pinMode(buttonReset, INPUT_PULLUP);

  randomSeed(analogRead(0));

  lcd.init();
  lcd.backlight();

}

void loop() {

  bool ending = game();

  if (ending == true) {
    lcd.setCursor(4, 0);
    lcd.print("You Win");
    delay(50);
    res();

    /*j = 0;
      skill;
      count = 0;
      currentSentence[32] = {0};
      ind = 0;

      chosen = false;
      skillChosen = false;*/
  }
  else if (ending == false) {
    lcd.setCursor(4, 0);
    lcd.print("You Lost");
    delay(50);
    res();

    /*
      j = 0;
      skill;
      count = 0;
      currentSentence[32] = {0};
      ind = 0;

      chosen = false;
      skillChosen = false;*/
  }
  //game();
  delay(500);


}
