#include <ESP8266WiFi.h>
#include "src/iotc/common/string_buffer.h"
#include "src/iotc/iotc.h"

#define WIFI_SSID "iPhone"
#define WIFI_PASSWORD "Nicoletaa"

const char* SCOPE_ID = "0ne00779B2B";
const char* DEVICE_ID = "1171zau1rq1";
const char* DEVICE_KEY = "ibSS8DMSL9QQfRHZbyBSdDHXIFg+YeXbzsJlSjg4TYI==";

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
  int length = 0 ;
    if (isConnected) {
      char msg[64] = {0};
      //check sentence length
      while (sentece[length] != 0){
        length ++;
      }

      int pos = 0, errorCode = 0;
      pos = snprintf(msg, sizeof(msg) - 1, "{\"Sentence length\": %f}",length);
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
int j=0;

const long lightUpTime = 1000;
const long inputTime = 5000;

int count = 0;
int currentSentence[32]= {0};
bool newWord;
bool chosen = false;


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
int userSentenceInput(){
  long previousInputMillis = millis();
  long inputMillis = millis();

  while (inputMillis - previousInputMillis >= inputTime){
    inputMillis = millis();
    if (digitalRead(buttonYellow) == HIGH){
      while(digitalRead(buttonYellow) == HIGH){
        digitalWrite(LEDYellow,HIGH);
      }
      digitalWrite(LEDYellow,LOW);
      return 1;
    }
    else if (digitalRead(buttonGreen) == HIGH){
      while(digitalRead(buttonGreen) == HIGH){
        digitalWrite(LEDGreen,HIGH);
      }
      digitalWrite(LEDGreen,LOW);
      return 2;
    }
    else if (digitalRead(buttonRed) == HIGH){
      while(digitalRead(buttonRed) == HIGH){
        digitalWrite(LEDRed,HIGH);
      }
      digitalWrite(LEDRed,LOW);
      return 3;
    }
    else if (digitalRead(buttonBlue) == HIGH){
      while(digitalRead(buttonBlue) == HIGH){
        digitalWrite(LEDBlue,HIGH);
      }
      digitalWrite(LEDBlue,LOW);
      return 4;
    }
  }
  return 5; 
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
              if (compare_sentence(currentSentence, input, j) == true) {
                j++;
              }
              else {
                return false;
                break;
              }
            }
            
            //win level
            if (j==sentenceLength-1){
              break;
              }
              break;
            //   break into gamemode switch
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

  pinMode(LEDRed, OUTPUT);
  pinMode(LEDBlue, OUTPUT);
  pinMode(LEDGreen, OUTPUT);
  pinMode(LEDYellow, OUTPUT);

  pinMode(buttonRed, INPUT_PULLUP);
  pinMode(buttonBlue, INPUT_PULLUP);
  pinMode(buttonGreen, INPUT_PULLUP);
  pinMode(buttonYellow, INPUT_PULLUP);
  //Azure hookup setup
  connect_wifi(WIFI_SSID, WIFI_PASSWORD);
  connect_client(SCOPE_ID, DEVICE_ID, DEVICE_KEY);

  if (context != NULL) {
    lastTick = 0;  // set timer in the past to enable first telemetry a.s.a.p
  }
}

void loop() {

  game();

  // put your main code here, to run repeatedly:

}