// Define pin numbers
const int incButtonPin = 2;  // Increment button
const int decButtonPin = 3;  // Decrement button
const int pwmPin = 13;        // PWM output pin (for LED or other device)

// Variables to hold button states and count
int incButtonState = HIGH;    // Increment button current state
int lastIncButtonState = HIGH;
int decButtonState = HIGH;    // Decrement button current state
int lastDecButtonState = HIGH;
int pressCount = 10;          // Starting count (adjust as needed)

// Variables for debounce handling
unsigned long lastIncDebounceTime = 0;
unsigned long lastDecDebounceTime = 0;
const unsigned long debounceDelay = 50;  // milliseconds

// Maximum count value used for mapping PWM (adjust as needed)
const int maxCount = 2000;  // Example max limit

void setup() {
  pinMode(incButtonPin, INPUT);
  pinMode(decButtonPin, INPUT);
  pinMode(pwmPin, OUTPUT);
  Serial.begin(9600);
  
  // Set initial PWM output based on starting count
  int pwmValue = map(pressCount, 0, maxCount, 0, 255);
  analogWrite(pwmPin, pwmValue);
}

void loop() {
  // Read the button states
  int incReading = digitalRead(incButtonPin);
  int decReading = digitalRead(decButtonPin);

  // Handle increment button debounce
  if (incReading != lastIncButtonState) {
    lastIncDebounceTime = millis();
  }
  if ((millis() - lastIncDebounceTime) > debounceDelay) {
    if (incReading != incButtonState) {
      incButtonState = incReading;
      if (incButtonState == LOW) {  // Button pressed
        pressCount++;
        if (pressCount > maxCount) {
          pressCount = maxCount;
        }
        Serial.print("Total button presses: ");
        Serial.println(pressCount);
      }
    }
  }

  // Handle decrement button debounce
  if (decReading != lastDecButtonState) {
    lastDecDebounceTime = millis();
  }
  if ((millis() - lastDecDebounceTime) > debounceDelay) {
    if (decReading != decButtonState) {
      decButtonState = decReading;
      if (decButtonState == LOW) {  // Button pressed
        pressCount--;
        if (pressCount < 0) {
          pressCount = 0;
        }
        Serial.print("Total button presses: ");
        Serial.println(pressCount);
      }
    }
  }

  // Update PWM output
  int pwmValue = map(pressCount, 0, maxCount, 0, 255);
  analogWrite(pwmPin, pwmValue);

  // Save last button states for next loop
  lastIncButtonState = incReading;
  lastDecButtonState = decReading;
}
