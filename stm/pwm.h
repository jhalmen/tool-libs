/** @file pwm.h
 *
 * Copyright (c) 2020 IACE
 */
#ifndef STM_PWM_H
#define STM_PWM_H

#include "stm/gpio.h"
#include "stm/hal.h"

/**
 * @brief Template class for hardware based PWM outputs
 */
class HardwarePWM {
public:
    /**
     * set pwm in percent
     * @param perc [0..1]
     */
    void pwm(double perc) {
        __HAL_TIM_SET_COMPARE(hTim, channel, perc * period);
    }

    /**
     * Initialize Timer Hardware and PWM pins
     *
     * @param hTim handle of configured HardwareTimer
     * @param iPin pin number of PWM output
     * @param gpioPort pin port of PWM output
     * @param iAlternate alternate function for PWM output
     * @param chan timer channel number (TIM_CHANNEL_x)
     */
    HardwarePWM(TIM_HandleTypeDef *hTim,
                uint32_t iPin, GPIO_TypeDef *gpioPort,
                uint8_t iAlternate, uint32_t chan) :
            hTim(hTim), channel(chan), period(hTim->Init.Period) {
        // pwm config of timer
        while (HAL_TIM_PWM_Init(hTim) != HAL_OK);

        // pin config
        AFIO(iPin, gpioPort, iAlternate);

        // channel config
        TIM_OC_InitTypeDef sConfigOC = {};
        sConfigOC.OCMode = TIM_OCMODE_PWM1;
        sConfigOC.Pulse = 0;
        sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
        sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
        while (HAL_TIM_PWM_ConfigChannel(hTim, &sConfigOC, chan) != HAL_OK);

        __HAL_TIM_SET_COMPARE(hTim, chan, 0);
        while (HAL_TIM_PWM_Start(hTim, chan) != HAL_OK);
    }

private:
    //\cond false
    TIM_HandleTypeDef *hTim = nullptr;
    uint32_t channel = 0;
    uint32_t period = 0;
    //\endcond
};

#endif //STM_PWM_H
