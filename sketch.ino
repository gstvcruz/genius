#define F_LEVEL 0
#define LEVELS 100
int level = F_LEVEL;

/*
Max and min interval between blinks when showing LED sequence
to the player. The interval gets progressively closer to MIN_INTER
as the level goes up
*/
#define MAX_INTER 750
#define MIN_INTER 250

#define F_LED 7
#define L_LED 10
int sequence[LEVELS];  // Keeps track of LED sequence to be displayed

// Start button, first button and last button ports
#define S_BTN 2
#define F_BTN 3
#define L_BTN 6

/*
Keeps track of buttons states

0 = Start button
1 = Blue button
2 = Yellow button
3 = Red button
4 = Green button
*/
bool btnStates[5][2];
#define C_READ 0 // Current value read: btnStates[i][C_READ]
#define P_READ 1 // Previous value read: btnStates[i][P_READ]

// Positions on btnStates: btnSates[Z_BTN_POS][i]
#define S_BTN_POS 0
#define B_BTN_POS 1
#define Y_BTN_POS 2
#define R_BTN_POS 3
#define G_BTN_POS 4

// Analogic port for seeding random LEDs into sequence
#define SEED_PORT 11

/* 
Keeps track wether the game is running (true)
or waiting for start button to be pressed (false)
*/
bool game_status = false;

void setup() {
  // Set buttons
  for (int btn = S_BTN; btn <= L_BTN; btn++) {
    pinMode(btn, INPUT_PULLUP);
  }
  // Set buttons states
  clearButtonStates();
  // Set LEDs
  for (int led = F_LED; led <= L_LED; led++) {
    pinMode(led, OUTPUT);
  }
  randomSeed(analogRead(SEED_PORT));
}

void loop() {
  // Wait for start button press
  while (!game_status) {
    btnStates[S_BTN_POS][C_READ] = digitalRead(S_BTN);
    if (!btnStates[S_BTN_POS][C_READ] && btnStates[S_BTN_POS][P_READ]) {
      game_status = true;
      updateSequence(0);
    }      
  }

  // Display sequence
  for (int i = F_LEVEL; i < LEVELS; i++) {
    if (sequence[i] == 0) {
      updateSequence(i);
      break;
    }
    blinkSingle(sequence[i]);
  }

  // Player's turn
  for (int i = F_LEVEL; i <= level; i++) {
    // Which button should be pressed based on LED color in sequence
    int btn = map(sequence[i], F_LED, L_LED, F_BTN, L_BTN);

    int j = B_BTN_POS;
    int k = F_BTN;
    // Scan for button presses
    while (true) {
      btnStates[j][C_READ] = digitalRead(k);
      if (!btnStates[j][C_READ] && btnStates[j][P_READ] && k == btn) {
        break;
      } else {
        if (j < G_BTN_POS && k < L_BTN) {
          j++;
          k++;
        } else {
          j = B_BTN_POS;
          k = F_BTN;
        }
      }
    }
  }

  if (game_status) {
    level++;
    blinkAll(1);
    clearButtonStates();
  } else {
    clearSequence();
    clearButtonStates();
    level = F_LEVEL;
    blinkAll(5);
  }
}

// Blink a single LED with the time between on and off based on game level
void blinkSingle(int led) {
  digitalWrite(led, HIGH);
  delay(map(level, F_LEVEL, LEVELS, MAX_INTER, MIN_INTER));
  digitalWrite(led, LOW);
  delay(map(level, F_LEVEL, LEVELS, MAX_INTER, MIN_INTER));
}

// All LEDs blink simultaneously n times
void blinkAll(int times) {
  while (times-- > 0) {
    for (int led = F_LED; led <= L_LED; led++)
      digitalWrite(led, HIGH);
    delay(250);
    for (int led = F_LED; led <= L_LED; led++)
      digitalWrite(led, LOW);
    delay(250);
  }
}

// Add a random LED to sequence in position i
void updateSequence(int i) {
  sequence[i] = random(F_LED, L_LED + 1);
}

// Clear LED sequence
void clearSequence() {
  for (int i = 0; sequence[i] > 0; i++)
    sequence[i] = 0;
}

// Set all button states to currently and previously not pressed (true)
void clearButtonStates() {
  for (int i = 0; i < 5; i++) {
    btnStates[i][C_READ] = true;
    btnStates[i][P_READ] = true;
  }
}
