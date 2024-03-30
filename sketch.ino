#define F_LEVEL 0
#define LEVELS 100
int level = F_LEVEL;

#define F_LED 7
#define L_LED 10
int sequence[LEVELS];  // Keeps track of LED sequence.

/*
Max and min interval between blinks when showing LED sequence
to the player. The interval gets progressively closer to MIN_INTER
as the level goes up.
*/
#define MAX_INTER 1000
#define MIN_INTER 250

// Start button, first button and last button ports.
#define S_BTN 2
#define F_BTN 3
#define L_BTN 6

/*
Keeps track wether buttons are pressed or not
  [i][0] = C_READ
  [i][1] = P_READ
*/
#define C_READ 0 // Current value read
#define P_READ 1 // Previous value read
bool btnStates[5][2] = {
  {true, true},  // Start button
  {true, true},  // Blue button
  {true, true},  // Yellow button
  {true, true},  // Red button
  {true, true}   // Green button
};
// Symbolic buttons positions on btnStates
#define S_BTN_POS 0
#define B_BTN_POS 1
#define Y_BTN_POS 2
#define R_BTN_POS 3
#define G_BTN_POS 4

// Analogic port for seeding a random first LED into sequence
#define SEED_PORT 11

// Keeps track wether the game is running (true) or waiting for start button to be pressed (false)
bool game_status = false;

void setup() {
  // Set buttons
  for (int btn = S_BTN; btn <= L_BTN; btn++) {
    pinMode(btn, INPUT_PULLUP);
  }
  // Set LEDs
  for (int led = F_LED; led <= L_LED; led++) {
    pinMode(led, OUTPUT);
  }
  randomSeed(analogRead(SEED_PORT));
}

void loop() {
  while (!game_status) {
    btnStates[S_BTN_POS][C_READ] = digitalRead(S_BTN);
    if (!btnStates[S_BTN_POS][C_READ] && btnStates[S_BTN][P_READ]) {
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
  int btn;
  for (int i = F_LEVEL; i <= level; i++) {
    // Whick button should be pressed based on LED color
    btn = map(sequence[i], F_LED, L_LED, B_BTN_POS, G_BTN_POS);  

    // Se btn não é pressionado
      game_status = false;
      break;
    // Senão
      blinkSingle(sequence[i]);
  }

  if (game_status) {
    level++;
    blinkAll(1);
  } else {
    clearSequence();
    level = F_LEVEL;
    blinkAll(5);
  }
}

/*
Pisca um único LED uma única vez, com o intervalo de tempo entre ligar
e desligar baseado no nível do jogador
*/
void blinkSingle(int led) {
  digitalWrite(led, HIGH);
  delay(map(level, F_LEVEL, LEVELS, MAX_INTER, MIN_INTER));
  digitalWrite(led, LOW);
  delay(map(level, F_LEVEL, LEVELS, MAX_INTER, MIN_INTER));
}

// Todos os LEDs piscam simultaneamente n vezes e em períodos regulares
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

// Adiciona um novo LED aleatório à sequência de LEDs na posição i
void updateSequence(int i) {
  sequence[i] = random(F_LED, L_LED + 1);
}

// Limpa a sequência de LEDs
void clearSequence() {
  for (int i = 0; sequence[i] > 0; i++)
    sequence[i] = 0;
}
