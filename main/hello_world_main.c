/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "spi_flash_mmap.h"

#define NUM_TIMERS 2
TimerHandle_t xTimers[NUM_TIMERS];

/* Task to be created. */
void vTask1(void *pvParameters)
{
    for (;;)
    {
        printf("Task 1\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

// void vTask2(void *pvParameters)
// {
//     for (;;)
//     {
//         printf("Task 2\n");
//         vTaskDelay(1000 / portTICK_PERIOD_MS);
//     }
// }

void vTimerCallback(TimerHandle_t xTimer)
{
    /* Optionally do something if the pxTimer parameter is NULL. */
    configASSERT(xTimer);
    ulCount = (uint32_t)pvTimerGetTimerID(xTimer);
    if (ulCount == 0)
    {
    }
    else if (ulCount == 1)
    {
        /* code */
    }
}

void app_main(void)
{
    xTimers[0] = xTimerCreate("TimerBlink", pdMS_TO_TICKS(500), pdTRUE, (void *)0, vTimerCallback);
    xTimers[1] = xTimerCreate("TimerPrint", pdMS_TO_TICKS(1000), pdTRUE, (void *)1, vTimerCallback);

    /* Create the task, storing the handle. */
    xTaskCreate(
        vTask1,   /* Function that implements the task. */
        "vTask1", /* Text name for the task. */
        2048,     /* Stack size in words, not bytes. */
        NULL,     /* Parameter passed into the task. */
        4,        /* Priority at which the task is created. */
        NULL);    /* Used to pass out the created task's handle. */

    // xTaskCreate(
    //     vTask2,   /* Function that implements the task. */
    //     "vTask2", /* Text name for the task. */
    //     2048,     /* Stack size in words, not bytes. */
    //     NULL,     /* Parameter passed into the task. */
    //     5,        /* Priority at which the task is created. */
    //     NULL);    /* Used to pass out the created task's handle. */
