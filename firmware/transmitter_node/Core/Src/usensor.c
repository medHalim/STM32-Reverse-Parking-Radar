/**
 * @file    usensor.c
 * @ingroup Transmitter_Node
 * @brief   Ultrasonic sensor measurement driver using STM32 timers.
 *
 * This file implements:
 *   - Trigger generation for two ultrasonic sensors
 *   - Microsecond delay using a hardware timer
 *   - Input Capture processing for echo pulse measurement
 *
 * The module converts echo pulse duration into a distance in centimeters
 * and updates the global variables Distance_1 and Distance_2.
 */
#include "usensor.h"

/** @brief Internal variable for input capture of sensor 1 */
static uint32_t IC1_Val1 = 0;
/** @brief Internal variable for input capture of sensor 1 (second reading) */
static uint32_t IC1_Val2 = 0;
/** @brief Flag to indicate first capture of sensor 1 */
static uint8_t IC1_FirstCaptured = 0;
/** @brief Time difference captured for sensor 1 */
static uint32_t IC1_Diff = 0;

/** @brief Internal variable for input capture of sensor 2 */
static uint32_t IC2_Val1 = 0;
/** @brief Internal variable for input capture of sensor 2 (second reading) */
static uint32_t IC2_Val2 = 0;
/** @brief Flag to indicate first capture of sensor 2 */
static uint8_t IC2_FirstCaptured = 0;
/** @brief Time difference captured for sensor 2 */
static uint32_t IC2_Diff = 0;

/**
 * @brief Microsecond delay using TIM1
 * @param us Number of microseconds to delay
 */
void USensor_DelayUs(uint16_t us)
{
    __HAL_TIM_SET_COUNTER(&htim1, 0);
    while (__HAL_TIM_GET_COUNTER(&htim1) < us);
}

/**
 * @brief Trigger the first ultrasonic sensor
 */
void USensor_Read1(void)
{
    HAL_GPIO_WritePin(USENSOR_GPIO_PORT, USENSOR1_TRIG_PIN, GPIO_PIN_SET);
    USensor_DelayUs(10);  /**< 10us trigger pulse */
    HAL_GPIO_WritePin(USENSOR_GPIO_PORT, USENSOR1_TRIG_PIN, GPIO_PIN_RESET);
    __HAL_TIM_ENABLE_IT(&htim1, TIM_IT_CC1);
}

/**
 * @brief Trigger the second ultrasonic sensor
 */
void USensor_Read2(void)
{
    HAL_GPIO_WritePin(USENSOR_GPIO_PORT, USENSOR2_TRIG_PIN, GPIO_PIN_SET);
    USensor_DelayUs(10);
    HAL_GPIO_WritePin(USENSOR_GPIO_PORT, USENSOR2_TRIG_PIN, GPIO_PIN_RESET);
    __HAL_TIM_ENABLE_IT(&htim2, TIM_IT_CC1);
}

/**
 * @brief Input capture callback called from HAL_TIM_IC_CaptureCallback
 * @param htim Pointer to the TIM handle
 *
 * Measures the pulse width for both ultrasonic sensors and calculates distance.
 */
void USensor_TIM_IC_Callback(TIM_HandleTypeDef *htim)
{
    /* Sensor 1 */
    if (htim->Instance == htim1.Instance && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
    {
        if (IC1_FirstCaptured == 0)
        {
            IC1_Val1 = HAL_TIM_ReadCapturedValue(&htim1, TIM_CHANNEL_1);
            IC1_FirstCaptured = 1;
            __HAL_TIM_SET_CAPTUREPOLARITY(&htim1, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
        }
        else
        {
            IC1_Val2 = HAL_TIM_ReadCapturedValue(&htim1, TIM_CHANNEL_1);
            __HAL_TIM_SET_COUNTER(&htim1, 0);
            IC1_Diff = (IC1_Val2 > IC1_Val1) ? (IC1_Val2 - IC1_Val1) : ((0xFFFF - IC1_Val1) + IC1_Val2);
            Distance_1 = (uint8_t)(IC1_Diff * 0.034 / 2);
            IC1_FirstCaptured = 0;
            __HAL_TIM_SET_CAPTUREPOLARITY(&htim1, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
            __HAL_TIM_DISABLE_IT(&htim1, TIM_IT_CC1);
        }
    }

    /* Sensor 2 */
    if (htim->Instance == htim2.Instance && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
    {
        if (IC2_FirstCaptured == 0)
        {
            IC2_Val1 = HAL_TIM_ReadCapturedValue(&htim2, TIM_CHANNEL_1);
            IC2_FirstCaptured = 1;
            __HAL_TIM_SET_CAPTUREPOLARITY(&htim2, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
        }
        else
        {
            IC2_Val2 = HAL_TIM_ReadCapturedValue(&htim2, TIM_CHANNEL_1);
            __HAL_TIM_SET_COUNTER(&htim2, 0);
            IC2_Diff = (IC2_Val2 > IC2_Val1) ? (IC2_Val2 - IC2_Val1) : ((0xFFFF - IC2_Val1) + IC2_Val2);
            Distance_2 = (uint8_t)(IC2_Diff * 0.034 / 2);
            IC2_FirstCaptured = 0;
            __HAL_TIM_SET_CAPTUREPOLARITY(&htim2, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
            __HAL_TIM_DISABLE_IT(&htim2, TIM_IT_CC1);
        }
    }
}
