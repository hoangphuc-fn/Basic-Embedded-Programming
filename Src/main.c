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
#include "spi.h"
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
#define EN_SPI	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
#define DIS_SPI HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void menu(void);
void menu_2(void);
void menu_4(void);
void reset_value(void);
void bai1(void);
void bai2(void);
void bai3(void);
void bai4(void);
void plus(void);
void setLED(void);
void setDirection(void);
void start(void);
void subtract(void);
void multiple(void);
void divide(void);
void return_bai4_menu(void);
char data_send[100];
char operator_1[10];
char operator_2[10];
uint8_t volatile bai4_data[2];
uint8_t volatile operation_flag = 0;
uint8_t volatile bai4_flag = 0;
uint8_t volatile is_complete = 0;
uint8_t volatile selection;
uint8_t volatile isOperator_2 = 0;
uint8_t volatile cnt2 = 0;
uint8_t volatile array_index =0;
uint8_t volatile is_bai2 = 0;
uint8_t volatile is_bai4 = 0;
uint8_t volatile count = 0;
uint8_t spi_send_data = 0;
/* USER CODE BEGIN PFP */

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
  SystemClock_Config();
  MX_GPIO_Init();
  MX_UART4_Init();
	MX_SPI1_Init();
	HAL_UART_Receive_IT(&huart4,(uint8_t *)&selection,1);
	
	menu();
  while (1){
	  switch(selection){
			case '1': 
				is_bai2 = 0;
				is_bai4 = 0;
				bai1();
				break;
			case '2':
				is_bai2 = 1;
				is_bai4 = 0;
				bai2();
				break;
			case '3':
				is_bai2 = 0;
				is_bai4 = 0;
				bai3();
				break;
			case '4':
				is_bai2 = 0;
				is_bai4 = 1;
				bai4();
				break;
			// show menu
			case 0x1B:
				menu();
				break;
		}
  }

}

void reset_value(void){
	operation_flag = 0;
	isOperator_2 = 0;
	bai4_flag = 0;
	cnt2 = 0;
	//is_bai2 = 0;
	is_bai4 = 0;
	array_index =0;
	for(int i=0; i<10; i++){
		operator_1[i] = 0;
		operator_2[i] = 0;
	}
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
	if (hspi->Instance == hspi1.Instance)
	{
		DIS_SPI;
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==huart4.Instance)
	{
		if(is_bai2){
			if(selection == 'a' || selection == 'b' || selection == 'c' ||selection == 'd'){
				operation_flag= 1;
			}
		}
		if(operation_flag){
			if(!isOperator_2){
				if(selection>=48 && selection <=57){
					operator_1[array_index++] = selection;
				}
				else if(selection == 8){
					operator_1[array_index-1] = 0;
					if(array_index >= 1) array_index --;
				}
				else if(selection == 10){
					isOperator_2 = 1;
					array_index = 0;
				}
			}
			else if(isOperator_2){
				if(selection>=48 && selection <=57){
					operator_2[array_index++] = selection;
				}
				else if(selection == 8){
					operator_2[array_index-1] = 0;
					if(array_index >= 1) array_index --;
				}
				else if(selection == 10) {
					operation_flag = 0;
					array_index = 0;
				}
			}
		}
		
		if (is_bai4){
			if((selection == 'a' || selection == 'b' || selection == 'c')){
				bai4_flag = 1;
			}
		}
		if (bai4_flag) {
			/* ---------------- SET LED ---------------- */
			if (selection == 'r'){
				bai4_data[count] = 'r';
				count++;
			}
			else if (selection == 'g'){
				bai4_data[count] = 'g';
				count++;
				}
				/* ---------------- SET DIRECTION ---------------- */
				else if (selection == 'c'){
					bai4_data[0] = 'c';
					count = 2;
				}
				else if (selection == 'a'){
					bai4_data[0] = 'a';
					count = 2;
				}
				// backspace
				else if(selection == 8){
					if (bai4_data[0] == 'a' || bai4_data[0] == 'c')
						count = 0;
					if (count >= 1)
						count--;
					bai4_data[count] = NULL;
				}
				
				else if(selection == 10 && count>=2) {
					count = 0;
					bai4_flag = 0;
				}
			}
		
		HAL_UART_Receive_IT(&huart4,(uint8_t *)&selection,1);
	}
}

void menu(void){
	sprintf(data_send, "Choose your option(1,2,3, ...)\n");
	UART_Print(&huart4,data_send);
	while(HAL_UART_GetState(&huart4)!= HAL_UART_STATE_BUSY_RX);
	sprintf(data_send, "1. Student Informations\n");
	UART_Print(&huart4,data_send);
	while(HAL_UART_GetState(&huart4)!= HAL_UART_STATE_BUSY_RX);
	sprintf(data_send, "2. Basic Operation\n");
	UART_Print(&huart4,data_send);
	while(HAL_UART_GetState(&huart4)!= HAL_UART_STATE_BUSY_RX);
	sprintf(data_send, "3. Simple Led\n");
	UART_Print(&huart4,data_send);
	while(HAL_UART_GetState(&huart4)!= HAL_UART_STATE_BUSY_RX);
	sprintf(data_send, "4. Advance Led\n");
	UART_Print(&huart4,data_send);
	while(HAL_UART_GetState(&huart4)!= HAL_UART_STATE_BUSY_RX);
	sprintf(data_send, "5. Audio\n");
	UART_Print(&huart4,data_send);
	while(HAL_UART_GetState(&huart4)!= HAL_UART_STATE_BUSY_RX);
	selection = 0;
}
void menu_2(void){
	sprintf(data_send, "2. Basic Operation\n");
	UART_Print(&huart4,data_send);
	while(HAL_UART_GetState(&huart4)!= HAL_UART_STATE_BUSY_RX);
	sprintf(data_send, "a. Plus\n");
	UART_Print(&huart4,data_send);
	while(HAL_UART_GetState(&huart4)!= HAL_UART_STATE_BUSY_RX);
	sprintf(data_send, "b. Subtract\n");
	UART_Print(&huart4,data_send);
	while(HAL_UART_GetState(&huart4)!= HAL_UART_STATE_BUSY_RX);
	sprintf(data_send, "c. Multiple\n");
	UART_Print(&huart4,data_send);
	while(HAL_UART_GetState(&huart4)!= HAL_UART_STATE_BUSY_RX);
	sprintf(data_send, "d. Divide\n");
	UART_Print(&huart4,data_send);
	while(HAL_UART_GetState(&huart4)!= HAL_UART_STATE_BUSY_RX);
	sprintf(data_send, "e. Module\n");
	UART_Print(&huart4,data_send);
	while(HAL_UART_GetState(&huart4)!= HAL_UART_STATE_BUSY_RX);
	sprintf(data_send, "ESC: Return previous menu\n");
	UART_Print(&huart4,data_send);
	while(HAL_UART_GetState(&huart4)!= HAL_UART_STATE_BUSY_RX);
}

void bai1(void){
	sprintf(data_send, "1. Student Informations\n");
	UART_Print(&huart4,data_send);
	while(HAL_UART_GetState(&huart4)!= HAL_UART_STATE_BUSY_RX);
	sprintf(data_send, "ID: 16520946\n");
	UART_Print(&huart4,data_send);
	while(HAL_UART_GetState(&huart4)!= HAL_UART_STATE_BUSY_RX);
	sprintf(data_send, "Full Name: Le Hoang Phuc\n");
	UART_Print(&huart4,data_send);
	while(HAL_UART_GetState(&huart4)!= HAL_UART_STATE_BUSY_RX);
	sprintf(data_send, "ID: 16521006\n");
	UART_Print(&huart4,data_send);
	while(HAL_UART_GetState(&huart4)!= HAL_UART_STATE_BUSY_RX);
	sprintf(data_send, "Full Name: Hoang Anh Quoc\n");
	UART_Print(&huart4,data_send);
	while(HAL_UART_GetState(&huart4)!= HAL_UART_STATE_BUSY_RX);
	sprintf(data_send, "ESC: Return previous menu\n");
	UART_Print(&huart4,data_send);
	while(HAL_UART_GetState(&huart4)!= HAL_UART_STATE_BUSY_RX);
	while(selection != 27);
}

void bai2(void){
	while(selection!=27){
		menu_2();
		while(selection != 'a' && selection != 'b' && selection != 'c' && selection != 'd'  && selection != 27);
		switch(selection){
			case 'a':
				plus();
				reset_value();
				selection = 0;
				break;
			case 'b':
				subtract();
				reset_value();
			selection = 0;
				break;
			case 'c':
				multiple();
				reset_value();
			selection = 0;
				break;
			case 'd':
				divide();
				reset_value();
			selection = 0;
				break;
			
		}
	}
}
void plus(void){
	sprintf(data_send, "a. Plus\n");
	UART_Print(&huart4,data_send);
	while(HAL_UART_GetState(&huart4)!= HAL_UART_STATE_BUSY_RX);
	sprintf(data_send, "Operand 1: ");
	UART_Print(&huart4,data_send);
	while(HAL_UART_GetState(&huart4)!= HAL_UART_STATE_BUSY_RX);
	while(!isOperator_2);
	sprintf(data_send, "Operand 2: ");
	UART_Print(&huart4,data_send);
	while(HAL_UART_GetState(&huart4)!= HAL_UART_STATE_BUSY_RX);
	while(operation_flag);
	sprintf(data_send, "Result: %d\n", atoi(operator_1) + atoi(operator_2));
	UART_Print(&huart4,data_send);
	while(HAL_UART_GetState(&huart4)!= HAL_UART_STATE_BUSY_RX);
	sprintf(data_send, "ESC: Return previous menu\n");
	UART_Print(&huart4,data_send);
	while(HAL_UART_GetState(&huart4)!= HAL_UART_STATE_BUSY_RX);
	while(selection != 27);
}
void subtract(void){
	sprintf(data_send, "b. Subtract\n");
	UART_Print(&huart4,data_send);
	while(HAL_UART_GetState(&huart4)!= HAL_UART_STATE_BUSY_RX);
	sprintf(data_send, "Operand 1: ");
	UART_Print(&huart4,data_send);
	while(HAL_UART_GetState(&huart4)!= HAL_UART_STATE_BUSY_RX);
	while(!isOperator_2);
	sprintf(data_send, "Operand 2: ");
	UART_Print(&huart4,data_send);
	while(HAL_UART_GetState(&huart4)!= HAL_UART_STATE_BUSY_RX);
	while(operation_flag);
	sprintf(data_send, "Result: %d\n", atoi(operator_1) - atoi(operator_2));
	UART_Print(&huart4,data_send);
	while(HAL_UART_GetState(&huart4)!= HAL_UART_STATE_BUSY_RX);
	sprintf(data_send, "ESC: Return previous menu\n");
	UART_Print(&huart4,data_send);
	while(HAL_UART_GetState(&huart4)!= HAL_UART_STATE_BUSY_RX);
	while(selection != 27);
}
void multiple(void){
	sprintf(data_send, "c. Multiple\n");
	UART_Print(&huart4,data_send);
	while(HAL_UART_GetState(&huart4)!= HAL_UART_STATE_BUSY_RX);
	sprintf(data_send, "Operand 1: ");
	UART_Print(&huart4,data_send);
	while(HAL_UART_GetState(&huart4)!= HAL_UART_STATE_BUSY_RX);
	while(!isOperator_2);
	sprintf(data_send, "Operand 2: ");
	UART_Print(&huart4,data_send);
	while(HAL_UART_GetState(&huart4)!= HAL_UART_STATE_BUSY_RX);
	while(operation_flag);
	sprintf(data_send, "Result: %d\n", atoi(operator_1) * atoi(operator_2));
	UART_Print(&huart4,data_send);
	while(HAL_UART_GetState(&huart4)!= HAL_UART_STATE_BUSY_RX);
	sprintf(data_send, "ESC: Return previous menu\n");
	UART_Print(&huart4,data_send);
	while(HAL_UART_GetState(&huart4)!= HAL_UART_STATE_BUSY_RX);
	while(selection != 27);
}
void divide(void){
	sprintf(data_send, "d. Divide\n");
	UART_Print(&huart4,data_send);
	while(HAL_UART_GetState(&huart4)!= HAL_UART_STATE_BUSY_RX);
	sprintf(data_send, "Operand 1: ");
	UART_Print(&huart4,data_send);
	while(HAL_UART_GetState(&huart4)!= HAL_UART_STATE_BUSY_RX);
	while(!isOperator_2);
	sprintf(data_send, "Operand 2: ");
	UART_Print(&huart4,data_send);
	while(HAL_UART_GetState(&huart4)!= HAL_UART_STATE_BUSY_RX);
	while(operation_flag);
	sprintf(data_send, "Result: %d\n", atoi(operator_1) / atoi(operator_2));
	UART_Print(&huart4,data_send);
	while(HAL_UART_GetState(&huart4)!= HAL_UART_STATE_BUSY_RX);
	sprintf(data_send, "ESC: Return previous menu\n");
	UART_Print(&huart4,data_send);
	while(HAL_UART_GetState(&huart4)!= HAL_UART_STATE_BUSY_RX);
	while(selection != 27);
}
void bai3(void){
	sprintf(data_send, "ESC: Return previous menu\n");
	UART_Print(&huart4,data_send);
	while(HAL_UART_GetState(&huart4)!= HAL_UART_STATE_BUSY_RX);
	while(selection != 27){
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
		HAL_Delay(200);
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
		HAL_Delay(200);
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
		HAL_Delay(200);
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
		HAL_Delay(200);
	}
}


void menu_4(void)
{
	sprintf(data_send, "4. Advance led (a, b, c):\n");
	UART_Print(&huart4,data_send);
	while(HAL_UART_GetState(&huart4)!= HAL_UART_STATE_BUSY_RX);
	sprintf(data_send, "a. Set LED\n");
	UART_Print(&huart4,data_send);
	while(HAL_UART_GetState(&huart4)!= HAL_UART_STATE_BUSY_RX);
	sprintf(data_send, "b. Set direction\n");
	UART_Print(&huart4,data_send);
	while(HAL_UART_GetState(&huart4)!= HAL_UART_STATE_BUSY_RX);
	sprintf(data_send, "c. Start\n");
	UART_Print(&huart4,data_send);
	while(HAL_UART_GetState(&huart4)!= HAL_UART_STATE_BUSY_RX);
	sprintf(data_send, "ESC: Return previous menu\n");
	UART_Print(&huart4,data_send);
	while(HAL_UART_GetState(&huart4)!= HAL_UART_STATE_BUSY_RX);
}

void return_bai4_menu(void)
{
	menu_4();
	selection = NULL;
}

void bai4(void){
	menu_4();
	while(selection != 'a' && selection != 'b' && selection != 'c' && selection != (char) 27);
	while (selection != (char) 27)
	{
		switch(selection){
			case 'a':
				setLED();
				return_bai4_menu();
				break;
			case 'b':
				setDirection();
				return_bai4_menu();
				break;
			case 'c':
				start();
				return_bai4_menu();
				break;
		}
	}
}

void setLED(void)
{
	sprintf(data_send, "a. Set LED (r, g):\n");
	UART_Print(&huart4,data_send);
	while(HAL_UART_GetState(&huart4)!= HAL_UART_STATE_BUSY_RX);
	while(bai4_flag);
	/* --------------------- Send SPI --------------------- */
	EN_SPI;
	uint8_t temp_data = bai4_data[0];
	HAL_SPI_Transmit_IT(&hspi1, &temp_data, 1);
	HAL_Delay(5);
	EN_SPI;
	temp_data = bai4_data[1];
	HAL_SPI_Transmit_IT(&hspi1, &temp_data, 1);
	HAL_Delay(5);
	/* --------------------- ESC to return --------------------- */
	sprintf(data_send, "ESC: Return previous menu\n");
	UART_Print(&huart4,data_send);
	while(HAL_UART_GetState(&huart4)!= HAL_UART_STATE_BUSY_RX);
	while(selection != 27);
}

void setDirection(void)
{
	sprintf(data_send, "a. Set Direction (c, a):\n");
	UART_Print(&huart4,data_send);
	while(HAL_UART_GetState(&huart4)!= HAL_UART_STATE_BUSY_RX);
	while(bai4_flag);
	/* --------------------- Send SPI --------------------- */
	uint8_t temp_data = bai4_data[0];
	EN_SPI;
	HAL_SPI_Transmit_IT(&hspi1, &temp_data, 1);
	HAL_Delay(5);
	/* --------------------- ESC to return --------------------- */
	sprintf(data_send, "ESC: Return previous menu\n");
	UART_Print(&huart4,data_send);
	while(HAL_UART_GetState(&huart4)!= HAL_UART_STATE_BUSY_RX);
	while(selection != 27);
}

void start(void)
{
	sprintf(data_send, "Running...\n");
	UART_Print(&huart4,data_send);
	while(HAL_UART_GetState(&huart4)!= HAL_UART_STATE_BUSY_RX);
	sprintf(data_send, "Press button to start LED\n");
	UART_Print(&huart4,data_send);
	while(HAL_UART_GetState(&huart4)!= HAL_UART_STATE_BUSY_RX);
	sprintf(data_send, "Press ESC to return previous menu\n");
	UART_Print(&huart4,data_send);
	while(HAL_UART_GetState(&huart4)!= HAL_UART_STATE_BUSY_RX);
	
	/* --------------------- ESC to return --------------------- */
	uint8_t temp_data = 's';
	EN_SPI;
	HAL_SPI_Transmit_IT(&hspi1, &temp_data, 1);
	HAL_Delay(5);
	temp_data = 'b';
	while(selection != 27)
	{
		if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0))
		{
			// delay anti vibrate
			HAL_Delay(100);
			// send data
			EN_SPI;
			HAL_SPI_Transmit_IT(&hspi1, &temp_data, 1);
			HAL_Delay(5);
		}
	}
	temp_data = 'x';
	EN_SPI;
	HAL_SPI_Transmit_IT(&hspi1, &temp_data, 1);
	HAL_Delay(5);
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
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

void Error_Handler(void){
}


