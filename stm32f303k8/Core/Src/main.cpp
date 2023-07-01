/* USER CODE BEGIN Header */
/**
/******************************************************************************
* @file           : main.c
* @brief          : Main program body
/******************************************************************************
* @attention
*
* Copyright (c) 2023 STMicroelectronics.
* All rights reserved.
*
* This software is licensed under terms that can be found in the LICENSE file
* in the root directory of this software component.
* If no LICENSE file comes with this software, it is provided AS-IS.
*
******************************************************************************
*/
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "can.hpp"
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
// #define TRANSMITTER
// #define RECEIVER
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
static void can_transmitdata(uint32_t stdid, uint32_t dlc, uint32_t grobaltime, int64_t data);
static void can_receivedata(uint32_t stdid, uint32_t rtr, uint32_t dlc, uint32_t timestamp, uint32_t matchindex, int64_t data, CanFilterInit canfinit);
static void System_Init();
static void Can_Init();
static void Can_Filter_Init();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
CanFilterInit cfinit;
//for use printf
extern "C"
{
  int __io_putchar(int c)
  {
    if (c == '\n')
    {
      int b = '\r';
      HAL_UART_Transmit(&huart2, (uint8_t *)&b, 1, 1);
    }

    HAL_UART_Transmit(&huart2, (uint8_t *)&c, 1, 1);
    return 0;
  }
}
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
  /* USER CODE BEGIN 1 */
  setbuf(stdout, NULL);
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  //SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  System_Init();
  Can_Init();
  Can_Filter_Init();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    printf("%f\n", 3.2);
#ifdef TRANSMITTER
    // uint32_t id = 3;
    // uint32_t datasize = 8;
    // uint32_t gtime = 0;
    // int64_t data = 32;

    // if (datasize > 8)
    // datasize = 8;
    // while (1)
    //{
    // can_transmitdata(id, datasize, gtime, data);
    //}
#endif
#ifdef RECEIVER
    // uint32_t id = 3;
    // uint32_t rtr = 0;
    // uint32_t datasize = 8;
    // uint32_t timestamp = 0;
    // uint32_t matchindex = 0;
    // int64_t data = -1;

    // while (1)
    //{
    // can_receivedata(id, rtr, datasize, timestamp, matchindex, data,cfinit);
    // printf("%d\n",data);
    //}
#endif
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

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void can_transmitdata(uint32_t stdid, uint32_t dlc, uint32_t grobaltime, int64_t data)
{
  CanTxHeader cantxheader;
  cantxheader.transmit_id_StdId = stdid;
  cantxheader.transmit_datasize_DLC = dlc;
  cantxheader.transmit_grobaltime = grobaltime;
  cantxheader.transmit_data = data;

  can_add_txmessage(cantxheader);
  while (can_get_mailboxfreelevel() != 3);
    //printf("%f", can_get_mailboxfreelevel()); // wait until transmit fiish
}
void can_receivedata(uint32_t stdid, uint32_t rtr, uint32_t dlc, uint32_t timestamp, uint32_t matchindex, int64_t data, CanFilterInit canfinit)
{
  CanRxHeader canrxheader;
  canrxheader.receive_id_StdId = stdid;
  canrxheader.receive_rtr = rtr;
  canrxheader.receive_datasize_DLC = dlc;
  canrxheader.receive_timestamp = timestamp;
  canrxheader.receive_filtermatchindex = matchindex;
  canrxheader.receive_data = data;

  can_get_rxmessage(canrxheader, canfinit);
}
static void System_Init()
{
  SYSTEMInit systeminit;
  systeminit.pll_mlutiple = 0b1110;
  systeminit.apb1_ps = 0b100;
  systeminit.latency = 0b010;
  system_init(systeminit);
}
static void Can_Init()
{
  CanInit caninit;
  caninit.mcr_time_triger = 0;   // disable time triger mode
  caninit.mcr_bussoff = 0;       // disable auto bussoff
  caninit.mcr_wakeup = 0;        // disable auto wakeup
  caninit.mcr_retransmit = 0;    // enable retransmit
  caninit.mcr_fifo_lock = 1;     // enable fifo lockmode
  caninit.mcr_fifo_priority = 1; // Priority is determined by the order of requests (0: identifier of message)
  caninit.btr_debug_silent = 0;
  caninit.btr_debug_loopback = 1;
  caninit.btr_prescalar = 0b100;
  caninit.btr_swj = 1;
  caninit.btr_tseg1 = 0b1011;
  caninit.btr_tseg2 = 0b100;
  can_init(caninit);
}

static void Can_Filter_Init()
{

  cfinit.fm1r_filtermode = 1;
  cfinit.filter_scale = 16;
  cfinit.fm1r_fifo_number = 0;
  cfinit.f0r1_device_id1 = 3;
  cfinit.f0r1_device_id2 = 0;
  cfinit.f0r1_device_id3 = 0;
  cfinit.f0r1_device_id4 = 0;
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
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
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
   //ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
