/**
 * This example turns the ESP32 into a Bluetooth LE keyboard that writes the words, presses Enter, presses a media key and then Ctrl+Alt+Delete
 * https://github.com/T-vK/ESP32-BLE-Keyboard/releases
 */
#include <BleKeyboard.h>

BleKeyboard bleKeyboard;

struct KeyPress {
  long unsigned time;
  int key;
}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  pinMode(25, OUTPUT);
  digitalWrite(25, LOW);

  pinMode(26, INPUT);
  digitalWrite(26, HIGH);
  pinMode(27, INPUT);
  digitalWrite(27, HIGH);

  
  bleKeyboard.begin();
}

void loop() {

    digitalWrite(24, HIGH);

    int r1 = 0;
    int r2 = 0;

    Serial.println("Reading COL1");
    r1 = digitalRead(26);
    r2 = digitalRead(27);

     
    if ( r2 == HIGH){

      bleKeyboard.press(KEY_LEFT_SHIFT);
      Serial.println("Sending shift");

    } 

    if ( r1 == HIGH){

      bleKeyboard.press('b');
      bleKeyboard.release('b');
      Serial.println("Sending b");

    }

    digitalWrite(24, LOW);

    Serial.println("Done scanning COL1");
    delay(2000);

    digitalWrite(14, HIGH);
    Serial.println("Scanning COL2");

    r1 = digitalRead(26);
    r2 = digitalRead(27);

    if ( r1 == HIGH ){

      Serial.println("Sending c");
      bleKeyboard.press('c');
      bleKeyboard.release('c');

    } 
    if ( r2 == HIGH){

      Serial.println("Sending d");
      bleKeyboard.press('d');
      bleKeyboard.release('d');

    } 

    Serial.println("Done scanning COL2");

    digitalWrite(14, LOW);

    bleKeyboard.releaseAll();


  delay(1000);
}
