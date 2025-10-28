// 2 Stop Jambs
// 6 columns, 10 rows (3 columns per jamb)
// Original keyboard matrix code from:
// https://www.baldengineer.com/arduino-keyboard-matrix-tutorial.html
//

#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define JAMB1_LED_PIN    24
#define JAMB2_LED_PIN    41

// How many NeoPixels are attached to the Arduino?
#define JAMB1_LED_COUNT 30
#define JAMB2_LED_COUNT 30

// the MIDI channel number to send messages
const int MIDI_CHAN = 1;

// output MIDI note values, not CC values
const int MIDI_MODE_NOTES = 0;

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

  // Serial.print("Columns: ");
  // Serial.print(colCount);
  // Serial.print(", Rows: ");
  // Serial.println(rowCount);

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
    // Serial.print(rowPins[x]);
    // Serial.println(" as input");
    pinMode(rowPins[x], INPUT);
  }

  // Write out to serial the columns which have been set to INPUT_PULLUP, and then set the pinMode accordingly.
  for (int x = 0; x < colCount; x++) {
    // Serial.print(colPins[x]);
    // Serial.println(" as input-pullup");
    pinMode(colPins[x], INPUT_PULLUP);
  }

  // Serial.print("Beginning Waterfall effect...");
  waterfall();
  // Serial.println("done.");

  // Set the MIDI note on and off handlers
  usbMIDI.setHandleNoteOn(myNoteOn);
  usbMIDI.setHandleNoteOff(myNoteOff);
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

      // if (keys[colIndex][rowIndex] == 0) {
      //   Serial.print("Switch pressed at ");
      //   Serial.print(colIndex);
      //   Serial.print(",");
      //   Serial.println(rowIndex);
      // }

      // if the read value does NOT match the cached value...
      if (keys[colIndex][rowIndex] != keycache[colIndex][rowIndex]) {
        // define the final pin index based on the column and row position
        byte midiNote = (colIndex * 10) + rowIndex;

        // write out the index and status
        // Serial.print(midiNote);
        // Serial.print(F(": "));

        if (keys[colIndex][rowIndex] == 1) {
          // Serial.print("off");
          usbMIDI.sendNoteOff(midiNote, 0, MIDI_CHAN);  // LOWEST_NOTE + i = MIDI note value

          if (midiNote < 30) {
            if (jamb1_strip.getPixelColor(midiNote) == jamb1_strip.Color(0, 0, 255)) {
              jamb1_strip.setPixelColor(midiNote, jamb1_strip.Color(0, 0, 0));
              jamb1_strip.show();
            }
          } else {
            if (jamb2_strip.getPixelColor(midiNote - 30) == jamb2_strip.Color(0, 0, 255)) {
              jamb2_strip.setPixelColor(midiNote - 30, jamb2_strip.Color(0, 0, 0));
              jamb2_strip.show();
            }
          }
        } else {
          // Serial.print("on");
          usbMIDI.sendNoteOn(midiNote, 99, MIDI_CHAN);

          if (midiNote < 30) {
            jamb1_strip.setPixelColor(midiNote, jamb1_strip.Color(0, 0, 255));
            jamb1_strip.show();
          } else {
            jamb2_strip.setPixelColor(midiNote - 30, jamb2_strip.Color(0, 0, 255));
            jamb2_strip.show();
          }
        }

        // Serial.println("");

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

  // Second section clears incoming MIDI signals - note that MIDI Controllers should discard incoming MIDI messages.
  while (usbMIDI.read()) {
    // read & ignore incoming messages
  }
}

void myNoteOn(byte channel, byte note, byte velocity) {
  // When using MIDIx4 or MIDIx16, usbMIDI.getCable() can be used
  // to read which of the virtual MIDI cables received this message.
  // Serial.print("Note On, ch=");
  // Serial.print(channel, DEC);
  // Serial.print(", note=");
  // Serial.print(note, DEC);
  // Serial.print(", velocity=");
  // Serial.println(velocity, DEC);

  if (note < 30) {
    jamb1_strip.setPixelColor(note, jamb1_strip.Color(255, 255, 255));
    jamb1_strip.show();
  } else {
    jamb2_strip.setPixelColor(note - 30, jamb2_strip.Color(255, 255, 255));
    jamb2_strip.show();
  }
}

void myNoteOff(byte channel, byte note, byte velocity) {
  // Serial.print("Note Off, ch=");
  // Serial.print(channel, DEC);
  // Serial.print(", note=");
  // Serial.print(note, DEC);
  // Serial.print(", velocity=");
  // Serial.println(velocity, DEC);

  if (note < 30) {
    jamb1_strip.setPixelColor(note, jamb1_strip.Color(0, 0, 0));
    jamb1_strip.show();
  } else {
    jamb2_strip.setPixelColor(note - 30, jamb2_strip.Color(0, 0, 0));
    jamb2_strip.show();
  }
}

void waterfall() {
  uint32_t colors[] = {jamb2_strip.Color(255, 0, 0), jamb2_strip.Color(0, 255, 0), jamb2_strip.Color(0, 0, 255), jamb2_strip.Color(255, 255, 255)};
  int colorCount = sizeof(colors) / sizeof(colors[0]);

  for (int colorIndex = 0; colorIndex < colorCount; colorIndex++) {
    for (int rowIndex = 0; rowIndex < rowCount; rowIndex++) {
      for (int colIndex = 0; colIndex < colCount; colIndex++) {
        int ledIndex = (colIndex * 10) + rowIndex;
        if (ledIndex < 30) {
          jamb1_strip.setPixelColor(ledIndex, colors[colorIndex]);
        }

        if (ledIndex >= 30 && ledIndex < 60) {
          jamb2_strip.setPixelColor(ledIndex - 30, colors[colorIndex]);
        }
      }

      jamb1_strip.show();
      jamb2_strip.show();

      delay(100);
    }
  }

  jamb1_strip.clear();
  jamb2_strip.clear();

  jamb1_strip.show();
  jamb2_strip.show();
}