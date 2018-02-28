//Three buttons doing some things. Debra Lemak 10/14/17
//Update 02/13/18

#include <FastLED.h>
#define NUM_LEDS1 42
#define NUM_LEDS2 40
#define NUM_LEDS3 50
#define DATA_PIN1 9
#define DATA_PIN2 10
#define DATA_PIN3 11

#define BRIGHTNESS  255
#define FRAMES_PER_SECOND 60

CRGB leds1[NUM_LEDS1];
CRGB leds2[NUM_LEDS2];
CRGB leds3[NUM_LEDS3];
CRGBPalette16 gPal;


// constants won't change. They're used here to set pin numbers and fade:
const int buttonPin01 = 4;     // the number of the pushbutton pin
const int buttonPin02 = 3;     // the number of the pushbutton pin
const int buttonPin03 = 6;     // the number of the pushbutton pin

//bool gReverseDirection = false;

// variables will change:
int buttonPressed = 0;


int ledMode = 0;
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
long boredomTimer = 0; 


///////Pulling in new color sequence from Andrew Tuline, Title: inoise8_pal_demo.ino
static uint16_t dist;         // A random number for our noise generator.
uint16_t scale = 30;          // Wouldn't recommend changing this on the fly, or the animation will be really blocky.
uint8_t maxChanges = 48;      // Value for blending between palettes.
 
CRGBPalette16 currentPalette(ForestColors_p);
CRGBPalette16 targetPalette(OceanColors_p);
/////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(9600);

  pinMode(buttonPin01, INPUT);
  pinMode(buttonPin02, INPUT);
  pinMode(buttonPin03, INPUT);
  FastLED.addLeds<WS2811, DATA_PIN1, RGB>(leds1, NUM_LEDS1);
  FastLED.addLeds<WS2811, DATA_PIN2, RGB>(leds2, NUM_LEDS2);
  FastLED.addLeds<WS2811, DATA_PIN3, RGB>(leds3, NUM_LEDS3);
  FastLED.setBrightness( BRIGHTNESS );
  FastLED.clear();
  gPal = CRGBPalette16( CRGB::Green,  CRGB::Blue);
//////////from Tuline
  dist = random16(12345);          // A semi-random number for our noise generator
/////////
}

/*
 * Main loop
 */
void loop() {
  checkInputs();
  renderEffects();
  FastLED.show();
  //FastLED.delay(1000/FRAMES_PER_SECOND); 
}

/**
 * Check our inputs and set the button state
 */
void checkInputs() {
  
 if (digitalRead(buttonPin01) == HIGH) {
    buttonPressed = buttonPin01;
    return buttonPressed;
  } 
  
  if (digitalRead(buttonPin02) == HIGH) {
    buttonPressed = buttonPin02;
    return buttonPressed;
  } 
  
  if (digitalRead(buttonPin03) == HIGH) {
    buttonPressed = buttonPin03;
    return buttonPressed;
  } 
  else {
  buttonPressed = 0;
  Serial.println(buttonPressed);
  }
}


/**
 * Handle the button state to render effects
 */
void renderEffects() {
    switch (buttonPressed) {
    case buttonPin01:
     Serial.println("Button 1 pressed.");  
      bpm(leds1, NUM_LEDS1);
      bpm(leds2, NUM_LEDS2);
      bpm(leds3, NUM_LEDS3);
      break;
    case buttonPin02:
      Serial.println("Button 2 pressed.");  
      juggle(leds1, NUM_LEDS1);
      juggle(leds2, NUM_LEDS2);
      juggle(leds3, NUM_LEDS3); 
      break;
    case buttonPin03:
      Serial.println("Button 3 pressed."); 
      rainbowWithGlitter(leds1, NUM_LEDS1); 
      rainbowWithGlitter(leds2, NUM_LEDS2);
      rainbowWithGlitter(leds3, NUM_LEDS3); 
      break;
    case 0:
      Serial.println("No button pressed.");  
      break;
  }
}

/*
 * Efect #1
 */
void juggle(CRGB* strand, int numlights) {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( strand, numlights, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    strand[beatsin16(i+7,0,numlights)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

/*
 * Efect #2
 */
void bpm(CRGB* strand, int numlights)
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = gPal;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < numlights; i++) { 
    strand[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}


/*
 * Efect #3
 */
void rainbowWithGlitter(CRGB* leds, int NUM_LEDS) 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow(leds, NUM_LEDS);
  addGlitter(leds, NUM_LEDS, 80);
}

void rainbow(CRGB* leds, int NUM_LEDS) 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void addGlitter(CRGB* leds, int NUM_LEDS, fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}
