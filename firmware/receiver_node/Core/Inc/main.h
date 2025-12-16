/**
 * @file    main.h
 * @ingroup Receiver_Node
 * @brief   Main application header for the CAN receiver node.
 *
 * This header file contains GPIO pin definitions, external peripheral handles,
 * and FreeRTOS task prototypes used by the CAN receiver application.
 */

#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

/* -------------------------------------------------------------------------- */
/* Exported functions                                                         */
/* -------------------------------------------------------------------------- */

/**
 * @brief  Handles unrecoverable application errors.
 * @retval None
 */
void Error_Handler(void);

/* -------------------------------------------------------------------------- */
/* GPIO Pin Definitions                                                       */
/* -------------------------------------------------------------------------- */

/** Green LED 1 */
#define Green1_Pin GPIO_PIN_6
#define Green1_GPIO_Port GPIOA

/** Green LED 2 */
#define Green2_Pin GPIO_PIN_7
#define Green2_GPIO_Port GPIOA

/** Green LED 3 */
#define Green3_Pin GPIO_PIN_14
#define Green3_GPIO_Port GPIOB

/** Blue LED 1 */
#define Blue1_Pin GPIO_PIN_15
#define Blue1_GPIO_Port GPIOB

/** Blue LED 2 */
#define Blue2_Pin GPIO_PIN_8
#define Blue2_GPIO_Port GPIOA

/** Red LED 1 */
#define Red1_Pin GPIO_PIN_9
#define Red1_GPIO_Port GPIOA

/** Red LED 2 */
#define Red2_Pin GPIO_PIN_10
#define Red2_GPIO_Port GPIOA

/** Buzzer output pin */
#define Buzzer_Pin GPIO_PIN_15
#define Buzzer_GPIO_Port GPIOA

/* ---------------------- 7-Segment Display Pins ----------------------------- */

/** Segment A */
#define A_Pin GPIO_PIN_13
#define A_GPIO_Port GPIOB

/** Segment B */
#define B_Pin GPIO_PIN_3
#define B_GPIO_Port GPIOB

/** Segment C */
#define C_Pin GPIO_PIN_1
#define C_GPIO_Port GPIOB

/** Segment D */
#define D_Pin GPIO_PIN_0
#define D_GPIO_Port GPIOB

/** Segment E */
#define E_Pin GPIO_PIN_9
#define E_GPIO_Port GPIOB

/** Segment F */
#define F_Pin GPIO_PIN_8
#define F_GPIO_Port GPIOB

/** Segment G */
#define G_Pin GPIO_PIN_10
#define G_GPIO_Port GPIOB

/** Decimal point */
#define DP_Pin GPIO_PIN_11
#define DP_GPIO_Port GPIOB

/** Digit 1 enable */
#define DIG1_Pin GPIO_PIN_5
#define DIG1_GPIO_Port GPIOB

/** Digit 2 enable */
#define DIG2_Pin GPIO_PIN_12
#define DIG2_GPIO_Port GPIOB

/* -------------------------------------------------------------------------- */
/* External Peripheral Handles                                                 */
/* -------------------------------------------------------------------------- */

/** CAN peripheral handle */
extern CAN_HandleTypeDef hcan;

/** UART2 peripheral handle (debug/serial output) */
extern UART_HandleTypeDef huart2;

/* -------------------------------------------------------------------------- */
/* FreeRTOS Task Prototypes                                                    */
/* -------------------------------------------------------------------------- */

/**
 * @brief Default FreeRTOS task.
 * @param argument Pointer to task parameters (unused)
 */
void StartDefaultTask(void *argument);

/**
 * @brief Serial communication task.
 * @param argument Pointer to task parameters (unused)
 */
void serialTask_init(void *argument);

/**
 * @brief Buzzer control task.
 * @param argument Pointer to task parameters (unused)
 */
void buzzerTask_init(void *argument);

/**
 * @brief LCD / 7-segment display handling task.
 * @param argument Pointer to task parameters (unused)
 */
void lcdTask_init(void *argument);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
