/**
 * @file    	main.c
 * @defgroup 	Receiver_Node Receiver Node
 * @ingroup 	Receiver_Node
 * @brief 		CAN Receiver node for the Reversing Radar system.
 *
 * This node receives distance data via CAN and displays it
 * using LEDs and a 7-segment display.
 * 
 */
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
#include "app_tasks.h"
#include "leds.h"
#include "sevenseg.h"
#include <stdio.h>
#include <string.h>

/* Private variables ---------------------------------------------------------*/
CAN_HandleTypeDef hcan;
UART_HandleTypeDef huart2;

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;

/* Definitions for serialTask */
osThreadId_t serialTaskHandle;

/* Definitions for buzzerTask */
osThreadId_t buzzerTaskHandle;

/* Definitions for lcdTask */
osThreadId_t lcdTaskHandle;

/* Buffers and variables for CAN, UART, and display */
float Distance;
int time = 500;
char Buffer[8];
char lcdBuffer[8];
uint8_t RxData[8];
uint32_t TxMailbox;
uint8_t digit1, digit2;

/* CAN Tx/Rx headers */
CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CAN_Init(void);
static void MX_USART2_UART_Init(void);

/**
 * @fn void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
 * @brief  CAN RX FIFO 0 message pending callback.
 *
 * @note This callback is invoked by the HAL when a CAN message is received
 * in FIFO0. The received frame is read and stored in the RX buffer.
 * In case of reception error, an error indicator LED is activated.
 *
 * @param  hcan Pointer to the CAN handle.
 * @retval None
 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK)
    {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET); /* Error indicator */
    }
}

int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
  MX_CAN_Init();
  MX_USART2_UART_Init();
	
	/* Start CAN and activate receive interrupt */
   HAL_CAN_Start(&hcan);
   HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);

  /* Init scheduler */
  osKernelInitialize();

  /* Create FreeRTOS tasks */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &(osThreadAttr_t){.name="defaultTask", .stack_size=128 * 4, .priority=osPriorityNormal});
  serialTaskHandle = osThreadNew(serialTask_init, NULL, &(osThreadAttr_t){.name="serialTask", .stack_size=128 * 4, .priority=osPriorityNormal});
  buzzerTaskHandle = osThreadNew(buzzerTask_init, NULL, &(osThreadAttr_t){.name="buzzerTask", .stack_size=128 * 4, .priority=osPriorityNormal});
  lcdTaskHandle = osThreadNew(lcdTask_init, NULL, &(osThreadAttr_t){.name="lcdTask", .stack_size=128 * 4, .priority=osPriorityNormal});
	
  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  while (1)
  {
	}
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CAN Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN_Init(void)
{

  /* USER CODE BEGIN CAN_Init 0 */

  /* USER CODE END CAN_Init 0 */

  /* USER CODE BEGIN CAN_Init 1 */

  /* USER CODE END CAN_Init 1 */
  hcan.Instance = CAN1;
  hcan.Init.Prescaler = 18;
  hcan.Init.Mode = CAN_MODE_NORMAL;
  hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan.Init.TimeSeg1 = CAN_BS1_2TQ;
  hcan.Init.TimeSeg2 = CAN_BS2_1TQ;
  hcan.Init.TimeTriggeredMode = DISABLE;
  hcan.Init.AutoBusOff = DISABLE;
  hcan.Init.AutoWakeUp = DISABLE;
  hcan.Init.AutoRetransmission = DISABLE;
  hcan.Init.ReceiveFifoLocked = DISABLE;
  hcan.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan) != HAL_OK)
  {
    Error_Handler();
  }
	/* USER CODE BEGIN CAN_Config */
	/** 
	* @brief  Configure CAN filter for receiving messages
	* @details 
	* This block sets up the CAN filter to accept messages from the
	* transmitter node with ID 0x103. Only messages matching this
	* ID will trigger the RX FIFO0 message pending callback.
	*/
    TxHeader.DLC = 2;							
    TxHeader.ExtId = 0;
    TxHeader.IDE = CAN_ID_STD;      /**< Standard CAN frame */
    TxHeader.RTR = CAN_RTR_DATA;    /**< Data frame */
    TxHeader.StdId = 0x104;         /**< STM32F103 transmitter ID */
    TxHeader.TransmitGlobalTime = DISABLE;

    CAN_FilterTypeDef canfilterconfig;
    canfilterconfig.FilterActivation = CAN_FILTER_ENABLE;
    canfilterconfig.FilterBank = 1;                   /**< Filter bank index */
    canfilterconfig.FilterFIFOAssignment = CAN_RX_FIFO0;
    canfilterconfig.FilterIdHigh = 0x103 << 5;        /**< Filter for ID 0x103 */
    canfilterconfig.FilterIdLow = 0x0000;
    canfilterconfig.FilterMaskIdHigh = 0x103 << 5;    /**< Mask for ID 0x103 */
    canfilterconfig.FilterMaskIdLow = 0x0000;
    canfilterconfig.FilterMode = CAN_FILTERMODE_IDMASK;
    canfilterconfig.FilterScale = CAN_FILTERSCALE_32BIT;
    canfilterconfig.SlaveStartFilterBank = 0;         /**< Number of filters for master CAN */

    HAL_CAN_ConfigFilter(&hcan, &canfilterconfig);
    /* USER CODE END CAN_Config */
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, Green1_Pin|Green2_Pin|Blue2_Pin|Red1_Pin
                          |Red2_Pin|Buzzer_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, D_Pin|C_Pin|G_Pin|DP_Pin
                          |DIG2_Pin|A_Pin|Green3_Pin|Blue1_Pin
                          |B_Pin|DIG1_Pin|F_Pin|E_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : Green1_Pin Green2_Pin Blue2_Pin Red1_Pin
                           Red2_Pin Buzzer_Pin */
  GPIO_InitStruct.Pin = Green1_Pin|Green2_Pin|Blue2_Pin|Red1_Pin
                          |Red2_Pin|Buzzer_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : D_Pin C_Pin G_Pin DP_Pin
                           DIG2_Pin A_Pin Green3_Pin Blue1_Pin
                           B_Pin DIG1_Pin F_Pin E_Pin */
  GPIO_InitStruct.Pin = D_Pin|C_Pin|G_Pin|DP_Pin
                          |DIG2_Pin|A_Pin|Green3_Pin|Blue1_Pin
                          |B_Pin|DIG1_Pin|F_Pin|E_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/**
  * @fn HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM2 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM2) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
