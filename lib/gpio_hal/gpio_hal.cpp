/**
 * @file        gpio_hal.c
 * @author      Atieq
 * @date        2026-05-15
 *
 * @brief       GPIO hardware abstraction layer implementation.
 *
 * @details
 * This source file implements generic GPIO handling functions for:
 *
 * - GPIO initialization
 * - GPIO input/output control
 * - GPIO state toggling
 * - GPIO hold control
 * - GPIO interrupt management
 *
 * The module provides a simplified abstraction over the Arduino and
 * ESP32 GPIO APIs for improved portability and maintainability.
 *
 * Hardware:
 * - ESP32
 *
 * Change Logs:
 * ----------------------------------------------------------------------------
 * Date         Author      Description
 * ----------------------------------------------------------------------------
 * 2026-05-15   Atieq       Initial creation
 * ----------------------------------------------------------------------------
 */
#include <gpio_hal.h>

/**
 * @brief      Initializes a GPIO pin.
 *
 * @details
 * Configures the specified GPIO pin with the selected mode.
 *
 * @param[in]  pin
 *             GPIO pin number.
 *
 * @param[in]  mode
 *             GPIO operating mode.
 */
void hal_gpio_init(int8_t pin, gpio_hal_mode_t mode)
{
    gpio_num_t pinNumber = (gpio_num_t)pin;
    if (!GPIO_IS_VALID_GPIO(pinNumber))
    {
        return;
    }

    switch (mode)
    {
    case GPIO_HAL_MODE_INPUT:
    {
        pinMode((uint8_t)pin, INPUT);
        break;
    }

    case GPIO_HAL_MODE_OUTPUT:
    {
        pinMode((uint8_t)pin, OUTPUT);
        break;
    }

    default:
    {
        break;
    }
    }

    return;
}

/**
 * @brief      Writes a logic level to a GPIO pin.
 *
 * @details
 * Sets the specified GPIO pin to either HIGH or LOW state.
 *
 * @param[in]  pin
 *             GPIO pin number.
 *
 * @param[in]  level
 *             Output logic level.
 */
void hal_gpio_write(int8_t pin, gpio_hal_state_t level)
{
    gpio_num_t pinNumber = (gpio_num_t)pin;
    if (!GPIO_IS_VALID_GPIO(pinNumber))
    {
        return;
    }

    // write the pin
    if (level == GPIO_HAL_HIGH)
    {
        digitalWrite(pin, HIGH);
    }
    else
    {
        digitalWrite(pin, LOW);
    }

    return;
}

/**
 * @brief      Reads the logic state of a GPIO pin.
 *
 * @details
 * Reads and returns the current logic level present on the GPIO pin.
 *
 * @param[in]  pin
 *             GPIO pin number.
 *
 * @return     GPIO logic level.
 */
uint8_t hal_gpio_read(int8_t pin)
{
    gpio_num_t pinNumber = (gpio_num_t)pin;
    if (!GPIO_IS_VALID_GPIO(pinNumber))
    {
        return 0;
    }

    // read the pin
    return (uint8_t)digitalRead(pin);
}

/**
 * @brief      Toggles the logic state of a GPIO pin.
 *
 * @details
 * Changes the GPIO output state:
 *
 * - HIGH -> LOW
 * - LOW  -> HIGH
 *
 * @param[in]  pin
 *             GPIO pin number.
 */
void hal_gpio_toggle(int8_t pin)
{
    gpio_num_t pinNumber = (gpio_num_t)pin;
    if (!GPIO_IS_VALID_GPIO(pinNumber))
    {
        return;
    }

    // toggle the pin.
    digitalWrite(pin, !(digitalRead(pin)));
    return;
}

/**
 * @brief      Disables GPIO hold functionality.
 *
 * @details
 * Disables the hold feature for the specified GPIO pin.
 *
 * @param[in]  pin
 *             GPIO pin number.
 */
void hal_gpio_hold_disable(int8_t pin)
{
    gpio_num_t pinNumber = (gpio_num_t)pin;
    if (!GPIO_IS_VALID_GPIO(pinNumber))
    {
        return;
    }
    gpio_hold_dis(pinNumber);
}

/**
 * @brief      Enables GPIO hold functionality.
 *
 * @details
 * Enables the hold feature for the specified GPIO pin,
 * preserving the pin state during deep sleep or reset conditions.
 *
 * @param[in]  pin
 *             GPIO pin number.
 */
void hal_gpio_hold_enable(int8_t pin)
{
    gpio_num_t pinNumber = (gpio_num_t)pin;
    if (!GPIO_IS_VALID_GPIO(pinNumber))
    {
        return;
    }

    gpio_hold_en(pinNumber);
}

/**
 * @brief      Validates whether a GPIO pin is usable.
 *
 * @details
 * Checks whether the specified GPIO pin is valid for the target hardware.
 *
 * @param[in]  pin
 *             GPIO pin number.
 *
 * @return     true  GPIO pin is valid
 * @return     false GPIO pin is invalid
 */
bool hal_gpio_valid_pin(int8_t pin)
{
    gpio_num_t pinNumber = (gpio_num_t)pin;
    if (!GPIO_IS_VALID_GPIO(pinNumber))
    {
        return false;
    }

    return true;
}

/**
 * @brief      Attaches an interrupt service routine to a GPIO pin.
 *
 * @details
 * Configures a GPIO interrupt and associates it with the provided ISR
 * callback function.
 *
 * Supported interrupt modes:
 * - Rising edge
 * - Falling edge
 * - Change
 * - High level
 * - Low level
 *
 * @param[in]  pin
 *             GPIO pin number.
 *
 * @param[in]  isrHandler
 *             Pointer to the ISR callback function.
 *
 * @param[in]  mode
 *             GPIO interrupt trigger mode.
 */
void hal_gpio_attach_isr(int8_t pin, void (*isrHandler)(void), gpio_hal_isr_mode_t mode)
{
    gpio_num_t pinNumber = (gpio_num_t)pin;
    if (!GPIO_IS_VALID_GPIO(pinNumber))
    {
        return;
    }

    switch (mode)
    {
    case GPIO_HAL_ISR_RISING:
    {
        attachInterrupt((uint8_t)pin, isrHandler, RISING);
        break;
    }
    case GPIO_HAL_ISR_FALLING:
    {
        attachInterrupt((uint8_t)pin, isrHandler, FALLING);
        break;
    }
    case GPIO_HAL_ISR_CHANGE:
    {
        attachInterrupt((uint8_t)pin, isrHandler, CHANGE);
        break;
    }
    case GPIO_HAL_ISR_HIGH:
    {
        attachInterrupt((uint8_t)pin, isrHandler, HIGH);
        break;
    }
    case GPIO_HAL_ISR_LOW:
    {
        attachInterrupt((uint8_t)pin, isrHandler, LOW);
        break;
    }
    default:
    {
        break;
    }
    }
}

/**
 * @brief      Detaches the interrupt from a GPIO pin.
 *
 * @details
 * Removes the interrupt service routine associated with the GPIO pin.
 *
 * @param[in]  pin
 *             GPIO pin number.
 */
void hal_gpio_detach_isr(int8_t pin)
{
    gpio_num_t pinNumber = (gpio_num_t)pin;
    if (!GPIO_IS_VALID_GPIO(pinNumber))
    {
        return;
    }

    detachInterrupt((uint8_t)pin);
}