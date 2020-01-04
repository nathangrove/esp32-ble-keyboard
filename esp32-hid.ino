/**
 * This example turns the ESP32 into a Bluetooth LE keyboard that writes the words, presses Enter, presses a media key and then Ctrl+Alt+Delete
 * https://github.com/T-vK/ESP32-BLE-Keyboard/releases
 */
#include <BleKeyboard.h>

BleKeyboard bleKeyboard;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  pinMode(36, INPUT);
  digitalWrite(36, LOW);
  pinMode(34, INPUT);
  digitalWrite(34, LOW);
  pinMode(35, INPUT);
  digitalWrite(35, LOW);

  
  bleKeyboard.begin();
}

void loop() {
if(bleKeyboard.isConnected()) {
  int a = digitalRead(34);
  int shft = digitalRead(35);
  int rtn = digitalRead(36);
  if (shft == HIGH) {
    bleKeyboard.press(KEY_LEFT_SHIFT);
    Serial.println("Sending shift");
  }
  if (a == HIGH){
    bleKeyboard.press('a');
    Serial.println("Sending a");
  }
  if (rtn == HIGH) {
    bleKeyboard.press(KEY_RETURN);
    Serial.println("Sending return");
  }
  bleKeyboard.releaseAll();

  /*
    Serial.println("Sending 'Hello world'...");
    bleKeyboard.print("Hello world");

    delay(1000);

    Serial.println("Sending Enter key...");
    bleKeyboard.write(KEY_RETURN);

    delay(1000);

    //Serial.println("Sending Play/Pause media key...");
    //bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);

  Serial.println("Sending cap A");
  bleKeyboard.press(KEY_LEFT_SHIFT);
  bleKeyboard.press('a');
  bleKeyboard.releaseAll();
  bleKeyboard.press('a');
  bleKeyboard.releaseAll();
  bleKeyboard.press(KEY_RETURN);
/*
    delay(1000);

    Serial.println("Sending Ctrl+Alt+Delete...");
    bleKeyboard.press(KEY_LEFT_CTRL);
    bleKeyboard.press(KEY_LEFT_ALT);
    bleKeyboard.press(KEY_DELETE);
    delay(100);
    bleKeyboard.releaseAll();
*/

  }
  Serial.println("Scanning...");
  delay(25);
}
