#include <main.h>

static bool adcInitialized = false;

void setup()
{
    // 1. Serial init
    Serial.begin(UART_BAUD_RATE);

    // 2. Read Saved Settings from NVS
    nvs_hal_init();
    if (dimmer_handler_read_settings(&dimmerControl) != true)
    {
        Serial.println("Main : Dimmer Config -> Reading Failed");
        dimmer_handler_set_default_settings(&dimmerControl);
    }
    else
    {
        Serial.println("Main : Dimmer Config -> Reading Success");
    }

    // 3. Init the Dimmer GPIOs
    hal_gpio_init(ZERO_CROSSING_PIN, GPIO_HAL_MODE_INPUT); // For zero crossing detection
    hal_gpio_init(TRIAC_DRIVER_PIN, GPIO_HAL_MODE_OUTPUT); // For controlling the triac driver circuit

    // 4. Init the ADC.
    adcInitialized = adc_init(ADC_SIGNAL_PIN, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12);

    // 5. Init the Dimmer
    if (dimmer_handler_init() != true)
    {
        Serial.println("Main : Dimmer Init -> Failed");
    }
    else
    {
        Serial.println("Main : Dimmer Init -> Success");
    }

    // Start the Wi-Fi AP
    wifi_handler_ap_init(WIFI_SSID, WIFI_PASSWORD);

    // Create the RTOS Tasks
    xTaskCreate(adcTask, "ADC Task", 2048, NULL, 1, NULL);
}

void loop()
{
    // handle the Web server.
    while (true)
    {
    }
}

void adcTask(void *pvParameters)
{
    uint32_t adcValue;

    while (true)
    {
        // If ADC is not initialized, skip the Reading.
        if (adcInitialized == true)
        {
            // Read the ADC Value and Convert it to Percentage (0-100)
            adcValue = adc_read_raw(ADC_SIGNAL_PIN);
            adcValue = (adcValue * 100) / 4095;
        }

        // Update the Brightness of Dimmer.
        dimmer_handler_set_brightness(adcValue);

        vTaskDelay(ADC_TASK_DELAY);
    }
}