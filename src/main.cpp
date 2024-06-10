#include <Arduino.h>
#include <FastLED.h>

#define LED_STRIPS 4

#define MIN_FADE_SPEED 10
#define MAX_FADE_SPEED (3 * MIN_FADE_SPEED)
#define MIN_FADE_LOWER_BOUND 0
#define MAX_FADE_LOWER_BOUND 0 // 80
#define MIN_FADE_UPPER_BOUND 255 //180
#define MAX_FADE_UPPER_BOUND 255
#define LED_WRITE_FREQUENCY_MS 50
#define RANDOMIZE_PARAMETERS_FREQUENCY_MS 5000

boolean debug = true;

byte led_values[LED_STRIPS];
int led_pins[LED_STRIPS] = {3,  5, 6, 9};
long fade_speed[LED_STRIPS];
int led_value_signs[LED_STRIPS]; // = {1,1,1,1};
long led_max_fade[LED_STRIPS];
long led_min_fade[LED_STRIPS];


void randomize_parameters() {
  for (int i = 0; i < LED_STRIPS; i++) {
//    fade_speed[i] = random(MIN_FADE_SPEED, MAX_FADE_SPEED);
//    led_max_fade[i] = random(MIN_FADE_UPPER_BOUND, MAX_FADE_UPPER_BOUND);
//    led_min_fade[i] = random(MIN_FADE_LOWER_BOUND, MAX_FADE_LOWER_BOUND);
    fade_speed[i] = 5;
    led_max_fade[i] = 255;
    led_min_fade[i] = 0;
  }
}

void print_state() {
  if (!debug) {
    return;
  }
  for (int i = 0; i < LED_STRIPS; i++) {
    Serial.print(led_values[i]);
    Serial.print('\t');
  }
  Serial.println();
}

void update_values() {
  print_state();
  for (int i = 0; i < LED_STRIPS; i++) {
    analogWrite(led_pins[i], led_values[i]);
    if (led_values[i] + fade_speed[i] >= led_max_fade[i]) {
      led_value_signs[i] = -1;
    }
    else if (led_values[i] - fade_speed[i] <= led_min_fade[i]){
      led_value_signs[i] = 1;
    }
    led_values[i] += (led_value_signs[i] * fade_speed[i]);
  }
}

void setup() {
  Serial.begin(9600);
  randomize_parameters();

  for (int i = 0; i < LED_STRIPS; i++) {
    pinMode(led_pins[i], OUTPUT);
    led_values[i] = random(0, 255);
    led_value_signs[i] = 1;
  }
  update_values();
}

void loop() {
  EVERY_N_MILLISECONDS(RANDOMIZE_PARAMETERS_FREQUENCY_MS) {
    randomize_parameters();
  }
  EVERY_N_MILLISECONDS(LED_WRITE_FREQUENCY_MS) {
    update_values();
  }
}