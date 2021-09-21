/*
   Free RTOS Challenge

   Mimic a backlight LED for an LCD Screen

   Echo any Serial Input back to the Serial
   terminal, if an input is received in the
   Serial terminal keep the LED off but it
   the Serial terminal is inactive for more
   than five seconds turn off the LED
*/

// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

// Globals
static TimerHandle_t one_shot_timer = NULL;

const int FLASH_LED = 4;

void turn_off_led(TimerHandle_t xTimer) {
  digitalWrite(FLASH_LED, 0);
}
void SerialEcho(void *parameters) {

  while (1) {
    String echo = Serial.readString();
    if (echo.length() > 0) {
      Serial.println(echo);
      digitalWrite(FLASH_LED, 1); // turn on led
      xTimerStart(one_shot_timer, portMAX_DELAY); // start timer
    }
  }
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(FLASH_LED, OUTPUT);
  digitalWrite(FLASH_LED, 0); // turn off led
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println("\n--- Free RTOS Software Timers Challenge---");

  // create tasks
  xTaskCreatePinnedToCore(SerialEcho,
                          "SerialEcho",
                          1024,
                          NULL,
                          1,
                          NULL,
                          app_cpu);

  // Create a one-shot timer
  one_shot_timer = xTimerCreate(
                     "One-shot timer",           // Name of timer
                     5000 / portTICK_PERIOD_MS,  // Period of timer (in ticks)
                     pdFALSE,                    // Auto-reload,(when the timer expires it should not restart)
                     (void *)0,                  // Timer ID
                     turn_off_led);           // Callback function (function to call when timer has expired

  // Delete "setup and loop" task
  vTaskDelete(NULL);
}

void loop() {
  // put your main code here, to run repeatedly:

}
