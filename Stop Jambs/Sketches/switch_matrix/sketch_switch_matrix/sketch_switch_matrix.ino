// 2 Stop Jambs
// 6 columns, 10 rows (3 columns per jamb)
// Original keyboard matrix code from:
// https://www.baldengineer.com/arduino-keyboard-matrix-tutorial.html
//
// TODO - migrate to Teensy MIDI to send note on/off values from the switches, and receive note on/off values to control the LEDs.

#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define JAMB1_LED_PIN    24
#define JAMB2_LED_PIN    41

// How many NeoPixels are attached to the Arduino?
#define JAMB1_LED_COUNT 30
#define JAMB2_LED_COUNT 30

// Declare our NeoPixel strip object:
Adafruit_NeoPixel jamb1_strip(JAMB1_LED_COUNT, JAMB1_LED_PIN, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel jamb2_strip(JAMB2_LED_COUNT, JAMB2_LED_PIN, NEO_RGB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

// rows come in on the top-right of the device
byte rowPins[] = {14, 15, 16, 17, 18, 19, 20, 21, 22, 23};
const int rowCount = sizeof(rowPins) / sizeof(rowPins[0]);

// columns come in on the top-left of the device
byte colPins[] = {0, 1, 2, 3, 4, 5};
const int colCount = sizeof(colPins) / sizeof(colPins[0]);

// arrays for current and cached/previous states of the buttons
byte keys[colCount][rowCount];
byte keycache[colCount][rowCount];

// Initiate the Teensy
void setup() {
  Serial.begin(115200);

  jamb1_strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  jamb2_strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  jamb1_strip.show();            // Turn OFF all pixels ASAP
  jamb2_strip.show();            // Turn OFF all pixels ASAP
  jamb1_strip.setBrightness(250); // Set BRIGHTNESS to about 1/5 (max = 255)
  jamb2_strip.setBrightness(250); // Set BRIGHTNESS to about 1/5 (max = 255)

  // initiate cached states to 1/off
  // Note:
  // - we're using INPUT_PULLUP so a closed/on switch completes the circuit, earthing the value to low/0.
  // - conversely an open/off switch breaks the circuit so the value is left high/1.
  for(int colIndex = 0; colIndex < colCount; colIndex++) {
    for (int rowIndex = 0; rowIndex < rowCount; rowIndex++) {
      keycache[colIndex][rowIndex] = 1;
    }
  }

  // Write out to serial the rows which have been set to INPUT, and then set the pinMode accordingly.
  for(int x = 0; x < rowCount; x++) {
    Serial.print(rowPins[x]);
    Serial.println(" as input");
    pinMode(rowPins[x], INPUT);
  }

  // Write out to serial the columns which have been set to INPUT_PULLUP, and then set the pinMode accordingly.
  for (int x = 0; x < colCount; x++) {
    Serial.print(colPins[x]);
    Serial.println(" as input-pullup");
    pinMode(colPins[x], INPUT_PULLUP);
  }
}

// Read the state of the key matrix at this specific instance in time.
void readMatrix() {
  // iterate the columns
  for (int colIndex = 0; colIndex < colCount; colIndex++) {
    // col: set output to low, so the pin acts as ground giving the circuit somewhere to complete to.
    byte colPin = colPins[colIndex];
    pinMode(colPin, OUTPUT);
    digitalWrite(colPin, LOW);

    // rows: iterate through the rows in the current column
    for (int rowIndex = 0; rowIndex < rowCount; rowIndex++) {
      // row: set to INPUT_PULLUP, meaning there's +ve voltage on the pin when the pin IS NOT grounded (i.e. there's no closed switch joining the +ve row pin to a GND/0 column pin)
      byte rowPin = rowPins[rowIndex];
      pinMode(rowPin, INPUT_PULLUP);
      
      delay(1); // delay 1ms to give the pin time to settle

      // read the current value on the pin (0 or 1) and set the pin back to INPUT, removing the +ve voltage.
      keys[colIndex][rowIndex] = digitalRead(rowPin);
      pinMode(rowPin, INPUT);

      // if the read value does NOT match the cached value...
      if (keys[colIndex][rowIndex] != keycache[colIndex][rowIndex]) {
        // define the final pin index based on the column and row position
        byte midiNote = (colIndex * 10) + rowIndex;

        // write out the index and status
        Serial.print(midiNote);
        Serial.print(F(": "));

        if (keys[colIndex][rowIndex] == 1) {
          Serial.print("off");

          if (midiNote < 30) {
            jamb1_strip.setPixelColor(midiNote, jamb1_strip.Color(0, 0, 0));
            jamb1_strip.show();
          } else {
            jamb2_strip.setPixelColor(midiNote - 30, jamb2_strip.Color(0, 0, 0));
            jamb2_strip.show();
          }
        } else {
          Serial.print("on");

          if (midiNote < 30) {
            jamb1_strip.setPixelColor(midiNote, jamb1_strip.Color(0, 0, 255));
            jamb1_strip.show();
          } else {
            jamb2_strip.setPixelColor(midiNote - 30, jamb2_strip.Color(0, 0, 255));
            jamb2_strip.show();
          }
        }

        Serial.println("");

        // update the cached value
        keycache[colIndex][rowIndex] = keys[colIndex][rowIndex];
      }
    }

    // disable the column
    pinMode(colPin, INPUT);
  }
}

// loop continuously runs once setup has completed until the Teensy is powered off
void loop() {
  // constantly read the key matrix.
  readMatrix();
}