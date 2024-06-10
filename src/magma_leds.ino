const int first_beat_brightness = 80;
const int second_beat_brightness = 255;
const int delay_between_beats_ms = 100; //30;
const int fade_delay1_ms = 2; //10;
const int fade_delay2_ms = 4; //17;
const int fade_steps = 120; //10;
const int first_beat_fade_delta = 10;
const int second_beat_brightness_fade_delta = 10;
const int delay_after_beat = 600; //300;
const int random_speed_delta = 20;



const int pins[] = {5, 6};
const int num_pins = 2;

int speed_factor = 100;

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < num_pins; i++){
    pinMode(pins[i], OUTPUT);
  }
}

void update_speed() {
  if (Serial.available()) {
    Serial.println("pre update");
    int newValue = Serial.parseInt();
    if (newValue > 0 && newValue < 400){
      speed_factor = newValue;
      Serial.print("updated speed: ");
      Serial.println(speed_factor);
    }
    Serial.println("post update");
  }
}

void write_val(int val) {
  for (int i = 0; i < num_pins; i++){
    analogWrite(pins[i], val);
  }
}

void beat(int max_brightness, long fade_delay, long post_delay_ms, long random_speed_factor) {
  int val;
  int step;

  fade_delay = (fade_delay * random_speed_factor) / speed_factor;
  post_delay_ms = (post_delay_ms * random_speed_factor) / speed_factor;

  for (int i = 0; i <= 2 * fade_steps; i++) {
    step = i < fade_steps ? i : (2 * fade_steps) - i;
    val = map(step, 0, fade_steps, 0, max_brightness);
    write_val(val);
    delay(fade_delay);
  }
  delay(post_delay_ms);
}

void loop() {
  int random_speed_factor = random(100 - random_speed_delta, 100 + random_speed_delta); 
  update_speed();
  beat(first_beat_brightness, fade_delay1_ms, delay_between_beats_ms, random_speed_factor);
  beat(second_beat_brightness, fade_delay2_ms, delay_after_beat, random_speed_factor);
}
