/**
 * @file    app_tasks.h
 * @ingroup Transmitter_Node
 * @brief   FreeRTOS task declarations for the transmitter node.
 *
 * This header exposes:
 *   - Prototypes for ultrasonic sensor tasks
 *   - The CAN transmission task
 *   - External CAN-related variables shared with the main program
 *
 * All tasks declared here are created and managed in main.c.
 */
#ifndef __APP_TASKS_H
#define __APP_TASKS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "cmsis_os.h"   /**< FreeRTOS CMSIS-RTOS API */
#include "usensor.h"    /**< Ultrasonic sensor module */

/* ---------------------------------------------------------------------------
 * RTOS Task Prototypes
 * ---------------------------------------------------------------------------*/

/**
 * @brief  Task for first ultrasonic sensor reading.
 * @param  argument: Pointer passed to the task (not used)
 * @retval None
 */
void us1Task_init(void *argument);

/**
 * @brief  Task for second ultrasonic sensor reading.
 * @param  argument: Pointer passed to the task (not used)
 * @retval None
 */
void us2Task_init(void *argument);

/**
 * @brief  Task for transmitting measured distances via CAN.
 * @param  argument: Pointer passed to the task (not used)
 * @retval None
 */
void TxTask_init(void *argument);

/* ---------------------------------------------------------------------------
 * CAN-related external variables
 * ---------------------------------------------------------------------------*/

/** CAN transmit header */
extern CAN_TxHeaderTypeDef TxHeader;

/** CAN filter configuration */
extern CAN_FilterTypeDef canfilterconfig;

/** CAN transmit mailbox index */
extern uint32_t TxMailbox;

/** CAN transmit data buffer (8 bytes) */
extern uint8_t TxData[8];

#ifdef __cplusplus
}
#endif

#endif /* __APP_TASKS_H */
