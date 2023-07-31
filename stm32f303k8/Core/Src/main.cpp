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
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "can.hpp"
#include "timer.hpp"
#include "servo.hpp"
#include "walk.hpp"
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//#define TRANSMITTER
//#define RECEIVER
#define MPU9250_ADDRESS 0
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
static void can_transmitdata(uint32_t stdid, uint32_t dlc, uint32_t grobaltime, int8_t data[8]);
static void can_receivedata(uint32_t stdid, uint32_t rtr, uint32_t dlc, uint32_t timestamp, uint32_t matchindex, int8_t data[8], CanFilterInit canfinit);
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
//  DBGMCU->APB1FZ|=DBGMCU_APB1_FZ_DBG_WWDG_STOP;
  //WWDG->CR&=~(0b1<<7);
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  // SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  System_Init();
  Can_Init();
  Can_Filter_Init();

  //timer count max 10000 pwm 50Hz 
  Timer1_PWM_Init(10000,64);
  Timer2_PWM_Init(10000,64);
  Timer3_PWM_Init(10000,64);
  Timer16_PWM_Init(10000,64);  

  //init leg servo {timer,channel}
  servo_num s0[3]={{1,1},{1,2},{1,3}};
  servo_num s1[3]={{2,1},{2,2},{2,3}};
  servo_num s2[3]={{2,4},{3,1},{3,2}};
  servo_num s3[3]={{3,3},{3,4},{16,1}};

  servo_num center_s={17,1};
  servo legs[4]={s0,s1,s2,s3};
  walk upper_leg;

  //wake up mpu 9250---------------
  uint8_t wu[1]={0};
  HAL_I2C_Mem_Write(&hi2c1,MPU9250_ADDRESS,0x6b,I2C_MEMADD_SIZE_8BIT,wu,0x01,100);
  
  //set accelsensor  range----------
  uint8_t st[1]={0x18};
  HAL_I2C_Mem_Write(&hi2c1,MPU9250_ADDRESS,0x1c,I2C_MEMADD_SIZE_8BIT,st,0x01,100);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
#ifdef MPU9250
    MPU9250_GetData();
#endif

#ifdef TRANSMITTER
    uint32_t id = 3;
    uint32_t datasize = 8;
    uint32_t gtime = 0;
    int8_t data[8];
    // printf("%d\n",data);

    if (datasize > 8)
      datasize = 8;
    can_transmitdata(id, datasize, gtime, data);
#endif

    upper_leg.spraddle_legs(center_s,50);
    upper_leg.walking(legs,M_PI/2);

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
extern "C"
{
  void CAN_RX0_IRQHandler(void)
  {
    uint32_t rid = 3;
    uint32_t rtr = 0;
    uint32_t rdatasize = 8;
    uint32_t timestamp = 0;
    uint32_t matchindex = 0;
    int8_t rdata[8] = {NULL};

    // Error_Handler();
    can_receivedata(rid, rtr, rdatasize, timestamp, matchindex, rdata, cfinit);

    //start move---------------------------------------------------------------
    if(rdata!=NULL){
      
    }
  }
}
void can_transmitdata(uint32_t stdid, uint32_t dlc, uint32_t grobaltime, int8_t data[8])
{
  CanTxHeader cantxheader;
  cantxheader.transmit_id_StdId = stdid;
  cantxheader.transmit_datasize_DLC = dlc;
  cantxheader.transmit_grobaltime = grobaltime;
  for (int i=0;i<8;i++)cantxheader.transmit_data[i]=data[i];

  

  can_add_txmessage(cantxheader);
  while (can_get_mailboxfreelevel() != 3)
    ;
  // printf("%f", can_get_mailboxfreelevel()); // wait until transmit fiish
}
void can_receivedata(uint32_t stdid, uint32_t rtr, uint32_t dlc, uint32_t timestamp, uint32_t matchindex, int8_t data[8], CanFilterInit canfinit)
{
  CanRxHeader canrxheader;
  canrxheader.receive_id_StdId = stdid;
  canrxheader.receive_rtr = rtr;
  canrxheader.receive_datasize_DLC = dlc;
  canrxheader.receive_timestamp = timestamp;
  canrxheader.receive_filtermatchindex = matchindex;
  for(int i=0;i<8;i++)canrxheader.receive_data[i] = data[i];

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
  caninit.mcr_fifo_lock = 0;     // enable fifo lockmode
  caninit.mcr_fifo_priority = 0; // Priority is determined by the order of requests (0: identifier of message)
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
  cfinit.fm1r_filter_number = 0;
  cfinit.f0r1_device_id1 = 3;
  cfinit.f0r1_device_id2 = 0;
  cfinit.f0r1_device_id3 = 0;
  cfinit.f0r1_device_id4 = 0;

  can_filter_init(cfinit);
}

static void MPU9250_GetData(){

    uint16_t accel_x, accel_y, accel_z;
    uint16_t gyro_x, gyro_y, gyro_z;

    HAL_I2C_Mem_Read(&hi2c1,MPU9250_ADDRESS,0x3b,I2C_MEMADD_SIZE_8BIT,(uint8_t*)accel_x,8,100);
    accel_x<<0x8;
    HAL_I2C_Mem_Read(&hi2c1,MPU9250_ADDRESS,0x3c,I2C_MEMADD_SIZE_8BIT,(uint8_t*)accel_x,8,100);
    HAL_I2C_Mem_Read(&hi2c1,MPU9250_ADDRESS,0x3d,I2C_MEMADD_SIZE_8BIT,(uint8_t*)accel_y,8,100);
    accel_y<<0x08;
    HAL_I2C_Mem_Read(&hi2c1,MPU9250_ADDRESS,0x3e,I2C_MEMADD_SIZE_8BIT,(uint8_t*)accel_y,8,100);
    HAL_I2C_Mem_Read(&hi2c1,MPU9250_ADDRESS,0x3f,I2C_MEMADD_SIZE_8BIT,(uint8_t*)accel_z,8,100);
    accel_z<<0x08;
    HAL_I2C_Mem_Read(&hi2c1,MPU9250_ADDRESS,0x40,I2C_MEMADD_SIZE_8BIT,(uint8_t*)accel_z,8,100);


    HAL_I2C_Mem_Read(&hi2c1,MPU9250_ADDRESS,0x43,I2C_MEMADD_SIZE_8BIT,(uint8_t*)gyro_x,8,100);
    gyro_x<<0x08;
    HAL_I2C_Mem_Read(&hi2c1,MPU9250_ADDRESS,0x44,I2C_MEMADD_SIZE_8BIT,(uint8_t*)gyro_x,8,100);
    HAL_I2C_Mem_Read(&hi2c1,MPU9250_ADDRESS,0x45,I2C_MEMADD_SIZE_8BIT,(uint8_t*)gyro_y,8,100);
    gyro_y<<0x08;
    HAL_I2C_Mem_Read(&hi2c1,MPU9250_ADDRESS,0x46,I2C_MEMADD_SIZE_8BIT,(uint8_t*)gyro_y,8,100);
    HAL_I2C_Mem_Read(&hi2c1,MPU9250_ADDRESS,0x47,I2C_MEMADD_SIZE_8BIT,(uint8_t*)gyro_z,8,100);
    gyro_z<<0x08;
    HAL_I2C_Mem_Read(&hi2c1,MPU9250_ADDRESS,0x48,I2C_MEMADD_SIZE_8BIT,(uint8_t*)gyro_z,8,100);
};
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
