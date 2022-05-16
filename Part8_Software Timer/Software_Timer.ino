

// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

static const TickType_t dim_delay = 2000 / portTICK_PERIOD_MS;
static const int led_pin = 12;

static TimerHandle_t one_shot_timer = NULL;

//*****************************************************************************
// Callbacks

// Turn off LED when timer expires
void autoDimmerCallback(TimerHandle_t xTimer) {
  digitalWrite(led_pin, LOW);
}

//*****************************************************************************
// Tasks

// Echo things back to serial port, turn on LED when while entering input
void doCLI(void *parameters) {

  char c;

  pinMode(led_pin, OUTPUT);

  while (1) {

    // See if there are things in the input serial buffer
    if (Serial.available() > 0) {
      c = Serial.read();
      Serial.print(c);
      digitalWrite(led_pin, HIGH);

      // Start timer (if timer is already running, this will act as
      // xTimerReset() instead)
      xTimerStart(one_shot_timer, portMAX_DELAY);
    }
  }
}

//*****************************************************************************//

void setup() {

  // Configure Serial
  Serial.begin(9600);

  // Wait a moment to start (so we don't miss Serial output)
  vTaskDelay(3000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("1626489  Software Timer");

  // Create a one-shot timer
  one_shot_timer = xTimerCreate("One-shot timer", dim_delay, pdFALSE,(void *)0, autoDimmerCallback); 

  // Start command line interface (CLI) task
  xTaskCreatePinnedToCore(doCLI, "Do CLI", 1024, NULL,1, NULL, app_cpu);

  // Delete "setup and loop" task
  vTaskDelete(NULL);
}
//************************************************************************************************
void loop() {
 
}
