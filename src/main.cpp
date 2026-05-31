#include <main.h>

// Triac Driver Control Variables.
volatile uint32_t driverOffTime = DRIVER_OFF_RESET_TIME_US;

// Zero Crossing Detection variables
volatile bool zeroCrossingDetected = false;

// Web Server and OTA Variables.
bool adcUsageFlag = true; // Enable/Disable the Usage of ADC.

// Timer for Triac Driver Control
timer_hal_t triacDriverTimer;

void IRAM_ATTR triac_driver_timer_isr(void *arg)
{
    // check if zero crossing is detected.
    if (zeroCrossingDetected == true)
    {
        // Reset the zero Crossing Detected Flag.
        zeroCrossingDetected = false;

        // Turn the Driver ON and Start the Timer for turning it OFF after the calculated ON Time.
        hal_gpio_write(TRIAC_DRIVER_PIN, GPIO_HAL_HIGH);
        timer_hal_restart(&triacDriverTimer, DRIVER_ON_TIME_US);
    }
    else
    {
        // turn off the triac driver pin and stop the timer.
        hal_gpio_write(TRIAC_DRIVER_PIN, GPIO_HAL_LOW);
        timer_hal_stop(&triacDriverTimer);
    }
}
void IRAM_ATTR zero_crossing_isr(void)
{
    // Check if its Fully OFF
    if (driverOffTime >= DRIVER_TIME_MAX_US)
    {
        return;
    }

    // Set the zero Crossing Flag and start the Delay timer for Turning ON.
    zeroCrossingDetected = true;
    timer_hal_restart(&triacDriverTimer, driverOffTime);
}

void setup()
{
    // Start the UART Logs
    Serial.begin(UART_BAUD_RATE);

    // Initialize the GPIO Pins
    hal_gpio_init(ZERO_CROSSING_PIN, GPIO_HAL_MODE_INPUT);
    hal_gpio_init(ADC_SIGNAL_PIN, GPIO_HAL_MODE_INPUT);
    hal_gpio_init(TRIAC_DRIVER_PIN, GPIO_HAL_MODE_OUTPUT);

    // Initialize the ADC Pin
    if (adc_init(ADC_SIGNAL_PIN, ADC_ATTEN_DB_12, ADC_WIDTH_BIT_12) == false)
    {
        // return if ADC initialization fails.
        return;
    }

    // Initialize the HW Timer for Triac Driver Pin control.
    if (timer_hal_create(&triacDriverTimer, "Triac_Driver_Timer", triac_driver_timer_isr, NULL) == false)
    {
        // return if Timer initialization fails.
        return;
    }

    // ToDo: start the web Server and Wifi AP mode.

    // Create the RTOS Task for ADC Reading and Triac Control.
    xTaskCreate(adcTask,           // Task function
                "ADC_Task",        // Name of the task
                ADC_TASK_STACK,    // Stack size in words
                NULL,              // Task input parameter
                ADC_TASK_PRIORITY, // Priority of the task
                NULL               // Task handle
    );

    // Attach the Interrupt for Zero Crossing Detection
    hal_gpio_attach_isr(ZERO_CROSSING_PIN, zero_crossing_isr, GPIO_HAL_ISR_FALLING);
}

// handles the Wifi AP Setup and OTA.
void loop()
{
    while (true)
    {
        // handle the Web-Server and OTA Updates

        vTaskDelay(MAIN_LOOP_DELAY);
    }
}

void adcTask(void *pvParameters)
{
    uint32_t adcValue = 0;
    while (true)
    {
        // Check if Usage of ADC or not.
        if (adcUsageFlag == true)
        {
            // Calculate and update the ADC Value.
            adcValue = adc_read_raw(ADC_SIGNAL_PIN);

            // map the ADC Value to the Triac On/OFF Time.
            driverOffTime = (DRIVER_TIME_MIN_US + (adcValue - DRIVER_ADC_MIN) * (DRIVER_TIME_MAX_US - DRIVER_TIME_MIN_US) / (DRIVER_ADC_MAX - DRIVER_ADC_MIN));
        }
        else
        {
            // Read the value from web server.
        }

        vTaskDelay(ADC_TASK_DELAY);
    }
}