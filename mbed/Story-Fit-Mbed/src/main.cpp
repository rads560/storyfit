/*
*   StoryFit
*   EE459 Spring 2021
*   Team 13
*   Radhika Agrawal and Justin Wilford
*/
#include <Arduino.h>
#include "boards.h"

#include "storyfit.h"
uint16_t lastTime = 0;
uint16_t frameTimer = 1000;
uint16_t deltaTime = 0;
int16_t textHeight = 9;

void setup() {
  digitalWrite(STATUS_LED, HIGH);
  Game game = Game();
  game.Run();
}

void loop() {
}