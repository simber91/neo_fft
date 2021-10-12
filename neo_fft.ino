#include <arduinoFFT.h>
#include <Adafruit_NeoPixel.h>
//#ifdef __AVR__
//#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
//#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        2 // On Trinket or Gemma, suggest changing this to 1
#define NUMPIXELS 24 // Popular NeoPixel ring size
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 250 // Time (in milliseconds) to pause between pixels

#define SAMPLES 128             //Must be a power of 2
#define SAMPLING_FREQUENCY 9600 //Hz, must be less than 10000 due to ADC
// 9216 / 128 = 72 /16 = 24
arduinoFFT FFT = arduinoFFT();

unsigned int sampling_period_us;
unsigned long microseconds;




double vReal[SAMPLES];
double vImag[SAMPLES];



void setup() {
//  Serial.begin(115200);
//  Serial.println("Starting...");
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQUENCY));
  pixels.clear(); // Set all pixel colors to 'off'
}




void loop() {
  /*SAMPLING*/
  for (int i = 0; i < SAMPLES; i++)
  {
    microseconds = micros();    //Overflows after around 70 minutes!

    vReal[i] = analogRead(A0)*11;
    vImag[i] = 0;

    while (micros() < (microseconds + sampling_period_us)) {
    }
  }

  /*FFT*/
  FFT.Windowing(FFT_WIN_TYP_BLACKMAN_NUTTALL, FFT_FORWARD);
  FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
  //  double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);
  //  Serial.print("off");
  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
  for (int i = 0; i < NUMPIXELS; i++) { // For each pixel...
    int shine = i + 1;
    if (vReal[shine] <= 100) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));  // No light
    }
    if (vReal[shine] > 100 && vReal[shine] <= 200) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 153)); // blue
    }
    if (vReal[shine] > 200 && vReal[shine] <= 300) {
      pixels.setPixelColor(i, pixels.Color(102, 0, 204)); // purple
    }

    if (vReal[shine] > 301 && vReal[shine] <= 500) {
      pixels.setPixelColor(i, pixels.Color(0, 153, 76)); // green
    }
    if (vReal[shine] > 501 && vReal[shine] <= 700) {
      pixels.setPixelColor(i, pixels.Color(255, 255, 0)); // yellow
    }
    if (vReal[shine] > 701 && vReal[shine] <= 1000) {
      pixels.setPixelColor(i, pixels.Color(255, 128, 0)); // orange
    }
    if (vReal[shine] >  1000) {
      pixels.setPixelColor(i, pixels.Color(255, 0, 0)); // red
    }
    pixels.setBrightness(20);
    pixels.show();   // Send the updated pixel colors to the hardware.
//        delay(100); // Pause before next pass through loop
    /*View all these three lines in serial terminal to see which frequencies has which amplitudes*/
//    Serial.print(shine);
//    Serial.print(" ");
//    Serial.print(": ");
//    Serial.print((shine * 1.0 * SAMPLING_FREQUENCY) / SAMPLES, 1);
//    Serial.print("Hz ");
    
//    Serial.print(vReal[shine], 1);    //View only this line in serial plotter to visualize the bins
//    Serial.print(" ");

  }
//  Serial.println();
}
