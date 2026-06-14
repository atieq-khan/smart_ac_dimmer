
#ifndef WIFI_SERVICE_H
#define WIFI_SERVICE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "nvs_flash.h"

    bool wifi_handler_ap_init(const char *ssid, const char *password);

#ifdef __cplusplus
}
#endif
#endif // WIFI_SERVICE_H