#include "adc_hal.h"

static esp_adc_cal_characteristics_t adc_chars;

bool adc_init(int8_t adcPin, int8_t adcAtten, int8_t adcWidth)
{
    adc1_channel_t adcChannel = (adc1_channel_t)adcPin;
    adc_atten_t adcAttenuation = (adc_atten_t)adcAtten;
    adc_bits_width_t adcBitWidth = (adc_bits_width_t)adcWidth;

    // check for pin
    if ((adcChannel < ADC1_CHANNEL_0) || (adcChannel >= ADC1_CHANNEL_MAX))
    {
        return false;
    }

    // check for attenuation
    if ((adcAttenuation < ADC_ATTEN_DB_0) || (adcAttenuation >= ADC_ATTEN_MAX))
    {
        return false;
    }

    // check for bit width
    if (adcBitWidth >= ADC_WIDTH_MAX)
    {
        return false;
    }

    adc1_config_channel_atten(adcChannel, adcAttenuation);
    adc1_config_width(adcBitWidth);
    esp_adc_cal_characterize(ADC_UNIT_1, adcAttenuation, adcBitWidth, DEFAULT_VREF, &adc_chars);
    return true;
}

uint32_t adc_read_raw(int8_t adcPin)
{
    adc1_channel_t adcChannel = (adc1_channel_t)adcPin;

    if ((adcChannel < ADC1_CHANNEL_0) || (adcChannel >= ADC1_CHANNEL_MAX))
    {
        return 0;
    }

    uint32_t adcReading = 0;
    for (uint8_t i = 0; i < ADC_SAMPLES; i++)
    {
        adcReading += adc1_get_raw(adcChannel);
        vTaskDelay(ADC_DELAY_MS);
    }

    return adcReading / ADC_SAMPLES;
}

uint32_t adc_read_voltage(int8_t adcPin)
{
    return esp_adc_cal_raw_to_voltage(adc_read_raw(adcPin), &adc_chars);
}