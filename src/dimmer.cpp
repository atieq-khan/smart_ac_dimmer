#include <board.h>
#include <dimmer.h>
#include <gpio_hal.h>
#include <timer_hal.h>

static dimmer_control_t dimmerControl;

// ISR Handler for Zero Crossing Detection
void IRAM_ATTR zero_crossing_isr_handler(void)
{
    // Handle the Brightness Control.
    if (dimmerControl.brightness == 0)
    {
        timer_hal_stop(&dimmerControl.pulseTimer);
        hal_gpio_write(TRIAC_DRIVER_PIN, GPIO_HAL_LOW);
        return; // Dimmer is Off.
    }

    // Start the HW Timmer for Delaying the Trigger Pulse based on the Brightness Setting.
    timer_hal_stop(&dimmerControl.pulseTimer);
    timer_hal_restart(&dimmerControl.pulseTimer, dimmerControl.delayUs);

    // Updated the State of th Dimmer to Detected Zero Crossing.
    dimmerControl.state = DIMMER_DETECTED_ZC;
}

void IRAM_ATTR timer_isr_handler(void *arg)
{
    // Start the Pulse after detecting the Zero Crossing.
    if (dimmerControl.state == DIMMER_DETECTED_ZC)
    {
        timer_hal_stop(&dimmerControl.pulseTimer);
        hal_gpio_write(TRIAC_DRIVER_PIN, GPIO_HAL_HIGH);             // Turn ON the Triac Driver Pin to Start the Trigger Pulse.
        timer_hal_restart(&dimmerControl.pulseTimer, TRIG_PULSE_US); // Restart the Timer for the Duration of Trigger Pulse.
        dimmerControl.state = DIMMER_PULSE_ACTIVE;
    }

    // End the Trigger Pulse after the specified time.
    else if (dimmerControl.state == DIMMER_PULSE_ACTIVE)
    {
        // stop the Timer and Turn OFF the Triac Driver Pin to End the Trigger Pulse.
        timer_hal_stop(&dimmerControl.pulseTimer);
        hal_gpio_write(TRIAC_DRIVER_PIN, GPIO_HAL_LOW);
        dimmerControl.state = DIMMER_OFF;
    }
}

// Function to set the Default Settings for the Dimmer
void dimmer_handler_set_default_settings(dimmer_control_t *pConfig)
{
    pConfig->state = DIMMER_OFF;
    pConfig->delayUs = DIMMER_DEFAULT_DELAY_US;
    pConfig->brightness = DIMMER_DEFAULT_BRIGHTNESS;
    pConfig->pulseTimer = NULL;
}

// Function to Read the Saved Settings for Dimmer
bool dimmer_handler_read_settings(dimmer_control_t *pConfig)
{
    if (nvs_hal_read_blob(NVS_CONFIG_KEY, &dimmerControl, sizeof(dimmer_control_t)) != true)
    {
        dimmer_handler_set_default_settings(pConfig);
        return false;
    }
    return true;
}

// Function to Initialize dimmer hardware
bool dimmer_handler_init(void)
{
    // turn off the Triac Driver Pin.
    hal_gpio_write(TRIAC_DRIVER_PIN, GPIO_HAL_LOW);

    // Create the HW Timer for Controlling the Trigger Pulse Timing.
    if (timer_hal_create(&dimmerControl.pulseTimer, "DimmerPulseTimer", timer_isr_handler, NULL) != true)
    {
        return false;
    }

    // Attached the ISR Handler zero-Crossing detection.
    hal_gpio_attach_isr(ZERO_CROSSING_PIN, zero_crossing_isr_handler, GPIO_HAL_ISR_CHANGE);

    return true;
}

// Function to Set the Brightness of the Dimmer
void dimmer_handler_set_brightness(uint8_t brightness)
{
    if (brightness > 100)
    {
        brightness = 100; // Cap the Brightness to 100%
    }

    // Set the Brightness value.
    dimmerControl.brightness = brightness;

    // Calculate the Delay for Trigger Pulse Based on the Brightness Setting.
    dimmerControl.delayUs = (MAX_DELAY_US - (brightness * (MAX_DELAY_US - MIN_DELAY_US) / 100));
}