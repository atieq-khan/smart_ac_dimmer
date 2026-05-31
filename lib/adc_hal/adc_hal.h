#ifndef ADC_HAL_H
#define ADC_HAL_H

#include <driver/adc.h>
#include <esp_adc_cal.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define DEFAULT_VREF 1100 // fallback if eFuse not burned
#define ADC_SAMPLES 20    // number of samples for multisampling
#define ADC_DELAY_MS 2    // delay in ms between samples

bool adc_init(int8_t adcPin, int8_t adcAtten, int8_t adcWidth);
uint32_t adc_read_raw(int8_t adcPin);
uint32_t adc_read_voltage(int8_t adcPin);

#endif // ADC_HAL_H