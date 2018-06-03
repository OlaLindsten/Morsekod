#include <LiquidCrystal.h>                                                //Include liquid crystal library

const int ledPinG = 13;                                                   //Declaring the green LED to pin number 13
const int ledPinR = 10;                                                   //Declaring the red LED to pin number 10
const int buttonPin = 8;                                                  //Declaring button to pin number 8

int buttonState = 0;                                                      //Value of button

byte previous = LOW;                                                      //Set previous startvalue to LOW

unsigned long timeHeld;                                                   //Varible to store how long the button was held for
unsigned long firstPress;                                                 //First time you press the button
unsigned long timeReleased;                                               //A variable to store how long there is between buttonpress
unsigned long counter = 0;

String returnLetter = "";                                                 //Letter that is returned
String morseWord = "";                                                    //String to store the morse code in

boolean myBool = false;                                                   //Variable that stops the for loop

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;               //Declaring the pins to the display
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int rows = 2;                                                       //Declaring amount of rows on display
const int columns = 16;                                                   //Declaring amount of columns

unsigned int rowNumber = 0;                                               //Get current row

int currentRow = 0;                                                       //Get current space on the row
int debug = 1;                                                            //If we should print to console, 1 = yes (true), 0 = no (false)

void setup() {
  lcd.begin(columns, rows);                                               //Starting display
  if(debug){
    Serial.begin(9600);                                                   //Starting console
  }
  pinMode(ledPinG, OUTPUT);                                               //Declare the green pin to output
  pinMode(ledPinR, OUTPUT);                                               //Declare the red pin to output
  pinMode(buttonPin, INPUT);                                              //Declare button to input
}

void loop() {
  buttonState = digitalRead(buttonPin);                                                 //Read the state of the button:

  if (buttonState == HIGH && previous == LOW && (millis() - firstPress) > 200) {        //If buttonState changes remember the start time
    firstPress = millis();                                                              //Get currentRow time and set it to firstPress
    digitalWrite(ledPinG, HIGH);                                                        //Turn on the LED
  }

  timeHeld = (millis() - firstPress);                                                   //Calculate how long the button was pressed for

  if (buttonState == LOW && previous == HIGH && (millis() - timeReleased) > 200) {      //If button is released get the current millis
    timeReleased = millis();
    myBool = true;
  }

  counter = (millis() - timeReleased);                                                  //Calculate how long the button was released for

  if (timeHeld > 50) {                            //This is a debounce tool that insures that the button is pressed for more than 100 millis
    //Check that the button is released
    if (buttonState == LOW && previous == HIGH) {

      if (timeHeld < 300) {                                                             //Check if the button was held for less than 300 millis
        morseWord += ".";
        delay(50);
      } else if (timeHeld >= 300 && timeHeld <= 800) {                                 //Check if the button is held between 300 and 800 millis
        morseWord += "-";
        delay(50);
      } else if (timeHeld > 800 && timeHeld <= 1500) {          //Check if the button is held for over 800 millis and less than 1500 millis
        lcd.print(" ");                                                               //Print a space on the display
        currentRow++;                                                                 //Increase currentRow
        myBool = false;
        Serial.print("Space written");
      }else if(timeHeld > 1500){
        Serial.print("Erasing last letter");
        currentRow--;
        lcd.setCursor(currentRow, rowNumber);
        lcd.print(" ");
        lcd.setCursor(currentRow, rowNumber);
        myBool = false;
      }
      digitalWrite(ledPinG, LOW);                                                     //Turn the green LED off
      Serial.println(morseWord);                                                      //Print word to console
    }
  }

  while (counter > 2000 && myBool == true) {                                    //While counter is bigger than 2 seconds and mybool is true
    returnLetter = "";
    morseToLetters();
    lcd.print(returnLetter);
    currentRow++;                                                //increase currentRow
  }
  if (currentRow == 16 && rowNumber == 0) {                      //Switch when on the last space
    rowNumber ++;                                                //Add one to rows
    currentRow = 0;                                              //Reset row
    lcd.setCursor(0, rowNumber);                                 //Jump down a line
  }else if(currentRow == 17 && rowNumber == 1){                  //Need to press an extra time for the lcd to clear
    lcd.clear();
    rowNumber = 0;
    currentRow = 0;
  }
  previous = buttonState;                                        //Set previous to buttonState
}

void morseToLetters() {

  String letters[26][2] = {                                      //2d array with every letter in my choosen morse code
    {"A", ".-"},
    {"B", "-..."},
    {"C", "-.-."},
    {"D", "-.."},
    {"E", "."},
    {"F", "..-."},
    {"G", "--."},
    {"H", "...."},
    {"I", ".."},
    {"J", ".---"},
    {"K", "-.-"},
    {"L", ".-.."},
    {"M", "--"},
    {"N", "-."},
    {"O", "---"},
    {"P", ".--."},
    {"Q", "--.-"},
    {"R", ".-."},
    {"S", "..."},
    {"T", "-"},
    {"U", "..-"},
    {"V", "...-"},
    {"W", ".--"},
    {"X", "-..-"},
    {"Y", "-.--"},
    {"Z", "--.."},
  };

  if (myBool == true) {
    for (int i = 0; i < 27; i++) {                    //Loop through each row in the array
      if (morseWord == letters[i][1]) {               //If the written code is equal to anyone in the morse alphabet
        morseWord = "";                               //Reset the input
        returnLetter = "";                            //Reset the word, isn't neccessary but i like to have it
        returnLetter = letters[i][0];                 //Get the letter that matches the morse code
        myBool = false;                               //Stop looping through the array
        return returnLetter;                          //Return the letter
      }
    }
    morseWord = "";
    digitalWrite(ledPinR, HIGH);                       //Turn on the red LED
    delay(600);                                      
    digitalWrite(ledPinR, LOW);                        //Turn off the red LED
    Serial.println("There was no letter which looked like that, you can start over");         //Print to console
    myBool = false;
  }
}

