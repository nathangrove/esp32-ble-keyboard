/**
 * This example turns the ESP32 into a Bluetooth LE keyboard that writes the words, presses Enter, presses a media key and then Ctrl+Alt+Delete
 * https://github.com/T-vK/ESP32-BLE-Keyboard/releases
 */
#include <BleKeyboard.h>
BleKeyboard bleKeyboard;


const int rowCount = 6;
const int colCount = 17;

boolean pressed[rowCount][colCount] = { { 0 } };



// row GPIO map
int rows[rowCount] = { 2, 4, 16, 17, 5, 18 };

// col GPIO map
int cols[colCount] = { 36, 39, 34, 35, 32, 33, 25, 26, 27, 14, 12, 13, 23, 22, 19, 3, 21 };

// key map
char keys[rowCount][colCount] = { 
  { KEY_ESC,        KEY_F1,       KEY_F2,       KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8,        '~', KEY_F9,        KEY_F10,         KEY_F11,         KEY_F12,        '~',            '~',            '~' }, // row 1 
  { '`',            '1',          '2',          '3',    '4',    '5',    '6',    '7',    '8',           '9', '0',           '-',             '=',             KEY_BACKSPACE,  KEY_INSERT,     KEY_HOME,       KEY_PAGE_UP }, // row 2
  { KEY_TAB,        'q',          'w',          'e',    'r',    't',    'y',    'u',    'i',           'o', 'p',           '[',             ']',             '\\',            KEY_DELETE,     KEY_END,        KEY_PAGE_DOWN }, // row 3
  { KEY_CAPS_LOCK,  'a',          's',          'd',    'f',    'g',    'h',    'j',    'k',           'l', ';',           '\'',             '~',             KEY_RETURN,     '~',            '~',            '~' }, // row 4
  { KEY_LEFT_SHIFT, 'z',          'x',          'c',    'v',    'b',    'n',    'm',    ',',           '.', '/',           '~',             KEY_RIGHT_SHIFT, '~',            '~',            KEY_UP_ARROW,   '~' }, // row 5
  { KEY_LEFT_CTRL,  KEY_LEFT_GUI, KEY_LEFT_ALT, '~',    '~',    '~',    ' ',    '~',    KEY_RIGHT_ALT, '~', KEY_RIGHT_GUI, '~',             KEY_RIGHT_CTRL,  '~',             KEY_LEFT_ARROW, KEY_DOWN_ARROW, KEY_RIGHT_ARROW }, // row 6
};


// function key stuffs...
const int fnRow = 5;
const int fnCol = 11;

uint8_t fnKeys[rowCount][colCount][2] = { { { 0 }, {128, 0}, {64, 0}, {32, 0}, {16, 0} } };


void setup() {

  

  //Serial.begin(115200);

  //Serial.println("Initializing rows");

  // init our scanning rows
  for ( int i=0; i < rowCount; i++){
    pinMode(rows[i], OUTPUT);
    digitalWrite(rows[i], HIGH);
  }

  //Serial.println("Initializing columns");
  // init our scanning cols
  for ( int i=0; i < colCount; i++){
    pinMode(cols[i], INPUT_PULLUP);
  }


  //Serial.println("Starting the keyboard");
  bleKeyboard.begin();
}

unsigned long last = 0;
int lastMinute = 0;
bool wasConnected = false;

void loop() {

  if (bleKeyboard.isConnected()){
    wasConnected = true;

    // iterate over the rows
    for (int i=0; i < rowCount; i++){

      // turn the row on

      digitalWrite(rows[i], LOW);

      // iterate over columns looking for high to indicate a key press
      for (int j=0; j < colCount; j++){

        if (keys[i][j] == '~') continue;

        // see if the pin is high
        uint8_t colState = digitalRead(cols[j]);

        // if it is pressed and not previously pressed...press it...
        if (colState == LOW && !pressed[i][j]){
          //Serial.print("Sending key press for ");
          //Serial.print(keys[i][j]);
          //Serial.print('-');
          //Serial.print(rows[i]);
          //Serial.print('-');
          //Serial.println(cols[j]);

          if (pressed[fnRow][fnCol]) bleKeyboard.press(fnKeys[i][j]);
          else bleKeyboard.press(keys[i][j]);

          pressed[i][j] = true;



        // if it isn't pressed and it prevously was...release it...
        } else if (colState == HIGH && pressed[i][j] ){
          //Serial.print("Sending release command for ");
          //Serial.print(keys[i][j]);
          //Serial.print('-');
          //Serial.print(rows[i]);
          //Serial.print('-');
          //Serial.println(cols[j]);
          
          if (pressed[fnRow][fnCol]) bleKeyboard.release(fnKeys[i][j]);
          else bleKeyboard.release(keys[i][j]);

          pressed[i][j] = false;

          last = millis();
        }
        
      }

      // turn the row off
      digitalWrite(rows[i], HIGH);
      delay(2);
    }
  
  }

  // if we were connected but are no longer connected...shut down
  if (wasConnected && !bleKeyboard.isConnected()){
    esp_deep_sleep_start();
  }

  // if there hasn't been a key up in the last 5 minutes...deep sleep
  if (millis() >= (5*60*1000) && last < millis() - (5 * 60 * 1000)){
    //Serial.println("Sleeping...");
    esp_deep_sleep_start();
  }


  

  


}
