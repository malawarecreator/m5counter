#include <Arduino.h>
#include <M5Unified.hpp>
#define LED 2

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
  M5.Display.setTextColor(WHITE);
  M5.Display.setTextSize(5);
  M5.Display.setCursor(10, 40);
  M5.Display.setBrightness(2);
  M5.Display.println("BENWARE");
  M5.Speaker.tone(500);
  delay(3000);
  M5.Speaker.stop();
  M5.Display.clear();
  M5.Display.fillScreen(BLACK);
  M5.Display.setTextSize(3);
  M5.Display.setCursor(0, 0);
  M5.Display.println("HELLO WORLD");
}

void loop() {
  M5.update();

  if (M5.BtnA.wasPressed()) {
    resetTimer();
  } 

  if (Serial.available() > 0) {
    char c = Serial.read();
    if (c == 'u') {
      Serial.println("Recieved: increaseVolume");
      M5.Display.setBrightness(M5.Display.getBrightness() + 1);
    } else if (c == 'd') {
      Serial.println("Recieved: decreaseVolume");
      M5.Display.setBrightness(M5.Display.getBrightness() - 1);
    } else if (c == 'r') {
      Serial.println("Recieved: resetTimer");
      resetTimer();
    }
  }
  if (count_min < 25) {
    M5.Display.clear();
    M5.Display.fillScreen(BLACK);
    M5.Display.setCursor(10, 40);

    M5.Display.printf("%dmin %dsec\n", count_min, count_sec);

    count_sec++;
    if (count_sec >= 60) {
      count_sec = 0;
      count_min++;
    }

    delay(1000);
  } else {
    if (!break_led_flashed) {
      M5.Speaker.tone(300, 200);
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

    if (break_sec == 0) {
      if (break_min == 0) {
        resetTimer();
        break_led_flashed = false;
        digitalWrite(LED, LOW);
      } else {
        break_min--;
        break_sec = 59;
      }
    } else {
      break_sec--;
    }
  }
}
