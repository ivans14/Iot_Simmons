
int current_sentence [10] = {0} ;
int count = 0;
int ledB = 13;     // LED pin
int ledG = 15;
int ledR = 0;
int ledY = 18;

int * SentenceAdd(int *  current_sentence) {
  int ran_number = rand() % 4 +1;
  current_sentence[count] = ran_number;
  count +=1;
  return current_sentence;
}

int TranslateList(int * sentence){
  for (int i =0; i< 10; i++){
  if (sentence[i]!=0){
    int color = sentence[i];
    if (color  == 1 ){
      digitalWrite(ledG, HIGH);
      digitalWrite(ledB,LOW);
      digitalWrite(ledR,LOW);
      digitalWrite(ledY,LOW);
      }
      else if (color  == 2) {
      digitalWrite(ledR, HIGH);
      digitalWrite(ledB,LOW);
      digitalWrite(ledG,LOW);
      digitalWrite(ledY,LOW);
      }
      else if (color == 3) {
      digitalWrite(ledB, HIGH);
      digitalWrite(ledG,LOW);
      digitalWrite(ledY,LOW);
      digitalWrite(ledR,LOW);
      }      
      else if (color == 4) {
      digitalWrite(ledY, HIGH);
      digitalWrite(ledG,LOW);
      digitalWrite(ledR,LOW);
      digitalWrite(ledB,LOW);
      };
  }
  else {
    break;
  }
  delay(1000);
  }
}

bool compare_sentence(int * sentence, int input, int index){
  if (sentence[index] == input) {
    return true;
  }
  else{
    return false;
  }
}
