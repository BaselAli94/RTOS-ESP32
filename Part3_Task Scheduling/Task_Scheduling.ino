#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

// Settings
/*static const uint8_t buf_len = 20;

char buf[buf_len];
bool Done = false;
uint8_t idx = 0; */

const char msg1[] = "task1";
const char msg2[] = "task2";


//task handles
static TaskHandle_t task_1 = NULL;
static TaskHandle_t task_2 = NULL;

int msg1_len = strlen(msg1);
int msg2_len = strlen(msg2);

//initialsing task1
void task1(void *parameters) {

  while (1) {

    Serial.println();
    for (int i = 0; i < msg1_len; i++)
    {
      Serial.print(msg1[i]);
    }
    Serial.println();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

//initialsing task2
void task2(void *par)
{
  while (1) {

    Serial.println();
    for (int i = 0; i<msg2_len; i++)
    {
      Serial.print(msg2[i]);
    }
    Serial.println();
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
  }


void setup() {
  // put your setup code here, to run once:
  Serial.begin(300);
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  xTaskCreatePinnedToCore(task1,"Reading task1",1024,NULL,1,&task_1,app_cpu);
  xTaskCreatePinnedToCore(task2,"Reading task2",1024,NULL,2,&task_2,app_cpu);
}

void loop() {
  for(int i = 0; i<3; i++)
  {
    vTaskSuspend(task_2);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    vTaskResume(task_2);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
if (task_1 != NULL)
{
  vTaskDelete(task_1);
  task_1=NULL;  
}

}
