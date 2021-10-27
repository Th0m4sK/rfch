#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN       D3 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 201 // Popular NeoPixel ring size
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 1000 // Time (in milliseconds) to pause between pixels

int Gr_Start[9] = {0,6, 25, 30, 40,  49,  54,  64, 112,};
int Gr_Anzahl[9] = {6, 19, 5, 10, 9, 5, 10, 48, 89};
int segel[3] = {0, 2, 4};
int mast_oben[1] = {1};
int boot[3] = {3,5, 7};
int schrift[1] = {6};
int ring[1] = {8};

void setPixelLine(int start, int Anzahl, uint8_t r, uint8_t g, uint8_t b ){
  Serial.print(start);
  Serial.print(";");
  Serial.print(Anzahl);
  Serial.print(";");
  Serial.print(r);
  Serial.print(";");
  Serial.print(g);
  Serial.print(";");
  Serial.println(b);



  
  for (int i = start; i < start+Anzahl; i++)
  {
    pixels.setPixelColor(i, pixels.Color(r, g, b));
  }
}

void setColor2Gr(int obj[],int size,uint8_t r,uint8_t g,uint8_t b){
  
  for (unsigned long i = 0; i < size ; i++)
  {

    Serial.println(size);
    Serial.println(Gr_Start[obj[i]]);
    Serial.println(Gr_Anzahl[obj[i]]);

    setPixelLine(Gr_Start[obj[i]], Gr_Anzahl[obj[i]], r, g, b);
 }

}
void setup() {
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.
  Serial.begin(115200);
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
}

void loop() {
  // Set all pixel colors to 'off'
  pixels.clear();

  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
  setColor2Gr(mast_oben,1, 255, 0, 0);
  setColor2Gr(segel,3, 0, 0, 255);
  setColor2Gr(boot,3, 0, 255, 0);
  setColor2Gr(schrift,1, 255, 255, 0);
   setColor2Gr(ring,1, 255, 255, 255);
  pixels.setBrightness(255);

  pixels.show();   // Send the updated pixel colors to the hardware.

    delay(500000);


}