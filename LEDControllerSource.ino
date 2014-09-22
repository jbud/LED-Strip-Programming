/*Includes*/
#include "FastLED.h"

/*LED Settings*/
#define NUM_LEDS 10          // Number of LEDs (Or LED controller chips on strip)
#define DATA_PIN 9           // Pin which the data cable is connected.
#define MAX_BRIGHTNESS 255   // Max Brightness (Default 255)
#define LED_CHIPSET TM1803   // LED Chipset.
#define COLOR_ORDER GBR     // Color order (RGB, RBG, BGR, BRG, GRB, GBR) See FastLED examples (RGB Calibration) for finding correct order.

//DEMO_MODE mode includes many flashing features and random colors, turning this off loops a gentle rainbow effect for at home use.
#define DEMO_MODE true

//Not currently functional. (Future feature will react to currently playing music, may require additional hardware).
#define MUSIC_MODE false

// This is only used for debugging functions and testing other stuff.
#define DEBUG_MODE false

//Global Variables.
CRGB leds[NUM_LEDS];
uint32_t g_color;
long n;

/*MANDATORY FUNCTIONS*/

// Create the LED object
/*setup() is called by the Arduino on powerup or reboot.*/
void setup()
{
	 FastLED.addLeds<LED_CHIPSET, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
}

// Main function loop.
/*loop() is called by arduino after setup() and after loop().*/
void loop()
{
  if (DEBUG_MODE){
    //n = randomFunction();
    //prntInColorInt(n);
    rColorTest();
  }else{
    if (DEMO_MODE){
      randFunction();
    }else{
      rainbow(5); 
    }
  }
}

/*UTILITY FUNCTIONS*/

uint32_t createRGB(uint8_t r, uint8_t g, uint8_t b)
{   
    return ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff);
}

void rseed(){
    randomSeed(millis());
    delay(10);
}

uint16_t random255(){
  rseed();
  long r = random(10,256);
  return r;  
}

uint32_t randomColor(){
    uint8_t r_R= random255();
    uint8_t g_G = random255();
    uint8_t b_B = random255();
   return createRGB(r_R, g_G, b_B); 
}

long randomFunction(){
    rseed();
    long r = random(0,6);
    return r;
}

uint32_t randomRGB(){
    rseed();
    long r = random(0,3);
    uint32_t c;
    if (r == 0){
      c = CRGB::Red;
    }else{
      if (r == 1){
        c = CRGB::Green;
      }else{
        if (r == 2){
          c = CRGB::Blue;
        }else{
          c = CRGB::White; 
        }
      }
    }
    return c;
}

void randFunction(){
  long e = randomFunction();
  switch(e){
    case 0:
      cyln();
      break;
    case 1:
      chase();
      break;
    case 2:
      rMissing_chase();
      break;
    case 3:
      //wave(5);
      rWave(5);
      break;
    case 4:
      //chase_slow();
      rChase_slow();
      break;
    case 5:
    default:
      rainbow(5);
      break;
  } 
}

/*MAIN LED EFFECTS*/

void color_chase(uint32_t color, uint8_t wait)
{
	FastLED.clear();
	FastLED.setBrightness(MAX_BRIGHTNESS);
	for(int led_number = 0; led_number < NUM_LEDS; led_number++)
	{
		leds[led_number] = color;
		FastLED.show();
		delay(wait);
		leds[led_number] = CRGB::Black;
	}
	return;
}

void missing_dot_chase(uint32_t color, uint8_t wait)
{
	for (int led_brightness = MAX_BRIGHTNESS; led_brightness > 10; led_brightness/=2)
	{
		FastLED.setBrightness(led_brightness);

		fill_solid(leds, NUM_LEDS, color);

		for(int led_number = 0; led_number < NUM_LEDS; led_number++)
		{
			leds[led_number] =  CRGB::Black;
			if( led_number > 0 && led_number < NUM_LEDS)
			{
				leds[led_number-1] = color;
			}
			FastLED.show();
			delay(wait);
		}
	}
	return;
}

void missing_dot_chase_reverse(uint32_t color, uint8_t wait)
{
	for (int led_brightness = 10; led_brightness < MAX_BRIGHTNESS; led_brightness*=2)
	{
		FastLED.setBrightness(led_brightness);

		fill_solid(leds, NUM_LEDS, color);

		for(int led_number = 0; led_number < NUM_LEDS; led_number++)
		{
			leds[led_number] =  CRGB::Black;
			if( led_number > 0 && led_number < NUM_LEDS)
			{
				leds[led_number-1] = color;
			}
			FastLED.show();
			delay(wait);
		}
	}
	return;
}

void cylon(CRGB color, uint16_t wait, uint8_t number_of_cycles)
{
	FastLED.setBrightness(255);
	for (uint8_t times = 0; times<=number_of_cycles; times++)
	{
		for(int led_number = 0; led_number < NUM_LEDS; led_number++)
		{
			leds[led_number] = color;
			FastLED.show();
			leds[led_number] = CRGB::Black;
			delay(wait);
		}

		for(int led_number = NUM_LEDS-1; led_number >= 0; led_number--)
		{
			leds[led_number] = color;
			FastLED.show();
			leds[led_number] = CRGB::Black;
			delay(wait);
		}
	}
	return;
}

void rainbow(uint8_t wait) 
{

	uint16_t hue;
	FastLED.clear();
        FastLED.setBrightness(MAX_BRIGHTNESS);
	for(hue=10; hue<255*3; hue++) 
	{

		fill_rainbow( &(leds[0]), NUM_LEDS, hue);		
		FastLED.show();
		delay(wait);
	}
	return;
}

/*DEBUG FUNCTIONS*/

void prntInColorInt(uint8_t i){
  
  if (i == 0){
    missing_dot_chase(CRGB::Red, 5);
  }else{
      for (uint8_t j = 1; j <= i; j++){
      missing_dot_chase(0xcc00cc, 5);
      delay(100);
    }
  }
  delay(5000);
}
void colorTest(){
  uint32_t c = randomRGB();
  missing_dot_chase(c, 5);
  delay(1000);
  
}

void rColorTest(){
    uint8_t r_R = random255();
    rseed();
    uint8_t g_G = random255();
    rseed();
    uint8_t b_B = random255();
    //prntInColorInt(r_R);
    uint32_t clor = createRGB( r_R, g_G, b_B);
  missing_dot_chase(clor, 5);
  delay(1000);
  
}

/*Function Modes*/

void cyln(){
  g_color = randomColor();
  cylon(g_color,25, 5);
}

void missing_chase(){
       	  missing_dot_chase(CRGB::White, 20);
      	  missing_dot_chase(CRGB::Red, 20);
      	  missing_dot_chase(CRGB::Yellow, 20);
      	  missing_dot_chase(CRGB::Green, 20);
      	  missing_dot_chase(CRGB::Cyan, 20);
      	  missing_dot_chase(CRGB::Blue, 20);
      	  missing_dot_chase(0x3000cc, 20) ; 
  
}

void chase(){
          color_chase(CRGB::Green, 15);
      	  color_chase(CRGB::BlueViolet, 15);
      	  color_chase(CRGB::Red, 15);
      	  color_chase(CRGB::Yellow, 15);
      	  color_chase(CRGB::Green, 15);
          color_chase(CRGB::Cyan, 15);
          color_chase(CRGB::Blue, 15);
}

void chase_slow(){  
          color_chase(CRGB::White, 150);
          color_chase(CRGB::Green, 150);
      	  color_chase(CRGB::BlueViolet, 150);
      	  color_chase(CRGB::Red, 150);
      	  color_chase(CRGB::Yellow, 150);
      	  color_chase(CRGB::Green, 150);
          color_chase(CRGB::Cyan, 150);
          color_chase(CRGB::Blue, 150);
}

void wave(uint8_t wait){
          missing_dot_chase(CRGB::White, wait);
      	  missing_dot_chase_reverse(CRGB::Red, wait);
      	  missing_dot_chase(CRGB::Yellow, wait);
      	  missing_dot_chase_reverse(CRGB::Green, wait);
      	  missing_dot_chase(CRGB::Cyan, wait);
      	  missing_dot_chase_reverse(CRGB::Blue, wait);
      	  missing_dot_chase(0x3000cc, wait) ;
          missing_dot_chase_reverse(CRGB::White, wait);
      	  missing_dot_chase(CRGB::Red, wait);
      	  missing_dot_chase_reverse(CRGB::Yellow, wait);
      	  missing_dot_chase(CRGB::Green, wait);
      	  missing_dot_chase_reverse(CRGB::Cyan, wait);
      	  missing_dot_chase(CRGB::Blue, wait);
      	  missing_dot_chase_reverse(0x3000cc, wait) ;
}

/*Random Simplified Function Modes*/

void rMissing_chase(){
  for (uint8_t i=0;i<10;i++){
   missing_dot_chase(randomColor(), 20); 
  }
}
void rWave(uint8_t wait){
  for (uint8_t i=0;i<10;i++){
    missing_dot_chase(randomColor(), wait);
    missing_dot_chase_reverse(randomColor(), wait);
  }
}

void rChase_slow(){
  for (uint8_t i=0;i<10;i++){
   color_chase(randomColor(), 150); 
  }
}
