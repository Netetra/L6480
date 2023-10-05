#include <L6480.h>

const int BUSY_PIN = 9;
const int SS_PIN = 10;

L6480 l6480(BUSY_PIN, SS_PIN);

void l6480_config() {}

void setup() {
  l6480.begin();
  l6480.reset_device();
  l6480_config();
  l6480.run(L6480::Dir::CW, 1000);
  delay(3000);
  l6480.run(L6480::Dir::CCW, 1000);
  delay(3000);
  l6480.soft_stop();
}

void loop() {}