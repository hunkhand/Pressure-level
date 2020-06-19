/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "menu_uart.h"
#include "In_Flash.h"
#include "usart_app.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef  void (*pFunction)(void);

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
static ModBus_Param_TypeDef ModBus_Param_Obj;
static uint32_t ModBus_RX_Buf[USART_ASCII_MAX_DATALEN / 4];

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static void ModBus_Param_Init(ModBus_Param_TypeDef *ModBus_Param)
{
    ModBus_Param->Device_Addr   = Read_Memory_1_Byte(ADDR_DEVICEADDR);
    ModBus_Param->ProgErase     = Read_Memory_1_Byte(ADDR_ERASEFLAG);
    ModBus_Param->RX_buf        = (uint8_t *)ModBus_RX_Buf;
}

static void JumpToUserApplication(void)
{
    uint32_t JumpAddress;                                    //��ת��ַ
    pFunction Jump_To_Application;                           //��ת��Ӧ�ó���
    
    SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
    
    /* Jump to user application */
    JumpAddress = *(__IO uint32_t*) (APPLICATION_ADDRESS + 4);
    Jump_To_Application = (pFunction) JumpAddress;

    /* Initialize user application's Stack Pointer */
    __set_MSP(*(__IO uint32_t*) APPLICATION_ADDRESS);

    /* Jump to application */
    Jump_To_Application();
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
    uint8_t Upgrade_Flag;
    uint32_t Old_Tick;

    HAL_Init();
    SystemClock_Config();
    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    ModBus_Param_Init(&ModBus_Param_Obj);
    Sensor_USART_Init(USART_BAUDRATE_9600_CODE, USART_PARITY_NONE_CODE);
    Upgrade_Flag = Read_Memory_1_Byte(ADDR_UPGRADEFLAG);

    if(Upgrade_Flag == UPGRADE_FLAG_NONE 
        || ModBus_Param_Obj.ProgErase == ERASE_FLAG_NONE)
    {
      ModBus_Param_Obj.UpgradeWaitTime = -1;
    }
    else
    {
        ModBus_Param_Obj.UpgradeWaitTime = 2;
    }
    Old_Tick = HAL_GetTick();
    while (1)
    {
        if(ModBus_Param_Obj.UpgradeWaitTime >= 0)
        {
            if(HAL_GetTick() > (ModBus_Param_Obj.UpgradeWaitTime * 1000 + Old_Tick))          //�ȴ�ʱ�䵽
            {
                break;
            }
        }

        if(Sensor_USART_Get_RX_Updata_Flag())                                               //�������������                                                      
        {
            Sensor_USART_Clear_RX_Updata_Flag();
            HandleFrameInformation(&ModBus_Param_Obj);                                        //������յ�֡����
        }
    }
    if (((*(__IO uint32_t*)APPLICATION_ADDRESS) & 0x2FFE0000 ) == 0x20000000)
    {
        if(ModBus_Param_Obj.ProgErase != ERASE_FLAG)
        {
            Write_Memory_1_Byte(ADDR_ERASEFLAG, ERASE_FLAG);        //��Ӧ�ó������ǲ�����־
        }
        Write_Memory_1_Byte(ADDR_UPGRADEFLAG, UPGRADE_FLAG);       //��Ӧ�ó�������������־
        Write_Memory_1_Byte(ADDR_DEVICEADDR, UPGRADED_DEVICEADDR);
        while(Sensor_USART_Get_TX_Cplt_Flag() == 0);
        BSP_USART_UART_DeInit();
        JumpToUserApplication();                        //ִ��Ӧ�ó���
    }

    while(1)
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

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

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
