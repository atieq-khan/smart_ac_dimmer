/**
 * @file        gpio_hal.h
 * @author      Atieq
 * @date        2026-05-15
 *
 * @brief       Hardware abstraction layer for GPIO operations.
 *
 * @details
 * This module provides a generic GPIO abstraction layer for:
 *
 * - GPIO initialization
 * - GPIO read/write operations
 * - GPIO toggling
 * - GPIO hold enable/disable
 * - GPIO interrupt attachment and detachment
 * - GPIO validation
 *
 * The module simplifies platform-specific GPIO handling and provides
 * a unified interface for application-level usage.
 *
 * Hardware:
 * - ESP32
 *
 * Dependencies:
 * - Arduino Framework
 * - ESP32 GPIO Driver
 *
 * Change Logs:
 * ----------------------------------------------------------------------------
 * Date         Author      Description
 * ----------------------------------------------------------------------------
 * 2026-05-15   Atieq       Initial creation
 * ----------------------------------------------------------------------------
 */

#ifndef GPIO_HAL_H
#define GPIO_HAL_H

#include <Arduino.h>
#include <stdbool.h>
#include <stdint.h>

typedef enum
{
    GPIO_HAL_MODE_INPUT = 0,
    GPIO_HAL_MODE_OUTPUT,
    GPIO_HAL_MODE_INPUT_PULLUP,
    GPIO_HAL_MODE_INPUT_PULLDOWN
} gpio_hal_mode_t;

typedef enum
{
    GPIO_HAL_LOW = 0,
    GPIO_HAL_HIGH = 1,
} gpio_hal_state_t;

typedef enum
{
    GPIO_HAL_ISR_RISING = 0,
    GPIO_HAL_ISR_FALLING = 1,
    GPIO_HAL_ISR_CHANGE = 2,
    GPIO_HAL_ISR_HIGH = 3,
    GPIO_HAL_ISR_LOW = 4,
} gpio_hal_isr_mode_t;

void hal_gpio_init(int8_t pin, gpio_hal_mode_t mode);

bool hal_gpio_valid_pin(int8_t pin);

void hal_gpio_write(int8_t pin, gpio_hal_state_t level);
uint8_t hal_gpio_read(int8_t pin);
void hal_gpio_toggle(int8_t pin);

void hal_gpio_hold_enable(int8_t pin);
void hal_gpio_hold_disable(int8_t pin);

void hal_gpio_attach_isr(int8_t pin, void (*isrHandler)(void), gpio_hal_isr_mode_t mode);
void hal_gpio_detach_isr(int8_t pin);

#endif