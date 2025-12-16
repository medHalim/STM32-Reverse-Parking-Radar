/**
 * @file    usensor.h
 * @ingroup Transmitter_Node
 * @brief   Ultrasonic sensor driver interface for STM32.
 *
 * This header provides:
 *   - Definitions of ultrasonic trigger pins
 *   - Access to measured distance variables
 *   - Function prototypes for sensor triggering and IC handling
 *
 * The implementation relies on hardware timers configured in main.c.
 */
#ifndef USENSOR_H
#define USENSOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

/** @brief Ultrasonic sensor 1 trigger pin */
#define USENSOR1_TRIG_PIN   TRIG1_Pin
/** @brief Ultrasonic sensor 2 trigger pin */
#define USENSOR2_TRIG_PIN   TRIG2_Pin
/** @brief GPIO port for ultrasonic sensors */
#define USENSOR_GPIO_PORT   GPIOA

/** @brief Distance measured by sensor 1 in cm */
extern uint8_t Distance_1;
/** @brief Distance measured by sensor 2 in cm */
extern uint8_t Distance_2;

/**
 * @brief Initialize timers and GPIO for ultrasonic sensors
 */
void USensor_Init(void);

/**
 * @brief Trigger the first ultrasonic sensor
 */
void USensor_Read1(void);

/**
 * @brief Trigger the second ultrasonic sensor
 */
void USensor_Read2(void);

/**
 * @brief Delay for a specified number of microseconds
 * @param us Number of microseconds to delay
 */
void USensor_DelayUs(uint16_t us);

/**
 * @brief Callback function for Timer Input Capture events
 * @param htim Pointer to the TIM handle
 */
void USensor_TIM_IC_Callback(TIM_HandleTypeDef *htim);

#ifdef __cplusplus
}
#endif

#endif /* USENSOR_H */
