/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/timers.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_system.h"
#include "spi_flash_mmap.h"
// #include "esp_spi_flash.h"
#include "input_iot.h"
#include "output_iot.h"

#define BLINK_GPIO 2

#define NUM_TIMERS 2
TimerHandle_t xTimers[NUM_TIMERS];
EventGroupHandle_t xEventGroup;

#define BIT_EVENT_BUTTON_PRESS (1 << 0)
#define BIT_EVENT_UART_RECV (1 << 1)

/* Task to be created. */
void vTask1(void *pvParameters)
{
    for (;;)
    {
        EventBits_t uxBits = xEventGroupWaitBits(
            xEventGroup,                                  /* The event group being tested. */
            BIT_EVENT_BUTTON_PRESS | BIT_EVENT_UART_RECV, /* The bits within the event group to wait for. */
            pdTRUE,                                       /* BIT_0 & BIT_4 should be cleared before returning. */
            pdFALSE,                                      /* Don't wait for both bits, either bit will do. */
            portMAX_DELAY);                               /* Wait a maximum of 100ms for either bit to be set. */

        if (uxBits & BIT_EVENT_BUTTON_PRESS)
        {
            printf("BUTTON PRESS\n");
            output_io_toggle(BLINK_GPIO);
        }
        if (uxBits & BIT_EVENT_UART_RECV)
        {
            printf("UART DATA\n");
        }
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
    int ulCount = (uint32_t)pvTimerGetTimerID(xTimer);
    if (ulCount == 0)
    {
        output_io_toggle(BLINK_GPIO);
    }
    else if (ulCount == 1)
    {
        printf("Hello World\n");
    }
}

void button_callback(int pin)
{
    if (pin == GPIO_NUM_0)
    {
        BaseType_t pxHigherPriorityTaskWoken;
        xEventGroupSetBitsFromISR(xEventGroup, BIT_EVENT_BUTTON_PRESS, &pxHigherPriorityTaskWoken);
    }
}

void app_main(void)
{
    xTimers[0] = xTimerCreate("TimerBlink", pdMS_TO_TICKS(500), pdTRUE, (void *)0, vTimerCallback);
    xTimers[1] = xTimerCreate("TimerPrint", pdMS_TO_TICKS(1000), pdTRUE, (void *)1, vTimerCallback);

    output_io_create(BLINK_GPIO);
    input_io_create(0, HI_TO_LO);
    input_set_callback(button_callback);

    xTimerStart(xTimers[0], 0);
    xTimerStart(xTimers[1], 0);

    xEventGroup = xEventGroupCreate();

    /* Create the task, storing the handle. */
    xTaskCreate(
        vTask1,   /* Function that implements the task. */
        "vTask1", /* Text name for the task. */
        1024 * 2, /* Stack size in words, not bytes. */
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
}