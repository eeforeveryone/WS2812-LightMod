#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

//Run ATTINY85 at 8Mhz internal oscillator.
//Example "strantdest_wheel" used as base library.

//const uint16_t PixelCount = 1; // 1 = testing
const uint16_t PixelCount = 20; // 20 = fully assembled
const uint16_t PixelPin = 4; //pin 4 for attiny85 
const uint16_t SwitchPin = 3; //pin to use for switch input / mode change, pulled up externally.

const uint16_t PixelFadeDuration = 500; // third of a second
const uint16_t NextPixelMoveDuration = 1000 / PixelCount; // how fast we move through the pixels

const uint32_t CYCLETIMER = 900000; // 15*60*1000 = 900000 (15 minute)

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PixelCount, PixelPin, NEO_GRB + NEO_KHZ800);

void setup() {
  // put your setup code here, to run once:


  strip.begin();
  strip.setBrightness(100); //full brightness
  strip.show(); // Initialize all pixels to 'off'

}

uint32_t oldmillis = millis();

uint16_t mainloop_index = 0;
void loop() {

  if( (millis() < oldmillis) || (millis() > (oldmillis + CYCLETIMER))){
    mainloop_index = mainloop_index +1; //increment mainloop index
    oldmillis = millis();
  }

  switch(mainloop_index){
    case 0:
      fire(0x00ff2000);
      break;
      
    case 1:
      colorWipe(strip.Color(random(0,255), random(0,255), random(0,255)), PixelFadeDuration); //pick a random color
      break;
      
    case 2:
      rainbowCycle(PixelFadeDuration);
      break;

    case 3:
      colorWipe(strip.Color(128, 128, 128), PixelFadeDuration); //fade to white. stay there.
      
    default:
      mainloop_index = 0;
      break;
  }

  
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {  
  
    uint8_t redpart = (c>>16);
    uint8_t greenpart = (c>>8);
    uint8_t bluepart = (c);

    uint32_t oldcolor = strip.getPixelColor(i);
    uint8_t oldred = (oldcolor>>16);
    uint8_t oldgreen = (oldcolor>>8);
    uint8_t oldblue = (oldcolor);

    while(oldred != redpart || oldgreen != greenpart || oldblue != bluepart){
      strip.setPixelColor(i, oldred, oldgreen, oldblue);
      strip.show();
      delay(5);

      if(oldred < redpart){
        oldred++;
      }else if(oldred > redpart){
        oldred--;
      }
      if(oldgreen < greenpart){
        oldgreen++;
      }else if(oldgreen > greenpart){
        oldgreen--;
      }
      if(oldblue < bluepart){
        oldblue++;
      }else if(oldblue > bluepart){
        oldblue--;
      }
    }

    delay(wait);
  }

  delay(wait*random(2,10)); //hold color for a while
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

void fire(uint32_t c){

  uint8_t red = (c>>16);
  uint8_t green = (c>>8);
  uint8_t blue = c;

  uint16_t i,j;


//  for(i=0; i<strip.numPixels(); i++) { //setup
//    strip.setPixelColor(i, c);
//  }
  

  for(j=0; j<256; j++){ //do 256 cycles
    
    for(i=0; i<strip.numPixels(); i++) {
      uint8_t brightness = random(15, 100);
      uint8_t redpart = (uint16_t)red*brightness/100;
       brightness = random(15, 100);
      uint8_t greenpart = (uint16_t)green*brightness/100;
       brightness = random(15, 100);
      uint8_t bluepart = (uint16_t)blue*brightness/100;

      uint32_t oldcolor = strip.getPixelColor(i);
      uint8_t oldred = (oldcolor>>16);
      uint8_t oldgreen = (oldcolor>>8);
      uint8_t oldblue = (oldcolor);


      if(oldred != redpart || oldgreen != greenpart || oldblue != bluepart){

        uint8_t down_qty = 12;
        uint8_t up_qty = 12;

        if(oldred < redpart && oldred < (255-up_qty)){
          oldred = oldred + up_qty;
        }else if(oldred > redpart && oldred > down_qty){
          oldred = oldred - down_qty;
        }
        if(oldgreen < greenpart&& oldgreen < (255-up_qty)){
          oldgreen = oldgreen + up_qty;
        }else if(oldgreen > greenpart && oldgreen > down_qty){
          oldgreen = oldgreen - down_qty;
        }
        if(oldblue < bluepart && oldblue < (255-up_qty)){
          oldblue = oldblue + up_qty;
        }else if(oldblue > bluepart && oldblue > down_qty){
          oldblue = oldblue - down_qty;
        }
  
  
        strip.setPixelColor(i, oldred, oldgreen, oldblue);
      
      }
    }
    strip.show();
    delay(75); //fast effect
    
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
