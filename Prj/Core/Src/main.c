/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
//#include "stm32f1xx_hal_i2c.h"

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
I2C_HandleTypeDef hi2c1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define MPU6050_ADDRESS_I2C     (uint8_t)0x68

#define MPU6050_REG_GYRO_CONFIG (uint8_t)0x1B
#define MPU6050_REG_ACCEL_CONFIG    (uint8_t)0x1C

#define MPU6050_REG_ACCEL_XOUT_H (uint8_t)0x3B
#define MPU6050_REG_ACCEL_XOUT_L (uint8_t)0x3C
#define MPU6050_REG_ACCEL_YOUT_H (uint8_t)0x3D
#define MPU6050_REG_ACCEL_YOUT_L (uint8_t)0x3E
#define MPU6050_REG_ACCEL_ZOUT_H (uint8_t)0x3F
#define MPU6050_REG_ACCEL_ZOUT_L (uint8_t)0x40


void MPU6050_Read(uint8_t I2C_ADDRESS, uint8_t reg_addr, uint8_t *p_tx_buf, uint8_t tx_buf_size);
void MPU6050_Write(uint8_t I2C_ADDRESS, uint8_t *p_rx_buf, uint8_t rx_buf_size);
void MPU6050_Init(void);
void MPU6050_GetAllData(int16_t *data);

void MPU6050_Read(uint8_t I2C_ADDRESS, uint8_t reg_addr, uint8_t *p_tx_buf, uint8_t tx_buf_size)
{
  MPU6050_Write(I2C_ADDRESS, &reg_addr, 1);
  while(HAL_I2C_Master_Transmit(&hi2c1, I2C_ADDRESS<<1, p_tx_buf, tx_buf_size, 100) != HAL_OK)
  {
    if(HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
    {
      Error_Handler();
    }
  }
  
  while(HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY){}
}

void MPU6050_Write(uint8_t I2C_ADDRESS, uint8_t *p_rx_buf, uint8_t rx_buf_size)
{
  while(HAL_I2C_Master_Receive(&hi2c1, I2C_ADDRESS<<1, p_rx_buf, rx_buf_size, 100) != HAL_OK)
  {
    Error_Handler();
  }
  while(HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY){}
}

void MPU6050_Init(void)
{
  uint8_t buf[7];
// включение модуля
  buf[0] = 0x6B; // 01101011
  buf[1] = 0x00;
  MPU6050_Write(MPU6050_ADDRESS_I2C, buf, 2);
// конфигурация гироскопа
  buf[0] = MPU6050_REG_GYRO_CONFIG;
  buf[1] = 0x08;
  MPU6050_Write(MPU6050_ADDRESS_I2C, buf, 2);
// конфигурация акселерометра 
  buf[0] = MPU6050_REG_ACCEL_CONFIG;
  buf[1] = 0x10;
  MPU6050_Write(MPU6050_ADDRESS_I2C, buf, 2);
}

void MPU6050_Calibrate(void)
{
  int16_t buf[6];
  uint16_t meg_num = 1000;
  
  for(int i = 0; i < meg_num; ++i)
  {
    MPU6050_GetAllData(buf);
    
  }
}

void MPU6050_GetAllData(int16_t *data)
{
  uint8_t buf[14];
  
  MPU6050_Read(MPU6050_ADDRESS_I2C, MPU6050_REG_ACCEL_XOUT_H, buf, 14);
  
  for(int i = 0; i < 3; ++i)
  {
    data[i] = (int16_t)((uint16_t)buf[2*i+1]);
    data[i] |= (int16_t)((uint16_t)buf[2*i]<<8);
  }
  
  for(int i = 4; i < 7; ++i)
  {
    data[i-1] = (int16_t)((uint16_t)buf[2*i+1]);
    data[i-1] |= (int16_t)((uint16_t)buf[2*i]<<8);
  }
}

int16_t gy_data[14];
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
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  MPU6050_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    MPU6050_GetAllData(gy_data);
    HAL_Delay(100);
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

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