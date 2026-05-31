#ifndef TIMER_HAL_H
#define TIMER_HAL_H

#include "esp_timer.h"
#include <stdint.h>

typedef void (*timer_callback_t)(void *arg);

typedef struct
{
    esp_timer_handle_t handle;
    const char *name;
} timer_hal_t;

bool timer_hal_create(timer_hal_t *timer, const char *name, timer_callback_t callback, void *arg);
bool timer_hal_start(timer_hal_t *timer, uint64_t delay_us);
bool timer_hal_stop(timer_hal_t *timer);
bool timer_hal_restart(timer_hal_t *timer, uint64_t delay_us);
void timer_hal_delete(timer_hal_t *timer);

#endif // TIMER_HAL_H