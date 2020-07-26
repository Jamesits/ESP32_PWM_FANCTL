// Simple 4-pin fan controller
// Copyright (c) 2020 James Swineson. Licensed under the GLWTS license.

#define GPIO_FAN 16
#define GPIO_LED 5
#define GPIO_BTN 27

#define PWM_FREQUENCY 25000
#define PWM_RESOUTION 8 // bits

#define DEBOUNCE_COOLDOWN_MICROS 10000

#define INITIAL_FAN_PERCENTAGE 100 // percent, step 10

volatile int percentage = INITIAL_FAN_PERCENTAGE;
volatile int debounce_status = 0; // 0: stable 1: triggering
unsigned long debounce_trigger_time;

void IRAM_ATTR btn_down() {
  debounce_trigger_time = micros();
  debounce_status = 1;
}

void setup()
{
  pinMode(GPIO_FAN, OUTPUT);
  pinMode(GPIO_LED, OUTPUT);
  ledcSetup(0, PWM_FREQUENCY, PWM_RESOUTION);
  ledcSetup(1, PWM_FREQUENCY, PWM_RESOUTION);
  ledcAttachPin(GPIO_FAN, 0);
  ledcAttachPin(GPIO_LED, 1);

  pinMode(GPIO_BTN, INPUT_PULLUP);
  attachInterrupt(GPIO_BTN, btn_down, FALLING);
}

void loop() 
{
  if (debounce_status == 1 && micros() - debounce_trigger_time > DEBOUNCE_COOLDOWN_MICROS) {
    debounce_status = 0;
    percentage += 10;
    if (percentage > 100) percentage = 0;
    int dutyCycle = 255 * percentage / 100;
    ledcWrite(0, dutyCycle);
    ledcWrite(1, 255-dutyCycle);
  }
  
  delay(10);
}
