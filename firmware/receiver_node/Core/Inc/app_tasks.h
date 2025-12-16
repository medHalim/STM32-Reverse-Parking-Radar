/**
 * @file    app_tasks.h
 * @ingroup Receiver_Node
 * @brief   FreeRTOS task declarations for the CAN receiver application.
 *
 * This file contains the task handles and task function prototypes
 * used by the receiver node. Each task is created in main.c and
 * executed under the FreeRTOS scheduler.
 */
#ifndef __APP_TASKS_H
#define __APP_TASKS_H

#include "main.h"
#include "cmsis_os.h"

/* --------------------------------------------------------------------------
 * FreeRTOS task handles
 * -------------------------------------------------------------------------- */

/** Handle for the default system task */
extern osThreadId_t defaultTaskHandle;

/** Handle for the UART serial communication task */
extern osThreadId_t serialTaskHandle;

/** Handle for the buzzer control task */
extern osThreadId_t buzzerTaskHandle;

/** Handle for the LCD display task */
extern osThreadId_t lcdTaskHandle;

/* --------------------------------------------------------------------------
 * FreeRTOS task function prototypes
 * -------------------------------------------------------------------------- */

/**
 * @brief Default FreeRTOS task.
 *
 * This task typically runs background or system-level operations.
 *
 * @param argument Pointer passed to the task (not used).
 */
void StartDefaultTask(void *argument);

/**
 * @brief UART serial communication task.
 *
 * Handles serial data transmission and/or reception over UART.
 *
 * @param argument Pointer passed to the task (not used).
 */
void serialTask_init(void *argument);

/**
 * @brief Buzzer control task.
 *
 * Controls the buzzer behavior based on application events.
 *
 * @param argument Pointer passed to the task (not used).
 */
void buzzerTask_init(void *argument);

/**
 * @brief LCD display task.
 *
 * Updates the LCD or display elements based on received CAN data.
 *
 * @param argument Pointer passed to the task (not used).
 */
void lcdTask_init(void *argument);

#endif /* __APP_TASKS_H */
