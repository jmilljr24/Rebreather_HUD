

#include <Wire.h>
#include <LSM303.h>

#include "U8glib.h"
#include <Snooze.h>

LSM303 compass;

SnoozeBlock config;


U8GLIB_SSD1306_128X64 u8g(13, 11, 10, 9, 8);	//***try this after sleep** SW SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9

#define revocropfinal_width 64
#define revocropfinal_height 19
static unsigned char revocropfinal_bits[] PROGMEM = {
  0x00, 0xe0, 0xff, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xff, 0x3f,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xff, 0x3f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0xe0, 0xff, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x0f, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x3e, 0xef, 0x0f, 0x00, 0x1f, 0xf8, 0xe0, 0x0f,
  0xbe, 0xef, 0x0f, 0x00, 0x3f, 0xfc, 0xf8, 0x3f, 0xfe, 0xef, 0xff, 0x3f,
  0x3f, 0x7c, 0xfc, 0x7f, 0xfe, 0xe7, 0xff, 0x3f, 0x3e, 0x7e, 0xfe, 0xff,
  0xfe, 0xe1, 0xff, 0x3f, 0x3e, 0x3e, 0x7e, 0xfc, 0x7e, 0xe0, 0xff, 0x3f,
  0x3e, 0x3e, 0x3f, 0xf8, 0x3e, 0xe0, 0x07, 0x00, 0x7c, 0x1f, 0x1f, 0xf8,
  0x3e, 0xe0, 0x07, 0x00, 0x7c, 0x1f, 0x1f, 0xf8, 0x3e, 0xe0, 0x07, 0x00,
  0xfc, 0x0f, 0x1f, 0xfc, 0x3e, 0xe0, 0x07, 0x00, 0xf8, 0x0f, 0x3f, 0xfc,
  0x3e, 0xe0, 0xff, 0x3f, 0xf8, 0x07, 0x3f, 0x7e, 0x3e, 0xe0, 0xff, 0x3f,
  0xf8, 0x07, 0xfe, 0x7f, 0x3e, 0xe0, 0xff, 0x3f, 0xf0, 0x03, 0xfc, 0x3f,
  0x3e, 0xe0, 0xff, 0x3f, 0xf0, 0x03, 0xf8, 0x0f };


const int ledPin = 13;
int headingInt = 0; //Compass heading as integer

//Button One
unsigned long TIME50 = 0;
//Create some variables for the button
byte BTN_IGNORE = 0;
byte BTN_COUNT = 0;
int buttonState = LOW;
int val;

//oxygen percent
int millivolts1;
float oxyModifier;
float oxyPercent = 0.10;



void buttonOneSampling(){
    analogReference(INTERNAL);
  val = analogRead(1);
  //Serial.println(val);
  if (val >= 1023) {
    buttonState = HIGH;
    delay(10);
  }
  else {
    buttonState = LOW;
  }
}





int Analog1Sampling(){
   analogReadResolution(12);
   analogReference(INTERNAL);
   int i;
   float sval = 0;
   
   for(i = 0; i < 10000; i++){
     sval = sval + analogRead(3);
   }
   
   sval = sval / 10000;
   sval = sval * (1.2 / 4095 * 100000);
   return sval;
 }
 



void CHECK_BUTTON1(void){

  if(BTN_IGNORE == 0){      //Anything above 0 means we are ignoring the buttons
    //Next line will increment the BTN_COUNT every 50 ms while it is less than 4 and the BTN is HIGH
    if((buttonState == HIGH) && (BTN_COUNT <= 3)) BTN_COUNT++;

    //Next line will see if the BTN is HIGH and if the 200ms of debounce has been met
    if((buttonState == HIGH) && (BTN_COUNT >= 4)){
      //BTN PRESS ACTIONS GO HERE
      Serial.println("Button One");
      oxyModifier = (0.21 / millivolts1);
      BTN_COUNT = 0;  //Reset the button counter
      BTN_IGNORE = 10;  //Set a 1 Second Ignore counter
    }

    //Next line will reset counters if the button press was a bounce
    if((buttonState == LOW) && (BTN_COUNT > 0)) BTN_COUNT = 0;
  }


  //Count Down the Button Ignore variable
  if(BTN_IGNORE >= 1) BTN_IGNORE = BTN_IGNORE - 1;
}





void draw(void) {
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_unifont);
  u8g.setPrintPos(0, 11); 
  u8g.print(oxyPercent);
  
  
    u8g.setFont(u8g_font_fub20);     //draw compass heading
  if (headingInt <= 9 ){          //center number on screen
    u8g.setPrintPos( 54, 45);
    u8g.print(headingInt);
    //u8g.setPrintPos( 36, 24);
    u8g.print((char)176);       //Degree symbol
   }
     else if (headingInt >= 10 && headingInt <= 99){ //center
       u8g.setPrintPos( 46, 45);
       u8g.print(headingInt);
       u8g.print((char)176);
     }
       else if (headingInt >= 100){              //center
         u8g.setPrintPos( 41, 45);
         u8g.print(headingInt);
         u8g.print((char)176);
       }

}



/*void sleepNow() {


  if (anal > 400 ){
    digitalWrite(ledPin, HIGH);
    delay(2000);
    digitalWrite(ledPin, LOW);
    digitalWrite(8, LOW);  //turns off display for snooze
    // rebuild the picture after some delay
    Snooze.sleep( config );
    u8g.begin(); //restarts display
    u8g.firstPage();  //Draw Splash Screen for 3 seconds
    do {
      u8g.drawXBMP( 34, 20, 64, 19, revocropfinal_bits); //Calls revo bitmap
    } 
    while( u8g.nextPage() );  
    delay(2000); 
  }
} */



void setup(void) {
  // flip screen, if required
  // u8g.setRot180();
  Serial.begin(9600);
  Wire.begin();
  compass.init();
  compass.enableDefault();
  
  compass.m_min = (LSM303::vector<int16_t>){-521, -608, -641};
  compass.m_max = (LSM303::vector<int16_t>){+653, +550, +447};

  u8g.firstPage();  //Draw Splash Screen for 3 seconds
  do {
    u8g.drawXBMP( 34, 20, 64, 19, revocropfinal_bits); //Calls revo bitmap
  } 
  while( u8g.nextPage() );  
  delay(2000); 

  pinMode(ledPin, OUTPUT);
  config.pinMode(0, INPUT_PULLUP, RISING); //Vibration wake interupt
}




void loop(void) {
   
  //button
    if(millis() - TIME50 >= 50){
    TIME50 = millis();
    //This if statement will run its contents every 50 Milliseconds. No main loop delays!

    CHECK_BUTTON1();
  }
  
  
 //oxygen
   buttonOneSampling();
  millivolts1 = Analog1Sampling();
  Serial.println(oxyPercent);
  
  oxyPercent = (millivolts1 * oxyModifier);
  
  
  //compass
  compass.read();
  float heading = compass.heading((LSM303::vector<int>){1, 0, 0}); 
   headingInt = heading;
  if (headingInt < 1) {
    headingInt = 360 + headingInt;
  } 
  //Serial.println(heading);
  
 //sleepNow();
 
 
  // picture loop
  u8g.firstPage();  
  do {
    draw();
  } 
  while( u8g.nextPage() );

  // rebuild the picture after some delay
  delay(10);

}
