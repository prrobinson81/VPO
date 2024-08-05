#include <Bounce.h>

// the MIDI channel number to send messages
const int MIDI_CHAN = 1;

// output MIDI note values, not CC values
const int MIDI_MODE_NOTES = 0;

// C1 - 2 octaves below middle-C
const int LOWEST_NOTE = 36;

// debounce time in ms
const int DEBOUNCE_MS = 5;

// Array of pointers for the 30 Bounce objects - one for each pin - Bounce objects will automatically deal with contact chatter or "bounce"
Bounce *buttons[30];

void setup() {
  // put your setup code here, to run once:

  // Initiate the new Bounce object for the pin, with the defined debounce time.
  // Configure the pins for input mode with pullup resistors.
  // The reed switches connect from each pin to ground.
  // - When closed, the pin reads LOW because the switch shorts it to ground.
  // - When open, the pin reads HIGH because the pullup resistor connects to +5 volts inside the chip.

  for (int i = 0; i < 30; i++) {
    buttons[i] = new Bounce(i, DEBOUNCE_MS);
    pinMode(i, INPUT_PULLUP);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  //==============================================================================
  // First section processes local events generating MIDI signals:
  for (int i = 0; i < 30; i++) {
    // Update the current pin. (-> syntax as the array contains pointers, not the Bounce objects themselves)
    buttons[i]->update();

    // Check the pin for "falling" edge - send a MIDI Note On message when each pedal is pressed
    if (buttons[i]->fallingEdge()) {
      usbMIDI.sendNoteOn(LOWEST_NOTE + i, 99, MIDI_CHAN);  // LOWEST_NOTE + i = MIDI note value
    }

    // Check each button for "rising" edge - send a MIDI Note Off message when each pedal is released
    if (buttons[i]->risingEdge()) {
      usbMIDI.sendNoteOff(LOWEST_NOTE + i, 0, MIDI_CHAN);  // LOWEST_NOTE + i = MIDI note value
    }
  }

//==============================================================================
// Second section clears incoming MIDI signals - note that MIDI Controllers should discard incoming MIDI messages.
  while (usbMIDI.read()) {
    // read & ignore incoming messages
  }
}