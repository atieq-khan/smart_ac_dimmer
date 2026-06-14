

#include "wifi_service.h"

// Wi-Fi Event Handler
static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT)
    {
        switch (event_id)
        {
        case WIFI_EVENT_AP_STACONNECTED:
            ESP_LOGI("WiFi Event", "Station Connected");
            break;
        case WIFI_EVENT_AP_STADISCONNECTED:
            ESP_LOGI("WiFi Event", "Station Disconnected");
            break;
        default:
            break;
        }
    }
}

bool wifi_handler_ap_init(const char *ssid, const char *password)
{
    // Initialize the TCP/IP stack and the default event loop.
    esp_netif_init();
    esp_event_loop_create_default();

    // Create the default Wi-Fi AP network interface.
    esp_netif_create_default_wifi_ap();

    // Initialize the Wi-Fi driver with default configuration.
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    if (esp_wifi_init(&cfg) != ESP_OK)
    {
        return false;
    }

    // Attach the Wi-Fi Driver Callback handler for Wi-Fi events.
    esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, NULL);

    // Configure the Wi-Fi AP settings.
    wifi_config_t wifi_config = {
        .ap =
            {
                .ssid = ssid,
                .password = password,
                .ssid_len = strlen(ssid),
                .channel = 1,
                .authmode = WIFI_AUTH_WPA_WPA2_PSK,
                .max_connection = 3,

            },
    };

    // Set the Wi-Fi mode to AP and apply the configuration.
    esp_wifi_set_mode(WIFI_MODE_AP);
    esp_wifi_set_config(WIFI_IF_AP, &wifi_config);

    // Start the Wi-Fi driver.
    esp_wifi_start();

    return true;
}