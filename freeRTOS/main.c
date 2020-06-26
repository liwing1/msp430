/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

TaskHandle_t myTask1Handle = NULL;
TaskHandle_t myTask2Handle = NULL;

SemaphoreHandle_t xMutex;

char mySharedResource[10];


void myTask1(void *p)
{


    while(1)
    {
        if(xSemaphoreTake(xMutex, (TickType_t) 0xFFFFFFFF) == 1)
        {
            char Task1Msg[] = "potatoes";
            uint8_t i;
            for(i=0; i<9; i++){
                mySharedResource[i] = Task1Msg[i];
                vTaskDelay(50);
            }
            mySharedResource[i]=0;

            xSemaphoreGive(xMutex);
        }
        vTaskDelay(100);
    }
}

void myTask2(void *p)
{


    while(1)
    {
        if(xSemaphoreTake(xMutex, (TickType_t) 0xFFFFFFFF) == 1)
        {
            char Task2Msg[] = "tomatoes";
            uint8_t i;
            for(i=0; i<9; i++){
                mySharedResource[i] = Task2Msg[i];
                vTaskDelay(50);
            }
            mySharedResource[i]=0;

            xSemaphoreGive(xMutex);
        }
        vTaskDelay(100);
    }
}


int main( void )
{

    /* Configure system. */
    WDTCTL = WDTPW | WDTHOLD;
    PM5CTL0 &= ~LOCKLPM5;

    xMutex = xSemaphoreCreateMutex();

    xTaskCreate(myTask1, "task1", 200, (void*) 0, tskIDLE_PRIORITY, &myTask1Handle);
    xTaskCreate(myTask2, "task2", 200, (void*) 0, tskIDLE_PRIORITY, &myTask2Handle);

    /* Start RTOS */
    vTaskStartScheduler();

}

