
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif
static const int led_pin = 12;
// Global
static SemaphoreHandle_t mutex;

//*********************************************************
// The Blinking Task

void blinkLED(void *parameters) {
  int num = *(int *)parameters;

  // Releasing the mutex 
  xSemaphoreGive(mutex);
     
  Serial.print("Received: ");
  Serial.println(num);
  
  while (1) {
  pinMode(led_pin, OUTPUT);
    digitalWrite(led_pin, HIGH);
    vTaskDelay(num / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(num / portTICK_PERIOD_MS);
  }
}

//*****************************************************************************

void setup() {

  long int delay_arg;

  // Configure Serial
  Serial.begin(115200);

  // Wait a moment to start (so we don't miss Serial output)
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("Welcome to Part6 Mutex");
   vTaskDelay(2000 / portTICK_PERIOD_MS);
  Serial.println("********************");
  Serial.println("Enter a number for delay (milliseconds)");

  // Wait for input from Serial
  while (Serial.available() <= 0);

  // Read integer value
  delay_arg = Serial.parseInt();
  Serial.print("Sending: ");
  Serial.println(delay_arg);
  
  // Creating mutex 
  mutex = xSemaphoreCreateMutex();

  // Taking the mutex
  xSemaphoreTake(mutex, portMAX_DELAY);

  // Starting the task 
  xTaskCreatePinnedToCore(blinkLED,"Blink LED",1024,(void *)&delay_arg, 1,NULL,app_cpu);

  // Do nothing until mutex has been returned (maximum delay)
  xSemaphoreTake(mutex, portMAX_DELAY);

 
  Serial.println("It Is Done!");
}

//*********************************************************************
void loop() {
  

  vTaskDelay(1000 / portTICK_PERIOD_MS);
}
