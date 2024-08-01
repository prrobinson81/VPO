#include <FastTouch.h>

// 0 when LED is Off, otherwise shows the pin number + 1 of the detected touch input
int LEDON = 0;

int PIN_SET = 24;
int LED_SET = 25;
int PIN_MB = 27;
int LED_MB = 28;

int inputDatum[CORE_NUM_DIGITAL];

int PIN_SWELL = A9;
int inputSwell = 0;
int avgSwell = 0;

// the MIDI channel number to send messages
const int MIDI_CHAN = 1;

// output MIDI note values, not CC values
const int MIDI_MODE_NOTES = 0;

// the lowest note to to send when button 0 is pressed
const int LOWEST_NOTE = 40;

void setup() {
  // put your setup code here, to run once:
  //SerialUSB.begin(9600);

  //SerialUSB.print("Total Digital Pin Count: ");
  //SerialUSB.println(CORE_NUM_DIGITAL);

  //SerialUSB.print("A9 analog pin: ");
  //SerialUSB.println(A9);

  //SerialUSB.println("Record empty pin status...");

  for(int j = 1; j <= 5; j++)
  {
    // Loop through the digital pins (except the LED pin 13) and record the base value, so we're adapting to the environment at power on.
    for(int i = 0; i < CORE_NUM_DIGITAL; i++)
    {
        if(i != LED_BUILTIN && i != LED_MB && i != LED_SET)
        {
          int baseValue = fastTouchRead(i);
          // SerialUSB.print(baseValue); SerialUSB.print(" ");
          inputDatum[i] = max(inputDatum[i], baseValue);
        }
    }

    //SerialUSB.println();
    delay(200);
  }

  //SerialUSB.println("done.");

   // Loop through the digital pins (except the LED pin 13) and display the detected base value.
   //for(int i = 0; i < CORE_NUM_DIGITAL; i++)
   //{
   //   if(i != LED_BUILTIN && i != LED_MB && i != LED_SET)
   //   {
   //     SerialUSB.print(inputDatum[i]); SerialUSB.print(" ");
   //   }
   //}

   //SerialUSB.println();

  // set the LED pins as output so we can turn the LEDs on and off
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_MB, OUTPUT);
  pinMode(LED_SET, OUTPUT);

  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(LED_MB, LOW);
  digitalWrite(LED_SET, LOW);

  usbMIDI.setHandleNoteOff(myNoteOff);
  usbMIDI.setHandleNoteOn(myNoteOn);
}

void loop() {
  int foundtouch = 0;

  // put your main code here, to run repeatedly:
  for(int i = 1; i <= CORE_NUM_DIGITAL; i++)
  {
      if(i - 1 != LED_BUILTIN && i - 1 != LED_MB && i - 1 != LED_SET)
      {
        // SerialUSB.print(fastTouchRead(i)); SerialUSB.print(" ");
        if (fastTouchRead(i - 1) > inputDatum[i-1] + 5)
        {
          foundtouch = i;
        }
      }
  }

if (foundtouch != LEDON)
{
  if (foundtouch > 0)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    LEDON = foundtouch;
    // SerialUSB.print("Touch began on pin "); SerialUSB.println(foundtouch - 1);
    usbMIDI.sendNoteOn (LOWEST_NOTE + foundtouch - 1, 110, MIDI_CHAN);
  }
  else
  {
    digitalWrite(LED_BUILTIN, LOW);
    // SerialUSB.print("Touch released on pin "); SerialUSB.println(LEDON - 1);
    usbMIDI.sendNoteOff (LOWEST_NOTE + LEDON - 1, 0, MIDI_CHAN);
    LEDON = 0;
  }
}

int analogValue = analogRead(PIN_SWELL);
avgSwell = (analogValue + inputSwell) / 2;

// SerialUSB.print("Analog Read: ");
// SerialUSB.println(analogValue);
if (avgSwell != inputSwell) {
  
  int sendValue = avgSwell * 10;
  if (sendValue > 127) {
    sendValue = 127;
  }

  usbMIDI.sendControlChange(11, sendValue, MIDI_CHAN);
  
  // SerialUSB.print("Swell changed: ");
  // SerialUSB.println(sendValue);

  inputSwell = avgSwell;
}

//==============================================================================
  usbMIDI.read(MIDI_CHAN);

//==============================================================================
  delay(200);
}

void myNoteOn(byte channel, byte note, byte velocity) {
  if (note == PIN_MB + LOWEST_NOTE) {
    digitalWrite(LED_MB, HIGH);
  }

  if (note == PIN_SET + LOWEST_NOTE) {
    digitalWrite(LED_SET, HIGH);
  }
}

void myNoteOff(byte channel, byte note, byte velocity) {
  if (note == PIN_MB + LOWEST_NOTE) {
    digitalWrite(LED_MB, LOW);
  }

  if (note == PIN_SET + LOWEST_NOTE) {
    digitalWrite(LED_SET, LOW);
  }
}
