#include <Keyboard.h>

const int BUTTON_PIN = 7;       // Push button input
const int VOLTAGE_PIN = 26;     // ADC pin
const float VREF = 3.3;         // ADC reference voltage
const int ADC_RESOLUTION = 1024;

bool buttonPushed = false;
unsigned long lastButtonPressTime = 0;
const unsigned long debounceTime = 100; // in milliseconds

void setup() {
  pinMode(BUTTON_PIN, INPUT);
  Serial.begin(9600);
  Keyboard.begin();
}

void loop() {
  // Read voltage
  float Vin = ((float)analogRead(VOLTAGE_PIN) / ADC_RESOLUTION) * VREF;
  if (Vin < 0.01) Vin = 0.0;

  // Read button state and debounce
  int buttonState = digitalRead(BUTTON_PIN);
  unsigned long currentTime = millis();

  if (currentTime - lastButtonPressTime > debounceTime) {
    if (buttonState == HIGH && !buttonPushed) {
      lastButtonPressTime = currentTime;
      buttonPushed = true;

      // Output to serial and keyboard
      Serial.print("Voltage: ");
      Serial.print(Vin, 3);
      Serial.println(" V");

      Keyboard.print(Vin, 3);
      Keyboard.press(KEY_RETURN);
      Keyboard.releaseAll();
    }

    if (buttonState == LOW) {
      buttonPushed = false;  // Reset when button is released
    }
  }
}
