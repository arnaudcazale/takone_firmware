/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "motor.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim15;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
#define BUFFSIZE 1
uint8_t tx1[BUFFSIZE] ;
uint8_t rx1[BUFFSIZE] ;
uint8_t tx2[BUFFSIZE] ;
uint8_t rx2[BUFFSIZE] ;
uint8_t tx3[BUFFSIZE] ;
uint8_t rx3[BUFFSIZE] ;
params gps ;
uint8_t demoStep = 100 ;
uint32_t time_start = 0 ;
uint32_t time_start_asserv = 0 ;
uint32_t gAdcValue = 0 ;
uint32_t SumAdc = 0 ;
uint32_t MoyAdc = 0 ;
uint8_t cpt = 0 ;
uint8_t channel = 0;
uint32_t adc[8], buffer[8];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM15_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_ADC1_Init(void);
/* USER CODE BEGIN PFP */
uint32_t getUs(void) ;
uint32_t tNbTour = 0 ;
void delayUs(uint16_t micros) ;

void initMotors(params* p) ;
void initVCaptors(params* p) ;
void initXCaptors(params* p) ;
void initUARTS(params* p) ;

void demoRun(uint32_t time_us) ;

void handle_Message(uint8_t* buf, uint8_t origin) ;

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM3_Init();
  MX_TIM1_Init();
  MX_TIM15_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_IT(&huart1, rx1, BUFFSIZE ) ;
  HAL_UART_Receive_IT(&huart2, rx2, BUFFSIZE ) ;
  HAL_UART_Receive_IT(&huart3, rx3, BUFFSIZE ) ;
  initMotors(&(gps));
  initVCaptors(&(gps)) ;
  HL_initGroups(&(gps)) ;
  initXCaptors(&(gps)) ;
  initUARTS(&(gps)) ;

  /*__HAL_TIM_SET_COMPARE(&(gps.motors[0].htim), 0, ((0)*MOTPERIOD)/100);
  __HAL_TIM_SET_COMPARE(&(gps.motors[1].htim), 1, ((0)*MOTPERIOD)/100);
  __HAL_TIM_SET_COMPARE(&(gps.motors[2].htim), 2, ((0)*MOTPERIOD)/100);
  __HAL_TIM_SET_COMPARE(&(gps.motors[3].htim), 3, ((0)*MOTPERIOD)/100);
  __HAL_TIM_SET_COMPARE(&(gps.motors[4].htim), 4, ((0)*MOTPERIOD)/100);
  __HAL_TIM_SET_COMPARE(&(gps.motors[5].htim), 5, ((0)*MOTPERIOD)/100);
  __HAL_TIM_SET_COMPARE(&(gps.motors[6].htim), 6, ((0)*MOTPERIOD)/100);
  __HAL_TIM_SET_COMPARE(&(gps.motors[7].htim), 7, ((0)*MOTPERIOD)/100);*/

  /*HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, SET);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, SET);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, SET);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, SET);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, SET);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, SET);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, SET);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, SET);*/

  for(int i=0; i<20; i++){
	  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_2);
	  HAL_Delay(50);
  }

  HAL_Delay(4000) ;

//  HORIZONTAL
//   demoStep = 200 ;

//  demoStep = 4 ;
//  demoStep = 19 ;
//  demoStep = 70 ;//GET FROM UP
//  demoStep = 31 ; // GO DOWN
//  demoStep = 24 ; // RESET VERTICAL
//  demoStep = 30 ; //GO LEFT
//  demoStep = 100 ; // STOP
//  demoStep = 28 ; //GO RIGHT
//  demoStep = 26 ; // GO UP
//  demoStep = 72 ; // GO BACKWARD
//  demoStep = 90 ; //GO FORWARD
//  demoStep = 84 ; // TEST COM
//  demoStep = 80 ; //DEMO HORI
//  demoStep = 74 ; //READY FOR TAKE OFF
//  demoStep = 100 ; // NOTHING
//  demoStep = 92 ; //GO LEFT DEMO
//demoStep = 1 ; // DEMO COMPLETE

demoStep = 130 ; //DEMO POUR ARNAUD CAZALE

  uint8_t group1[] = {4,6} ;
  addGroup(&(gps), GROUP_TYPE_MOTOR, 0, group1, 2) ;
  uint8_t group4[] = {5,7} ;
  addGroup(&(gps), GROUP_TYPE_MOTOR, 2, group4, 2) ;
  uint8_t group5[] = {0,1,2,3} ;
  addGroup(&(gps), GROUP_TYPE_CAPTOR, 4, group5, 4) ;
//  uint8_t group6[] = {0} ;
//  addGroup(&(gps), GROUP_TYPE_MOTOR, 5, group6, 1) ;
//  uint8_t group7[] = {6} ;
//  addGroup(&(gps), GROUP_TYPE_MOTOR, 6, group7, 1) ;
//  uint8_t group8[] = {7} ;
//  addGroup(&(gps), GROUP_TYPE_MOTOR, 7, group8, 1) ;
	time_start = getUs() ;
	time_start_asserv = getUs() ;

	startMeasure(&(gps.vcs[0]));
	startMeasure(&(gps.vcs[1]));
	startMeasure(&(gps.vcs[2]));
	startMeasure(&(gps.vcs[3]));
	startMeasure(&(gps.vcs[4]));
	startMeasure(&(gps.vcs[5]));
	startMeasure(&(gps.vcs[6]));
	startMeasure(&(gps.vcs[7]));

	HAL_TIM_Base_Start(&htim1)	;
	HAL_TIM_Base_Start(&htim15) ;
	HAL_TIM_Base_Start(&htim3) ;
	HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);

	//HAL_ADC_Start(&hadc1);
	HAL_ADC_Start_DMA(&hadc1, buffer, 8); // start adc in DMA mode

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      uint32_t time_us = getUs() ;

      HL_update(&gps, time_us) ;
	  demoRun(time_us) ;
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage 
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the peripherals clocks 
  */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_USART2
                              |RCC_PERIPHCLK_ADC;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_SYSCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_AnalogWDGConfTypeDef AnalogWDGConfig = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.LowPowerAutoPowerOff = DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.NbrOfConversion = 8;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.SamplingTimeCommon1 = ADC_SAMPLETIME_160CYCLES_5;
  hadc1.Init.SamplingTimeCommon2 = ADC_SAMPLETIME_160CYCLES_5;
  hadc1.Init.OversamplingMode = DISABLE;
  hadc1.Init.TriggerFrequencyMode = ADC_TRIGGER_FREQ_HIGH;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analog WatchDog 2 
  */
  AnalogWDGConfig.WatchdogMode = ADC_ANALOGWATCHDOG_SINGLE_REG;
  if (HAL_ADC_AnalogWDGConfig(&hadc1, &AnalogWDGConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel 
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  sConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel 
  */
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = ADC_REGULAR_RANK_3;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel 
  */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = ADC_REGULAR_RANK_4;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel 
  */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = ADC_REGULAR_RANK_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel 
  */
  sConfig.Channel = ADC_CHANNEL_5;
  sConfig.Rank = ADC_REGULAR_RANK_6;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel 
  */
  sConfig.Channel = ADC_CHANNEL_6;
  sConfig.Rank = ADC_REGULAR_RANK_7;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel 
  */
  sConfig.Channel = ADC_CHANNEL_7;
  sConfig.Rank = ADC_REGULAR_RANK_8;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */
  /*ADC_ChannelConfTypeDef adcChannel;
  adcChannel.Channel = ADC_CHANNEL_0;
  adcChannel.Rank = 2;

  if (HAL_ADC_ConfigChannel(&hadc1, &adcChannel) != HAL_OK)
  {
    Error_Handler();
  }*/
  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 3;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 500;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.BreakFilter = 0;
  sBreakDeadTimeConfig.BreakAFMode = TIM_BREAK_AFMODE_INPUT;
  sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
  sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
  sBreakDeadTimeConfig.Break2Filter = 0;
  sBreakDeadTimeConfig.Break2AFMode = TIM_BREAK_AFMODE_INPUT;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 3;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 500;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief TIM15 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM15_Init(void)
{

  /* USER CODE BEGIN TIM15_Init 0 */

  /* USER CODE END TIM15_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM15_Init 1 */

  /* USER CODE END TIM15_Init 1 */
  htim15.Instance = TIM15;
  htim15.Init.Prescaler = 4;
  htim15.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim15.Init.Period = 500;
  htim15.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim15.Init.RepetitionCounter = 0;
  htim15.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim15) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim15, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim15, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim15, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.BreakFilter = 0;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim15, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM15_Init 2 */

  /* USER CODE END TIM15_Init 2 */
  HAL_TIM_MspPostInit(&htim15);

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_TXINVERT_INIT;
  huart1.AdvancedInit.TxPinLevelInvert = UART_ADVFEATURE_TXINV_ENABLE;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

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
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_TXINVERT_INIT;
  huart2.AdvancedInit.TxPinLevelInvert = UART_ADVFEATURE_TXINV_ENABLE;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_TXINVERT_INIT;
  huart3.AdvancedInit.TxPinLevelInvert = UART_ADVFEATURE_TXINV_ENABLE;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

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
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12|GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2|GPIO_PIN_10|GPIO_PIN_13|GPIO_PIN_3 
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
                          |GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC12 PC8 PC9 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PC13 PC14 PC15 PC4 
                           PC5 */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_4 
                          |GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PF0 PF1 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB2 PB10 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB13 PB3 PB4 PB5 
                           PB8 PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5 
                          |GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PA12 PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PD0 PD1 PD2 PD3 
                           PD4 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
                          |GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

//____________________________________________INITIALISATION METHODS_______________________________________________________

void initXCaptors(params* p)
{
	ExtCaptor x1 ;
	x1.id = 0 ;
	x1.inputPinF = GPIOA ;//TODO
	x1.inputPinN = 1 ;//TODO
	standardInitExtCaptor(&x1) ;
	p->xcs[0] = x1 ;

	ExtCaptor x2 ;
	x2.id = 1 ;
	x2.inputPinF = GPIOA ;//TODO
	x2.inputPinN = 1 ;//TODO
	standardInitExtCaptor(&x2) ;
	p->xcs[1] = x2 ;
}

void initMotors(params* p){

	Motor m1 ;
	m1.id = 0 ;
	m1.pinFamTim = GPIOA ;
	m1.pinNumTim = GPIO_PIN_11 ;
	m1.htim = htim1 ;
	m1.channel = TIM_CHANNEL_4;
	m1.pinFam1 = GPIOC ;
	m1.pinNum1 = GPIO_PIN_12 ;
	m1.pinFam2 = GPIOB ;
	m1.pinNum2 = GPIO_PIN_9 ;
	p->motors[0] = m1 ;
	standardInitMotor(&(p->motors[0]));
	p->motors[0].trans.type = TYPE_VERT ;

	Motor m2 ;
	m2.id = 1 ;
	m2.pinFamTim = GPIOA ;
	m2.pinNumTim = GPIO_PIN_10 ;
	m2.htim = htim1 ;
	m2.channel = TIM_CHANNEL_3 ;
	m2.pinFam1 = GPIOB ;
	m2.pinNum1 = GPIO_PIN_8 ;
	m2.pinFam2 = GPIOB ;
	m2.pinNum2 = GPIO_PIN_5 ;
	p->motors[1] = m2 ;
	standardInitMotor(&(p->motors[1]));
	p->motors[1].trans.type = TYPE_VERT ;

	Motor m3 ;
	m3.id = 2 ;
	m3.pinFamTim = GPIOC ;
	m3.pinNumTim = GPIO_PIN_7 ;
	m3.htim = htim3 ;
	m3.channel = TIM_CHANNEL_2 ;
	m3.pinFam1 = GPIOB ;
	m3.pinNum1 = GPIO_PIN_4 ;
	m3.pinFam2 = GPIOB ;
	m3.pinNum2 = GPIO_PIN_3 ;
	p->motors[2] = m3 ;
	standardInitMotor(&(p->motors[2]));
	p->motors[2].trans.type = TYPE_VERT ;

	Motor m4 ;
	m4.id = 3 ;
	m4.pinFamTim = GPIOC ;
	m4.pinNumTim = GPIO_PIN_6 ;
	m4.htim = htim3 ;
	m4.channel = TIM_CHANNEL_1 ;
	m4.pinFam1 = GPIOD ;
	m4.pinNum1 = GPIO_PIN_4 ;
	m4.pinFam2 = GPIOD ;
	m4.pinNum2 = GPIO_PIN_3 ;
	p->motors[3] = m4 ;
	standardInitMotor(&(p->motors[3]));
	p->motors[3].trans.type = TYPE_VERT ;

	Motor m5 ;
	m5.id = 4 ;
	m5.pinFamTim = GPIOA ;
	m5.pinNumTim = GPIO_PIN_9 ;
	m5.htim = htim1 ;
	m5.channel = TIM_CHANNEL_2 ;
	m5.pinFam1 = GPIOD ;
	m5.pinNum1 = GPIO_PIN_2 ;
	m5.pinFam2 = GPIOD ;
	m5.pinNum2 = GPIO_PIN_1 ;
	p->motors[4] = m5 ;
	standardInitMotor(&(p->motors[4]));
	p->motors[4].trans.type = TYPE_HORI ;

	Motor m6 ;
	m6.id = 5 ;
	m6.pinFamTim = GPIOA ;
	m6.pinNumTim = GPIO_PIN_8 ;
	m6.htim = htim1 ;
	m6.channel = TIM_CHANNEL_1 ;
	m6.pinFam1 = GPIOD ;
	m6.pinNum1 = GPIO_PIN_0 ;
	m6.pinFam2 = GPIOC ;
	m6.pinNum2 = GPIO_PIN_9 ;
	p->motors[5] = m6 ;
	standardInitMotor(&(p->motors[5]));
	p->motors[5].trans.type = TYPE_HORI ;

	Motor m7 ;
	m7.id = 6 ;
	m7.pinFamTim = GPIOB ;
	m7.pinNumTim = GPIO_PIN_15 ;
	m7.htim = htim15 ;
	m7.channel = TIM_CHANNEL_2 ;
	m7.pinFam1 = GPIOC ;
	m7.pinNum1 = GPIO_PIN_8 ;
	m7.pinFam2 = GPIOA ;
	m7.pinNum2 = GPIO_PIN_15 ;
	p->motors[6] = m7 ;
	standardInitMotor(&(p->motors[6]));
	p->motors[6].trans.type = TYPE_HORI ;

	Motor m8 ;
	m8.id = 7 ;
	m8.pinFamTim = GPIOB ;
	m8.pinNumTim = GPIO_PIN_14 ;
	m8.htim = htim15 ;
	m8.channel = TIM_CHANNEL_1 ;
	m8.pinFam1 = GPIOA ;
	m8.pinNum1 = GPIO_PIN_12 ;
	m8.pinFam2 = GPIOB ;
	m8.pinNum2 = GPIO_PIN_13 ;
	p->motors[7] = m8 ;
	standardInitMotor(&(p->motors[7]));
	p->motors[7].trans.type = TYPE_HORI ;
}

void initVCaptors(params* p)
{
	VCaptor v1 ;
	v1.id = 0 ;
	v1.inputPinF =GPIOC ;
	v1.inputPinN = GPIO_PIN_4 ;
	v1.mot = &(p->motors[0]);
	p->vcs[0] = v1 ;
	standardInitVCaptor(&(p->vcs[0])) ;

	VCaptor v2 ;
	v2.id = 1 ;
	v2.inputPinF =GPIOC ;
	v2.inputPinN = GPIO_PIN_5 ;
	v2.mot = &(p->motors[1]) ;
	p->vcs[1] = v2 ;
	standardInitVCaptor(&(p->vcs[1])) ;

	VCaptor v3 ;
	v3.id = 2 ;
	v3.inputPinF =GPIOB ;
	v3.inputPinN = GPIO_PIN_0 ;
	v3.mot = &(p->motors[2]);
	p->vcs[2] = v3 ;
	standardInitVCaptor(&(p->vcs[2])) ;

	VCaptor v4 ;
	v4.id = 3 ;
	v4.inputPinF = GPIOB ;
	v4.inputPinN = GPIO_PIN_1 ;
	v4.mot = &(p->motors[3]) ;
	p->vcs[3] = v4 ;
	standardInitVCaptor(&(p->vcs[3])) ;

	VCaptor v5 ;
	v5.id = 0 ;
	v5.inputPinF =GPIOC ;
	v5.inputPinN = GPIO_PIN_13 ;
	v5.mot = &(p->motors[7]);
	p->vcs[4] = v5 ;
	standardInitVCaptor(&(p->vcs[4])) ;

	VCaptor v6 ;
	v6.id = 0 ;
	v6.inputPinF =GPIOC ;
	v6.inputPinN = GPIO_PIN_14 ;
	v6.mot = &(p->motors[6]);
	p->vcs[5] = v6 ;
	standardInitVCaptor(&(p->vcs[5])) ;

	VCaptor v7 ;
	v7.id = 0 ;
	v7.inputPinF =GPIOC ;
	v7.inputPinN = GPIO_PIN_15 ;
	v7.mot = &(p->motors[5]);
	p->vcs[6] = v7 ;
	standardInitVCaptor(&(p->vcs[6])) ;

	VCaptor v8 ;
	v8.id = 0 ;
	v8.inputPinF =GPIOF ;
	v8.inputPinN = GPIO_PIN_0 ;
	v8.mot = &(p->motors[4]);
	p->vcs[7] = v8 ;
	standardInitVCaptor(&(p->vcs[7])) ;
}

void initUARTS(params* p){
	p->huarts[0] = huart1 ;
	p->huarts[1] = huart2 ;
	p->huarts[2] = huart3 ;
}


//_______________________________________________________________TIME MEASUREMENT_________________________________________________

uint32_t getUs(void) {
uint32_t usTicks = HAL_RCC_GetSysClockFreq() / 1000000;
register uint32_t ms, cycle_cnt;
do {
ms = HAL_GetTick();
cycle_cnt = SysTick->VAL;
} while (ms != HAL_GetTick());
return (ms * 1000) + (usTicks * 1000 - cycle_cnt) / usTicks;
}

void delayUs(uint16_t micros) {
	uint32_t start = getUs();
	while (getUs()-start < (uint32_t) micros) {
		asm("nop");
	}
}

//_______________________________________DEMORUN_____________________________________________________

void demoRun(uint32_t time_us)
{
	if(demoStep == 1)
	{
		uint8_t power = 70 ;
		uint8_t sense = SENSE_UP ;
		MR_startBlindMotion(&(gps.gss[0]), &power, &sense, 2000000) ;
		demoStep = 2 ;
	}
	if(demoStep == 2 && gps.gss[0].MR_mode == MR_MODE_IDLE)
	{
		uint8_t power = 70 ;
		uint8_t sense = SENSE_DOWN ;
		MR_startBlindMotion(&(gps.gss[0]), &power, &sense, 2000000) ;
		demoStep = 3 ;
	}


	if(demoStep == 3 && gps.gss[0].MR_mode == MR_MODE_IDLE)
	{
		uint8_t power = 70 ;
		uint8_t sense = SENSE_UP ;
		MR_startBlindMotion(&(gps.gss[1]), &power, &sense, 2000000) ;
		demoStep = 4 ;
	}
	if(demoStep == 4 && gps.gss[1].MR_mode == MR_MODE_IDLE)
	{
		uint8_t power = 70 ;
		uint8_t sense = SENSE_DOWN ;
		MR_startBlindMotion(&(gps.gss[1]), &power, &sense, 2000000) ;
		demoStep = 5 ;
	}

	if(demoStep == 5 && gps.gss[1].MR_mode == MR_MODE_IDLE)
		{
			uint8_t power = 70 ;
			uint8_t sense = SENSE_UP ;
			MR_startBlindMotion(&(gps.gss[2]), &power, &sense, 2000000) ;
			demoStep = 6 ;
		}
	if(demoStep == 6 && gps.gss[2].MR_mode == MR_MODE_IDLE)
		{
			uint8_t power = 70 ;
			uint8_t sense = SENSE_DOWN ;
			MR_startBlindMotion(&(gps.gss[2]), &power, &sense, 2000000) ;
			demoStep = 7 ;
		}

	if(demoStep == 7 && gps.gss[2].MR_mode == MR_MODE_IDLE)
			{
				uint8_t power = 70 ;
				uint8_t sense = SENSE_UP ;
				MR_startBlindMotion(&(gps.gss[3]), &power, &sense, 2000000) ;
				demoStep = 8 ;
			}
	if(demoStep == 8 && gps.gss[3].MR_mode == MR_MODE_IDLE)
			{
				uint8_t power = 70 ;
				uint8_t sense = SENSE_DOWN ;
				MR_startBlindMotion(&(gps.gss[3]), &power, &sense, 2000000) ;
				demoStep = 9 ;
			}

	if(demoStep == 9 && gps.gss[3].MR_mode == MR_MODE_IDLE)
			{
				uint8_t power = 70 ;
				uint8_t sense = SENSE_UP ;
				MR_startBlindMotion(&(gps.gss[4]), &power, &sense, 5000000) ;
				demoStep = 10 ;
			}
	if(demoStep == 10 && gps.gss[4].MR_mode == MR_MODE_IDLE)
			{
				uint8_t power = 70 ;
				uint8_t sense = SENSE_DOWN ;
				MR_startBlindMotion(&(gps.gss[4]), &power, &sense, 5000000) ;
				demoStep = 11 ;
			}

	if(demoStep == 11 && gps.gss[4].MR_mode == MR_MODE_IDLE)
			{
				uint8_t power = 70 ;
				uint8_t sense = SENSE_UP ;
				MR_startBlindMotion(&(gps.gss[5]), &power, &sense, 5000000) ;
				demoStep = 12 ;
			}
	if(demoStep == 12 && gps.gss[5].MR_mode == MR_MODE_IDLE)
			{
				uint8_t power = 70 ;
				uint8_t sense = SENSE_DOWN ;
				MR_startBlindMotion(&(gps.gss[5]), &power, &sense, 5000000) ;
				demoStep = 13 ;
			}

	if(demoStep == 13 && gps.gss[5].MR_mode == MR_MODE_IDLE)
			{
				uint8_t power = 70 ;
				uint8_t sense = SENSE_UP ;
				MR_startBlindMotion(&(gps.gss[6]), &power, &sense, 5000000) ;
				demoStep = 14 ;
			}
	if(demoStep == 14 && gps.gss[6].MR_mode == MR_MODE_IDLE)
			{
				uint8_t power = 70 ;
				uint8_t sense = SENSE_DOWN ;
				MR_startBlindMotion(&(gps.gss[6]), &power, &sense, 5000000) ;
				demoStep = 15 ;
			}

	if(demoStep == 15 && gps.gss[6].MR_mode == MR_MODE_IDLE)
			{
				uint8_t power = 70 ;
				uint8_t sense = SENSE_UP ;
				MR_startBlindMotion(&(gps.gss[7]), &power, &sense, 5000000) ;
				demoStep = 16 ;
			}
	if(demoStep == 16 && gps.gss[7].MR_mode == MR_MODE_IDLE)
			{
				uint8_t power = 70 ;
				uint8_t sense = SENSE_DOWN ;
				MR_startBlindMotion(&(gps.gss[7]), &power, &sense, 5000000) ;
				demoStep = 17 ;
			}

	if (demoStep == 80)
	{
		MR_goHori(&(gps.gss[0]),0, SENSE_UP, 7000000, 1) ;
		demoStep = 81 ;
	}
	if (demoStep == 81 && gps.gss[0].MR_mode == MR_MODE_IDLE)
	{
		MR_goHori(&(gps.gss[0]),0,SENSE_DOWN, 7000000, 1) ;
		demoStep = 82 ;
	}
	if (demoStep == 82 && gps.gss[0].MR_mode == MR_MODE_IDLE)
	{
		MR_goHori(&(gps.gss[2]),0,SENSE_UP, 7000000, 0) ;
		demoStep = 83 ;
	}
	if (demoStep == 83 && gps.gss[2].MR_mode == MR_MODE_IDLE)
	{
		MR_goHori(&(gps.gss[2]),0, SENSE_DOWN, 7000000, 0) ;
		demoStep = 100 ;
	}

	if(demoStep == 84)
	{
		uint8_t tx7[] = {COM_A} ;
		HAL_UART_Transmit(&huart1, tx7, 1, 1000) ;
		demoStep = 100 ;
	}

	if(demoStep == 20)
	{
		HL_StartGetFromDown(&(gps.gss[4])) ;
		demoStep = 100 ;
	}

	if(demoStep == 21)
	{
		HL_StartGetFromUp(&(gps.gss[4])) ;
		demoStep = 28 ;
	}

	if(demoStep == 22 && gps.gss[2].HL_mode == HL_MODE_IDLE)
	{
		HL_StartGoUp(&(gps.gss[4])) ;
		demoStep = 100 ;
	}
	if(demoStep == 23)
	{
		HL_StartGoDown(&(gps.gss[4])) ;
		demoStep = 100 ;
	}
	if(demoStep == 24)
	{
		GS_startResetPosition(&(gps.gss[4])) ;
		demoStep = 100 ;
	}
	if(demoStep == 95)
	{
		GS_startResetPosition(&(gps.gss[4])) ;
		demoStep = 100 ;
	}
	if(demoStep == 25 && gps.gss[4].mode == MODE_IDLE)
	{
		demoStep = 22 ;
	}
	if(demoStep == 26)
	{
		GS_startResetPosition(&(gps.gss[4])) ;
		demoStep = 22 ;
	}
	if(demoStep == 27)
	{
		HL_StartGetFromLeft(&(gps.gss[2])) ;
		demoStep = 100;
	}
	if(demoStep == 28 && gps.gss[4].HL_mode == HL_MODE_IDLE && gps.gss[0].HL_mode == HL_MODE_IDLE)
	{
		HL_StartGoRight(&(gps.gss[2])) ;
		demoStep = 78 ;
	}
	if(demoStep == 29)
	{
		HL_StartGetFromRight(&(gps.gss[2])) ;
		demoStep = 100 ;
	}
	if(demoStep == 30 && gps.gss[4].HL_mode == HL_MODE_IDLE && gps.gss[0].HL_mode == HL_MODE_IDLE && gps.gss[2].HL_mode == HL_MODE_IDLE)
	{
		HL_StartGoLeft(&(gps.gss[2])) ;
		demoStep = 100 ;
	}
	if(demoStep == 31)
	{
		GS_startResetPosition(&(gps.gss[4])) ;
		demoStep = 32 ;
	}
	if(demoStep == 32 && gps.gss[4].mode == MODE_IDLE)
	{
		HL_StartGoDown(&(gps.gss[4])) ;
		demoStep = 100 ;
	}
	if(demoStep == 70)
	{
		GS_startResetPosition(&(gps.gss[4])) ;
		demoStep = 71 ;
	}
	if(demoStep == 71 && gps.gss[4].mode == MODE_IDLE)
	{
		HL_StartGetFromUp(&(gps.gss[4])) ;
		demoStep = 28 ;
	}
	if(demoStep == 72)
	{
		HL_StartGoBackward(&(gps.gss[0]));
		demoStep = 100 ;
	}
	if(demoStep == 73 && gps.gss[4].mode == MODE_IDLE)
	{
		HL_StartGetFromForward(&(gps.gss[0])) ;
		demoStep = 100 ;
	}
	if(demoStep == 74)
	{
		GS_startResetPosition(&(gps.gss[4])) ;
		demoStep = 75 ;
	}
	if(demoStep == 75 && gps.gss[4].mode == MODE_IDLE)
	{
		uint8_t ids[] = {2,3} ;
		addGroup(&gps, GROUP_TYPE_CAPTOR, 6, ids, 2) ;
		GS_startTargeting(&(gps.gss[6]), LOW_AVSPEED, gps.gss[6].vcs[0]->target_currTour + 2) ;
		demoStep = 76 ;
	}
	if(demoStep == 76 && gps.gss[6].mode == MODE_IDLE)
	{
		eraseGroup(&gps, 6) ;
		demoStep = 28 ;
	}
	if(demoStep == 77 && gps.gss[0].HL_mode == HL_MODE_IDLE)
	{
		uint8_t tx7[] = {COM_6} ;
		HAL_UART_Transmit(&huart1, tx7, 1, 1000) ;
		demoStep = 100 ;
	}
	if(demoStep == 78 && gps.gss[2].HL_mode == HL_MODE_IDLE)
	{
		uint8_t tx7[] = {COM_7} ;
		HAL_UART_Transmit(&huart3, tx7, 1, 1000) ;
		demoStep = 100 ;
	}
	if(demoStep == 90)
	{
		HL_StartGoForward(&(gps.gss[0])) ;
		demoStep = 94 ;
	}
	if(demoStep == 91)
	{
		HL_StartGetFromBackward(&(gps.gss[0])) ;
		demoStep = 100 ;
	}
	if(demoStep == 92 && gps.gss[4].mode == MODE_IDLE)
	{
		HL_StartGoLeft(&(gps.gss[2])) ;
		demoStep = 93 ;
	}
	if(demoStep == 93 && gps.gss[2].HL_mode == HL_MODE_IDLE)
	{
		uint8_t tx7[] = {COM_9} ;
		HAL_UART_Transmit(&huart1, tx7, 1, 1000) ;
		demoStep = 100 ;
	}
	if(demoStep == 94 && gps.gss[0].HL_mode == HL_MODE_IDLE)
	{
		uint8_t tx7[] = {COM_10} ;
		HAL_UART_Transmit(&huart1, tx7, 1, 1000) ;
		demoStep = 100 ;
	}

	if(demoStep == 130)
	{
		for (int i=0; i<8; i++)
		{
			uint8_t idMotor = i ; //Modifiable, id du moteur à tester, a changer de 0 à 7
			uint8_t sense = 0 ; //Modifiable, sens du moteur, à mettre à 0 ou 1
			uint16_t power = 100 ; //Modifiable, puissance du moteur, va de 0 à 100

			setMotorTSense(&(gps.motors[idMotor]), sense) ;
			setMotorTPower(&(gps.motors[idMotor]), power) ;
		}

		time_start = time_us ;
		demoStep = 131 ;
	}

	if(demoStep == 131)
	{
		if(time_us - time_start > 4000000)
		{
			/*HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, SET);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, SET);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, SET);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, SET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, SET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, SET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, SET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, SET);*/

			for (int i=0; i<8; i++)
			{
				uint8_t idMotor = i ; //Modifiable, id du moteur à tester, a changer de 0 à 7
				uint16_t power = 0 ; //Modifiable, puissance du moteur, va de 0 à 100
				setMotorTPower(&(gps.motors[idMotor]), power) ;
			}

			demoStep = 132 ;
		}
	}

	if(demoStep == 132)
	{
		if(time_us - time_start > 6000000)
		{
			for (int i=0; i<8; i++)
			{
				uint8_t idMotor = i ; //Modifiable, id du moteur à tester, a changer de 0 à 7
				uint8_t sense = 1 ; //Modifiable, sens du moteur, à mettre à 0 ou 1
				uint16_t power = 100 ; //Modifiable, puissance du moteur, va de 0 à 100

				setMotorTSense(&(gps.motors[idMotor]), sense) ;
				setMotorTPower(&(gps.motors[idMotor]), power) ;
			}
		}

		/*if (HAL_ADC_PollForConversion(&hadc1, 1000000) == HAL_OK)
		{

			gAdcValue = HAL_ADC_GetValue(&hadc1);
			SumAdc += gAdcValue;
			cpt++;
			if(cpt==100)
			{
				MoyAdc = SumAdc/cpt;
				cpt = 0;
				SumAdc = 0;
			}
		}*/

		if (HAL_ADC_PollForConversion(&hadc1, 1000000) == HAL_OK)
		{
			channel++;

			if(channel == 1)
			{
				gAdcValue = HAL_ADC_GetValue(&hadc1);
				SumAdc += gAdcValue;
				cpt++;
				if(cpt==10)
				{
					MoyAdc = SumAdc/cpt;
					cpt = 0;
					SumAdc = 0;
				}
			}

			if (channel == 8)
			{
				channel = 0;
			}

		}

	}

	if(demoStep == 133)
	{
		uint8_t tx7[] = {'a'} ;
		HAL_UART_Transmit(&huart1, tx7, 1, 1000) ;
		demoStep = 100;
	}
}

//COM FUNCTIONS
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
	{
		__NOP() ;
	}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
	{
		if(huart->Instance==USART1)
			{
				handle_Message(rx1, 1) ;

	 		    HAL_UART_Receive_IT(&huart1, rx1, BUFFSIZE) ;
			}
			if(huart->Instance==USART2)
			{
				handle_Message(rx2, 2) ;

				HAL_UART_Receive_IT(&huart2, rx2, BUFFSIZE) ;
			}
			if(huart->Instance==USART3)
			{
				handle_Message(rx3, 3) ;

				HAL_UART_Receive_IT(&huart3, rx3, BUFFSIZE) ;
			}
	}
void handle_Message(uint8_t* buf, uint8_t origin)
{
	HL_handle_message(&gps, buf) ;
	if(buf[0] == COM_1 ){
		demoStep = 20;
	}
	if(buf[0] == COM_2){
		demoStep = 21 ;
	}
	if(buf[0] == COM_3){
		demoStep = 27 ;
	}
	if(buf[0] == COM_4){
		demoStep = 29 ;
	}
	if(buf[0] == COM_5){
		demoStep = 73 ;
	}
	if(buf[0] == COM_6){
		demoStep = 30 ;
	}
	if(buf[0] == COM_7){
		demoStep = 72 ;
	}
	if(buf[0] == COM_8)
	{
		demoStep = 91 ;
	}
	if(buf[0] == COM_9)
	{
		demoStep = 90 ;
	}
	if(buf[0] == COM_10)
	{
		demoStep = 22 ;
	}

//	if(buf[0] == 'O')
//		demoStep = 19 ;
//	if(buf[0] == 39)
//		demoStep = 1 ;
//	if(buf[0] == 'N')
//		demoStep = 24 ;
//	if(buf[0] == 19)
//		demoStep = 4 ;
//	if(buf[0] == 'K')
//		demoStep = 110 ;
}

//uint64_t flash_read(uint32_t address){
//    return *((uint32_t*)address);
//}
//
//void flash_write(uint32_t address, uint64_t data){
//    HAL_FLASH_Unlock();
//    HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,address,data);
//    HAL_FLASH_Lock();
//}
//
//void flash_page_erase(uint32_t page)
//{
//    HAL_FLASH_Unlock();
//    uint32_t error = 0 ;
//    FLASH_EraseInitTypeDef FLASH_EraseInitStruct =
//        {
//        .TypeErase = FLASH_TYPEERASE_PAGES,
//    	.Page = page,
//    	.NbPages = 1,
//        };
//    HAL_FLASHEx_Erase(&FLASH_EraseInitStruct,&error);
//    HAL_FLASH_Lock();
//}
//
//


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
