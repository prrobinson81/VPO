#include <FastTouch.h>

// 0 when LED is Off, otherwise shows the pin number + 1 of the detected touch input
int LEDON = 0;

int PIN_SET = 24;
int LED_SET = 25;
int PIN_MB = 27;
int LED_MB = 28;

int inputDatum[CORE_NUM_DIGITAL];

// the MIDI channel number to send messages
const int MIDI_CHAN = 1;

// output MIDI note values, not CC values
const int MIDI_MODE_NOTES = 0;

// the lowest note to to send when button 0 is pressed
const int LOWEST_NOTE = 40;

void setup() {
  // put your setup code here, to run once:

  // Record empty pin status...
  for(int j = 1; j <= 5; j++)
  {
    // Loop through the digital pins (except the LED pin 13) and record the base value, so we're adapting to the environment at power on.
    for(int i = 0; i < CORE_NUM_DIGITAL; i++)
    {
        if(i != LED_BUILTIN && i != LED_MB && i != LED_SET)
        {
          int baseValue = fastTouchRead(i);
          inputDatum[i] = max(inputDatum[i], baseValue);
        }
    }
    
    delay(200);
  }

  // set the LED pins as output so we can turn the LEDs on and off
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_MB, OUTPUT);
  pinMode(LED_SET, OUTPUT);

  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(LED_MB, LOW);
  digitalWrite(LED_SET, LOW);

  // define the MIDI input handler routines
  usbMIDI.setHandleNoteOff(myNoteOff);
  usbMIDI.setHandleNoteOn(myNoteOn);
}

void loop() {
  // put your main code here, to run repeatedly:

  //==============================================================================
  // First section processes local event generating MIDI signals:
  
  int foundtouch = 0;

  // loop through all the input pins (skipping the LED output pins) and compare the current capacitance value with the datum value previously stored
  // if the new value is higher than the old value + 5 (to act as a noise buffer) then mark the pin as having a valid input detected.
  for(int i = 1; i <= CORE_NUM_DIGITAL; i++)
  {
      if(i - 1 != LED_BUILTIN && i - 1 != LED_MB && i - 1 != LED_SET)
      {
        if (fastTouchRead(i - 1) > inputDatum[i-1] + 5)
        {
          foundtouch = i;
        }
      }
  }

// check the identified pin isn't the one already process during the last loop (avoid flooding multiple signals for a single 'long' touch)
if (foundtouch != LEDON)
{
  if (foundtouch > 0)
  {
    // Touch identified on pin 'foundtouch - 1', so 
    // - turn on the builtin LED for visual feedback during testing.
    // - cache the pin number
    // - send a MIDI NOTE ON message for the pin number.
    digitalWrite(LED_BUILTIN, HIGH);
    LEDON = foundtouch;
    usbMIDI.sendNoteOn (LOWEST_NOTE + foundtouch - 1, 110, MIDI_CHAN);
  }
  else
  {
    // Touch has been released on pin 'LEDON - 1', so
    // - turn off the buildin LED for visual feedback during testing.
    // - send a MIDI NOTE OFF message for the pin number.
    // - clear the cached pin number
    digitalWrite(LED_BUILTIN, LOW);
    usbMIDI.sendNoteOff (LOWEST_NOTE + LEDON - 1, 0, MIDI_CHAN);
    LEDON = 0;
  }
}

//==============================================================================
// Second section reads incoming MIDI signals to trigger the relevant event handlers.
  usbMIDI.read(MIDI_CHAN);

//==============================================================================
// Third section adds a 200ms delay to the loop - avoids needless churn, as 200ms seems plenty fast for 'real-time' performance.
  delay(200);
}

// Handle incoming MIDI NOTE ON events
void myNoteOn(byte channel, byte note, byte velocity) {
  // check if the incoming note is either the MB or SET value, and set the relevant LED pin to high to turn it on.
  if (note == PIN_MB + LOWEST_NOTE) {
    digitalWrite(LED_MB, HIGH);
  }

  if (note == PIN_SET + LOWEST_NOTE) {
    digitalWrite(LED_SET, HIGH);
  }
}

// Handle incoming MIDI NOTE OFF events
void myNoteOff(byte channel, byte note, byte velocity) {
  // check if the incoming note is either the MB or SET value, and set the relevant LED pin to low to turn it off.
  if (note == PIN_MB + LOWEST_NOTE) {
    digitalWrite(LED_MB, LOW);
  }

  if (note == PIN_SET + LOWEST_NOTE) {
    digitalWrite(LED_SET, LOW);
  }
}
