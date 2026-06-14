#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <board.h>

#include <dimmer.h>
#include <wifi_service.h>

#include <adc_hal.h>
#include <gpio_hal.h>
#include <timer_hal.h>

#define UART_BAUD_RATE 115200

// Triac Driver Time MAcros
#define DRIVER_OFF_RESET_TIME_US 9500UL
#define DRIVER_ON_TIME_US 100UL

// Triac Driver Range Macros
#define DRIVER_TIME_MIN_US 0UL
#define DRIVER_TIME_MAX_US 10000UL
#define DRIVER_ADC_MIN 0UL
#define DRIVER_ADC_MAX 4095UL

// RTOS Tasks MAcros
#define MAIN_LOOP_DELAY pdMS_TO_TICKS(100)

#define ADC_TASK_STACK 2048
#define ADC_TASK_PRIORITY 2
#define ADC_TASK_DELAY pdMS_TO_TICKS(100)
void adcTask(void *pvParameters);

#endif // MAIN_H