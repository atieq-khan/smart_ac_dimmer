#ifndef DIMMER_H
#define DIMMER_H

#include <nvs_hal.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

// NVS Key for Dimmer Configuration
#define NVS_CONFIG_KEY "dimmerConfig"

// Dimmer Default Settings
#define DIMMER_DEFAULT_STATE DIMMER_OFF
#define DIMMER_DEFAULT_DELAY_US 5000UL // Default Delay in us.
#define DIMMER_DEFAULT_BRIGHTNESS 50UL // Default Brightness in Percentage

#define MIN_DELAY_US 500
#define MAX_DELAY_US 9500

#define TRIG_PULSE_US 200UL

    // state to contol the Triac Driver
    typedef enum
    {
        DIMMER_OFF,         // Dimmer is Off
        DIMMER_DETECTED_ZC, // Dimmer is Detected Zero Crossing
        DIMMER_PULSE_ACTIVE // Dimmer is Generating the Trigger Pulse for Triac Driver
    } dimmer_state_t;

    typedef struct
    {
        volatile dimmer_state_t state; // Current State of the Dimmer.
        volatile uint8_t brightness;   // Brightness in Percentage (0-100)
        volatile uint64_t delayUs;     // Delay in Microseconds for Sending Trigger Pulse
        timer_hal_t pulseTimer;        // Timer Handle for Controlling the Trigger Pulse Timing and delay.

    } dimmer_control_t;

    // Initialize dimmer hardware
    bool dimmer_handler_init(void);

    // Set the Default Settings for the Dimmer
    void dimmer_handler_set_default_settings(dimmer_control_t *pConfig);

    // Read the Saved Settings for Dimmer
    bool dimmer_handler_read_settings(dimmer_control_t *pConfig);

    // Function to Update the Dimmer Brightness
    void dimmer_handler_set_brightness(uint8_t brightness);

#ifdef __cplusplus
}
#endif

#endif