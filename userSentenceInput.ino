
//userSentence(inputs)
  // Adds the users input to a list
  // return userSentence

//int numberOfElements = 31;
int userSentence[31] = {};
int j = 0; 

int ledBlue = D8;
int ledGreen = D7;
int ledRed = D6;
int buttonYellow = 1;
int buttonGreen = 1;
int buttonRed = 1;
int buttonBlue = 1;
//int x = 0;

void userSentenceInput () {
  if (digitalRead(buttonYellow) == LOW){
  //if (x = 1){
    j++;
    userSentence[j]=1; //1 represents yellow

    //light led
    analogWrite(ledRed, 251);
    analogWrite(ledGreen, 163);
    analogWrite(ledBlue, 26);
  }
  
  else if (digitalRead(buttonGreen) == LOW){
    j++;
    userSentence[j]=2; //2 represents green
    analogWrite(ledRed, 0);
    analogWrite(ledGreen, 255);
    analogWrite(ledBlue, 0);
  }
  
  else if (digitalRead(buttonRed) == LOW){
    j++;
    userSentence[j]=3; //3 represents red
    analogWrite(ledRed, 255);
    analogWrite(ledGreen, 0);
    analogWrite(ledBlue, 0);   
  }
  
  else if (digitalRead(buttonBlue) == LOW){
    j++;
    userSentence[j]=4; //4 represents blue
    analogWrite(ledRed, 0);
    analogWrite(ledGreen, 0);
    analogWrite(ledBlue, 255);
  }
  return userSentence;
}


void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  //prepare LED'S
  pinMode(buttonYellow, INPUT_PULLUP);
  pinMode(buttonGreen, INPUT_PULLUP);
  pinMode(buttonRed, INPUT_PULLUP);
  pinMode(buttonBlue, INPUT_PULLUP);
  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledBlue, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  //x = 1;

  userSentenceInput();
  
  //Serial.println(userSentence[1]);

  delay(10000);
}
