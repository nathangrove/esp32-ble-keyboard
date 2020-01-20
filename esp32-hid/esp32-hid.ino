/**
 * This example turns the ESP32 into a Bluetooth LE keyboard that writes the words, presses Enter, presses a media key and then Ctrl+Alt+Delete
 * https://github.com/T-vK/ESP32-BLE-Keyboard/releases
 */
#include <BleKeyboard.h>

BleKeyboard bleKeyboard;

boolean pressed[6][18] = { { 0 } };

const int rowCount = 6;
const int colCount = 18;

// row GPIO map
int rows[rowCount] = { 2, 4, 16, 17, 5, 18 };

// col GPIO map
int cols[colCount] = { 36, 39, 34, 35, 32, 33, 25, 26, 27, 14, 12, 13, 23, 22, 1, 3, 21, 19};

// key map
char keys[rowCount][colCount] = { 
  { KEY_ESC, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, ' ', KEY_F9, KEY_F10, KEY_F11, KEY_F12, ' ', ' ', ' ' }, // row 1 
  { '`', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', KEY_BACKSPACE, KEY_INSERT, KEY_HOME, KEY_PAGE_UP }, // row 2
  { KEY_TAB, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\\', KEY_DELETE, KEY_END, KEY_PAGE_DOWN }, // row 3
  { KEY_CAPS_LOCK, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', KEY_RETURN, ' ', ' ', ' ' }, // row 4
  { KEY_LEFT_SHIFT, 'z','x','c','v','b','n','m',',','.','/',KEY_RIGHT_SHIFT, ' ', KEY_UP_ARROW, ' ' }, // row 5
  { KEY_LEFT_CTRL, KEY_LEFT_GUI, KEY_LEFT_ALT, ' ', ' ', ' ', ' ', ' ', KEY_RIGHT_ALT, KEY_RIGHT_CTRL, KEY_LEFT_ARROW, KEY_DOWN_ARROW, KEY_RIGHT_ARROW, KEY_PAGE_UP, KEY_PAGE_DOWN, KEY_HOME, KEY_END }, // row 6
};

void setup() {
  

  Serial.begin(115200);

  Serial.println("Initializing rows");

  // init our scanning rows
  for ( int i=0; i < rowCount; i++){
    pinMode(rows[i], OUTPUT);
    digitalWrite(rows[i], HIGH);
  }

  Serial.println("Initializing columns");
  // init our scanning cols
  for ( int i=0; i < colCount; i++){
    pinMode(cols[i], INPUT_PULLUP);
  }


  Serial.println("Starting the keyboard");
  bleKeyboard.begin();
  delay(2000);
}


unsigned long last = 0;
int lastMinute = 0;

unsigned long powerOff = 0;

void loop() {

    // iterate over the rows
    for (int i=0; i < rowCount; i++){

      // turn the row on
      digitalWrite(rows[i], LOW);

      // iterate over columns looking for high to indicate a key press
      for (int j=0; j < colCount; j++){

        // see if the pin is high
        uint8_t colState = digitalRead(cols[j]);

        // if it is pressed and not previously pressed...press it...
        if (colState == LOW && !pressed[i][j]){
          Serial.print("Sending key press for ");
          Serial.print(keys[i][j]);
          Serial.print('-');
          Serial.print(rows[i]);
          Serial.print('-');
          Serial.println(cols[j]);
          if (bleKeyboard.isConnected()) bleKeyboard.press(keys[i][j]);
          pressed[i][j] = true;


        // if it isn't pressed and it prevously was...release it...
        } else if (pressed[i][j] && colState == HIGH ){
          Serial.print("Sending release command for ");
          Serial.print(keys[i][j]);
          Serial.print('-');
          Serial.print(rows[i]);
          Serial.print('-');
          Serial.println(cols[j]);
          if (bleKeyboard.isConnected()) bleKeyboard.release(keys[i][j]);
          pressed[i][j] = false;

          last = millis();
        }
        
      }

      // turn the row off
      digitalWrite(rows[i], HIGH);
    }


    // if there hasn't been a key up in the last 2 minutes...deep sleep
    if (millis() >= (2*60*1000) && last < millis() - (2 * 60 * 1000)){
      Serial.println("Sleeping...");
      esp_deep_sleep_start();
    }

  
    // log minutes booted
    if (millis() % 60000 == 0 && millis() / 60000 != lastMinute){
      lastMinute = millis() / 60000;
      Serial.print(lastMinute);
      Serial.println(" minutes powered up");
    }


    // check for power off. If both ALT keys are pressed for 3 seconds...
    if (pressed[5][2] && pressed[5][4] && !powerOff) powerOff = millis();
    else if (!pressed[5][2] || !pressed[5][4]) powerOff = 0;

    if (powerOff > 0 && powerOff < millis() - 3000) {
      Serial.println("Powering off");
      for ( int i=0; i < rowCount; i++){
       digitalWrite(rows[i],LOW);
      }
      delay(30000);
    }
    
}
