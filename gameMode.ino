//gameMode(choice)
//depending on the choice redirect to specific gamemode
//return chosenGamemode

//The game also features 4 skill levels:
//1. Repeat a sequence of 8 colours
//2. Repeat a sequence of 14 colours
//3. Repeat a sequence of 20 colours
//4. Repeat a sequence of 31 colours


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


void setup() {

  Serial.begin(9600);
  // put your setup code here, to run once:

}

void loop() {

  gameMode();

  // put your main code here, to run repeatedly:

}
