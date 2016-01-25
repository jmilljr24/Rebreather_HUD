//#include <COMPASS64X13.h>
#include "U8glib.h"
#include <Wire.h>
#include <LSM303.h>

LSM303 compass;
int headingInt = 0;          //Compass heading as integer


U8GLIB_SSD1306_64X48 u8g(14, 11, 10, 8, 9);	// SW SPI Com: SCK = 13, MOSI = 11, CS = 10, D/C = 8, RST 9

float millivolts1;                  // converted millivolts output for #1 input (A2)

float voltage = 0.0;
float partialPressureO2 = 0.0;
float ppo2mod = 0.0;
float ppo2 = 0.0;
float mvCal;

//piezo button input and debounce
int knockThreshold = 150;
int timeThreshold = 100;
int pinKnock = A1;
int counter = 0;
long lastKnockTime = 0;
int buttonState = LOW;
long timeNow;


/*const unsigned char* compassString[]={
 compass1_bits, compass2_bits, compass3_bits,compass4_bits, compass5_bits, compass6_bits, compass7_bits, compass8_bits, compass9_bits, compass10_bits,
 compass11_bits, compass12_bits, compass13_bits,compass14_bits, compass15_bits, compass16_bits, compass17_bits, compass18_bits, compass19_bits, compass20_bits,
 compass21_bits, compass22_bits, compass23_bits,compass24_bits, compass25_bits, compass26_bits, compass27_bits, compass28_bits, compass29_bits, compass30_bits,
 compass31_bits, compass32_bits, compass33_bits,compass34_bits, compass35_bits, compass36_bits, compass37_bits, compass38_bits, compass39_bits, compass40_bits,
 compass41_bits, compass42_bits, compass43_bits,compass44_bits, compass45_bits, compass46_bits, compass47_bits, compass48_bits, compass49_bits, compass50_bits,
 compass51_bits, compass52_bits, compass53_bits,compass54_bits, compass55_bits, compass56_bits, compass57_bits, compass58_bits, compass59_bits, compass60_bits,
 compass61_bits, compass62_bits, compass63_bits,compass64_bits, compass65_bits, compass66_bits, compass67_bits, compass68_bits, compass69_bits, compass70_bits,
 compass71_bits, compass72_bits, compass73_bits,compass74_bits, compass75_bits, compass76_bits, compass77_bits, compass78_bits, compass79_bits, compass80_bits,
 compass81_bits, compass82_bits, compass83_bits,compass84_bits, compass85_bits, compass86_bits, compass87_bits, compass88_bits, compass89_bits, compass90_bits,
 compass91_bits, compass92_bits, compass93_bits,compass94_bits, compass95_bits, compass96_bits, compass97_bits, compass98_bits, compass99_bits, compass100_bits,
 
 compass101_bits, compass102_bits, compass103_bits,compass104_bits, compass105_bits, compass106_bits, compass107_bits, compass108_bits, compass109_bits, compass110_bits,
 compass111_bits, compass112_bits, compass113_bits,compass114_bits, compass115_bits, compass116_bits, compass117_bits, compass118_bits, compass119_bits, compass120_bits,
 compass121_bits, compass122_bits, compass123_bits,compass124_bits, compass125_bits, compass126_bits, compass127_bits, compass128_bits, compass129_bits, compass130_bits,
 compass131_bits, compass132_bits, compass133_bits,compass134_bits, compass135_bits, compass136_bits, compass137_bits, compass138_bits, compass139_bits, compass140_bits,
 compass141_bits, compass142_bits, compass143_bits,compass144_bits, compass145_bits, compass146_bits, compass147_bits, compass148_bits, compass149_bits, compass150_bits,
 compass151_bits, compass152_bits, compass153_bits,compass154_bits, compass155_bits, compass156_bits, compass157_bits, compass158_bits, compass159_bits, compass160_bits,
 compass161_bits, compass162_bits, compass163_bits,compass164_bits, compass165_bits, compass166_bits, compass167_bits, compass168_bits, compass169_bits, compass170_bits,
 compass171_bits, compass172_bits, compass173_bits,compass174_bits, compass175_bits, compass176_bits, compass177_bits, compass178_bits, compass179_bits, compass180_bits,
 compass181_bits, compass182_bits, compass183_bits,compass184_bits, compass185_bits, compass186_bits, compass187_bits, compass188_bits, compass189_bits, compass190_bits,
 compass191_bits, compass192_bits, compass193_bits,compass194_bits, compass195_bits, compass196_bits, compass197_bits, compass198_bits, compass199_bits, compass200_bits,
 
 compass201_bits, compass202_bits, compass203_bits,compass204_bits, compass205_bits, compass206_bits, compass207_bits, compass208_bits, compass209_bits, compass210_bits,
 compass211_bits, compass212_bits, compass213_bits,compass214_bits, compass215_bits, compass216_bits, compass217_bits, compass218_bits, compass219_bits, compass220_bits,
 compass221_bits, compass222_bits, compass223_bits,compass224_bits, compass225_bits, compass226_bits, compass227_bits, compass228_bits, compass229_bits, compass230_bits,
 compass231_bits, compass232_bits, compass233_bits,compass234_bits, compass235_bits, compass236_bits, compass237_bits, compass238_bits, compass239_bits, compass240_bits,
 compass241_bits, compass242_bits, compass243_bits,compass244_bits, compass245_bits, compass246_bits, compass247_bits, compass248_bits, compass249_bits, compass250_bits,
 compass251_bits, compass252_bits, compass253_bits,compass254_bits, compass255_bits, compass256_bits, compass257_bits, compass258_bits, compass259_bits, compass260_bits,
 compass261_bits, compass262_bits, compass263_bits,compass264_bits, compass265_bits, compass266_bits, compass267_bits, compass268_bits, compass269_bits, compass270_bits,
 compass271_bits, compass272_bits, compass273_bits,compass274_bits, compass275_bits, compass276_bits, compass277_bits, compass278_bits, compass279_bits, compass280_bits,
 compass281_bits, compass282_bits, compass283_bits,compass284_bits, compass285_bits, compass286_bits, compass287_bits, compass288_bits, compass289_bits, compass290_bits,
 compass291_bits, compass292_bits, compass293_bits,compass294_bits, compass295_bits, compass296_bits, compass297_bits, compass298_bits, compass299_bits, compass300_bits,
 
 compass301_bits, compass302_bits, compass303_bits,compass304_bits, compass305_bits, compass306_bits, compass307_bits, compass308_bits, compass309_bits, compass310_bits,
 compass311_bits, compass312_bits, compass313_bits,compass314_bits, compass315_bits, compass316_bits, compass317_bits, compass318_bits, compass319_bits, compass320_bits,
 compass321_bits, compass322_bits, compass323_bits,compass324_bits, compass325_bits, compass326_bits, compass327_bits, compass328_bits, compass329_bits, compass330_bits,
 compass331_bits, compass332_bits, compass333_bits,compass334_bits, compass335_bits, compass336_bits, compass337_bits, compass338_bits, compass339_bits, compass340_bits,
 compass341_bits, compass342_bits, compass343_bits,compass344_bits, compass345_bits, compass346_bits, compass347_bits, compass348_bits, compass349_bits, compass350_bits,
 compass351_bits, compass352_bits, compass353_bits,compass354_bits, compass355_bits, compass356_bits, compass357_bits, compass358_bits, compass359_bits, compass360_bits,
 };*/


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

/*#define ECWD_logo_width 48
 #define ECWD_logo_height 48
 static unsigned char ECWD_logo_bits[] PROGMEM = {
 0x00, 0x00, 0xf8, 0x0f, 0x00, 0x00, 0x00, 0x80, 0xaf, 0xf8, 0x00, 0x00,
 0x00, 0xe0, 0xf1, 0x83, 0x03, 0x00, 0x00, 0x38, 0xfe, 0x3f, 0x0e, 0x00,
 0x00, 0x0e, 0x1e, 0xf8, 0x18, 0x00, 0x00, 0xff, 0x18, 0xc0, 0x73, 0x00,
 0x80, 0xff, 0x33, 0x00, 0xe7, 0x00, 0xc0, 0x8f, 0x6f, 0x00, 0x8c, 0x01,
 0xe0, 0x01, 0xce, 0x00, 0xb8, 0x03, 0xe0, 0xfc, 0xd8, 0x00, 0x30, 0x03,
 0x70, 0x8e, 0xbb, 0x00, 0x60, 0x06, 0x38, 0x73, 0xb7, 0x81, 0xc6, 0x0c,
 0x98, 0xfb, 0xb6, 0xf1, 0xdf, 0x0c, 0xcc, 0xdd, 0x36, 0x1d, 0xf0, 0x1d,
 0xec, 0x2c, 0xb7, 0x07, 0xc3, 0x19, 0xe6, 0x64, 0xb7, 0xc3, 0x9f, 0x3b,
 0xb6, 0x74, 0xb8, 0x61, 0x30, 0x3b, 0xb2, 0xec, 0xdc, 0x71, 0x6f, 0x3b,
 0x92, 0xed, 0xcf, 0xb0, 0x6f, 0x3b, 0x1b, 0x19, 0x70, 0xd8, 0x6f, 0x3b,
 0x1b, 0x7b, 0x3c, 0xc8, 0x70, 0x7b, 0x1b, 0xe2, 0x0f, 0x68, 0x72, 0x7b,
 0x09, 0x06, 0x00, 0x48, 0x86, 0x69, 0x0d, 0x0c, 0x00, 0xc8, 0xfe, 0x6c,
 0x0d, 0x38, 0x00, 0xd8, 0x7c, 0x6e, 0x0d, 0xe0, 0x07, 0xd8, 0x01, 0x67,
 0x19, 0xc0, 0x3f, 0xb0, 0xff, 0x63, 0x1a, 0xc0, 0xf2, 0x70, 0xfe, 0x28,
 0x1a, 0x70, 0xce, 0xf1, 0x01, 0x2f, 0x1a, 0x98, 0xbf, 0xe1, 0xff, 0x27,
 0x12, 0xd8, 0x7b, 0x63, 0xfe, 0x36, 0x36, 0xec, 0xe4, 0x66, 0x00, 0x16,
 0x34, 0x64, 0xcf, 0x66, 0x00, 0x1b, 0x6c, 0x36, 0xcf, 0x66, 0x00, 0x1b,
 0x48, 0xb6, 0xe1, 0x26, 0x80, 0x09, 0xd8, 0x9b, 0xf6, 0x36, 0x80, 0x05,
 0xb0, 0x9b, 0x76, 0x33, 0xe0, 0x06, 0x20, 0xbb, 0x8e, 0x13, 0x70, 0x02,
 0x60, 0xb2, 0xfd, 0x09, 0x38, 0x03, 0xc0, 0x30, 0x73, 0x0c, 0x9c, 0x00,
 0x80, 0x63, 0x0f, 0x87, 0xe7, 0x00, 0x00, 0xe7, 0xfe, 0xe1, 0x33, 0x00,
 0x00, 0xec, 0xf8, 0x7f, 0x1c, 0x00, 0x00, 0xf8, 0x03, 0x00, 0x0f, 0x00,
 0x00, 0xe0, 0x1f, 0xe0, 0x03, 0x00, 0x00, 0x00, 0xff, 0x7f, 0x00, 0x00,
 0x00, 0x00, 0xf0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
 */




float Analog1Sampling(){                //Convert analogread(A2) into millivolts

  analogReadResolution(12);
  analogReference(INTERNAL);

  int i;
  float sval = 0;

  for(i = 0; i < 10000; i++){          // sample A2 reading 20 times
    sval = sval + analogRead(A2);
  }

  sval = sval / 10000;                    //average results
  sval = sval * (1.2 / 4096 * 1000);   //convert results to millivolts
  return sval;
}

void draw(void) {
  // graphic commands to redraw the complete screen should be placed here  
  //u8g.setFont(u8g_font_unifont);    //default font
  u8g.setFont(u8g_font_courR08);
  //u8g.drawStr( 22, 10, "Test"); //Draw main page static text
  u8g.setFont(u8g_font_4x6);
  u8g.drawStr( 4, 48, "PPo2");
  u8g.drawStr( 49, 48, "MV");

  //partialPressureO2 = PPo2_Calc();

  u8g.setFont(u8g_font_4x6);

  if (mvCal == 0.0) {
    u8g.setFont(u8g_font_6x12);
    u8g.setPrintPos( 25, 22);
    //u8g.print("Calibrate!");
  }
  else {
    u8g.setFont(u8g_font_6x12);
    u8g.setPrintPos( 0, 38);
    u8g.print(ppo2);
    // Serial.println(PPo2);
  }

  u8g.setPrintPos( 41, 38);
  u8g.print(millivolts1);
  // Serial.println(millivolts1);

  u8g.setFont(u8g_font_helvB12); //draw compass heading
  if (headingInt <= 9 ){
    u8g.setPrintPos( 28, 23);
    u8g.print(headingInt);
    //u8g.setPrintPos( 37, 24);
    u8g.print((char)176);       //Degree symbol
  }
  else if (headingInt >= 10 && headingInt <= 99){
    u8g.setPrintPos( 22, 23);
    u8g.print(headingInt);
    u8g.print((char)176);
  }
  else if (headingInt >= 100){
    u8g.setPrintPos( 20, 23);
    u8g.print(headingInt);
    u8g.print((char)176);
  }

  // u8g.drawXBMP( 0, 0, 64, 13, compassString[headingInt]);


  // graphic commands to redraw the complete screen should be placed here  

}



void setup(void) {
  Serial.begin(9600);
  Wire.begin();
  compass.init();
  compass.enableDefault();
  /*
  Calibration values; the default values of +/-32767 for each axis
   lead to an assumed magnetometer bias of 0. Use the Calibrate example
   program to determine appropriate values for your particular unit.
   */
  //compass.m_min = (LSM303::vector<int16_t>){ -32767, -32767, -32767  };
  //compass.m_max = (LSM303::vector<int16_t>){ +32767, +32767, +32767  };
  compass.m_min = (LSM303::vector<int16_t>){ -592, -748, -473  };
  compass.m_max = (LSM303::vector<int16_t>){ +619, +430, +571  };

  // flip screen, if required
  // u8g.setRot180();

  u8g.firstPage();  //Draw Splash Screen for 3 seconds
  do {
    u8g.drawXBMP( 0, 15, 64, 19, revocropfinal_bits); //Calls revo bitmap
  } 
  while( u8g.nextPage() );  
  delay(500);

  /*      u8g.firstPage();  //Draw Splash Screen for 3 seconds
   do {
   u8g.drawXBMP( 8, 0, 48, 48, ECWD_logo_bits ); //Calls revo bitmap
   } while( u8g.nextPage() );  
   delay(4000);
   */
}

void loop(void) {
  millivolts1 = Analog1Sampling();          // sets millitvolts1 equal to the return output of sampling function

  // picture loop
  u8g.firstPage();  
  do {
    draw();
  } 
  while( u8g.nextPage() );


  ppo2mod = (20.9 / mvCal ) / 100;     //gets relationship between air and millivolt reading
  ppo2 = ppo2mod * millivolts1;        // gives acutal o2 percentage based on current millivolts


  //button press and debounce
  int knockValue = analogRead(pinKnock);

  if (knockValue >= knockThreshold) {
    timeNow = millis();

    if (timeNow > (lastKnockTime + timeThreshold)) {
      // Serial.println(++ counter);
      buttonState = HIGH;
    }
  }

  lastKnockTime = timeNow;

  if (buttonState == HIGH) {
    mvCal = millivolts1;
    buttonState = LOW;
  }

  /*Compass Calculations*/
  compass.read();
  float heading = compass.heading((LSM303::vector<int>){ 1, 0, 0  }); //Vector reference for noth heading {X, Y, Z}
  headingInt = heading;
  if (headingInt < 1) {
    headingInt = 360 + headingInt;
  }

  Serial.println(heading);
  Serial.println(headingInt);

  // rebuild the picture after some delay
  delay(300);
}
