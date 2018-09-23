#pragma once
#include "Arduino.h"

static unsigned long m_flash_period = 0;
static unsigned long m_last_flash = 0;
static bool m_led_is_on_when = false;
static int8_t m_led_pin = -1;

#define LED_PIN_CHECK()                                                                                                \
    if(m_led_pin == -1)                                                                                                \
        return;

inline void DebugLedFlash(long half_period)
{
    LED_PIN_CHECK();
    m_flash_period = half_period;
}

inline void DebugLedToggle()
{
    LED_PIN_CHECK();
    digitalWrite(m_led_pin, !digitalRead(m_led_pin));
}

inline void DebugLedState(bool state)
{
    LED_PIN_CHECK();
    if(state)
    {
        m_flash_period = 0;
        digitalWrite(m_led_pin, m_led_is_on_when);
    }
    else
    {
        m_flash_period = 0;
        digitalWrite(m_led_pin, !m_led_is_on_when);
    }
}

inline void DebugLedLoop()
{
    LED_PIN_CHECK();
    if(m_flash_period > 0 && millis() - m_last_flash > m_flash_period)
    {
        DebugLedToggle();
        m_last_flash = millis();
    }
}

inline void DebugLedPinSet(int8_t led_pin)
{
    m_led_pin = led_pin;
    if(m_led_pin != -1)
    {
        pinMode(m_led_pin, OUTPUT);
    }
}