// Stop Jambs
// 6 columns, 10 rows
// Original keyboard matrix code from:
// https://www.baldengineer.com/arduino-keyboard-matrix-tutorial.html
//
// TODO - migrate to Teensy MIDI to send noe on/off values, instead of writing out to Serial
// TODO - Add in addressable LED code so status LEDs can be turned on and off by the MIDI controller (i.e. not the local buttons)

byte rowPins[] = {3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
const int rowCount = sizeof(rowPins) / sizeof(rowPins[0]);

byte colPins[] = {0, 1, 2, 21, 22, 23};
const int colCount = sizeof(colPins) / sizeof(colPins[0]);

byte keys[colCount][rowCount];
byte keycache[colCount][rowCount];

void setup() {
  Serial.begin(115200);

  for(int colIndex = 0; colIndex < colCount; colIndex++) {
    for (int rowIndex = 0; rowIndex < rowCount; rowIndex++) {
      keycache[colIndex][rowIndex] = 1;
    }
  }

  for(int x = 0; x < rowCount; x++) {
    Serial.print(rowPins[x]);
    Serial.println(" as input");
    pinMode(rowPins[x], INPUT);
  }

  for (int x = 0; x < colCount; x++) {
    Serial.print(colPins[x]);
    Serial.println(" as input-pullup");
    pinMode(colPins[x], INPUT_PULLUP);
  }
}

void readMatrix() {
  // iterate the columns
  for (int colIndex = 0; colIndex < colCount; colIndex++) {
    // col: set output to low
    byte colPin = colPins[colIndex];
    pinMode(colPin, OUTPUT);
    digitalWrite(colPin, LOW);

    // rows: iterate through the rows in the current column
    for (int rowIndex = 0; rowIndex < rowCount; rowIndex++) {
      byte rowPin = rowPins[rowIndex];
      pinMode(rowPin, INPUT_PULLUP);
      
      delay(1); // delay 1ms to give the pin time to settle

      keys[colIndex][rowIndex] = digitalRead(rowPin);
      pinMode(rowPin, INPUT);

      if (keys[colIndex][rowIndex] != keycache[colIndex][rowIndex]) {
        byte midiNote = (colIndex * 10) + rowIndex;
        Serial.print(midiNote);
        Serial.print(F(": "));

        if (keys[colIndex][rowIndex] == 1) {
          Serial.print("off");
        } else {
          Serial.print("on");
        }

        Serial.println("");

        keycache[colIndex][rowIndex] = keys[colIndex][rowIndex];
      }
    }

    // disable the column
    pinMode(colPin, INPUT);
  }
}

void loop() {
  readMatrix();
}