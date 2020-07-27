// Simple 4-pin fan controller
// Copyright (c) 2020 James Swineson. Licensed under the GLWTS license.

#define GPIO_FAN 16
#define GPIO_LED 5
#define GPIO_BTN 27

#define PWM_FREQUENCY 25000
#define PWM_RESOUTION 8 // bits

#define DEBOUNCE_COOLDOWN_MICROS 10000

#define INITIAL_FAN_PERCENTAGE 100
#define MIN_PERCENTAGE 0
#define MAX_PERCENTAGE 100
#define PERCENTAGE_STEP 10

int percentage = INITIAL_FAN_PERCENTAGE;
volatile int debounce_status = 0; // 0: stable 1: triggering
volatile unsigned long debounce_trigger_time;

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
    
    percentage += PERCENTAGE_STEP;
    if (percentage > MAX_PERCENTAGE) percentage = MIN_PERCENTAGE;
    
    int dutyCycle = 255 * percentage / 100;
    if (dutyCycle > 255) dutyCycle = 255;
    if (dutyCycle < 0) dutyCycle = 0;
    
    ledcWrite(0, dutyCycle);
    ledcWrite(1, 255 - dutyCycle);  // the built-in LED is pulled up
  }
  
  delay(10);
}
