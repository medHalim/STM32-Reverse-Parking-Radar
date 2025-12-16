/**
 * @file    app_tasks.c
 * @ingroup Receiver_Node
 * @brief   FreeRTOS task implementations for the CAN receiver node.
 *
 * This file contains all FreeRTOS task functions used by the receiver node.
 * Tasks handle LED indication, UART output, buzzer control, and 7-segment
 * display updates based on received CAN distance data.
 */
#include "app_tasks.h"
#include "sevenseg.h"
#include "leds.h"
#include <stdio.h>
#include <string.h>

/* --------------------------------------------------------------------------
 * External variables imported from main.c
 * -------------------------------------------------------------------------- */

/** Distance value computed from received CAN data (meters) */
extern float Distance;

/** Buzzer timing variable (ms) */
extern int time;

/** CAN received data buffer */
extern uint8_t RxData[8];

/* --------------------------------------------------------------------------
 * 7-segment display variables
 * -------------------------------------------------------------------------- */

/** First displayed digit */
extern uint8_t digit1;

/** Second displayed digit */
extern uint8_t digit2;

/* --------------------------------------------------------------------------
 * UART and display buffers
 * -------------------------------------------------------------------------- */

/** UART transmission buffer */
extern char Buffer[8];

/** LCD/7-segment buffer */
extern char lcdBuffer[8];

/* --------------------------------------------------------------------------
 * GPIO macros for 7-segment multiplexing
 * -------------------------------------------------------------------------- */
#define DIG1_HIGH() HAL_GPIO_WritePin(GPIOB, DIG1_Pin, GPIO_PIN_SET)
#define DIG1_LOW()  HAL_GPIO_WritePin(GPIOB, DIG1_Pin, GPIO_PIN_RESET)
#define DIG2_HIGH() HAL_GPIO_WritePin(GPIOB, DIG2_Pin, GPIO_PIN_SET)
#define DIG2_LOW()  HAL_GPIO_WritePin(GPIOB, DIG2_Pin, GPIO_PIN_RESET)
#define DP_ON()     HAL_GPIO_WritePin(GPIOB, DP_Pin, GPIO_PIN_SET)
#define DP_OFF()    HAL_GPIO_WritePin(GPIOB, DP_Pin, GPIO_PIN_RESET)

/* --------------------------------------------------------------------------
 * FreeRTOS Tasks
 * -------------------------------------------------------------------------- */

/**
 * @brief Default task handling LED indication logic.
 *
 * This task selects the shortest distance received via CAN and updates
 * LED patterns accordingly. It also updates the buzzer timing variable.
 *
 * @param argument Pointer passed to the task (not used).
 */
void StartDefaultTask(void *argument)
{
    (void)argument;

    for (;;)
    {
        /* Select shortest distance from CAN data */
        if (RxData[0] < RxData[1])
            Distance = RxData[0] / 100.0f;
        else
            Distance = RxData[1] / 100.0f;

        /* Update LEDs based on distance */
        if (Distance <= 0.3f) leds_7();
        else if (Distance <= 0.5f) { leds_6(); time = 50; }
        else if (Distance <= 0.7f) { leds_5(); time = 100; }
        else if (Distance <= 0.9f) { leds_4(); time = 300; }
        else if (Distance <= 1.1f) { leds_3(); time = 400; }
        else if (Distance <= 1.3f) { leds_2(); time = 600; }
        else leds_1();

        osDelay(1);
    }
}

/**
 * @brief UART serial output task.
 *
 * Periodically transmits the measured distance value via UART
 * for debugging or monitoring purposes.
 *
 * @param argument Pointer passed to the task (not used).
 */
void serialTask_init(void *argument)
{
    (void)argument;

    for (;;)
    {
        sprintf(Buffer, "%.1f\r\n", Distance);
        HAL_UART_Transmit(&huart2, (uint8_t *)Buffer, strlen(Buffer), 10);
        osDelay(60);
    }
}

/**
 * @brief Buzzer control task.
 *
 * Controls the buzzer behavior based on the measured distance.
 * The buzzer toggles faster as the distance decreases.
 *
 * @param argument Pointer passed to the task (not used).
 */
void buzzerTask_init(void *argument)
{
    (void)argument;

    osDelay(200);

    for (;;)
    {
        if (Distance <= 0.3f)
        {
            HAL_GPIO_WritePin(GPIOA, Buzzer_Pin, GPIO_PIN_SET);
        }
        else if (Distance <= 1.3f)
        {
            HAL_GPIO_TogglePin(GPIOA, Buzzer_Pin);
            osDelay(time);
        }
        else
        {
            HAL_GPIO_WritePin(GPIOA, Buzzer_Pin, GPIO_PIN_RESET);
        }

        osDelay(1);
    }
}

/**
 * @brief 7-segment display task.
 *
 * Displays the shortest received distance on a multiplexed 2-digit
 * 7-segment display. If the distance exceeds the maximum threshold,
 * a warning pattern is shown.
 *
 * @param argument Pointer passed to the task (not used).
 */
void lcdTask_init(void *argument)
{
    (void)argument;

    for (;;)
    {
        /* Extract digits from shortest distance */
        if (RxData[0] < RxData[1])
        {
            digit1 = ((RxData[0] / 100) % 10);
            digit2 = ((RxData[0] / 10) % 10);
        }
        else
        {
            digit1 = ((RxData[1] / 100) % 10);
            digit2 = ((RxData[1] / 10) % 10);
        }

        if (Distance <= 1.3f)
        {
            SevenSegment_Update(digit1);
            DIG1_LOW();
            HAL_Delay(7);
            DIG1_HIGH();

            SevenSegment_Update(digit2);
            DP_OFF();
            DIG2_LOW();
            HAL_Delay(7);
            DP_ON();
            DIG2_HIGH();
        }
        else
        {
            /* Display warning pattern when distance is too large */
            SevenSegment_Update(0x08);
            DIG1_LOW(); DP_OFF(); HAL_Delay(7); DIG1_HIGH();

            SevenSegment_Update(0x08);
            DIG2_LOW(); HAL_Delay(7); DIG2_HIGH();
        }
    }
}
