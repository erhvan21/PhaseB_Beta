// Pin Definitions
#define CLOCK_PIN 2      // Button pin for clock increment
#define START_STOP_PIN 3 // Button pin for start/stop
#define RESET_PIN 4      // Button pin for reset
#define BUZZER_PIN 5     // Pin for the buzzer
#define DATA_PIN 6       // Data pin for shift register
#define LATCH_PIN 7      // Latch pin for shift register
#define CLOCK_PIN_SR 8   // Clock pin for shift register
#define ENABLE_PIN 9     // Enable pin for 7-segment display

// Global Variables
unsigned long startTime = 0; // Start time in milliseconds
unsigned long currentTime = 0; // Current time in milliseconds
unsigned long remainingTime = 0; // Remaining time in milliseconds
bool isCounting = false; // Flag to indicate if the timer is counting
bool isStopped = false; // Flag to indicate if the timer is stopped
int buzzerState = LOW; // Current state of the buzzer

// 7-segment display segment patterns
const byte SEGMENT_PATTERNS[] = {
  B11111100,  // 0
  B01100000,  // 1
  B11011010,  // 2
  B11110010,  // 3
  B01100110,  // 4
  B10110110,  // 5
  B10111110,  // 6
  B11100000,  // 7
  B11111110,  // 8
  B11110110   // 9
};

// Function to update the display with a given time value
void updateDisplay(unsigned long time) {
  byte digit1 = time / 1000 % 10; // Extract thousands digit
  byte digit2 = time / 100 % 10;  // Extract hundreds digit
  byte digit3 = time / 10 % 10;   // Extract tens digit
  byte digit4 = time % 10;        // Extract ones digit
  
  digitalWrite(ENABLE_PIN, LOW); // Disable the display

  // Shift out the segment patterns for each digit
  shiftOut(DATA_PIN, CLOCK_PIN_SR, LSBFIRST, SEGMENT_PATTERNS[digit1]);
  shiftOut(DATA_PIN, CLOCK_PIN_SR, LSBFIRST, SEGMENT_PATTERNS[digit2]);
  shiftOut(DATA_PIN, CLOCK_PIN_SR, LSBFIRST, SEGMENT_PATTERNS[digit3]);
  shiftOut(DATA_PIN, CLOCK_PIN_SR, LSBFIRST, SEGMENT_PATTERNS[digit4]);

  digitalWrite(LATCH_PIN, HIGH); // Latch the data to the display
  digitalWrite(LATCH_PIN, LOW);  // Reset the latch
  
  digitalWrite(ENABLE_PIN, HIGH); // Enable the display
}

// Function to start or stop the timer
void startStopTimer() {
  if (!isCounting) {
    startTime = millis();
    isCounting = true;
    isStopped = false;
    digitalWrite(START_STOP_PIN, HIGH); // Turn on start/stop LED
  } else {
    currentTime = millis();
    remainingTime -= currentTime - startTime;
    isCounting = false;
    isStopped = true;
    digitalWrite(START_STOP_PIN, LOW); // Turn off start/stop LED
  }
}

// Function to reset the timer
void resetTimer() {
  remainingTime = 0;
  isCounting = false;
  isStopped = false;
  digitalWrite(START_STOP_PIN, LOW);