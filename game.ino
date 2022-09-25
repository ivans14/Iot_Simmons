//game(gameMode)
  //runs the game loop
  //return true/false depending on won or lose

int gameModeChoice = 1; 
bool compareSentence = true;

boolean game() {

  switch (gameModeChoice) {
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
              //1. Repeat a sequence of 8 colours
              if (compareSentence == true){
                return true;
              }

              else {
                return false;
              }
            }
            break;

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
