/**
 * @file    app_tasks.c
 * @ingroup Transmitter_Node
 * @brief   FreeRTOS task implementations for the transmitter node.
 *
 * This file contains the RTOS task loops responsible for:
 *   - Periodic ultrasonic sensor triggering (Sensor 1 and Sensor 2)
 *   - Packaging and transmitting distance measurements through CAN
 *
 * Each task runs independently under FreeRTOS and uses modules provided
 * in usensor.c and the CAN HAL driver.
 */
#include "app_tasks.h"
#include <string.h> // For strlen if UART debug is enabled

/** ---------------------------------------------------------------------------
 * Task: us1Task_init
 * @brief  RTOS thread to periodically read the first ultrasonic sensor.
 * @param  argument: Not used
 * @retval None
 * --------------------------------------------------------------------------- */
void us1Task_init(void *argument)
{
    (void) argument;  /**< Unused parameter */

    for(;;)
    {
        USensor_Read1();   /**< Trigger first sensor measurement */
        osDelay(60);       /**< Wait minimum 60 ms between measurements */
    }
}

/** ---------------------------------------------------------------------------
 * Task: us2Task_init
 * @brief  	RTOS thread to periodically read the second ultrasonic sensor.
 * @param  	argument: Not used
 * @retval 	None
 * --------------------------------------------------------------------------- */
void us2Task_init(void *argument)
{
    (void) argument;  /**< Unused parameter */

    for(;;)
    {
        USensor_Read2();   /**< Trigger second sensor measurement */
        osDelay(60);       /**< Wait minimum 60 ms between measurements */
    }
}

/** ---------------------------------------------------------------------------
 * Task: TxTask_init
 * @brief  RTOS thread to transmit sensor distances via CAN bus.
 * @param  argument: Not used
 * @retval None
 * --------------------------------------------------------------------------- */
void TxTask_init(void *argument)
{
    (void) argument;  /**< Unused parameter */
    //char Buffer[50]; /**< Optional: For UART debug */

    for(;;)
    {
        /**< Fill CAN transmit buffer */
        TxData[0] = Distance_1;
        TxData[1] = Distance_2;

        // Optional: UART debug
        // sprintf(Buffer, "Sensor1: %d, Sensor2: %d\r\n", Distance_1, Distance_2);
        // HAL_UART_Transmit(&huart2, (uint8_t *)Buffer, strlen(Buffer), 10);

        /**< Transmit CAN message */
        if(HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox) != HAL_OK)
        {
            /**< CAN transmission failed, signal with LED (optional) */
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
        }

        osDelay(60);  /**< Wait 60 ms before next transmission */
    }
}
