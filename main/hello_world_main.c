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

/* Task to be created. */
void vTask1(void *pvParameters)
{
    for (;;)
    {
        printf("Task 1\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void vTask2(void *pvParameters)
{
    for (;;)
    {
        printf("Task 2\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void vTask3(void *pvParameters)
{
    for (;;)
    {
        printf("Task 3\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    /* Create the task, storing the handle. */
    xTaskCreate(
        vTask1,   /* Function that implements the task. */
        "vTask1", /* Text name for the task. */
        2048,     /* Stack size in words, not bytes. */
        NULL,     /* Parameter passed into the task. */
        4,        /* Priority at which the task is created. */
        NULL);    /* Used to pass out the created task's handle. */

    xTaskCreate(
        vTask2,   /* Function that implements the task. */
        "vTask2", /* Text name for the task. */
        2048,     /* Stack size in words, not bytes. */
        NULL,     /* Parameter passed into the task. */
        5,        /* Priority at which the task is created. */
        NULL);    /* Used to pass out the created task's handle. */

    xTaskCreate(
        vTask3,   /* Function that implements the task. */
        "vTask3", /* Text name for the task. */
        2048,     /* Stack size in words, not bytes. */
        NULL,     /* Parameter passed into the task. */
        6,        /* Priority at which the task is created. */
        NULL);    /* Used to pass out the created task's handle. */
}
