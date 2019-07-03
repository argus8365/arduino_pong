//We always have to include the library
 #include "LedControl.h"
 byte rightscore = 0;
 byte leftscore = 0 ;
 int toneloop=1000;

      byte pongdisplay[]={       // Used upon start up to display "Pong"
     
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
 
 byte zero[]={      // Used to display a '0' for when displaying the score
   B00000000,
   B00011100,
   B00100010,
   B00100010,
   B00100010,
   B00100010,
   B00011100,
   B00000000  
 };
 byte one[]={       // Used to display a '1' for when displaying the score
  B00111110,
  B00001000,
  B00001000,
  B00001000,
  B00001010,
  B00001100,
  B00001000,
  B00000000
 };
 
byte two[]={        // Used to display a '2' for when displaying the score
  B00000000,
  B00111110,
  B00000010,
  B00001100,
  B00010000,
  B00100000,
  B00100010,
  B00011100
};
byte three[]={      // Used to display a '3' for when displaying the score
  B00000000,
  B00001100,
  B00010010,
  B00010000,
  B00001000,
  B00010010,
  B00010010,
  B00001100
};

byte four[]={        // Used to diplay a '4' for when displaying the score
  B00100000,
  B00100000,
  B00100000,
  B00100000,
  B00111100,
  B00100100,
  B00100100,
  B00100100
};
byte five[]={        // Used to display a '5' for when displaying the score
  B00111100,
  B00100000,
  B00100000,
  B00111100,
  B00000100,
  B00000100,
  B00111100,
  B00000000
};


byte displayevens=0;
byte displayrow=0;
byte displayindex=0;
byte displayodds=1;
unsigned long time;
unsigned long currenttime;
/*
 Now we need a LedControl to work with.
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 We have 6 MAX72XX.
 */

int row;
int col;
byte mode;
byte deflect;
byte carry;
int line;
LedControl lc=LedControl(12,11,10,9);
long randnumber;
byte dirx;
byte dir;
int startpos;
//byte row;
//byte column;
long randNumber;
byte start;
byte diry;
int pass;
byte limit;
#define leftpaddle 0  //left pong knob is connected to analog input 0
#define rightpaddle 1 // right pong knob is connected to analog intput 1
int directionpong;
int rightpongval;
int leftpongval;
byte blinkx;

byte pong[]={            // Stores the position of the ball and paddles
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
 B00000000, B00000000};


 

void setup(){
  pinMode(leftpaddle, INPUT);   //the paddle is an input. 
  pinMode(rightpaddle, INPUT);  // the paddle is an input.
   
  Serial.begin(9600);  // serial communication for debugging. Set to 9600 baud
  
  // These statemetns below go through and sets up the matrices to the given settings
  lc.shutdown(0,false);
  lc.setIntensity(0,1);
  lc.clearDisplay(0);
 
  lc.shutdown(1,false);
  lc.setIntensity(1,1);
  lc.clearDisplay(1);
  
  lc.shutdown(2,false);
  lc.setIntensity(2,1);
  lc.clearDisplay(2);
  
  lc.shutdown(3,false);
  lc.setIntensity(3,1);
  lc.clearDisplay(3);
  
  lc.shutdown(4,false);
  lc.setIntensity(4,1);
  lc.clearDisplay(4);
  
  lc.shutdown(5,false);
  lc.setIntensity(5,1);
  lc.clearDisplay(5);

  lc.shutdown(6,false);
  lc.setIntensity(6,1);
  lc.clearDisplay(6);

  lc.shutdown(7,false);
  lc.setIntensity(7,1);
  lc.clearDisplay(7);
  
  while(displayevens<4){
    while(displayrow<=7){
      lc.setRow(displayevens,displayrow,pongdisplay[displayindex]);
      displayrow++;
      displayindex++;
    }
    displayrow=0;
    displayevens++;
  }

  delay(1000);
  displayscreen();
  currenttime = millis();
  displayindex = 1;
  start = 1;
}
 
void loop(){
   paddles();
   pongsim();
   displayscreen();
  //delay(500);
}
 
 
void paddles(){  //reads data from the paddles and displays that in the array
  // Serial.print("LeftPaddle:");
  // Serial.println(analogRead(leftpaddle));
  leftpongval=analogRead(leftpaddle);
  leftpongval=map(leftpongval,0,1010,0,13);
  rightpongval=analogRead(rightpaddle);
  rightpongval=map(rightpongval,1023,20,0,13);

  //  Serial.print("RightPaddle:");
  //  Serial.println(rightpongval);
 
  //clears the previous paddle inorder to display the next one
  for(int x = 0; x<2; x++){
    for(int i = 0; i<8; i++){
      bitClear(pong[i+32*x], 0);
    }
  }

  for(int x = 0; x<2; x++){
    for(int i = 0; i<8; i++){
      bitClear(pong[24 + i + 32*x], 7);
    }
  }
  
  //-------------------------------right paddle

   if(rightpongval<6){
    bitSet(pong[rightpongval + 24], 7);   
    bitSet(pong[rightpongval+25], 7);
    bitSet(pong[rightpongval+26], 7);
  }
 
  if(rightpongval>7){
    bitSet(pong[47 - rightpongval], 0);
    bitSet(pong[47 - rightpongval-2], 0);
    bitSet(pong[47 - rightpongval-1], 0);
  }

  if(rightpongval == 6){
    bitSet(pong[30], 7);
    bitSet(pong[31], 7);
    bitSet(pong[39], 0);
  }

  if(rightpongval == 7){
    bitSet(pong[31], 7);
    bitSet(pong[39], 0);
    bitSet(pong[38], 0);
  }
   
 
//----------------------------------left paddle
  
  if(leftpongval<6){
    bitSet(pong[leftpongval], 0);   
    bitSet(pong[leftpongval+1], 0);
    bitSet(pong[leftpongval+2], 0);
  }
 
  if(leftpongval>7){
    //leftpongval=map(leftpongval,8,13,7,limit);
    bitSet(pong[71 - leftpongval], 7);
    bitSet(pong[71 - leftpongval-2], 7);
    bitSet(pong[71 - leftpongval-1], 7);
  }

  if(leftpongval == 6){
    bitSet(pong[6], 0);
    bitSet(pong[7], 0);
    bitSet(pong[63], 7);
  }

  if(leftpongval == 7){
    bitSet(pong[7], 0);
    bitSet(pong[63], 7);
    bitSet(pong[62], 7);
  }

}


void placeball(int row, int col){
  if(row>7){
    int x = (31-col)%8;  
    int y = col / 8;  
    bitSet(pong[71 - row - 8*y], x);
  }
  else{
    int x = (31-col)%8; //
    int y =  col / 8;
    bitSet(pong[8*y + row], 7-x);
  }
}

void removeball(int row, int col){
  if(row>7){
    int x = (31-col)%8;  
    int y = col / 8;  
    bitClear(pong[71 - row - 8*y], x);
  }
  else{
    int x = (31-col)%8; //
    int y =  col / 8;
    bitClear(pong[8*y + row], 7-x);
  }
}


void pongsim(){

  if( (col == 1 && dirx == 0) || (col == 30 && dirx==1)){
    ball_meets_paddle();
  }

  if(start==1){                      // Starting a new game
    //randnumber=random(1,7);          // Come up with a random starting bit
    randomSeed(analogRead(leftpaddle));
    dirx=random(2);                   // Come up with a random starting left to right or right to left motion
    diry=random(2);                  // Come up with a random starting y movement direction
    /*if(diry==0){                     // If the y direction is 0. Movement from bottom to top
      line=random(12,19);            // Come up with a random starting line between 30-34
      bitSet(pong[line], (line%8)); // Set the bit on the random line and random bit equal to 1 in the pong array
    }
    if(diry==1){                     // If the y direction is 1. Movement from top to bottom
      line=random(12,16);            // Come up with a random starting line between 12-16
      bitSet(pong[line],randnumber); // Set the bit on the random line and random bit equal to 1 in the pong array
    }*/
    col = random(12,20);
    row = random(4, 12);
    placeball(row, col);
    start=0;                         // Set the start varaible back equal to 0
  }

 
  if(diry==0){                        // If the ball is moving from bottom to top
    if(dirx==0){ /*                      // If the ball is moving from right to left
      if(deflect==0 && carry==0){     // If the ball is not deflecting or carrying over to another matrix
        line-=2;                      // Progress the ball towards the other side by subtracting 2 from the line
        pong[line]= pong[line+2] << 1;// Shift the ball left and set it equal to the new line
        pong[line+2]=B00000000;       // Clear the old line of the ball
      }
      if(carry==1){                   // If the ball has to carry over to a new matrix
        line-=3;                      // Progress the ball towards the other side by subtracting 3 from the line
        pong[line]=B00000001;         // Set the new line
        pong[line+3]=B00000000;       // Clear the old line of the ball
        carry=0;                      // Set the carry variable back to 0
        return;                       // We are done here
      }
      if(deflect==1){                 // If the ball has to deflect off the wall
        line-=2;                      // Progress the ball towards the other side by subtracting 2 from the line
        pong[line]=B01000000;         // Set the new line
        pong[line+2]=B00000000;       // Clear the old line of the ball
        deflect=0;                    // Set the deflect variable back to 0
        dir=1;                        // Switch the direction to left to right
        return;                       // We are done here
      }
      if(bitRead(pong[line],7)==1){   // If the ball is on the 7th bit of the line
        if(line==3|| line==5 || line==7 || line==9 || line==11 || line==13 || line==15 ||    // If the line is an odd line
        line==17 || line==19 || line==21 || line==23 || line==25 || line==27 ||
        line==29 || line==31 || line==33 || line==35 || line==37 || line==39 || line==41 ||line==43 || line==45){
          carry=1;                    // The ball needs to carry over to another matrix
          return;                     // We are done here
        }
        else{                         // Else the ball is on an even line
          if(line!=2){                // The line can not be 2 because the ball meets the paddle at 2
            deflect=1;                // The ball needs to deflect off the wall
            return;                   // We are done here
          }
        }
      }*/
      removeball(row, col);
      if(deflect==0){
        row++;
        col--;
        placeball(row,col);
        if(row == 15){
          deflect = 1;
        }
        return;
      }
      else{
        row--;
        col--;
        placeball(row,col);
        diry = 1;
        deflect = 0;
        return;
      }
    }
    
    // The comments above should explain all of the stuff below. Some stuff, different parameters.

    if(dirx==1){                     // If the ball is moving from left to right
      removeball(row, col);
      if(deflect==0){
        row++;
        col++;
        placeball(row,col);
        if(row == 15){
          deflect = 1;
        }
        return;
      }
      else{
        row--;
        col++;
        placeball(row,col);
        diry = 1;
        deflect = 0;
        return;
      }
    }
  }

//--------------------------------------------diry = 1 below
  
  if(diry==1){                        // If the ball is moving from top to bottom
    if(dirx==0){                      // right to left
      removeball(row, col);
      if(deflect==0){
        row--;
        col--;
        placeball(row,col);
        if(row == 0){
          deflect = 1;
        }
        return;
      }
      else{
        row++;
        col--;
        placeball(row,col);
        diry = 0;
        deflect = 0;
        return;
      }
    }

    if(dirx==1){                     // If the ball is moving from left to right
      removeball(row, col);
      if(deflect==0){
        row--;
        col++;
        placeball(row,col);
        if(row == 0){
          deflect = 1;
        }
        return;
      }
      else{
        row++;
        col++;
        placeball(row,col);
        diry = 0;
        deflect = 0;
        return;
      }
    }
  }
}




void displayscreen(){
  displayevens=0;
  displayrow=0;
  displayindex=0;
  displayodds=1;
  
  while(displayevens<8){
    while(displayrow<=7){
      lc.setRow(displayevens,displayrow,pong[displayindex]);
      displayrow++;
      displayindex++;
    }
    displayrow=0;
    displayevens++;
  }
  
} 




void clearscreen(){
 int clearing=0;
 
  while(clearing<64){
    pong[clearing]=B00000000;
    clearing++;
  }
}




void ball_meets_paddle(){
  byte search1=0;
  tone(9,500,100);

if(diry==0){ //going up
  if(dirx==0){ //going left
    if(row==15){ // deflects off wall
      if(leftpongval==13){ //blocked
        diry = 1;
        dirx = 1;
        deflect = 0;
      }
      else{ //missed it
        clearscreen();
        start = 1;
        delay(1000);
        scoreleft();
      }
    }
    else if(row-leftpongval>=0 && row-leftpongval<=2){ //normal 45 degree bounce
      dirx = 1;
    }
    else if(leftpongval-(row+1)==0){ //hit the corner and bounce straight
      dirx = 1;
      diry = 1;
    }
    else{ //missed
      clearscreen();
      start = 1;
      delay(1000);
      scoreleft();
    }
  }
  else if(dirx==1){ // going right
    if(row==15){ // deflects off wall
      if(rightpongval==13){ //blocked
        diry = 1;
        dirx = 0;
        deflect = 0;
      }
      else{ //missed it
        clearscreen();
        start = 1;
        delay(1000);
        scoreright();
      }
    }
    else if(row-rightpongval>=0 && row-rightpongval<=2){ //normal 45 degree bounce
      dirx = 0;
    }
    else if(rightpongval-(row+1)==0){ //hit the corner and bounce straight
      dirx = 0;
      diry = 1;
    }
    else{ //missed
      clearscreen();
      start = 1;
      delay(1000);
      scoreright();
    }
  }
  
}
else if(diry==1){ // going down
    if(dirx==0){ //going left
      if(row==0){ // deflects off wall
      if(leftpongval==0){ //blocked
        diry = 0;
        dirx = 1;
        deflect = 0;
      }
      else{ //missed it
        clearscreen();
        start = 1;
        delay(1000);
        scoreleft();
      }
    }
    else if(row-leftpongval>=0 && row-leftpongval<=2){ //normal 45 degree bounce
      dirx = 1;
    }
    else if(row-leftpongval == 3){ //hit the corner and bounce straight
      dirx = 1;
      diry = 0;
    }
    else{ //missed
      clearscreen();
      start = 1;
      delay(1000);
      scoreleft();
    }
  }
  else if(dirx==1){ // going right
    if(row==0){ // deflects off wall
      if(rightpongval==0){ //blocked
        diry = 0;
        dirx = 0;
        deflect = 0;
      }
      else{ //missed it
        clearscreen();
        start = 1;
        delay(1000);
        scoreright();
      }
    }
    else if(row-rightpongval>=0 && row-rightpongval<=2){ //normal 45 degree bounce
      dirx = 0;
    }
    else if(row-rightpongval == 3){ //hit the corner and bounce straight
      dirx = 0;
      diry = 0;
    }
    else{ //missed
      clearscreen();
      start = 1;
      delay(1000);
      scoreright();
    }
  }
}
  
    
    pass=1;
  search1=0;
}





void scoreleft(){
  clearscreen();
  rightscore++;
  setscore();
  displayscreen();

  if(rightscore==5){
    int blinkx = 0;
    while(blinkx<5){
      clearscreen();
      displayscreen();
      delay(500);
      setscore();
      displayscreen();
      delay(500);
      blinkx++;
    }
    rightscore=0;
    leftscore=0;
  }
  else{
    delay(2000);
  }
  
  clearscreen();

}





void scoreright(){
  clearscreen();
  leftscore++;
  setscore();
  displayscreen();
   
  if(leftscore==5){
    int blinkx = 0;
    while(blinkx < 5){
      clearscreen();
      displayscreen();
      delay(500);
      setscore();
      displayscreen();
      delay(500);
      blinkx++;
    }
    rightscore=0;
    leftscore=0;
  }
  else{
    delay(2000);
  }

  clearscreen();

}





void setscore(){

  for(int i = 0; i < 8; i++){
    if(leftscore==0){
      pong[i] = zero[i];
    }
    else if(leftscore==1){
      pong[i] = one[i];
    }
    else if(leftscore==2){
      pong[i] = two[i];
    }
    else if(leftscore==3){
      pong[i] = three[i];
    }
    else if(leftscore==4){
      pong[i] = four[i];
    }
    else if(leftscore==5){
      pong[i] = five[i];
    }

    if(rightscore==0){
      pong[i + 24] = zero[i];
    }
    else if(rightscore==1){
      pong[i + 24] = one[i];
    }
    else if(rightscore==2){
      pong[i + 24] = two[i];
    }
    else if(rightscore==3){
      pong[i + 24] = three[i];
    }
    else if(rightscore==4){
      pong[i + 24] = four[i];
    }
    else if(rightscore==5){
      pong[i + 24] = five[i];
  }

  }

    
  
 
}
  

  
 
