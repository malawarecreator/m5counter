#include <Arduino.h>
#include <M5Unified.hpp>
#define LED 19

int count_min = 0;
int count_sec = 0;

int break_min = 5;
int break_sec = 0;
bool break_led_flashed = false;

void resetTimer() {
  count_min = 0;
  count_sec = 0;
}

void setup() {
  Serial.begin(115200);

  auto cfg = M5.config();
  cfg.internal_imu = false;
  cfg.internal_rtc = false;
  cfg.output_power = false;
  cfg.internal_spk = true;
  cfg.led_brightness = 3;
  M5.begin(cfg);

  pinMode(LED, OUTPUT);

  M5.Display.setRotation(1);
  M5.Display.fillScreen(BLACK);
  M5.Display.setTextColor(CYAN);
  M5.Display.setTextSize(5);
  M5.Display.setCursor(10, 40);
  M5.Display.setBrightness(2);
  M5.Display.println("BENWARE");
  M5.Speaker.tone(500);
  delay(3000);
  M5.Speaker.stop();
  M5.Display.clear();
  M5.Display.fillScreen(BLACK);
  M5.Display.setTextSize(2);
  M5.Display.setCursor(0, 0);
  M5.Display.println("HELLO WORLD");
  M5.Speaker.setVolume(200);
}

void loop() {
  M5.update();

  if (M5.BtnA.wasPressed()) {
    resetTimer();
  } 


  if (Serial.available() > 0) {
    char c = Serial.read();

    if (c == 'r') {
      Serial.println("Recieved: resetTimer");
      resetTimer();
    } else if (c == 't') {
      Serial.println("Recieved: triggerBreak");
      count_min = 24;
      count_sec = 59;
    }
  }
  if (count_min < 25) {

    if (M5.BtnB.wasPressed()) {
      count_min = 24;
      count_sec = 59;
    }

    M5.Display.clear();
    M5.Display.fillScreen(BLACK);
    M5.Display.setCursor(10, 40);

    M5.Display.printf("Work: %dmin %dsec\n", count_min, count_sec);

    count_sec++;
    if (count_sec >= 60) {
      count_sec = 0;
      count_min++;
    }

    delay(1000);
  } else {

    if (M5.BtnB.wasPressed()) {
      break_sec = 0;
    }
    if (!break_led_flashed) {
      M5.Speaker.tone(600, 700);
      digitalWrite(LED, HIGH);
      break_led_flashed = true;
      break_min = 5;    
      break_sec = 0;
    }

    M5.Display.clear();
    M5.Display.fillScreen(BLACK);
    M5.Display.setCursor(10, 40);
    M5.Display.printf("Break: %dmin %dsec\n", break_min, break_sec);

    delay(1000);

    if (break_min == 4 && break_sec <= 55) {
      digitalWrite(LED, LOW);
    }

    if (break_sec == 0) {
      if (break_min == 0) {
        resetTimer();
        break_led_flashed = false;
      } else {
        break_min--;
        break_sec = 59;
      }
    } else {
      break_sec--;
    }
  }
}
