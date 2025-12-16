/**
 * @file		main.c
 * @defgroup	Transmitter_Node Transmitter Node
 * @ingroup 	Transmitter_Node
 * @brief   	CAN Transmitter node for the Reversing Radar system.
 *
 * This node acquires distance information and transmits it
 * over CAN to the receiver node.
 *
 * This file contains the main function for the transmitter node, which:
 *   - Initializes STM32 peripherals (GPIO, TIM, CAN, UART)
 *   - Handles ultrasonic sensor measurements
 *   - Starts FreeRTOS scheduler with tasks for sensor reading and CAN transmission
 * 
 * @note	HAL_TIM_IC_CaptureCallback is forwarded to the ultrasonic sensor module.
 */
#include "main.h"
#include "cmsis_os.h"
#include "app_tasks.h"
#include "usensor.h"

/* ---------------------------------------------------------------------------
 * Private variables
 * ---------------------------------------------------------------------------*/
TIM_HandleTypeDef htim1;  /**< Timer 1 handle */
TIM_HandleTypeDef htim2;  /**< Timer 2 handle */
CAN_HandleTypeDef hcan;   /**< CAN handle */
UART_HandleTypeDef huart2; /**< UART2 handle */

/* RTOS thread handles */
osThreadId_t us1TaskHandle; /**< Ultrasonic sensor 1 task handle */
osThreadId_t us2TaskHandle; /**< Ultrasonic sensor 2 task handle */
osThreadId_t TxTaskHandle;  /**< CAN transmit task handle */

/* CAN transmission variables */
CAN_TxHeaderTypeDef TxHeader; /**< CAN transmit header */
CAN_FilterTypeDef canfilterconfig; /**< CAN filter config */
uint32_t TxMailbox;                 /**< CAN mailbox index */
uint8_t TxData[8];                  /**< CAN transmit buffer */

/* Ultrasonic sensor distances */
uint8_t Distance_1 = 0; /**< Distance measured by sensor 1 */
uint8_t Distance_2 = 0; /**< Distance measured by sensor 2 */

/* ---------------------------------------------------------------------------
 * Function prototypes
 * ---------------------------------------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
static void MX_CAN_Init(void);
static void MX_USART2_UART_Init(void);

/**
 * @brief  Main program entry point.
 * @retval int (not used, FreeRTOS scheduler takes over)
 */
int main(void)
{
    HAL_Init();
    SystemClock_Config();

    /* Initialize peripherals */
    MX_GPIO_Init();
    MX_TIM1_Init();
    MX_TIM2_Init();
    MX_CAN_Init();
    MX_USART2_UART_Init();

    /* Start timers in input capture mode for ultrasonic sensors */
    HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_1);
    HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
		
    /* Start CAN controller */
    HAL_CAN_Start(&hcan);

    /* Initialize FreeRTOS */
    osKernelInitialize();

    /* Create tasks */
    us1TaskHandle = osThreadNew(us1Task_init, NULL, &(osThreadAttr_t){.name="us1Task", .stack_size=512, .priority=osPriorityNormal});
    us2TaskHandle = osThreadNew(us2Task_init, NULL, &(osThreadAttr_t){.name="us2Task", .stack_size=512, .priority=osPriorityNormal});
    TxTaskHandle  = osThreadNew(TxTask_init, NULL,  &(osThreadAttr_t){.name="TxTask",  .stack_size=512, .priority=osPriorityNormal});

    /* Start scheduler */
    osKernelStart();

    while (1) { } /**< Infinite loop: should never reach here */
}

/**
 * @brief  HAL Timer Input Capture callback
 * @param  htim: Pointer to the timer handle
 * @note   Forwarded to ultrasonic sensor module
 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    USensor_TIM_IC_Callback(htim);
}

/**
 * @brief  System Clock Configuration
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
 * @brief  GPIO Initialization Function
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
  HAL_GPIO_WritePin(GPIOA, TRIG1_Pin|TRIG2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : TRIG1_Pin TRIG2_Pin */
  GPIO_InitStruct.Pin = TRIG1_Pin|TRIG2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/**
 * @brief  TIM1 Initialization Function for input capture
 */
static void MX_TIM1_Init(void)
{
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 72-1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 0xffff-1;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_IC_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
 * @brief  TIM2 Initialization Function for input capture
 */
static void MX_TIM2_Init(void)
{
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 72-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 0xFFFF-1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_IC_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
 * @brief  CAN Initialization Function
 */
static void MX_CAN_Init(void)
{
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
	* @brief  Configure CAN transmit header and filters
	* @details 
	* This block sets up the CAN message header for transmitting data
	* and configures the filter to accept messages from the receiver node.
	*/

	/* Configure CAN transmit header */
	TxHeader.DLC = 2;							
	TxHeader.ExtId = 0;
	TxHeader.IDE = CAN_ID_STD;      /**< Standard CAN frame */
	TxHeader.RTR = CAN_RTR_DATA;    /**< Data frame */
	TxHeader.StdId = 0x103;         /**< Transmitter ID */
	TxHeader.TransmitGlobalTime = DISABLE;

	/* Configure CAN filter */
	canfilterconfig.FilterActivation = CAN_FILTER_ENABLE;  
	canfilterconfig.FilterBank = 1;                         /**< Filter bank index */
	canfilterconfig.FilterFIFOAssignment = CAN_RX_FIFO0;
	canfilterconfig.FilterIdHigh = 0x104 << 5;              /**< Accept messages from ID 0x104 */
	canfilterconfig.FilterIdLow = 0x0000;
	canfilterconfig.FilterMaskIdHigh = 0x104 << 5;          /**< Mask to match ID 0x104 */
	canfilterconfig.FilterMaskIdLow = 0x0000;
	canfilterconfig.FilterMode = CAN_FILTERMODE_IDMASK;     /**< ID mask mode */
	canfilterconfig.FilterScale = CAN_FILTERSCALE_32BIT;    /**< 32-bit filter scale */
	canfilterconfig.SlaveStartFilterBank = 0;              /**< Only relevant for dual CAN controllers */

	HAL_CAN_ConfigFilter(&hcan, &canfilterconfig); /**< Apply filter configuration */
	/* USER CODE END CAN_Config */
}

/**
 * @brief  USART2 Initialization Function
 */
static void MX_USART2_UART_Init(void)
{
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
}

/**
 * @brief  Error Handler
 */
void Error_Handler(void)
{
    __disable_irq();
    while (1) {}
}
