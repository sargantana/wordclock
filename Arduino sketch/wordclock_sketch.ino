/* 
--------------WORDCLOCK-----------------

-----------------CODE-------------------
  Arduino based Wordclock with NeoPixels  
  
  Get NeoPixel Libary here: https://github.com/adafruit/Adafruit_NeoPixel
  Get Time Libary here: https://github.com/PaulStoffregen/Time
  
  An Array is build with different states of time been spelled out by words and combinations.
  Time Libary is used to keep time and after its parsed Neopixels light up according to the flags.
  
*/

#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#include <TimeLib.h>
#include "WordMatrix.h"
#include "Pins.h"

//---------------------------------------------------START-OF-USER-DEFINED-SETUP-AREA


// Set starting time here in hh-mm-ss dd.mm.yyyy format
int start_hour=14;
int start_min=0;
int start_sec=0;
int start_day=05;
int start_month=11;
int start_year=2017;


//Different Settings

int STEP =5; 			// Declare step size of time adjustement

// RGB values for AM
int AMr=244;
int AMg=146;
int AMb=66;

// RGB values for PM
int PMr=66;
int PMg=134;
int PMb=244;

//-----------------------------------------------------END-OF-USER-DEFINED-SETUP-AREA

// Use NeoPixel libary to set up your strip
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// Current and previous states for button pins -- in setup initialize all to HIGH
int minusPrevState=HIGH;
int minusCurrState=HIGH;
int plusPrevState=HIGH;
int plusCurrState=HIGH;

// Time variables used
int h;
int m;
int s;

void setup()
{

  pinMode(MINUS, INPUT_PULLUP); //Define as input, enable pull-up resistor
  pinMode(PLUS, INPUT_PULLUP); //Define as input, enable pull-up resistor
  
  setTime(start_hour,start_min,start_sec,start_day,start_month,start_year); //Initialize starting time as defined
  pixels.begin(); //Begin Neopixel string
  Serial.begin(9600); //Begin Serial for debugging
  
}

void loop()
{
  
  // Declare integer array with size coresponding to number of Neopixels in chain
  int individualPixels[NUMPIXELS];
  
  // Fill integer array with zeros according to number of pixels
  for (int i=0; i<NUMPIXELS; i++) {
individualPixels[i]=0;
}
  /* Check for button presses adjust time */
  minusCurrState=digitalRead(MINUS); //Get current state of MINUS button
  
  /* If current state is different from previous state and value is now LOW consider this as button press and subtract STEP minutes from current time */
  if ((minusCurrState!=minusPrevState) && (minusCurrState==LOW)){
    adjustTime(-STEP*60); //Shift time STEP minutes backwards
    minusPrevState=minusCurrState;
  }
  else{
   minusPrevState=minusCurrState; 
  }
  
  plusCurrState=digitalRead(PLUS); //Get current state of PLUS button
  
  /* If current state is different from previous state and value is now LOW consider this as button press and add STEP minutes to current time */
  if ((plusCurrState!=plusPrevState) && (plusCurrState==LOW)){
    adjustTime(STEP*60); //Shift time STEP minutes forwards
    plusPrevState=plusCurrState;
  }
    else{
   plusPrevState=plusCurrState; 
  }
    
  /* Get current time */
  h=hourFormat12();    // Returns the hour of current time in 12h format
  m=minute();        // Returns the minute of current time
  s=second();        // Returns the second of current time		
  
  Serial.print(h);   // for serial debugging print out the current time
  Serial.print(":");
  Serial.print(m);
  Serial.print(":");
  Serial.println(s);

  
 /* Minutes 1 and 6 - Light EdgePixel + */
  while ((m=1) || (m=6) || (m=11) || (m=16) || (m=21) || (m=26) || (m=31) || (m=36) || (m=41) || (m=46) || (m=51) || (m=56)){ 
  individualPixels[min1]=1;
    individualPixels[min2]=0;
    individualPixels[min3]=0;
    individualPixels[min4]=0;

  }
  
  /* Minutes 2 and 7 - Light EdgePixel ++ */
  while ((m=2) || (m=7) || (m=12) || (m=17) || (m=22) || (m=27) || (m=32) || (m=37) || (m=42) || (m=47) || (m=52) || (m=57)){
  individualPixels[min1]=1;
  individualPixels[min2]=1;
    individualPixels[min3]=0;
    individualPixels[min4]=0;
  }
  
  /* Minutes 3 and 8 - Light EdgePixel +++ */
  while ((m=3) || (m=8) || (m=13) || (m=18) || (m=23) || (m=28) || (m=33) || (m=38) || (m=43) || (m=48) || (m=53) || (m=58)){
  individualPixels[min1]=1;
  individualPixels[min2]=1;
  individualPixels[min3]=1;
    individualPixels[min4]=0;
  }
  
  /* Minutes 4 and 9 - Light EdgePixel ++++ */
  while ((m=4) || (m=9) || (m=14) || (m=19) || (m=24) || (m=29) || (m=34) || (m=39) || (m=44) || (m=49) || (m=54) || (m=59)){
  individualPixels[min1]=1;
  individualPixels[min2]=1;
  individualPixels[min3]=1;
  individualPixels[min4]=1;
  }

 /* Minutes 5 and 0 - Light no EdgePixel */
  while ((m=5) || (m=10) || (m=15) || (m=20) || (m=25) || (m=30) || (m=35) || (m=40) || (m=45) || (m=50) || (m=55) || (m=0)){
  individualPixels[min1]=0;
  individualPixels[min2]=0;
  individualPixels[min3]=0;
  individualPixels[min4]=0;
  }
 
  
  }

  
  /* Parse time values to light corresponding pixels */
  individualPixels[IT]=1; //Light "IT"
  individualPixels[IS]=1; //Light "IS" 
  
  /* Minutes between 0-5 - Light "CLOCK" */
  if ((m>=0) && (m<5)){
    individualPixels[OCLOCK]=1;
  }
  
  /* Minutes between 5-10 or 55-60 - Light "FIVE_min" */
  if ((m>=5) && (m<10)){
    individualPixels[FIVE_min]=1;
    individualPixels[PAST]=1;
  }
  
  /* Minutes between 10-15 or 50-55 - Light "TEN_min" */
  if ((m>=10) && (m<15)){
    individualPixels[TEN_min]=1;
    individualPixels[PAST]=1;
  }
  
  /* Minutes between 15-20 or 45-50 - Light "QUARTER" */
  if ((m>=15) && (m<20)){
    individualPixels[QUARTER]=1;
    individualPixels[QUARTER2]=1;
    individualPixels[PAST]=1;
  }
  
  /* Minutes between 20-25 or 40-45 - Light "TEN_min," "HALF" */
  if ((m>=20) && (m<25)){
    individualPixels[TEN_min]=1;
    individualPixels[HALF]=1;
    individualPixels[TO2]=1;
  }  

  /* Minutes between 25-30 or 35-40 - Light "FIVE_min", "HALF" */
  if ((m>=25) && (m<30)){                                              
    individualPixels[FIVE_min]=1;
    individualPixels[HALF]=1;
    individualPixels[TO2]=1;
  }

  /* Minutes between 30-35 - Light "HALF" */
  if ((m>=30) && (m<35)){
    individualPixels[HALF]=1;
  }
  
  /* Minutes between 35-40 - Light "FIVE_min", "PAST", "HALF" */
  if ((m>=35) && (m<40)){
    individualPixels[FIVE_min]=1;
    individualPixels[HALF]=1;
    individualPixels[PAST]=1;
  }
  
  /* Minutes between 40-45 - Light "TEN_min", "PAST", "HALF" */
  if ((m>=40) && (m<45)){
    individualPixels[TEN_min]=1;
    individualPixels[HALF]=1;
    individualPixels[PAST]=1;
  }
  
  /* Minutes between 45-50 - Light "QUARTER", "TO" */
  if ((m>=45) && (m<50)){
    individualPixels[QUARTER]=1;
    individualPixels[QUARTER2]=1;
    individualPixels[TO]=1;
  }

  /* Minutes between 50-55 - Light "TEN_min", "TO" */
  if ((m>=50) && (m<55)){
    individualPixels[TEN_min]=1;
    individualPixels[TO]=1;
  }

  /* Minutes between 55-00 - Light "FIVE_min", "TO" */
  if ((m>=55) && (m<60)){
    individualPixels[FIVE_min]=1;
    individualPixels[TO]=1;
  }

  /* Minutes between 20-60 - MODIFY CURRENT HOUR VALUE */
  if (m>=20){
    h++; //Add 1 from current hour
    /*Set time to twelve for hour around midnight, noon */
    if (h==0){
      h=12; 
    }
    /*Corner case for 12:35-12:59 */
    if (h==13){
      h=1;
    }
  }

  /* Hour=1 - Light "ONE" */
  if (h==1){
    individualPixels[ONE]=1;
  }
  
  /* Hour=2 - Light "TWO" */
  if (h==2){
    individualPixels[TWO]=1;
  }
  
  /* Hour=3 - Light "THREE" */
  if (h==3){
    individualPixels[THREE]=1;    
  }
  
  /* Hour=4 - Light "FOUR" */
  if (h==4){
    individualPixels[FOUR]=1;
  }
  
  /* Hour=5 - Light "FIVE" */
  if (h==5){
    individualPixels[FIVE]=1;
  }
  
  /* Hour=6 - Light "SIX" */
  if (h==6){
    individualPixels[SIX]=1;
  }
  
  /* Hour=7 - Light "SEVEN" */
  if (h==7){
    individualPixels[SEVEN]=1;
    individualPixels[SEVEN2]=1;
  }
  
  /* Hour=8 - Light "EIGHT" */
  if (h==8){
    individualPixels[EIGHT]=1;
  }
  
  /* Hour=9 - Light "NINE" */
  if (h==9){
    individualPixels[NINE]=1;
  }
  
  /* Hour=10 - Light "TEN" */
  if (h==10){
    individualPixels[TEN]=1;
  }
  
  /* Hour=11 - Light "ELEVEN" */
  if (h==11){
    individualPixels[ELEVEN]=1;
  }
  
  /* Hour=12 - Light "TWELVE" */
  if (h==12){
    individualPixels[TWELVE]=1;
  }
  
  /* Light pixels corresponding to current time */
  for (int i=0; i<sizeof(individualPixels); i++){
    if (individualPixels[i]==1){
      if (isAM() == true){
        pixels.setPixelColor(i, pixels.Color(AMr,AMg,AMb)); //Set Neopixel color to AM Settings
    }
      else{
        if (isPM() == true){
        pixels.setPixelColor(i, pixels.Color(PMr, PMg, PMb)); //Set Neopixel color to PM Settings
    }
      }
    }
    else{
      pixels.setPixelColor(i,pixels.Color(0,0,0));
    }
  }
  
  pixels.show(); //Display Neopixel color
  
}
