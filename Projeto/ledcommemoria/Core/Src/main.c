/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "TM1637.h"
#include "EEPROM.h"
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

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
extern I2C_HandleTypeDef hi2c1;
#define EEPROM_I2C &hi2c1

// EEPROM ADDRESS (8bits)
#define EEPROM_ADDR 0x50

// Define the Page Size and number of pages
#define PAGE_SIZE 3     // in Bytes
#define PAGE_NUM  9    // number of pages

uint8_t StartMSG[] = "\n\n\n\n\n\nStarting program . . . \n ";
uint8_t Senhasalva[] = "Senha salva: ";
uint8_t Senharepetida[] = "Senha repetida nao salva: ";
uint8_t Loginfeito[] = "Login feito: ";
uint8_t Loginerro[] = "Senha incorreta: ";
uint8_t Memoriacheia[] = "Numero maximo de senhas alcancado, senha nao salva. ";
uint8_t Erro[] = "Erro ";
uint8_t Space[] = "\n\n ";

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

void reseta_banco_senha(){
	uint8_t dataWrite[5];
	for (int j=1; j<9; j++)
	  {
		for (int i=0; i<9; i++)
		  	  {
		  		  dataWrite[i] = 10;
		  	  }
	  	EEPROM_Write(j, 0, dataWrite, 9);
	  }
		for (int i=0; i<5; i++)
		  	  {
		  		  dataWrite[i] = 0;
		  	  }
	  	EEPROM_Write(0, 0, dataWrite, 9);
}

unsigned int salva_senha(int *vetorSenhas[], int conta){

	uint8_t dataRead[5];
	uint8_t dataWrite[5];
	uint8_t new[1];

	for (int i=0; i<5; i++)
		  {
			  dataWrite[i] = 10;

		  }

	for (int i=0; i<conta; i++)
	  {
		  dataWrite[i] = vetorSenhas[i];
		  dataWrite[i] = dataWrite[i]-48;
	  }
	dataWrite[4] = conta;

	  EEPROM_Read(0, 0, dataRead, 5);

	  if(dataRead[0]<8){
		  new[0] = dataRead[0]+1;
	  	  EEPROM_Write(0, 0, new, 1);
		  EEPROM_Write(new[0], 0, dataWrite, 5);

		  return 1;
	  }
	  else{
		  return 0;
	  }
}

unsigned int valida_senha(int *vetorSenhas[], int conta){

	uint8_t dataRead[5];
	uint8_t dataWrite[5];

	  int verif = 0;

		for (int i=0; i<5; i++)
			  {
				  dataWrite[i] = 10;

			  }

		for (int i=0; i<conta; i++)
		  {
			  dataWrite[i] = vetorSenhas[i];
			  dataWrite[i] = dataWrite[i]-48;
		  }
		dataWrite[4] = conta;

	  for (int j=1; j<9; j++)
	  {
		  EEPROM_Read(j, 0, dataRead, 9);
		  verif = 0;

		  for(int i=0; i<5; i++){

			  if(dataRead[i] == dataWrite[i]){
				  verif++;
			  }
		  }
		  if(verif==5){
			  return 1;
		  }
	  }
	  return 0;
}


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
volatile uint8_t displayColon = 0;
unsigned int data_read = 0;
uint8_t dataRead[5];
uint8_t dataWrite[5];
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
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */


/*  reseta_banco_senha();

  for (int i=0; i<4; i++)
  {
	  dataWrite[i] = i+1;
  }

  EEPROM_Write(0, 0, dataWrite, 4);
  EEPROM_Read(0, 0, dataRead, 4);

  int verif = 0;
  for(int i=0; i<4; i++){
	  if(dataRead[i] == i+1){
		  verif++;
	  }
  }
  if(verif==4){
	  EEPROM_PageErase(0);
	  EEPROM_Read(0, 0, dataRead, 4);
  }
	*/


  //reseta_banco_senha();
  EEPROM_Read(0, 0, dataRead, 4);
  unsigned int senha = 0;

  unsigned int vetorSenhas[4];
  for (int i = 0; i < 4; ++i) {
	  vetorSenhas[i] = 0;
  }

  int conta = 0;
  data_read = 0;
  unsigned char myData;

  unsigned char digitArr[4];
  for (int i = 0; i < 4; ++i) {
      digitArr[i] = 0x08;
  }

  TM1637_Start();
  TM1637_WriteByte(0x40);
  TM1637_ReadResult();
  TM1637_Stop();

  TM1637_Start();
  TM1637_WriteByte(0xc0);
  TM1637_ReadResult();

  for (int i = 0; i < 4; ++i) {
      TM1637_WriteByte(digitArr[3 - i]);
      TM1637_ReadResult();
  }

  TM1637_Stop();

  HAL_UART_Transmit(&huart1, Space, sizeof(Space)-1, 100);
  HAL_UART_Transmit(&huart1, StartMSG, sizeof(StartMSG)-1, 100);
  HAL_UART_Transmit(&huart1, Space, sizeof(Space)-1, 100);


  //pisca pisca
  HAL_Delay(200);
  TM1637_SetBrightness(0);
  HAL_Delay(200);
  TM1637_SetBrightness(7);
  HAL_Delay(200);
  TM1637_SetBrightness(0);
  HAL_Delay(200);
  TM1637_SetBrightness(7);
  HAL_Delay(200);
  TM1637_SetBrightness(0);
  HAL_Delay(200);
  TM1637_SetBrightness(7);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  char pressionado = 'a';

	      for (int i = 0; i < 4; i++) {

	        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
	        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
	        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);
	        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
	        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
	        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
	        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);

	        if (i == 0) {
	          HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
	        } else if (i == 1) {
	          HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
	        } else if (i == 2) {
	          HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
	        } else if (i == 3) {
	          HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
	        }

	        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) == 0) {

	          if (i == 0) {
	            pressionado = '1';
	            if(conta < 4){
	            	  digitArr[conta] = 0x06;

	            	  TM1637_Start();
	            	  TM1637_WriteByte(0x40);
	            	  TM1637_ReadResult();
	            	  TM1637_Stop();

	            	  TM1637_Start();
	            	  TM1637_WriteByte(0xc0);
	            	  TM1637_ReadResult();

	            	  for (int i = 0; i < 4; ++i) {
	            	      TM1637_WriteByte(digitArr[i]);
	            	      TM1637_ReadResult();
	            	  }

	            	  TM1637_Stop();

		            	senha = (senha*10)+1;
		            	vetorSenhas[conta]= 49;
		            	conta++;
		            	HAL_Delay(500);
	            }

	          } else if (i == 1) {
	            pressionado = '4';
	            if(conta < 4){
	            	  digitArr[conta] = 0x66;

	            	  TM1637_Start();
	            	  TM1637_WriteByte(0x40);
	            	  TM1637_ReadResult();
	            	  TM1637_Stop();

	            	  TM1637_Start();
	            	  TM1637_WriteByte(0xc0);
	            	  TM1637_ReadResult();

	            	  for (int i = 0; i < 4; ++i) {
	            	      TM1637_WriteByte(digitArr[i]);
	            	      TM1637_ReadResult();
	            	  }

	            	  TM1637_Stop();

		            	senha = (senha*10)+4;
		            	vetorSenhas[conta]= 52;
		            	conta++;
		            	HAL_Delay(500);
	            }

	          } else if (i == 2) {
	            pressionado = '7';
	            if(conta < 4){
					  digitArr[conta] = 0x07;

					  TM1637_Start();
					  TM1637_WriteByte(0x40);
					  TM1637_ReadResult();
					  TM1637_Stop();

					  TM1637_Start();
					  TM1637_WriteByte(0xc0);
					  TM1637_ReadResult();

					  for (int i = 0; i < 4; ++i) {
						  TM1637_WriteByte(digitArr[i]);
						  TM1637_ReadResult();
					  }

					  TM1637_Stop();

						senha = (senha*10)+7;
						vetorSenhas[conta]= 55;
						conta++;
						HAL_Delay(500);
				}

	          } else if (i == 3) {
	            pressionado = '*';

	            if(valida_senha(vetorSenhas,conta)==1){
						if(conta-1 == -1){//caso senha nula

			            	// ERRO

			            digitArr[0] = 0x79;
			            digitArr[1] = 0x50;
			            digitArr[2] = 0x50;
			            digitArr[3] = 0x5c;


			            TM1637_Start();
			            TM1637_WriteByte(0x40);
			            TM1637_ReadResult();
			            TM1637_Stop();

			            TM1637_Start();
			            TM1637_WriteByte(0xc0);
			            TM1637_ReadResult();

			            for (int i = 0; i < 4; ++i) {
			                TM1637_WriteByte(digitArr[i]);
			                TM1637_ReadResult();
			            }

			            TM1637_Stop();
							HAL_UART_Transmit(&huart1, Erro, sizeof(Erro)-1, 100);
							HAL_Delay(2000);
						}


						else{//senhas válidas

								// OPEN

							digitArr[0] = 0x5c;
							digitArr[1] = 0x73;
							digitArr[2] = 0x79;
							digitArr[3] = 0x54;


							TM1637_Start();
							TM1637_WriteByte(0x40);
							TM1637_ReadResult();
							TM1637_Stop();

							TM1637_Start();
							TM1637_WriteByte(0xc0);
							TM1637_ReadResult();

							for (int i = 0; i < 4; ++i) {
								TM1637_WriteByte(digitArr[i]);
								TM1637_ReadResult();
							}

							TM1637_Stop();
							HAL_UART_Transmit(&huart1, Loginfeito, sizeof(Loginfeito)-1, 100);


							for(int k=0;k<conta;k++){
								myData=vetorSenhas[k];
								HAL_UART_Transmit(&huart1, &myData, 1, 100);
							}



						}
						  HAL_UART_Transmit(&huart1, Space, sizeof(Space)-1, 100);
							HAL_Delay(2000);

	            }
	            if(valida_senha(vetorSenhas,conta)==0){
		            	if(conta-1 == -1){//caso senha nula

			            	// ERRO

			            digitArr[0] = 0x79;
			            digitArr[1] = 0x50;
			            digitArr[2] = 0x50;
			            digitArr[3] = 0x5c;


			            TM1637_Start();
			            TM1637_WriteByte(0x40);
			            TM1637_ReadResult();
			            TM1637_Stop();

			            TM1637_Start();
			            TM1637_WriteByte(0xc0);
			            TM1637_ReadResult();

			            for (int i = 0; i < 4; ++i) {
			                TM1637_WriteByte(digitArr[i]);
			                TM1637_ReadResult();
			            }

			            TM1637_Stop();
		            		HAL_UART_Transmit(&huart1, Erro, sizeof(Erro)-1, 100);
							HAL_Delay(2000);

		            	}
		            	else{//senhas válidas

			            	// ERRO

			            digitArr[0] = 0x79;
			            digitArr[1] = 0x50;
			            digitArr[2] = 0x50;
			            digitArr[3] = 0x5c;


			            TM1637_Start();
			            TM1637_WriteByte(0x40);
			            TM1637_ReadResult();
			            TM1637_Stop();

			            TM1637_Start();
			            TM1637_WriteByte(0xc0);
			            TM1637_ReadResult();

			            for (int i = 0; i < 4; ++i) {
			                TM1637_WriteByte(digitArr[i]);
			                TM1637_ReadResult();
			            }

			            TM1637_Stop();

							  HAL_UART_Transmit(&huart1, Loginerro, sizeof(Loginerro)-1, 100);


		            		for(int k=0;k<conta;k++){
		            			myData=vetorSenhas[k];
		            			HAL_UART_Transmit(&huart1, &myData, 1, 100);
		            		}
							HAL_Delay(2000);
		            	}
						HAL_UART_Transmit(&huart1, Space, sizeof(Space)-1, 100);



	            }

	            senha = 0;
	            for (int i = 0; i < 4; ++i) {
	                digitArr[i] = 0x08;
	            }

	            TM1637_Start();
	            TM1637_WriteByte(0x40);
	            TM1637_ReadResult();
	            TM1637_Stop();

	            TM1637_Start();
	            TM1637_WriteByte(0xc0);
	            TM1637_ReadResult();

	            for (int i = 0; i < 4; ++i) {
	                TM1637_WriteByte(digitArr[3 - i]);
	                TM1637_ReadResult();
	            }

	            TM1637_Stop();
	            conta = 0;

	            HAL_Delay(200);
	            TM1637_SetBrightness(0);
	            HAL_Delay(200);
	            TM1637_SetBrightness(7);
	            HAL_Delay(200);
	            TM1637_SetBrightness(0);
	            HAL_Delay(200);
	            TM1637_SetBrightness(7);
	            HAL_Delay(200);
	            TM1637_SetBrightness(0);
	            HAL_Delay(200);
	            TM1637_SetBrightness(7);
	          }
	        }else if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == 0) {
	          if (i == 0) {
	            pressionado = '2';
	            if(conta < 4){
					  digitArr[conta] = 0x5b;

					  TM1637_Start();
					  TM1637_WriteByte(0x40);
					  TM1637_ReadResult();
					  TM1637_Stop();

					  TM1637_Start();
					  TM1637_WriteByte(0xc0);
					  TM1637_ReadResult();

					  for (int i = 0; i < 4; ++i) {
						  TM1637_WriteByte(digitArr[i]);
						  TM1637_ReadResult();
					  }

					  TM1637_Stop();

						senha = (senha*10)+2;
						vetorSenhas[conta]= 50;
						conta++;
						HAL_Delay(500);
				}

	          } else if (i == 1) {
	            pressionado = '5';
	            if(conta < 4){
					  digitArr[conta] = 0x6d;

					  TM1637_Start();
					  TM1637_WriteByte(0x40);
					  TM1637_ReadResult();
					  TM1637_Stop();

					  TM1637_Start();
					  TM1637_WriteByte(0xc0);
					  TM1637_ReadResult();

					  for (int i = 0; i < 4; ++i) {
						  TM1637_WriteByte(digitArr[i]);
						  TM1637_ReadResult();
					  }

					  TM1637_Stop();

						senha = (senha*10)+5;
						vetorSenhas[conta]= 53;
						conta++;
						HAL_Delay(500);
				}

	          } else if (i == 2) {
	            pressionado = '8';
	            if(conta < 4){
					  digitArr[conta] = 0x7f;

					  TM1637_Start();
					  TM1637_WriteByte(0x40);
					  TM1637_ReadResult();
					  TM1637_Stop();

					  TM1637_Start();
					  TM1637_WriteByte(0xc0);
					  TM1637_ReadResult();

					  for (int i = 0; i < 4; ++i) {
						  TM1637_WriteByte(digitArr[i]);
						  TM1637_ReadResult();
					  }

					  TM1637_Stop();

						senha = (senha*10)+8;
						vetorSenhas[conta]= 56;
						conta++;
						HAL_Delay(500);
				}

	          } else if (i == 3) {
	            pressionado = '0';
	            if(conta < 4){
					  digitArr[conta] = 0x3f;

					  TM1637_Start();
					  TM1637_WriteByte(0x40);
					  TM1637_ReadResult();
					  TM1637_Stop();

					  TM1637_Start();
					  TM1637_WriteByte(0xc0);
					  TM1637_ReadResult();

					  for (int i = 0; i < 4; ++i) {
						  TM1637_WriteByte(digitArr[i]);
						  TM1637_ReadResult();
					  }

					  TM1637_Stop();

						senha = (senha*10)+0;
						vetorSenhas[conta]= 48;
						conta++;
						HAL_Delay(500);
				}

	          }
	        }else if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6) == 0) {
	          if (i == 0) {
	            pressionado = '3';
	            if(conta < 4){
					  digitArr[conta] = 0x4f;

					  TM1637_Start();
					  TM1637_WriteByte(0x40);
					  TM1637_ReadResult();
					  TM1637_Stop();

					  TM1637_Start();
					  TM1637_WriteByte(0xc0);
					  TM1637_ReadResult();

					  for (int i = 0; i < 4; ++i) {
						  TM1637_WriteByte(digitArr[i]);
						  TM1637_ReadResult();
					  }

					  TM1637_Stop();

						senha = (senha*10)+3;
						vetorSenhas[conta]= 51;
						conta++;
						HAL_Delay(500);
				}

	          } else if (i == 1) {
	            pressionado = '6';
	            if(conta < 4){
					  digitArr[conta] = 0x7d;

					  TM1637_Start();
					  TM1637_WriteByte(0x40);
					  TM1637_ReadResult();
					  TM1637_Stop();

					  TM1637_Start();
					  TM1637_WriteByte(0xc0);
					  TM1637_ReadResult();

					  for (int i = 0; i < 4; ++i) {
						  TM1637_WriteByte(digitArr[i]);
						  TM1637_ReadResult();
					  }

					  TM1637_Stop();

						senha = (senha*10)+6;
						vetorSenhas[conta]= 54;
						conta++;
						HAL_Delay(500);
				}

	          } else if (i == 2) {
	            pressionado = '9';
	            if(conta < 4){
					  digitArr[conta] = 0x6f;

					  TM1637_Start();
					  TM1637_WriteByte(0x40);
					  TM1637_ReadResult();
					  TM1637_Stop();

					  TM1637_Start();
					  TM1637_WriteByte(0xc0);
					  TM1637_ReadResult();

					  for (int i = 0; i < 4; ++i) {
						  TM1637_WriteByte(digitArr[i]);
						  TM1637_ReadResult();
					  }

					  TM1637_Stop();

						senha = (senha*10)+9;
						vetorSenhas[conta]= 57;
						conta++;
						HAL_Delay(500);
				}

	          } else if (i == 3) {
	            pressionado = '#';

            	//Quebra de linha

	            if(valida_senha(vetorSenhas,conta)==1){
						if(conta-1 == -1){//caso senha nula

								// ERRO

							digitArr[0] = 0x79;
							digitArr[1] = 0x50;
							digitArr[2] = 0x50;
							digitArr[3] = 0x5c;


							TM1637_Start();
							TM1637_WriteByte(0x40);
							TM1637_ReadResult();
							TM1637_Stop();

							TM1637_Start();
							TM1637_WriteByte(0xc0);
							TM1637_ReadResult();

							for (int i = 0; i < 4; ++i) {
								TM1637_WriteByte(digitArr[i]);
								TM1637_ReadResult();
							}

							TM1637_Stop();
							HAL_UART_Transmit(&huart1, Erro, sizeof(Erro)-1, 100);
							HAL_Delay(2000);

							}

						else{//senhas válidas

			            	// ERRO

			            digitArr[0] = 0x79;
			            digitArr[1] = 0x50;
			            digitArr[2] = 0x50;
			            digitArr[3] = 0x5c;


			            TM1637_Start();
			            TM1637_WriteByte(0x40);
			            TM1637_ReadResult();
			            TM1637_Stop();

			            TM1637_Start();
			            TM1637_WriteByte(0xc0);
			            TM1637_ReadResult();

			            for (int i = 0; i < 4; ++i) {
			                TM1637_WriteByte(digitArr[i]);
			                TM1637_ReadResult();
			            }

			            TM1637_Stop();

							HAL_UART_Transmit(&huart1, Senharepetida, sizeof(Senharepetida)-1, 100);


							for(int k=0;k<conta;k++){
								myData=vetorSenhas[k];
								HAL_UART_Transmit(&huart1, &myData, 1, 100);
							}

							HAL_Delay(2000);

						}
						  HAL_UART_Transmit(&huart1, Space, sizeof(Space)-1, 100);
	            }
	            if(valida_senha(vetorSenhas,conta)==0){
		            	if(conta-1 == -1){//caso senha nula

			            	// ERRO

			            digitArr[0] = 0x79;
			            digitArr[1] = 0x50;
			            digitArr[2] = 0x50;
			            digitArr[3] = 0x5c;


			            TM1637_Start();
			            TM1637_WriteByte(0x40);
			            TM1637_ReadResult();
			            TM1637_Stop();

			            TM1637_Start();
			            TM1637_WriteByte(0xc0);
			            TM1637_ReadResult();

			            for (int i = 0; i < 4; ++i) {
			                TM1637_WriteByte(digitArr[i]);
			                TM1637_ReadResult();
			            }

			            TM1637_Stop();
		            		HAL_UART_Transmit(&huart1, Erro, sizeof(Erro)-1, 100);
							HAL_Delay(2000);


		            	}
		            	else{//senhas válidas

		            		if(salva_senha(vetorSenhas,conta)==1){

				            	// DONE

				            digitArr[0] = 0x5e;
				            digitArr[1] = 0x5c;
				            digitArr[2] = 0x54;
				            digitArr[3] = 0x79;


				            TM1637_Start();
				            TM1637_WriteByte(0x40);
				            TM1637_ReadResult();
				            TM1637_Stop();

				            TM1637_Start();
				            TM1637_WriteByte(0xc0);
				            TM1637_ReadResult();

				            for (int i = 0; i < 4; ++i) {
				                TM1637_WriteByte(digitArr[i]);
				                TM1637_ReadResult();
				            }

				            TM1637_Stop();

								HAL_UART_Transmit(&huart1, Senhasalva, sizeof(Senhasalva)-1, 100);

			            		for(int k=0;k<conta;k++){
			            			myData=vetorSenhas[k];
			            			HAL_UART_Transmit(&huart1, &myData, 1, 100);
			            		}
		            		}
		            		else{

				            	// ERRO

				            digitArr[0] = 0x79;
				            digitArr[1] = 0x50;
				            digitArr[2] = 0x50;
				            digitArr[3] = 0x5c;


				            TM1637_Start();
				            TM1637_WriteByte(0x40);
				            TM1637_ReadResult();
				            TM1637_Stop();

				            TM1637_Start();
				            TM1637_WriteByte(0xc0);
				            TM1637_ReadResult();

				            for (int i = 0; i < 4; ++i) {
				                TM1637_WriteByte(digitArr[i]);
				                TM1637_ReadResult();
				            }

				            TM1637_Stop();

		            			HAL_UART_Transmit(&huart1, Memoriacheia, sizeof(Memoriacheia)-1, 100);
		            		}

		            	}
						HAL_UART_Transmit(&huart1, Space, sizeof(Space)-1, 100);
						HAL_Delay(2000);

	            }

	            //APAGAR

	            senha = 0;
	            for (int i = 0; i < 4; ++i) {
	                digitArr[i] = 0x08;
	            }

	            TM1637_Start();
	            TM1637_WriteByte(0x40);
	            TM1637_ReadResult();
	            TM1637_Stop();

	            TM1637_Start();
	            TM1637_WriteByte(0xc0);
	            TM1637_ReadResult();

	            for (int i = 0; i < 4; ++i) {
	                TM1637_WriteByte(digitArr[3 - i]);
	                TM1637_ReadResult();
	            }

	            TM1637_Stop();
	            conta = 0;

	            HAL_Delay(200);
	            TM1637_SetBrightness(0);
	            HAL_Delay(200);
	            TM1637_SetBrightness(7);
	            HAL_Delay(200);
	            TM1637_SetBrightness(0);
	            HAL_Delay(200);
	            TM1637_SetBrightness(7);
	            HAL_Delay(200);
	            TM1637_SetBrightness(0);
	            HAL_Delay(200);
	            TM1637_SetBrightness(7);

	          }
	        }

	      }
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
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
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
  hi2c1.Init.Timing = 0x2000090E;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

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
  huart1.Init.BaudRate = 38400;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_8;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_ENABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

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
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |CLK_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(DATA_GPIO_Port, DATA_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA0 PA1 PA2 PA3 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA4 PA5 PA6 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : CLK_Pin */
  GPIO_InitStruct.Pin = CLK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(CLK_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : DATA_Pin */
  GPIO_InitStruct.Pin = DATA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(DATA_GPIO_Port, &GPIO_InitStruct);

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
