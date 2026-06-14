#ifndef NVS_HAL_H
#define NVS_HAL_H

#include "esp_err.h"
#include "nvs.h"
#include "nvs_flash.h"
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    bool nvs_hal_init(void);
    bool nvs_hal_read_blob(const char *key, void *out, uint32_t size);

#ifdef __cplusplus
}
#endif