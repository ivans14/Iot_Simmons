////////////////////////////// DEFINING VARIABLES //////////////////////////////////

#include <ESP8266WiFi.h>
#include "src/iotc/common/string_buffer.h"
#include "src/iotc/iotc.h"

#define WIFI_SSID "Emilie H"
#define WIFI_PASSWORD "hej12345"

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const char* SCOPE_ID = "0ne007F8E89";
const char* DEVICE_ID = "8dv2yjd051";
const char* DEVICE_KEY = "4cU5CGDpNCWRLn/ShEr3YNTFVock2dflMapKPOYMYN4=";

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

int HighScore = 0;


////////////////////////////// AZURE //////////////////////////////////

//connecting to wifi
void on_event(IOTContext ctx, IOTCallbackInfo* callbackInfo);
#include "src/connection.h"

void on_event(IOTContext ctx, IOTCallbackInfo* callbackInfo) {
  // ConnectionStatus
  if (strcmp(callbackInfo->eventName, "ConnectionStatus") == 0) {
    LOG_VERBOSE("Is connected ? %s (%d)",
                callbackInfo->statusCode == IOTC_CONNECTION_OK ? "YES" : "NO",
                callbackInfo->statusCode);
    isConnected = callbackInfo->statusCode == IOTC_CONNECTION_OK;
    return;
  }

  AzureIOT::StringBuffer buffer;
  if (callbackInfo->payloadLength > 0) {
    buffer.initialize(callbackInfo->payload, callbackInfo->payloadLength);
  }

  LOG_VERBOSE("- [%s] event was received. Payload => %s\n",
              callbackInfo->eventName, buffer.getLength() ? *buffer : "EMPTY");

  if (strcmp(callbackInfo->eventName, "Command") == 0) {
    LOG_VERBOSE("- Command name was => %s\r\n", callbackInfo->tag);
  }
}

//send current sentence to azure
void Azure(int * sentece) {
  int index = 0 ;
  float length = 0;
    if (isConnected) {
      char msg[64] = {0};
      //check sentence length
      while (sentece[index] != 0){
        length ++;
        index++;
      }
      
      Serial.println("length");
      Serial.println(length);

      int pos = 0, errorCode = 0;
      pos = snprintf(msg, sizeof(msg) - 1, "{\"Sentence length\": %f }",length);
      errorCode = iotc_send_telemetry(context, msg, pos);
          
      msg[pos] = 0;
      if (errorCode != 0) {
        LOG_ERROR("Sending message has failed with error code %d", errorCode);
      }
    
    iotc_do_work(context);  // do background work for iotc
  } else {
    iotc_free_context(context);
    context = NULL;
    connect_client(SCOPE_ID, DEVICE_ID, DEVICE_KEY);
  }

}

//compare highscore with previous highscore and replace if higher
void compareScore (int j){
  Serial.println(j);
  if (j > HighScore){
    HighScore = j;
    Serial.println("New HighScore");
    lcd.setCursor(0, 1);
    lcd.print("HighScore:");
    lcd.setCursor(11, 1);
    lcd.print(j);
  }
}


//////////////////////////////// FUNCTIONS ///////////////////////////////////////

//skill level function that allows you to choose between 4 skill levels; 8, 14, 20, 31
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

    if (analogRead(buttonRed) == 1024) {
      skill = 8;
      skillChosen = true;
    }
    else if (digitalRead(buttonYellow) == LOW) {
      skill = 16;
      skillChosen = true;
    }
    else if (digitalRead(buttonBlue) == LOW) {
      skill = 24;
      skillChosen = true;
    }
    else if (digitalRead(buttonGreen) == LOW) {
      skill = 32;
      skillChosen = true;
    }
  }
  for (int i = 0; i < skill; i++) {
      currentSentence[i]=0;
  }
  return skill;
}

//sentenceAdd function that adds a number to the random generated sentence
int * SentenceAdd(int *  current_sentence) {
  int randNumber = random(1, 5);
  current_sentence[count] = randNumber;
  count += 1;
  return current_sentence;
}

//lightLED function lights LED and buzzer in a specific time according to input
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
}

//simonSpeaks function that speaks according to random sequence number (1,2,3,4) and uses lightLED function to display
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

//userSentenceInput that reads the users input and turns on LED and buzzer while pressing
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

//compare_sentence function that compares user input with given sequence
bool compare_sentence(int * sentence, int input, int ind) {
  if (sentence[ind] == input) {
    return true;
  }
  else {
    return false;
  }
}

//game function that runs runs the entire game while displaying on lcd screen
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

    simonSpeaks(currentSentence, sentenceLength);

    while (currentSentence[j] != 0) { //add a number to sequence if a round is won
      int input = userSentenceInput();
      delay(200);
      if (compare_sentence(currentSentence, input, j) == true) {
        j++;
      }
      else { //
        Serial.println("compare");
        compareScore(ind);
        return false;
      }
    }

    //send current sentence to azure
    Azure(currentSentence);

    delay(300);
    ind++;
  }

  //checks if sentence lenght has reached skill level, returns win if you have
  if (j >= sentenceLength) {
    //Serial.println("congrats");
    lcd.setCursor(11, 1);
    lcd.print(ind);
    compareScore(ind);
    return true;
  }
}

////////////////////////////// VOID SETUP - SETUP GAME //////////////////////////////////

//prepares leds, buttons, lcd
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

  randomSeed(analogRead(0));  

  lcd.init();
  lcd.backlight();

  // AZURE wifi connection
  connect_wifi(WIFI_SSID, WIFI_PASSWORD);
  connect_client(SCOPE_ID, DEVICE_ID, DEVICE_KEY);

  if (context != NULL) {
    lastTick = 0;  // set timer in the past to enable first telemetry a.s.a.p
  }


}

////////////////////////////// VOID LOOP - RUN GAME //////////////////////////////////

void loop() {

  //runs game
  bool ending = game();

  //returns win
  if (ending == true) {
    lcd.setCursor(4, 0);
    lcd.print("You Win   ");
    delay(5000);

    j = 0;
    skill;
    count = 0;
    currentSentence[32] = {0};
    ind = 0;

    chosen = false;
    skillChosen = false;
    
  }

  //returns loose
  else if (ending == false) {
    lcd.setCursor(4, 0);
    lcd.print("You Lost  ");
    delay(5000);

    j = 0;
    skill;
    count = 0;
    currentSentence[32] = {0};
    ind = 0;

    chosen = false;
    skillChosen = false;
    
  }
  delay(500);


}
