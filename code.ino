//We always have to include the library
#include "LedControl.h"
byte rightscore = 0;
byte leftscore = 0 ;

byte pongdisplay[] = {     // Used upon start up to display "Pong"

  B00000100,
  B00000100,
  B00000100,
  B00000100,
  B01111100,
  B01000100,
  B01000100,
  B01111100,
  B01111100,
  B01000100,
  B01000100,
  B01000100,
  B01111100,
  B00000000,
  B00000000,
  B00000000,
  B01000100,
  B01000100,
  B01000100,
  B01000100,
  B01111100,
  B00000000,
  B00000000,
  B00000000,
  B01111100,
  B01000000,
  B01000000,
  B01111100,
  B01000100,
  B01000100,
  B01111100,
  B00000000,
};

byte zero[] = {    // Used to display a '0' for when displaying the score
  B00000000,
  B00011100,
  B00100010,
  B00100010,
  B00100010,
  B00100010,
  B00011100,
  B00000000
};
byte one[] = {     // Used to display a '1' for when displaying the score
  B00111110,
  B00001000,
  B00001000,
  B00001000,
  B00001010,
  B00001100,
  B00001000,
  B00000000
};

byte two[] = {      // Used to display a '2' for when displaying the score
  B00000000,
  B00111110,
  B00000010,
  B00001100,
  B00010000,
  B00100000,
  B00100010,
  B00011100
};
byte three[] = {    // Used to display a '3' for when displaying the score
  B00000000,
  B00001100,
  B00010010,
  B00010000,
  B00001000,
  B00010010,
  B00010010,
  B00001100
};

byte four[] = {      // Used to diplay a '4' for when displaying the score
  B00100000,
  B00100000,
  B00100000,
  B00100000,
  B00111100,
  B00100100,
  B00100100,
  B00100100
};
byte five[] = {      // Used to display a '5' for when displaying the score
  B00111100,
  B00100000,
  B00100000,
  B00111100,
  B00000100,
  B00000100,
  B00111100,
  B00000000
};


byte matrix = 0;      //variables to help display paddles, scores, and "pong"
byte displayrow = 0;
byte displayindex = 0;

/*
  Now we need a LedControl to work with.
  pin 12 is connected to the DataIn
  pin 11 is connected to the CLK
  pin 10 is connected to LOAD
  there are 8 matrices to control
*/

int row;                                    //these variables store the location of the ball
int col;
byte deflect;                               //used if the ball must bounce off a wall

LedControl lc = LedControl(12, 11, 10, 8);  //initializes LED controller

byte dirx;                                  //these variables store the direction the ball is moving
byte diry;
byte start;                                 //used to start a new game after a player scores

#define leftpaddle 0  //left pong knob is connected to analog input 0
#define rightpaddle 1 // right pong knob is connected to analog intput 1
int rightpongval;     //holds the position of the right paddle
int leftpongval;      //holds the position of the left paddle

byte pong[] = {          // Stores the position of the ball and paddles, initially set as blank
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,

  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,

  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,

  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,

  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,

  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,

  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,

  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000
};




void setup() {
  pinMode(leftpaddle, INPUT);   //the paddle is an input.
  pinMode(rightpaddle, INPUT);  // the paddle is an input.

  Serial.begin(9600);  // serial communication for debugging. Set to 9600 baud

  // These statements below go through and set up the matrices to the given settings
  lc.shutdown(0, false);
  lc.setIntensity(0, 15);
  lc.clearDisplay(0);

  lc.shutdown(1, false);
  lc.setIntensity(1, 15);
  lc.clearDisplay(1);

  lc.shutdown(2, false);
  lc.setIntensity(2, 15);
  lc.clearDisplay(2);

  lc.shutdown(3, false);
  lc.setIntensity(3, 15);
  lc.clearDisplay(3);

  lc.shutdown(4, false);
  lc.setIntensity(4, 15);
  lc.clearDisplay(4);

  lc.shutdown(5, false);
  lc.setIntensity(5, 15);
  lc.clearDisplay(5);

  lc.shutdown(6, false);
  lc.setIntensity(6, 15);
  lc.clearDisplay(6);

  lc.shutdown(7, false);
  lc.setIntensity(7, 15);
  lc.clearDisplay(7);

  //display the initial "pong" text
  while (matrix < 4) {
    while (displayrow <= 7) {
      lc.setRow(matrix, displayrow, pongdisplay[displayindex]);
      displayrow++;
      displayindex++;
    }
    displayrow = 0;
    matrix++;
  }

  delay(1000);
  displayscreen();
  start = 1;
}

void loop() {
  
  paddles();
  pongsim();
  displayscreen();
}


void paddles() { //reads data from the paddles and displays that in the array

  // reads in the values from the potentiometers used to control the paddles
  // converts the values to an integer from 0-13 corresponding to the row of the bottom bit of the paddle
  leftpongval = analogRead(leftpaddle);
  leftpongval = map(leftpongval, 0, 1010, 0, 13);
  rightpongval = analogRead(rightpaddle);
  rightpongval = map(rightpongval, 1020, 20, 0, 13);

  //clears the previous paddle inorder to display the next one
  for (int x = 0; x < 2; x++) {
    for (int i = 0; i < 8; i++) {
      bitClear(pong[i + 32 * x], 0);
      bitClear(pong[24 + i + 32 * x], 7);
    }
  }

  //-------------------------------right paddle

  if (rightpongval < 6) {                      //display the paddle on the lower matrix
    bitSet(pong[rightpongval + 24], 7);
    bitSet(pong[rightpongval + 25], 7);
    bitSet(pong[rightpongval + 26], 7);
  }

  else if (rightpongval > 7) {                       // display the paddle on the upper matrix
    bitSet(pong[47 - rightpongval], 0);
    bitSet(pong[47 - rightpongval - 2], 0);
    bitSet(pong[47 - rightpongval - 1], 0);
  }

  else if (rightpongval == 6) {                      // cases for displaying the paddle across both matrices
    bitSet(pong[30], 7);
    bitSet(pong[31], 7);
    bitSet(pong[39], 0);
  }

  else if (rightpongval == 7) {
    bitSet(pong[31], 7);
    bitSet(pong[39], 0);
    bitSet(pong[38], 0);
  }


  //----------------------------------left paddle
  //due to the configuration of the cascading displays, there is a small amount of lag when moving the left paddle across matrices

  if (leftpongval < 6) {                                // display the paddle on the lower matrix
    bitSet(pong[leftpongval], 0);
    bitSet(pong[leftpongval + 1], 0);
    bitSet(pong[leftpongval + 2], 0);
  }

  else if (leftpongval > 7) {                               // display the paddle on the upper matrix
    bitSet(pong[71 - leftpongval], 7);
    bitSet(pong[71 - leftpongval - 2], 7);
    bitSet(pong[71 - leftpongval - 1], 7);
  }

  else if (leftpongval == 6) {                             // display the paddle across matrices
    bitSet(pong[6], 0);
    bitSet(pong[7], 0);
    bitSet(pong[63], 7);
  }

  else if (leftpongval == 7) {
    bitSet(pong[7], 0);
    bitSet(pong[63], 7);
    bitSet(pong[62], 7);
  }

}


//this function sets a bit to on, corresponding to a row and column
//used to display the ball, and greatly simplifies other functions
void placeball(int row, int col) {
  if (row > 7) {
    int x = (31 - col) % 8;
    int y = col / 8;
    bitSet(pong[71 - row - 8 * y], x);
  }
  else {
    int x = (31 - col) % 8; //
    int y =  col / 8;
    bitSet(pong[8 * y + row], 7 - x);
  }
}

//same as the placeball function above, but clears the bit instead
//used when the ball moves to clear the previous position of the ball
void removeball(int row, int col) {
  if (row > 7) {
    int x = (31 - col) % 8;
    int y = col / 8;
    bitClear(pong[71 - row - 8 * y], x);
  }
  else {
    int x = (31 - col) % 8; //
    int y =  col / 8;
    bitClear(pong[8 * y + row], 7 - x);
  }
}

//this function controls the movement of the ball
void pongsim() {

  //if the ball is at the edge of the display, call a function to check if it is blocked by the paddle
  if ( (col == 1 && dirx == 0) || (col == 30 && dirx == 1)) {
    ball_meets_paddle();
  }

  if (start == 1) {                  // Starting a new game
    randomSeed(analogRead(leftpaddle)); // randomSeed is necessary to ensure truly random starting position and movement. Here the input from a paddle is used as input for randomization
    dirx = random(2);                 // Come up with a random starting left to right or right to left motion
    diry = random(2);                // Come up with a random starting up or down movement direction
    col = random(12, 20);            // generate a random starting column near the middle of the display
    row = random(4, 12);             // generate a random starting row near the middle of the display
    placeball(row, col);
    start = 0;                       // Set the start varaible back equal to 0
  }


  if (diry == 0) {                    // If the ball is moving from bottom to top
    if (dirx == 0) {                  // from right to left
      removeball(row, col);
      if (deflect == 0) {             // most of the time, simply move the ball in its direction
        row++;
        col--;
        placeball(row, col);
        if (row == 15) {
          deflect = 1;
        }
        return;
      }
      else {                          // if the ball is next to an edge we must change its direction to bounce it off the edge
        row--;
        col--;
        placeball(row, col);
        diry = 1;
        deflect = 0;
        return;
      }
    }

    // The comments above should explain all of the stuff below. Some stuff, different parameters.

    if (dirx == 1) {                 // If the ball is moving from left to right
      removeball(row, col);
      if (deflect == 0) {
        row++;
        col++;
        placeball(row, col);
        if (row == 15) {
          deflect = 1;
        }
        return;
      }
      else {
        row--;
        col++;
        placeball(row, col);
        diry = 1;
        deflect = 0;
        return;
      }
    }
  }

  //--------------------------------------------diry = 1 below

  if (diry == 1) {                    // If the ball is moving from top to bottom
    if (dirx == 0) {                  // right to left
      removeball(row, col);
      if (deflect == 0) {
        row--;
        col--;
        placeball(row, col);
        if (row == 0) {
          deflect = 1;
        }
        return;
      }
      else {
        row++;
        col--;
        placeball(row, col);
        diry = 0;
        deflect = 0;
        return;
      }
    }

    if (dirx == 1) {                 // If the ball is moving from left to right
      removeball(row, col);
      if (deflect == 0) {
        row--;
        col++;
        placeball(row, col);
        if (row == 0) {
          deflect = 1;
        }
        return;
      }
      else {
        row++;
        col++;
        placeball(row, col);
        diry = 0;
        deflect = 0;
        return;
      }
    }
  }
}



//this function simply displays the locations of the paddles and ball stored in the pong[] variable
void displayscreen() {
  matrix = 0;
  displayrow = 0;
  displayindex = 0;

  while (matrix < 8) {
    while (displayrow <= 7) {
      lc.setRow(matrix, displayrow, pong[displayindex]);
      displayrow++;
      displayindex++;
    }
    displayrow = 0;
    matrix++;
  }

}



//this function turns off every LED in case we need to clear the display
void clearscreen() {
  int clearing = 0;
  while (clearing < 64) {
    pong[clearing] = B00000000;
    clearing++;
  }
}


//this function checks if the ball is blocked by the paddle or not
void ball_meets_paddle() {
  tone(9, 500, 100);  //begin by sending a signal to the buzzer on either a block or a miss

  if (diry == 0) { //case if the ball is going up
    if (dirx == 0) { //going left
      if (deflect == 1) { // if the ball deflects off wall
        if (leftpongval == 13) { //blocked, ball bounces off both wall and paddle
          diry = 1;
          dirx = 1;
          deflect = 0;
        }
        else { //missed it
          clearscreen();
          start = 1;
          delay(1000);
          scoreleft();
        }
      }
      else if (row - leftpongval >= 0 && row - leftpongval <= 2) { //normal 45 degree bounce
        dirx = 1;
      }
      else if (leftpongval - (row + 1) == 0) { //if the ball hits the corner of the paddle, it bounces straight
        dirx = 1;
        diry = 1;
      }
      else { //missed
        clearscreen();
        start = 1;
        delay(1000);
        scoreleft();
      }
    }
    else if (dirx == 1) { // going right
      if (deflect == 1) { // if the ball deflects off wall
        if (rightpongval == 13) { //blocked, ball counces off both wall and paddle
          diry = 1;
          dirx = 0;
          deflect = 0;
        }
        else { //missed it
          clearscreen();
          start = 1;
          delay(1000);
          scoreright();
        }
      }
      else if (row - rightpongval >= 0 && row - rightpongval <= 2) { //normal 45 degree bounce
        dirx = 0;
      }
      else if (rightpongval - (row + 1) == 0) { //if the ball hits the corner of the paddle, it bounces straight
        dirx = 0;
        diry = 1;
      }
      else { //missed
        clearscreen();
        start = 1;
        delay(1000);
        scoreright();
      }
    }

  }

  //same logic as the above cases
  else if (diry == 1) { // going down
    if (dirx == 0) { //going left
      if (deflect == 1) { // deflects off wall
        if (leftpongval == 0) { //blocked
          diry = 0;
          dirx = 1;
          deflect = 0;
        }
        else { //missed it
          clearscreen();
          start = 1;
          delay(1000);
          scoreleft();
        }
      }
      else if (row - leftpongval >= 0 && row - leftpongval <= 2) { //normal 45 degree bounce
        dirx = 1;
      }
      else if (row - leftpongval == 3) { //hit the corner and bounce straight
        dirx = 1;
        diry = 0;
      }
      else { //missed
        clearscreen();
        start = 1;
        delay(1000);
        scoreleft();
      }
    }
    else if (dirx == 1) { // going right
      if (deflect == 1) { // deflects off wall
        if (rightpongval == 0) { //blocked
          diry = 0;
          dirx = 0;
          deflect = 0;
        }
        else { //missed it
          clearscreen();
          start = 1;
          delay(1000);
          scoreright();
        }
      }
      else if (row - rightpongval >= 0 && row - rightpongval <= 2) { //normal 45 degree bounce
        dirx = 0;
      }
      else if (row - rightpongval == 3) { //hit the corner and bounce straight
        dirx = 0;
        diry = 0;
      }
      else { //missed
        clearscreen();
        start = 1;
        delay(1000);
        scoreright();
      }
    }
  }
}




//if the ball scores of the left side of the screen, i.e. the right player scores
void scoreleft() {
  clearscreen();
  rightscore++;
  setscore();
  displayscreen();          //increase the score and display it

  if (rightscore == 5) {    //if the player has 5 points, flash the score then reset the score to 0-0
    int blinkx = 0;
    while (blinkx < 5) {
      clearscreen();
      displayscreen();
      delay(500);
      setscore();
      displayscreen();
      delay(500);
      blinkx++;
    }
    rightscore = 0;
    leftscore = 0;
  }
  else {
    delay(2000);
  }

  clearscreen();

}




//same as above function, but for the left player scoring
void scoreright() {
  clearscreen();
  leftscore++;
  setscore();
  displayscreen();

  if (leftscore == 5) {
    int blinkx = 0;
    while (blinkx < 5) {
      clearscreen();
      displayscreen();
      delay(500);
      setscore();
      displayscreen();
      delay(500);
      blinkx++;
    }
    rightscore = 0;
    leftscore = 0;
  }
  else {
    delay(2000);
  }

  clearscreen();

}




//this function simply sets the screen to display the score
void setscore() {

  for (int i = 0; i < 8; i++) {
    if (leftscore == 0) {
      pong[i] = zero[i];
    }
    else if (leftscore == 1) {
      pong[i] = one[i];
    }
    else if (leftscore == 2) {
      pong[i] = two[i];
    }
    else if (leftscore == 3) {
      pong[i] = three[i];
    }
    else if (leftscore == 4) {
      pong[i] = four[i];
    }
    else if (leftscore == 5) {
      pong[i] = five[i];
    }

    if (rightscore == 0) {
      pong[i + 24] = zero[i];
    }
    else if (rightscore == 1) {
      pong[i + 24] = one[i];
    }
    else if (rightscore == 2) {
      pong[i + 24] = two[i];
    }
    else if (rightscore == 3) {
      pong[i + 24] = three[i];
    }
    else if (rightscore == 4) {
      pong[i + 24] = four[i];
    }
    else if (rightscore == 5) {
      pong[i + 24] = five[i];
    }

  }

}
