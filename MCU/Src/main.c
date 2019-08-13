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
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usbd_cdc_if.h"
#include "CUSB.h"
#include "CADC.h"
#include "tools.h"


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
ADC_HandleTypeDef hadc2;
ADC_HandleTypeDef hadc3;
DMA_HandleTypeDef hdma_adc1;

/* USER CODE BEGIN PV */
struct CustomADCStruct CADC;

uint8_t USBBufTx[USBBufTxSize];
struct CQueue	USBBufRxQ;
//struct CQueue	USBBufTxQ;
int USBBufRxQOverload = 0;
uint16_t USBTxTimeout = 0;
uint8_t USBTxTimeoutFlag = 0;
uint8_t		triggerEvent;
uint16_t 	ADCBufSize = ADC_BUF_SIZE_DEFAULT;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_ADC2_Init(void);
static void MX_ADC3_Init(void);
/* USER CODE BEGIN PFP */
void USB_RX_handler(void);
void ADC_ContConv_handler(void);
uint8_t USB_TX(uint8_t* Buf, uint16_t Len);
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
	CQueueInit(&USBBufRxQ, USBBufRxQSize);
	//CQueueInit(&USBBufTxQ, USBBufTxQSize);
	
	// ADC custom struct init
	CADC.data = (uint16_t *)malloc(sizeof(uint16_t) * ADC_BUF_SIZE_DEFAULT);
	CADC.convCompltFlag = 0;
	CADC.state = STATE_OFF;
	//CADC.triggerEvent = 0;
	triggerEvent = 0;
	
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
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_ADC3_Init();
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */
  /*if(HAL_ADC_Start(&hadc3) != HAL_OK)
    Error_Handler();
  if(HAL_ADC_Start(&hadc2) != HAL_OK)
    Error_Handler();
  if(HAL_ADCEx_MultiModeStart_DMA(&hadc1, (uint32_t*)CADC.ADCDataBuf, ADCBufSize) != HAL_OK)
    Error_Handler();
	CADC.ADCState = STATE_ON;
	CADC.ADCStartTime = SysTick->VAL;*/
	
	//volatile int i = 0;
	//volatile uint8_t res = 0;
	//USBBufTx[7] = '\n';
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
			
		ADC_ContConv_handler();
		
		USB_RX_handler();
			
		//HAL_Delay(5000);
		
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

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 144;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 6;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
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

  ADC_MultiModeTypeDef multimode = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
  hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_Ext_IT11;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the ADC multi-mode 
  */
  multimode.Mode = ADC_TRIPLEMODE_INTERL;
  multimode.DMAAccessMode = ADC_DMAACCESSMODE_2;
  multimode.TwoSamplingDelay = ADC_TWOSAMPLINGDELAY_5CYCLES;
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief ADC2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC2_Init(void)
{

  /* USER CODE BEGIN ADC2_Init 0 */

  /* USER CODE END ADC2_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC2_Init 1 */

  /* USER CODE END ADC2_Init 1 */
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
  */
  hadc2.Instance = ADC2;
  hadc2.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc2.Init.Resolution = ADC_RESOLUTION_12B;
  hadc2.Init.ScanConvMode = DISABLE;
  hadc2.Init.ContinuousConvMode = ENABLE;
  hadc2.Init.DiscontinuousConvMode = DISABLE;
  hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc2.Init.NbrOfConversion = 1;
  hadc2.Init.DMAContinuousRequests = DISABLE;
  hadc2.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc2) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC2_Init 2 */

  /* USER CODE END ADC2_Init 2 */

}

/**
  * @brief ADC3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC3_Init(void)
{

  /* USER CODE BEGIN ADC3_Init 0 */

  /* USER CODE END ADC3_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC3_Init 1 */

  /* USER CODE END ADC3_Init 1 */
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
  */
  hadc3.Instance = ADC3;
  hadc3.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc3.Init.Resolution = ADC_RESOLUTION_12B;
  hadc3.Init.ScanConvMode = DISABLE;
  hadc3.Init.ContinuousConvMode = ENABLE;
  hadc3.Init.DiscontinuousConvMode = DISABLE;
  hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc3.Init.NbrOfConversion = 1;
  hadc3.Init.DMAContinuousRequests = DISABLE;
  hadc3.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc3) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC3_Init 2 */

  /* USER CODE END ADC3_Init 2 */

}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

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
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15 
                          |GPIO_PIN_0, GPIO_PIN_RESET);

  /*Configure GPIO pin : PE11 */
  GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PD12 PD13 PD14 PD15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PD0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc){
	CADC.convCompltFlag =	1;
}

void ADC_ContConv_handler(){
	if ((CADC.convCompltFlag == 1) || ((CADC.state == STATE_ON) && (CADC.startTime > ADC_WHATCHDOG_TIMEOUT))){
	//if ((CADC.convCompltFlag == 1) || ((CADC.state == STATE_ON) && (CADC.startTime > ADC_WHATCHDOG_TIMEOUT) && (triggerEvent > 0))){
		CADC.convCompltFlag = 0;
		//CADC.triggerEvent = 0;
		triggerEvent = 0;
		if(HAL_ADCEx_MultiModeStop_DMA(&hadc1) != HAL_OK)
			Error_Handler();
		//if(HAL_ADC_Stop(&hadc1) != HAL_OK)
		//	Error_Handler();
		if(HAL_ADC_Stop(&hadc2) != HAL_OK)
			Error_Handler();
		if(HAL_ADC_Stop(&hadc3) != HAL_OK)
			Error_Handler();
		CADC.state = STATE_OFF;
		
		USBBufTx[0]= USB_0_DATA;
		USBBufTx[1] = USB_1_DATA_ADC_CONTCONV;
		memcpy(&USBBufTx[2], CADC.data, ADCBufSize * 2);
		volatile uint8_t result = USB_TX(USBBufTx, ADCBufSize * 2 + 2);
		/*
		volatile uint8_t result;
		for (int i = 0; i < (ADCBufSize / 62 + 1); i++){
			USBBufTx[0]= USB_0_DATA;
			USBBufTx[1] = USB_1_DATA_ADC_CONTCONV;
			memcpy(&USBBufTx[2], &CADC.data[i * 62], 62);
			result = USB_TX(USBBufTx, 62 + 2);
			HAL_Delay(100);
		}*/
		
				
		//MX_DMA_Init();
		MX_ADC1_Init();
		MX_ADC2_Init();
		MX_ADC3_Init();
		if(HAL_ADC_Start(&hadc3) != HAL_OK)
			Error_Handler();
		if(HAL_ADC_Start(&hadc2) != HAL_OK)
			Error_Handler();
		//if(HAL_ADC_Start(&hadc1) != HAL_OK)
		//	Error_Handler();
		if(HAL_ADCEx_MultiModeStart_DMA(&hadc1, (uint32_t*)CADC.data, ADCBufSize) != HAL_OK)
			Error_Handler();
		CADC.state = STATE_ON;
		CADC.startTime = 0;
		
		//HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
	}
}

uint8_t ADC_dataBuf_resize(uint16_t size, uint8_t *state_extended){
	if (CADC.state == STATE_ON){
		*state_extended = USB_1_ERROR_ADC_SET_BUF_SIZE_ADCRUN;
		return USB_0_ERROR;
	}
	if (size > ADC_MAX_BUF_SIZE){
		*state_extended = USB_1_ERROR_ADC_SET_BUF_SIZE_UPPERLIMIT;
		return USB_0_ERROR;
	}
	if (size < ADC_MIN_BUF_SIZE){
		*state_extended = USB_1_ERROR_ADC_SET_BUF_SIZE_LOWERLIMIT;
		return USB_0_ERROR;
	}
	free(CADC.data);
	CADC.data = (uint16_t *)malloc(sizeof(uint16_t) * size);
	ADCBufSize = size;
	
	*state_extended = USB_1_INFO_ADC_SET_BUF_SIZE_OK;
	return USB_0_INFO;
}

void ADCContConvStart(){
	if (CADC.state == STATE_ON){
		USBBufTx[0] = USB_0_ERROR;
		USBBufTx[1] = USB_1_ERROR_ADC_CONTCONV_START;
		CDC_Transmit_FS(USBBufTx, 2);
		return;
	}
	MX_ADC1_Init();
	MX_ADC2_Init();
	MX_ADC3_Init();
	if(HAL_ADC_Start(&hadc3) != HAL_OK)
		Error_Handler();
	if(HAL_ADC_Start(&hadc2) != HAL_OK)
		Error_Handler();
	//if(HAL_ADC_Start(&hadc1) != HAL_OK)
	//	Error_Handler();
	if(HAL_ADCEx_MultiModeStart_DMA(&hadc1, (uint32_t*)CADC.data, ADCBufSize) != HAL_OK)
		Error_Handler();
	CADC.state = STATE_ON;
	CADC.startTime = 0;
	triggerEvent = 0;
}

void ADCContConvStop(){
	if (CADC.state == STATE_OFF){
		USBBufTx[0] = USB_0_ERROR;
		USBBufTx[1] = USB_1_ERROR_ADC_CONTCONV_STOP;
		CDC_Transmit_FS(USBBufTx, 2);
		return;
	}
	if(HAL_ADCEx_MultiModeStop_DMA(&hadc1) != HAL_OK)
		Error_Handler();
		//if(HAL_ADC_Stop(&hadc1) != HAL_OK)
		//	Error_Handler();
	if(HAL_ADC_Stop(&hadc2) != HAL_OK)
		Error_Handler();
	if(HAL_ADC_Stop(&hadc3) != HAL_OK)
		Error_Handler();
	CADC.state = STATE_OFF;
}

void ADCRequestBufSize(){
		USBBufTx[0] = USB_0_DATA;
		USBBufTx[1] = USB_1_DATA_ADC_BUF_SIZE;
		int res =	NumberToRAWMSBFirst(USBBufTx, 2, ADCBufSize, 2);
		CDC_Transmit_FS(USBBufTx, 4);
}

void ADCRequestState(){
		USBBufTx[0] = USB_0_INFO;
		if (CADC.state == STATE_ON)
			USBBufTx[1] = USB_1_INFO_ADC_STATE_ON;
		else
			USBBufTx[1] = USB_1_INFO_ADC_STATE_OFF;
		CDC_Transmit_FS(USBBufTx, 2);	
}

void Rx_CMD_handler(uint8_t *data){
	switch (data[1]){
		case USB_1_CMD_ADC_CONTCONV_START:
			ADCContConvStart();
		break;
		case USB_1_CMD_ADC_CONTCONV_STOP:
			ADCContConvStop();
		break;
		case USB_1_CMD_ADC_REQUEST_BUF_SIZE:
			ADCRequestBufSize();
		break;
		case USB_1_CMD_ADC_REQUEST_STATE:
			ADCRequestState();
		break;
		
		
	}
}
void Rx_DATA_handler(uint8_t *data){
	switch (data[1]){
		case USB_1_DATA_ADC_BUF_SIZE:
			USBBufTx[0] = ADC_dataBuf_resize((uint16_t)RAWToNumberMSBFirst(data, 2, 2), &USBBufTx[1]);
			CDC_Transmit_FS(USBBufTx, 2);
		break;
	}
}

void Rx_ERROR_handler(uint8_t *data){

}

void Rx_INFO_handler(uint8_t *data){

}

void USB_RX_handler(){
	if (USBBufRxQ.current_size != 0){
		uint8_t data[USBBufRxSize];
		int i = 0;
		while (CQueuePop(&USBBufRxQ, data, USBBufRxSize) == 1){
			if (USBBufRxQOverload != 0){
				USBBufTx[0] = USB_0_ERROR;
				USBBufTx[1] = USB_1_ERROR_USB_FIFO_OVERLOAD;
				USBBufTx[2] = USBBufRxQOverload; 	// amount of data FIFO missed 
				CDC_Transmit_FS(USBBufTx, 3);
				USBBufRxQOverload = 0;
			} else {
				switch (data[0]){
				case USB_0_CMD: 
					Rx_CMD_handler(data);
				break;
				case USB_0_DATA: 
					Rx_DATA_handler(data);
				break;
				case USB_0_ERROR: 
					Rx_ERROR_handler(data);
				break;
				case USB_0_INFO: 
					Rx_INFO_handler(data);
				break;
				}
			}
			i++;
		}
		USBBufTx[0] = USB_0_INFO;
		USBBufTx[1] = USB_1_INFO_RX_FIFO_CLEAR;
		USBBufTx[2] = i;					// amount of data received from FIFO
		CDC_Transmit_FS(USBBufTx, 3);
		//HAL_Delay(5000);				// for FIFO overload test
	}
}

uint8_t USB_TX(uint8_t* Buf, uint16_t Len){
	USBTxTimeoutFlag = 1;
	uint8_t result = CDC_Transmit_FS(Buf, Len);
	while ((result != USBD_OK)){
		result = CDC_Transmit_FS(Buf, Len);
		if (USBTxTimeout > USB_TX_TIMEOUT){
			USBTxTimeoutFlag = 0;
			return result; 
		}	
	}
	USBTxTimeoutFlag = 0;
	return result; 
}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
	USBBufTx[0] = USB_0_ERROR;
	USBBufTx[1] = USB_1_ERROR_COMMON;
	CDC_Transmit_FS(USBBufTx, 2);
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
