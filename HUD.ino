


#include "U8glib.h"
#include "Wire.h"
#include "LSM303.h"

LSM303 compass;


U8GLIB_SSD1306_64X48 u8g(13, 11, 10, 8, 9);	// SW SPI Com: SCK = 13, SDI = 11, CS = 10, RST 8, D/C = 9

int headingInt = 0; //Compass heading as integer

/* Oxygen Cell Variables */
float millivolts1;
float millivolts2;
float voltage = 0.0;
float ppo1 = 0.0;
float ppo2 = 0.0;
float mvCalc1;
float mvCalc2;

/* Button Variables */
const int buttonPressThreshold = 4090;
const int timeThreshold = 150;
int counter1 = 0;
int buttonState1 = LOW;

#define revocropfinal_width 64
#define revocropfinal_height 19
static unsigned char revo_logo[] PROGMEM = {
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

void draw(void) {
  /* Draw compass and keep centered */
  u8g.setFont(u8g_font_6x13);     //draw compass heading
  if (headingInt <= 9 ){          //center number on screen
    u8g.setPrintPos( 30, 24);
    u8g.print(headingInt);
    u8g.setPrintPos( 36, 24);
    u8g.print((char)176);       //Degree symbol
  }
  else if (headingInt >= 10 && headingInt <= 99){ //center
    u8g.setPrintPos( 27, 24);
    u8g.print(headingInt);
    u8g.print((char)176);
  }
  else if (headingInt >= 100){              //center
    u8g.setPrintPos( 23, 24);
    u8g.print(headingInt);
    u8g.print((char)176);
  }
  
    u8g.setFont(u8g_font_6x12);
    u8g.setPrintPos( 0, 38);
    u8g.print(ppo1);
    
    u8g.setPrintPos( 41, 38);
    u8g.print(ppo2);
  
}

void CompassReading(){
  compass.read();

  float heading = compass.heading((LSM303::vector<int>){ 1, 0, 0  }); //Vector reference for noth heading {X, Y, Z}
  headingInt = heading;
  if (headingInt < 1) {
    headingInt = 360 + headingInt;
  }
  Serial.println(headingInt);
}

float AnalogSampling1(){                //Convert analogread(A0) into millivolts

  analogReadResolution(12);
  analogReference(INTERNAL);

  int i;
  float sval = 0;

  for(i = 0; i < 10000; i++){          // sample A0 reading 10000 times
    sval = sval + analogRead(A0);
  }

  sval = sval / 10000;                    //average results
  sval = sval * (1.2 / 4096 * 1000);   //convert results to millivolts
  return sval;
}

float AnalogSampling2(){                //Convert analogread(A1) into millivolts

  analogReadResolution(12);
  analogReference(INTERNAL);

  int i;
  float sval = 0;

  for(i = 0; i < 10000; i++){          // sample A1 reading 10000 times
    sval = sval + analogRead(A1);
  }

  sval = sval / 10000;                    //average results
  sval = sval * (1.2 / 4096 * 1000);   //convert results to millivolts
  return sval;
}

float PPo1Calc(){
    millivolts1 = AnalogSampling1();
  
    //--------Fix!---------//
    //mvCalc1 = 9.5;
    float ppoMod = (20.9 / mvCalc1 ) / 100;     //gets relationship between air and millivolt reading
    float ppo = ppoMod * millivolts1;        // gives acutal o2 percentage based on current millivolts
    
    return ppo;
}

float PPo2Calc(){
    millivolts2 = AnalogSampling2();
  
    //--------Fix!---------//
    //mvCalc2 = 9.5;
    float ppoMod = (20.9 / mvCalc2 ) / 100;     //gets relationship between air and millivolt reading
    float ppo = ppoMod * millivolts2;        // gives acutal o2 percentage based on current millivolts
    
    return ppo;
}

int ButtonPress1() {
  analogReadResolution(12);
  analogReference(INTERNAL);
  int i;
  float rawValue = 0;

  for(i = 0; i < 500; i++){
    rawValue = rawValue + analogRead(A9);
  }

  rawValue = rawValue / 500;

  long lastPressTime = 0;

  if (rawValue >= buttonPressThreshold) {
    long timeNow = millis();
    if (timeNow > (lastPressTime + timeThreshold)) {
      Serial.println(++ counter1);
      buttonState1 = HIGH;
    }
    lastPressTime = timeNow;
  }
}

void setup(void) {

  Serial.begin(9600);

  //Compass setup
  Wire.begin();
  compass.init();
  compass.enableDefault();
  //Run calibration sketch and input max values
  compass.m_min = (LSM303::vector<int16_t>){ -592, -748, -473 };
  compass.m_max = (LSM303::vector<int16_t>){ +619, +430, +571 };

  // flip screen, if required
  // u8g.setRot180();

  u8g.firstPage();  //Draw Splash Screen for 3 seconds
  do {
    u8g.drawXBMP( 0, 15, 64, 19, revo_logo); //Calls revo bitmap
  } 
  while( u8g.nextPage() );  
  delay(3000);

}

void loop(void) {
  /* Compass read and set between 1-360 */
  CompassReading();
  
  /* Partial Pressure of Oxygen Calc*/
  ppo1 = PPo1Calc();
  ppo2 = PPo2Calc();
  
  /* picture loop */
  u8g.firstPage();  
  do {
    draw();
  } 
  while( u8g.nextPage() );

  ButtonPress1();
    if (buttonState1 == HIGH) {
    digitalWrite(13, HIGH);
    delay(100);
  }

  else {
    digitalWrite(13, LOW);
  }
  
  if (buttonState1 == HIGH) {
    mvCalc1 = millivolts1;
    mvCalc2 = millivolts2;
  }
  buttonState1 = LOW;
  

  // rebuild the picture after some delay
  delay(50);
}
