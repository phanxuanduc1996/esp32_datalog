/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "input_iot.h"
#include "output_iot.h"

#define BLINK_GPIO CONFIG_BLINK_GPIO

void input_event_callback(int pin)
{
    if (pin == GPIO_NUM_0)
    {
        static int x = 0;
        gpio_set_level(BLINK_GPIO, x);
        x = 1 - x;
    }
}

void output_event_callback(int pin)
{
    if (pin == GPIO_NUM_0)
    {
        output_io_toggle(BLINK_GPIO);
    }
}

void app_main(void)
{
    // ------- INPUT -------------
    // esp_rom_gpio_pad_select_gpio(BLINK_GPIO);
    // /* Set the GPIO as a push/pull output */
    // gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

    // input_io_create(GPIO_NUM_0, HI_TO_LO);
    // input_set_callback(input_event_callback);

    // -------- OUTPUT -------
    output_io_create(BLINK_GPIO);
    input_io_create(GPIO_NUM_0, HI_TO_LO);
    input_set_callback(output_event_callback);
}
