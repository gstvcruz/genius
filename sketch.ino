#define RANDOM_SEED_PORT 11

/*
Max and min interval between blinks when showing LED sequence
to the player. The interval gets progressively closer to MIN_INTER
as the level goes up.
*/
#define MAX_INTER 750
#define MIN_INTER 250

#define FIRST_LVL 0
#define LEVELS 100
int level = FIRST_LVL;

#define FIRST_LED 7
#define LAST_LED 10
int LEDSequence[LEVELS];

#define START_BTN 2
#define FIRST_BTN 3
#define LAST_BTN 6

/*
Keeps track of whether buttons are pressed or not

0 = Start button
1 = Blue button
2 = Yellow button
3 = Red button
4 = Green button
*/
bool buttonsState[5];

bool gameIsRunning = false;

void setup() {
  for (int btn = START_BTN; btn <= LAST_BTN; btn++)
    pinMode(btn, INPUT_PULLUP);
  clearButtonsState();
  for (int led = FIRST_LED; led <= LAST_LED; led++)
    pinMode(led, OUTPUT);
  randomSeed(analogRead(RANDOM_SEED_PORT));
}

void loop() {
  // Wait for start button press
  while (!gameIsRunning) {
    if (buttonsState[0] = digitalRead(START_BTN)) continue;
    gameIsRunning = true;
    updateLEDSequence(0);
  }

  // Display LED sequence
  for (int i = FIRST_LVL; i < LEVELS; i++) {
    if (LEDSequence[i] == 0) {
      updateLEDSequence(i);
      break;
    }
    blinkSingle(LEDSequence[i]);
  }
  
  int i = FIRST_LVL;
  int j = 0;
  int k = START_BTN;
  int btn = updateButtonToBePressed(i);
  // Scan for button presses on player's turn
  while (gameIsRunning) {
    buttonsState[j] = digitalRead(k);
    // If button being read was pressed
    if (!buttonsState[j]) {
      // It is the right button
      if (k == btn) {
        blinkSingle(LEDSequence[i]);
        if (i < level) {
          btn = updateButtonToBePressed(++i);
          clearButtonsState();
        } else break;
      } 
      // It is the wrong button
      else {
        gameIsRunning = false;
        break;
      }
    }

    // No button was pressed
    if (j < 4 && k < LAST_BTN) {
      j++;
      k++;
    } else {
      j = 0;
      k = START_BTN;
    }
  }

  clearButtonsState();
  if (gameIsRunning) {
    level++;
    blinkAll(1);
  } else {
    clearLEDSequence();
    level = FIRST_LVL;
    blinkAll(5);
  }
}

// Blink a single LED with the time between on and off based on game level
void blinkSingle(int led) {
  digitalWrite(led, HIGH);
  delay(map(level, FIRST_LVL, LEVELS, MAX_INTER, MIN_INTER));
  digitalWrite(led, LOW);
  delay(map(level, FIRST_LVL, LEVELS, MAX_INTER, MIN_INTER));
}

// All LEDs blink simultaneously n times
void blinkAll(int times) {
  while (times-- > 0) {
    for (int led = FIRST_LED; led <= LAST_LED; led++)
      digitalWrite(led, HIGH);
    delay(MIN_INTER);
    for (int led = FIRST_LED; led <= LAST_LED; led++)
      digitalWrite(led, LOW);
    delay(MIN_INTER);
  }
}

// Add a random LED to LEDSequence in position i
void updateLEDSequence(int i) {
  LEDSequence[i] = random(FIRST_LED, LAST_LED + 1);
}

// Determines which button should be pressed based on sequence's LED color
int updateButtonToBePressed(int i) {
  return map(LEDSequence[i], FIRST_LED, LAST_LED, FIRST_BTN, LAST_BTN);
}

// Clear LED sequence
void clearLEDSequence() {
  for (int i = FIRST_LVL; LEDSequence[i] != 0; i++)
    LEDSequence[i] = 0;
}

// Set all buttons to not pressed
void clearButtonsState() {
  for (int btn = 0; btn < 4; btn++)
    buttonsState[btn] = true;
}
