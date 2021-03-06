/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
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
#include "adc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

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
/* 75 ms works great at this clock speed */
#define DEBOUNCE(x) (x)=75
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint16_t SPITx[10], SPIRx[10];	/* SPI data buffers - casting to uint16_t or it doesn't work */
static int s7_refresh_counter = 0;
static int button1_counter = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void s7_display_update(const int);
void s7_single_display_update(const int);
void s7_select_display(int);
void s7_unselect_display(int);

int __io_putchar(const int);
int getchar(void);

uint8_t SPIRead(uint8_t); /* read from spi device */
void SPIWrite(uint8_t, uint8_t); /* write to spi device */
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
  MX_ADC1_Init();
  MX_SPI1_Init();
  MX_TIM1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim1);
  HAL_ADC_Start_IT(&hadc1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  /* here put code like variable value init, spi init etc */
  while (1)
  {
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

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
uint8_t SPIRead( uint8_t Address ) {
 SPITx[0] = (0x80 | Address) << 8;
 HAL_GPIO_WritePin(GPIOA, SPI_NSS_Pin, GPIO_PIN_RESET);
 HAL_SPI_TransmitReceive(&hspi1, SPITx, SPIRx, 1, 100);
 HAL_GPIO_WritePin(GPIOA, SPI_NSS_Pin, GPIO_PIN_SET);
 return( (uint8_t)(SPIRx[0] & 0xFF) );
}

void SPIWrite( uint8_t AddressAndAtributes, uint8_t Data ) {
 SPITx[0] = (AddressAndAtributes << 8) + Data;
 HAL_GPIO_WritePin(GPIOA, SPI_NSS_Pin, GPIO_PIN_RESET);
 HAL_SPI_TransmitReceive(&hspi1, SPITx, SPIRx, 1, 100);
 HAL_GPIO_WritePin(GPIOA, SPI_NSS_Pin, GPIO_PIN_SET);
}

int __io_putchar(const int ch) {
	HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);
	return ch;
}

int getchar(void) {
	uint8_t value;
	if( __HAL_UART_GET_FLAG(&huart2, UART_FLAG_RXNE) != SET ) return( -1 );
	if (__HAL_UART_GET_FLAG(&huart2, UART_FLAG_RXNE) == SET) HAL_UART_Receive(&huart2, &value, 1, 100);
	__io_putchar( value );
	return ((int) value);
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == Button1_Pin) {
		button1_counter++;
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim == &htim1) { /* on timer 1 */
		s7_display_update(0);
		s7_refresh_counter++;

		/* can be done better, this way it's really easy to understand */
		if (s7_refresh_counter >= 24) {
			s7_refresh_counter = 0;
		}
	}
}

void s7_display_update(const int num) {
	if (s7_refresh_counter < 6) {
		s7_select_display(4);
		s7_single_display_update(num % 10);
		s7_unselect_display(4);
	} else if (s7_refresh_counter < 12) {
		s7_select_display(3);
		s7_single_display_update((num/10) % 10);
		s7_unselect_display(3);
	} else if (s7_refresh_counter < 18) {
		s7_select_display(2);
		s7_single_display_update((num/100) % 10);
		s7_unselect_display(2);
	} else if (s7_refresh_counter < 24) {
		s7_select_display(1);
		s7_single_display_update((num/1000) % 10);
		s7_unselect_display(1);
	}
}

void s7_single_display_update(const int num) {
	switch (number) {
	case 0:
		HAL_GPIO_WritePin(GPIOC, S7A_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7B_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7C_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7D_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7E_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7F_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7G_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, S7DP_Pin, GPIO_PIN_RESET);
		break;

	case 1:
		HAL_GPIO_WritePin(GPIOC, S7A_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, S7B_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7C_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7D_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, S7E_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, S7F_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, S7G_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, S7DP_Pin, GPIO_PIN_RESET);
		break;

	case 2:
		HAL_GPIO_WritePin(GPIOC, S7A_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7B_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7C_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, S7D_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7E_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7F_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, S7G_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7DP_Pin, GPIO_PIN_RESET);
		break;

	case 3:
		HAL_GPIO_WritePin(GPIOC, S7A_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7B_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7C_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7D_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7E_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, S7F_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, S7G_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7DP_Pin, GPIO_PIN_RESET);
		break;

	case 4:
		HAL_GPIO_WritePin(GPIOC, S7A_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, S7B_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7C_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7D_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, S7E_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, S7F_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7G_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7DP_Pin, GPIO_PIN_RESET);
		break;

	case 5:
		HAL_GPIO_WritePin(GPIOC, S7A_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7B_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, S7C_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7D_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7E_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, S7F_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7G_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7DP_Pin, GPIO_PIN_RESET);
		break;

	case 6:
		HAL_GPIO_WritePin(GPIOC, S7A_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7B_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, S7C_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7D_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7E_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7F_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7G_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7DP_Pin, GPIO_PIN_RESET);
		break;

	case 7:
		HAL_GPIO_WritePin(GPIOC, S7A_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7B_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7C_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7D_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, S7E_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, S7F_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, S7G_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, S7DP_Pin, GPIO_PIN_RESET);
		break;

	case 8:
		HAL_GPIO_WritePin(GPIOC, S7A_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7B_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7C_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7D_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7E_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7F_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7G_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7DP_Pin, GPIO_PIN_RESET);
		break;

	case 9:
		HAL_GPIO_WritePin(GPIOC, S7A_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7B_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7C_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7D_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7E_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, S7F_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7G_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7DP_Pin, GPIO_PIN_RESET);
		break;

	default:
		break;
	}
}

void s7_unselect_display(const int disp) {
	switch (disp) {
	case 1:
		HAL_GPIO_WritePin(GPIOC, S7Com1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, S7Com2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7Com3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7Com4_Pin, GPIO_PIN_SET);
		break;

	case 2:
		HAL_GPIO_WritePin(GPIOC, S7Com1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7Com2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, S7Com3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7Com4_Pin, GPIO_PIN_SET);
		break;

	case 3:
		HAL_GPIO_WritePin(GPIOC, S7Com1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7Com2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7Com3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, S7Com4_Pin, GPIO_PIN_SET);
		break;

	case 4:
		HAL_GPIO_WritePin(GPIOC, S7Com1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7Com2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7Com3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7Com4_Pin, GPIO_PIN_RESET);
		break;

	default:
		HAL_GPIO_WritePin(GPIOC, S7Com1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7Com2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7Com3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7Com4_Pin, GPIO_PIN_SET);
		break;
	}
}

void s7_select_display(const int disp) {
	switch (disp) {
	case 1:
		HAL_GPIO_WritePin(GPIOC, S7Com1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7Com2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, S7Com3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, S7Com4_Pin, GPIO_PIN_RESET);
		break;

	case 2:
		HAL_GPIO_WritePin(GPIOC, S7Com1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, S7Com2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7Com3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, S7Com4_Pin, GPIO_PIN_RESET);
		break;

	case 3:
		HAL_GPIO_WritePin(GPIOC, S7Com1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, S7Com2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, S7Com3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7Com4_Pin, GPIO_PIN_RESET);
		break;

	case 4:
		HAL_GPIO_WritePin(GPIOC, S7Com1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, S7Com2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, S7Com3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, S7Com4_Pin, GPIO_PIN_SET);
		break;

	default:
		HAL_GPIO_WritePin(GPIOC, S7Com1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7Com2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7Com3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, S7Com4_Pin, GPIO_PIN_SET);
		break;
	}
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
