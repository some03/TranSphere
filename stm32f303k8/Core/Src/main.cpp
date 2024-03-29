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
#include "i2c.h"
#include "tim.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "can.hpp"
#include "walk.hpp"
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
// #define TRANSMITTER
//  #define RECEIVER
#define MPU9250_ADDRESS 0b1101000<<1
#define MPU9250
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

static void can_transmitdata(uint32_t stdid, uint32_t dlc, uint32_t grobaltime, uint32_t data[8]);
static void can_receivedata(uint32_t stdid, uint32_t rtr, uint32_t dlc, uint32_t timestamp, uint32_t matchindex, uint32_t data[8], CanFilterInit canfinit);
static void System_Init();
static void Can_Init();
static void Can_Filter_Init();
static void MPU9250_GetData();

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

CanFilterInit cfinit;
extern "C" void CAN_RX0_IRQHandler(void);
// for use printf
extern "C"
{
  int _write(int file, char *ptr, int len)
  {
    HAL_UART_Transmit(&huart2, (uint8_t *)ptr, len, 10);
    return len;
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
  /* CAN clock enable */
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */

  MX_GPIO_Init();
  MX_TIM16_Init();
  MX_TIM17_Init();
  MX_I2C1_Init();
  MX_USART2_UART_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM15_Init();

  /* USER CODE BEGIN 2 */

  // System_Init();

  // Can_Init();
  // Can_Filter_Init();

  // timer count max 10000 pwm 50Hz
 //PA6 PA5 can't use pwm
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
  HAL_TIMEx_PWMN_Start(&htim15, TIM_CHANNEL_1);
  //HAL_TIM_PWM_Start(&htim16, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim17, TIM_CHANNEL_1);

  // init leg servo {timer,channel}
  servo_num s0[3] = {{&htim1, TIM_CHANNEL_1}, {&htim1, TIM_CHANNEL_2}, {&htim1, TIM_CHANNEL_3}};   // PA8 PA9 PA10
  servo_num s1[3] = {{&htim2, TIM_CHANNEL_1}, {&htim2, TIM_CHANNEL_2}, {&htim2, TIM_CHANNEL_4}};   // PA0 PB3 PA3
  servo_num s2[3] = {{&htim3, TIM_CHANNEL_1}, {&htim3, TIM_CHANNEL_2}, {&htim3, TIM_CHANNEL_3}};   // PA6 PA4 PB0
  servo_num s3[3] = {{&htim3, TIM_CHANNEL_4}, {&htim16, TIM_CHANNEL_1}, {&htim17, TIM_CHANNEL_1}}; // PB1 PB4 PA7

  servo_num center_s = {&htim15, TIM_CHANNEL_1}; // PA1
  tr_legs legs[4] = {s0, s1, s2, s3};
  walk upper_leg(legs, center_s);

  // wake up mpu 9250---------------

  uint8_t wu[1] = {0x00};
  int b = HAL_I2C_Mem_Write(&hi2c1, MPU9250_ADDRESS, 0x6b, I2C_MEMADD_SIZE_8BIT, wu, 0x01, 100);
  printf("%d\n", b);
  // set accelsensor  range----------
  uint8_t st[1] = {0x18};
  HAL_I2C_Mem_Write(&hi2c1, MPU9250_ADDRESS, 0x1c, I2C_MEMADD_SIZE_8BIT, st, 0x01, 100);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  //I2C1->CR1|=0b1<<1;
  //I2C1->CR1|=0b1<<7;
  while (1)
  {
#ifdef MPU9250
    /* 
     uint8_t who_i_im[1]={0x00};
     int a=HAL_I2C_Mem_Read(&hi2c1, MPU9250_ADDRESS, 0x75, I2C_MEMADD_SIZE_8BIT, who_i_im, 0x01, 100);
     printf("%s","a");
     printf("%d\n",who_i_im[0]);
    */ 
    MPU9250_GetData();
#endif

// printf("%d\n", 1);
#ifdef TRANSMITTER
    uint32_t id = 3;
    uint32_t datasize = 8;
    uint32_t gtime = 0;
    uint32_t data[8] = {0, 0, 0, 0, 1, 1, 1, 1};

    if (datasize > 8)
      datasize = 8;
    can_transmitdata(id, datasize, gtime, data);
#endif
    // Servo_Start(center_s,0);
    //__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,5000);
    //  upper_leg.spraddle_legs(M_PI/2);
    //  upper_leg.walking(M_PI/2);
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

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1 | RCC_PERIPHCLK_TIM1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  PeriphClkInit.Tim1ClockSelection = RCC_TIM1CLK_HCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}
/* USER CODE BEGIN 4 */

extern "C"
{
  void CAN_RX0_IRQHandler(void)
  {
    uint32_t rid = 3;
    uint32_t rtr = 0;
    uint32_t rdatasize = 8;
    uint32_t timestamp = 0;
    uint32_t matchindex = 0;
    uint32_t rdata[8] = {NULL};

    // Error_Handler();
    can_receivedata(rid, rtr, rdatasize, timestamp, matchindex, rdata, cfinit);

    // start move---------------------------------------------------------------
    if (rdata != NULL)
    {
      // printf("%d",3);
    }
  }
}
void can_transmitdata(uint32_t stdid, uint32_t dlc, uint32_t grobaltime, uint32_t data[8])
{
  CanTxHeader cantxheader;
  cantxheader.transmit_id_StdId = stdid;
  cantxheader.transmit_datasize_DLC = dlc;
  cantxheader.transmit_grobaltime = grobaltime;
  for (int i = 0; i < 8; i++)
    cantxheader.transmit_data[i] = data[i];

  can_add_txmessage(cantxheader);
  while (can_get_mailboxfreelevel() != 3)
    ;
  // printf("%f", can_get_mailboxfreelevel()); // wait until transmit fiish
}
void can_receivedata(uint32_t stdid, uint32_t rtr, uint32_t dlc, uint32_t timestamp, uint32_t matchindex, uint32_t data[8], CanFilterInit canfinit)
{
  CanRxHeader canrxheader;
  canrxheader.receive_id_StdId = stdid;
  canrxheader.receive_rtr = rtr;
  canrxheader.receive_datasize_DLC = dlc;
  canrxheader.receive_timestamp = timestamp;
  canrxheader.receive_filtermatchindex = matchindex;
  for (int i = 0; i < 8; i++)
    canrxheader.receive_data[i] = data[i];

  can_get_rxmessage(canrxheader, canfinit);
  /*
   printf("%s", "data[7]:");
   printf("%u\n", canrxheader.receive_data[7]);

   printf("%s", "data[0]:");
   printf("%u\n", canrxheader.receive_data[0]);
  */
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
  caninit.mcr_retransmit = 0;    // 0 restrict retransmit
  caninit.mcr_fifo_lock = 0;     // enable fifo lockmode
  caninit.mcr_fifo_priority = 0; // Priority is determined by the order of requests (0: identifier of message)
  caninit.btr_debug_silent = 0;
  caninit.btr_debug_loopback = 0; // 1 enable 0 disable
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
  cfinit.fm1r_filter_number = 0;
  cfinit.f0r1_device_id1 = 3;
  cfinit.f0r1_device_id2 = 0;
  cfinit.f0r1_device_id3 = 0;
  cfinit.f0r1_device_id4 = 0;

  can_filter_init(cfinit);
}

static void MPU9250_GetData()
{

  uint8_t accelx_h[1]={0}, accelx_l[1]={0},accely_h[1]={0},accely_l[1]={0}, accelz_h[1]={0},accelz_l[1]={0};
  uint8_t gyrox_h[1]={0}, gyrox_l[1]={0},gyroy_h[1]={0},gyroy_l[1]={0} ,gyroz_h[1]={0},gyroz_l[1]={0};

  HAL_I2C_Mem_Read(&hi2c1, MPU9250_ADDRESS, 0x3b, I2C_MEMADD_SIZE_8BIT, accelx_h, 1, 100);
  HAL_I2C_Mem_Read(&hi2c1, MPU9250_ADDRESS, 0x3c, I2C_MEMADD_SIZE_8BIT, accelx_l, 1, 100);
  HAL_I2C_Mem_Read(&hi2c1, MPU9250_ADDRESS, 0x3d, I2C_MEMADD_SIZE_8BIT, accely_h, 1, 100);
  HAL_I2C_Mem_Read(&hi2c1, MPU9250_ADDRESS, 0x3e, I2C_MEMADD_SIZE_8BIT, accely_l, 1, 100);
  HAL_I2C_Mem_Read(&hi2c1, MPU9250_ADDRESS, 0x3f, I2C_MEMADD_SIZE_8BIT, accelz_h, 1, 100);
  HAL_I2C_Mem_Read(&hi2c1, MPU9250_ADDRESS, 0x40, I2C_MEMADD_SIZE_8BIT, accelz_l, 1, 100);

  HAL_I2C_Mem_Read(&hi2c1, MPU9250_ADDRESS, 0x43, I2C_MEMADD_SIZE_8BIT, gyrox_h, 1, 100);
  HAL_I2C_Mem_Read(&hi2c1, MPU9250_ADDRESS, 0x44, I2C_MEMADD_SIZE_8BIT, gyrox_l, 1, 100);
  HAL_I2C_Mem_Read(&hi2c1, MPU9250_ADDRESS, 0x45, I2C_MEMADD_SIZE_8BIT, gyroy_h, 1, 100);
  HAL_I2C_Mem_Read(&hi2c1, MPU9250_ADDRESS, 0x46, I2C_MEMADD_SIZE_8BIT, gyroy_l, 1, 100);
  HAL_I2C_Mem_Read(&hi2c1, MPU9250_ADDRESS, 0x47, I2C_MEMADD_SIZE_8BIT, gyroz_h, 1, 100);
  HAL_I2C_Mem_Read(&hi2c1, MPU9250_ADDRESS, 0x48, I2C_MEMADD_SIZE_8BIT, gyroz_l, 1, 100);

  int16_t accelx=(accelx_h[0]<<8)|accelx_l[0];
  int16_t accely=(accely_h[0]<<8)|accely_l[0];
  int16_t accelz=(accelz_h[0]<<8)|accelz_l[0];
  
  int16_t gyrox=(gyrox_h[0]<<8)|gyrox_l[0];
  int16_t gyroy=(gyroy_h[0]<<8)|gyroy_l[0];
  int16_t gyroz=(gyroz_h[0]<<8)|gyroz_l[0];

  //int16_t a[6]={accelx,accely,accelz,gyrox,gyroy,gyroz};
  //return a;

   printf("%s","x:");
   printf("%d\n",gyroz_h[0]);
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
