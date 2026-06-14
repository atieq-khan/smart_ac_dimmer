
#include "nvs_hal.h"

bool nvs_hal_init(void)
{
    esp_err_t err = nvs_flash_init();

    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    return err == ESP_OK;
}

bool nvs_hal_read_blob(const char *key, void *out, uint32_t size)
{
    nvs_handle_t handle;

    if (nvs_open(NVS_NAMESPACE, NVS_READONLY, &handle) != ESP_OK)
    {
        return false;
    }

    size_t stored_size = size;
    esp_err_t err = nvs_get_blob(handle, key, out, &stored_size);
    nvs_close(handle);

    return (err == ESP_OK && stored_size == size);
}
