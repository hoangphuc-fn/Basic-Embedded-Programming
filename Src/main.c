#include "main.h"
#include "usart.h"
#include "gpio.h"

void SystemClock_Config(void);
void menu(void);
void menu_2(void);
void reset_value(void);
void bai1(void);
void bai2(void);
void bai3(void);
void plus(void);
char data_send[100];
char operator_1[10];
char operator_2[10];
uint8_t volatile operation_flag = 0;
uint8_t volatile is_complete = 0;

uint8_t volatile selection;
uint8_t volatile isOperator_2 = 0;
uint8_t volatile cnt2 = 0;
uint8_t volatile array_index =0;
int main(void)
{

  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_UART4_Init();
	HAL_UART_Receive_IT(&huart4,(uint8_t *)&selection,1);
	menu();
  while (1){
	  switch(selection){
			case '1': 
				bai1();
				break;
			case '2':
				bai2();
				break;
			case '3':
				bai3();
				break;
			// show menu
			case 0x1B:
				menu();
				break;
		}
  }
}

void reset_value(void){
	operation_flag = 1;
	isOperator_2 = 0;
	cnt2 = 0;
	array_index =0;
	for(int i=0; i<10; i++){
		operator_1[i] = 0;
		operator_2[i] = 0;
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==huart4.Instance)
	{
		if(selection == 'a'){
			operation_flag = 1;
		}
		if(operation_flag){
			if(!isOperator_2){
				if(selection>=48 && selection <=57){
					operator_1[array_index++] = selection;
				}
				else if(selection == 10) {
					isOperator_2 = 1;
					array_index = 0;
				}
			}
			else if(isOperator_2){
				if(selection>=48 && selection <=57){
					operator_2[array_index++] = selection;
				}
				else if(selection == 10) {
					operation_flag = 0;
					array_index = 0;
				}
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
	selection = 2;
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
	sprintf(data_send, "ESC: Return previous menu\n");
	UART_Print(&huart4,data_send);
	while(HAL_UART_GetState(&huart4)!= HAL_UART_STATE_BUSY_RX);
	while(selection != 27);
}
void bai2(void){
	menu_2();
	switch(selection){
		case 'a':
			plus();
			reset_value();
			break;
		case 27:
			menu_2();
			break;
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
