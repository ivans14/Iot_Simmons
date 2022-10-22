//Missing LCD Screen, Highscore Function /Able to save highscore.
#include <ESP8266WiFi.h>
#include "src/iotc/common/string_buffer.h"
#include "src/iotc/iotc.h"

#define WIFI_SSID "iPhone"
#define WIFI_PASSWORD "Nicoletaa"

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

int HighScore = 0;


////////////////////////////// AZURE //////////////////////////////////



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

  // payload buffer doesn't have a null ending.
  // add null ending in another buffer before print
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


///////////////////////////////////////////////////////////////////////

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
      skill = 8;
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
      currentSentence[i]=0;
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
        Serial.println("compare");
        compareScore(ind);
        return false;
      }
    }

    Azure(currentSentence);

    delay(300);
    ind++;
  }

  if (j >= sentenceLength) {
    //Serial.println("congrats");
    lcd.setCursor(11, 1);
    lcd.print(ind);
    compareScore(ind);
    return true;
  }
}

void res() {
  resett = true;
  while (resett = true) {
    j = 0;
    skill;
    count = 0;
    currentSentence[32] = {0};
    ind = 0;

    chosen = false;
    skillChosen = false;

    if (analogRead(buttonRed) == 1024) {
      resett = false;
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

  lcd.begin();
  lcd.backlight();

  // AZURE
  connect_wifi(WIFI_SSID, WIFI_PASSWORD);
  connect_client(SCOPE_ID, DEVICE_ID, DEVICE_KEY);

  if (context != NULL) {
    lastTick = 0;  // set timer in the past to enable first telemetry a.s.a.p
  }


}

void loop() {

  bool ending = game();


  if (ending == true) {
    //Serial.print("YOU WON");
    lcd.setCursor(4, 0);
    lcd.print("You Win");
    delay(50);
    res();
  }
  else if (ending == false) {
    //Serial.print("YOU LOST");
    lcd.setCursor(4, 0);
    lcd.print("You Lost");
    delay(50);
    res();
  }
  //game();
  delay(500);


}
