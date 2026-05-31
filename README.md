# AC Dimmer — ESP32-C3

Phase angle controlled AC dimmer using ESP32-C3 and MOC3021 optocoupler triac driver.
Power level is set via a potentiometer (ADC) with web server control planned.

---

## How it works

AC power is controlled via **phase angle control**:

```
Zero Cross → [phase delay] → Triac pulse ON → Triac latches → Zero Cross (auto OFF)
```

- Zero crossing of the AC sine wave is detected via hardware interrupt on GPIO4
- A hardware timer (`esp_timer`) fires after a calculated phase delay
- The MOC3021 triac driver is pulsed HIGH for 500us, latching the triac
- The triac stays ON for the remainder of the half cycle, then turns OFF automatically at the next zero crossing
- Power is controlled by adjusting the phase delay — earlier = more power

---

## Hardware

| Component          | Description                                        |
|--------------------|----------------------------------------------------|
| ESP32-C3 DevKitM-1 | Main microcontroller                               |
| MOC3021            | Optocoupler triac driver (non-zero cross type)     |
| Potentiometer      | Analog input for power level (ADC)                 |
| Zero Cross Circuit | Detects AC zero crossing, outputs falling edge     |

---

## Pin Configuration

| Signal            | GPIO |
|-------------------|------|
| Zero Crossing     | 4    |
| ADC Signal (Pot)  | 0    |
| Triac Driver      | 3    |

---

## Timing (50Hz AC)

| Parameter           | Value      | Description                              |
|---------------------|------------|------------------------------------------|
| Half cycle          | 10,000us   | One half of 50Hz AC sine wave            |
| Min firing delay    | 500us      | Full power — fire early in half cycle    |
| Max firing delay    | 10,000us   | Fully OFF — delay exceeds half cycle     |
| Triac pulse width   | 500us      | MOC3021 trigger pulse for reliable latch |

---

## ADC Mapping

```
ADC Value    →   Phase Delay    →   Power
   0–100     →   10,000us       →   Fully OFF   (dead zone)
  100–3995   →   9500–500us     →   0–100%
 3995–4095   →   500us          →   Full power  (dead zone)
```

---

## Software Architecture

```
setup()
  ├── GPIO init (zero crossing, ADC, triac)
  ├── ADC init + calibration (esp_adc_cal)
  ├── Hardware timer init (esp_timer one-shot)
  ├── RTOS task: adcTask (priority 2)
  └── Zero crossing ISR attached (falling edge)

zero_crossing_isr()          [ISR — IRAM]
  └── If not fully OFF: set flag + start phase delay timer

triac_driver_timer_isr()     [ISR — IRAM]
  ├── First fire: GPIO HIGH + restart timer (pulse width)
  └── Second fire: GPIO LOW + stop timer

adcTask()                    [RTOS Task — 100ms]
  ├── Read ADC (20x multisampled, calibrated)
  ├── Apply dead zones
  └── Map to driverOffTime (phase delay)

loop()
  └── Reserved for WiFi AP + OTA (todo)
```

---

## Project Structure

```
├── src/
│   └── main.cpp
├── include/
│   ├── main.h
│   └── board.h
├── lib/
│   ├── adc_hal/
│   │   ├── adc_hal.cpp
│   │   └── adc_hal.h
│   ├── gpio_hal/
│   │   ├── gpio_hal.cpp
│   │   └── gpio_hal.h
│   └── timer_hal/
│       ├── timer_hal.cpp
│       └── timer_hal.h
├── platformio.ini
└── README.md
```

---

## Build

```ini
[env:esp32-c3]
platform  = espressif32
board     = esp32-c3-devkitm-1
framework = arduino
```

---

## Todo

- [ ] WiFi AP mode
- [ ] Web server for remote power control
- [ ] OTA firmware updates
- [ ] Switch `adcUsageFlag` based on web server connection state
- [ ] Handle Dead Zones (Fully ON/OFF)
