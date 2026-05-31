#include "timer_hal.h"

bool timer_hal_create(timer_hal_t *timer, const char *name, timer_callback_t callback, void *arg)
{
    if (timer == NULL || callback == NULL)
        return false;

    esp_timer_create_args_t args = {
        .callback = callback,
        .arg = arg,
        .name = name,
    };

    timer->name = name;
    return esp_timer_create(&args, &timer->handle) == ESP_OK;
}

bool timer_hal_start(timer_hal_t *timer, uint64_t delay_us)
{
    if (timer == NULL || timer->handle == NULL)
        return false;

    return esp_timer_start_once(timer->handle, delay_us) == ESP_OK;
}

bool timer_hal_stop(timer_hal_t *timer)
{
    if (timer == NULL || timer->handle == NULL)
        return false;

    esp_timer_stop(timer->handle);
    return true;
}

bool timer_hal_restart(timer_hal_t *timer, uint64_t delay_us)
{
    timer_hal_stop(timer);
    return timer_hal_start(timer, delay_us);
}

void timer_hal_delete(timer_hal_t *timer)
{
    if (timer == NULL || timer->handle == NULL)
        return;

    esp_timer_stop(timer->handle);
    esp_timer_delete(timer->handle);
    timer->handle = NULL;
}