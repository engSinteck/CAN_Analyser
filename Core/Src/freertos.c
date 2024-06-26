/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart.h"
#include "spi.h"
#include "fatfs.h"
#include "rtc.h"
#include "log.h"
#include "usb_device.h"
#include "wizchip_conf.h"
#include "socket.h"
#include "httpServer.h"
#include "rs485.h"
#include "index.h"
#include "loopback.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define LOOPBACK_DATA_BUF_SIZE			2048


int32_t server_tcp(uint8_t sn);


extern USBD_HandleTypeDef hUsbDeviceFS;
extern CAN_RxHeaderTypeDef   RxHeader1;
extern CAN_RxHeaderTypeDef   RxHeader2;
extern uint8_t RxData1[], RxData2[];
extern uint8_t Flag_CAN_1, Flag_CAN_2;
extern volatile uint8_t rx_rs485[];
extern volatile uint8_t rx_rs232[];
extern volatile uint16_t ptr_232;
extern volatile uint16_t ptr_485;
extern volatile uint8_t byte_rs232[];
extern volatile uint8_t byte_rs485[];

RTC_TimeTypeDef gTime = {0};
RTC_DateTypeDef gDate = {0};

uint32_t timer_led = 0;
uint32_t timer_teste = 0;
uint32_t timer_rs485 = 0;
uint32_t timer_rs232 = 0;
uint32_t tmr_keep_alive = 0;

char line[100] = {0}; /* Line buffer */
extern char SDPath[4];   /* SD logical drive path */
FIL USERFile;       /* File object for USER */
extern FATFS SDFatFS;    /* File system object for SD logical drive */
FATFS *pfs;
FRESULT fr;     /* FatFs return code */
DWORD fre_clust;
uint32_t totalSpace, freeSpace, SpaceUsed;
uint32_t duracao = 0;
uint32_t size = 0;
unsigned int ByteRead;
volatile unsigned long ulHighFrequencyTimerTicks = 0;

uint8_t	bufSize[] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
uint8_t WEBRX_BUF[2048] = {0};
uint8_t WEBTX_BUF[2048] = {0};
uint8_t socknumlist[] = {2, 3};
static uint8_t dest_ip[4] = {192, 168, 10, 13};

uint16_t last_232 = 0;
uint16_t last_485 = 0;

uint8_t isConnected = 0;
uint16_t socket_size = 0;

uint8_t buf_tx[LOOPBACK_DATA_BUF_SIZE] = {0};
uint8_t buf_rx[LOOPBACK_DATA_BUF_SIZE] = {0};
uint8_t ret = 0;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
void cs_sel(void);
void cs_desel(void);
uint8_t spi_rb(void);
void spi_wb(uint8_t b);
void spi_rb_burst(uint8_t *buf, uint16_t len);
void spi_wb_burst(uint8_t *buf, uint16_t len);
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define MAX_HTTPSOCK	2
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for TaskETH */
osThreadId_t TaskETHHandle;
const osThreadAttr_t TaskETH_attributes = {
  .name = "TaskETH",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartTaskETH(void *argument);

extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void configureTimerForRunTimeStats(void);
unsigned long getRunTimeCounterValue(void);

/* USER CODE BEGIN 1 */
/* Functions needed when configGENERATE_RUN_TIME_STATS is on */
__weak void configureTimerForRunTimeStats(void)
{
	ulHighFrequencyTimerTicks = 0;
}

__weak unsigned long getRunTimeCounterValue(void)
{
	return ulHighFrequencyTimerTicks;
}
/* USER CODE END 1 */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of TaskETH */
  TaskETHHandle = osThreadNew(StartTaskETH, NULL, &TaskETH_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN StartDefaultTask */
  if (FATFS_LinkDriver(&SD_Driver, SDPath) == 0) {
	  // Mount SD CARD
	  fr = f_mount(&SDFatFS, (TCHAR const*)SDPath, 0);
	  if(fr != FR_OK){
		  logI("STM32 FatFs - GetMount ERROR...\n\r");
	  }
	  else {
		  logI("STM32 FatFs - GetMount OK...\n\r");
	  }

//	  fr = f_mkfs((TCHAR const*)SDPath, FM_ANY, 0, buffer, sizeof(buffer));
//	  if(fr != FR_OK){
//		  logI("STM32 FatFs - Mkfs ERROR...\n\r");
//	  }
//	  else {
//		  logI("STM32 FatFs - Mkfs OK...\n\r");
//	  }

	  // Check freeSpace space
	  fr = f_getfree("", &fre_clust, &pfs);
	  if(fr != FR_OK){
		  logI("STM32 FatFs - GetFree ERROR...\n\r");
	  }
	  else {
		  logI("STM32 FatFs - GetFree OK...\n\r");
		  totalSpace = (uint32_t)((pfs->n_fatent - 2) * pfs->csize * 0.5);
		  freeSpace = (uint32_t)(fre_clust * pfs->csize * 0.5);
		  SpaceUsed = totalSpace - freeSpace;
		  logI("Total Space: %ld , SpeceUsed: %ld , FreeSpace: %ld \n\r", totalSpace, SpaceUsed, freeSpace);
	  }
//	  // Test Open File + 8 Caracteres Tela_Principal.bin
//	  duracao = HAL_GetTick();
//	  fr = f_open(&USERFile, "Tela_Principal.bin", FA_READ);
//	  if(fr != FR_OK) {
//	 	 logI("FatFs - Tela_Principal.bin Error...Result: %d\n\r", fr);
//	  }
//	  else {
//		 duracao = HAL_GetTick() - duracao;
//	 	 size = f_size(&USERFile);
//	 	 logI("FatFs - Open File Tela_Principal.bin... Result: %d  Size:%d Duracao: %d\n\r", fr, size, duracao);
	      //fr = f_read(&USERFile, line , 100, &ByteRead);
	      //if(fr == FR_OK) {
	     //	 logI("FatFs ReadFile...line: %s\n\r", line);
	      //}
	      //else {
	     //	 logI("FatFs ReadFile...Error:  Result: %d \n\r", fr);
	      //}
//
  }
  /* Infinite loop */
  for(;;)
  {
	  HAL_RTC_GetTime(&hrtc, &gTime, RTC_FORMAT_BIN);
	  HAL_RTC_GetDate(&hrtc, &gDate, RTC_FORMAT_BIN);

	  if(HAL_GetTick() - timer_led > 100) {
		  timer_led = HAL_GetTick();
		  HAL_GPIO_TogglePin(LED_INT_GPIO_Port, LED_INT_Pin);
	  }
	  if( Flag_CAN_1 ) {
		  Flag_CAN_1 = 0;
		  HAL_GPIO_WritePin(LED_CAN1_RX_GPIO_Port, LED_CAN1_RX_Pin, GPIO_PIN_RESET);
		  logI("CAN1 0x%08lX MSG: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X  %ld\n\r", RxHeader1.ExtId,
			    RxData1[0], RxData1[1], RxData1[2], RxData1[3], RxData1[4], RxData1[5], RxData1[6], RxData1[7], HAL_GetTick());

	  }
	  if( Flag_CAN_2 ) {
		  Flag_CAN_2 = 0;
		  HAL_GPIO_WritePin(LED_CAN2_RX_GPIO_Port, LED_CAN2_RX_Pin, GPIO_PIN_RESET);
		  logI("CAN2 0x%08lX MSG: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X  %ld\n\r", RxHeader2.ExtId,
			    RxData2[0], RxData2[1], RxData2[2], RxData2[3], RxData2[4], RxData2[5], RxData2[6], RxData2[7], HAL_GetTick());
	  }
	  // RS485
	  if(HAL_GetTick() - timer_rs485 > 1000) {
		  timer_rs485 = HAL_GetTick();
		  test_485();
	  }
	  //
	  if(HAL_GetTick() - timer_rs232 > 1000) {
		  timer_rs232 = HAL_GetTick();
		  if(last_485 != ptr_485) {
			  last_485 = ptr_485;
			  HAL_GPIO_WritePin(LED_RS485_RX_GPIO_Port, LED_RS485_RX_Pin, GPIO_PIN_RESET);
			  logI("RS485[%d]\n\r",	last_485 );
		  }
		  if(last_232 != ptr_232) {
			  last_232 = ptr_232;
			  HAL_GPIO_WritePin(LED_232_RX_GPIO_Port, LED_232_RX_Pin, GPIO_PIN_RESET);
			  logI("RS232[%d]\n\r",	last_232 );
		  }
	  }
	  if(hUsbDeviceFS.dev_state == USBD_STATE_CONFIGURED) {
		  //logI("Cable USB Connected\n\r");
	  }
	  else {
		  //logI("Cable USB Not Connected\n\r");
	  }
    osDelay(2);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTaskETH */
/**
* @brief Function implementing the TaskETH thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskETH */
void StartTaskETH(void *argument)
{
  /* USER CODE BEGIN StartTaskETH */
	reg_wizchip_cs_cbfunc(cs_sel, cs_desel);
	reg_wizchip_spi_cbfunc(spi_rb, spi_wb);
	reg_wizchip_spiburst_cbfunc(spi_rb_burst, spi_wb_burst);
	reg_wizchip_cris_cbfunc(vPortEnterCritical, vPortExitCritical);

	wizchip_init(bufSize, bufSize);

	wiz_NetInfo netInfo = { .mac 	= {0x00, 0x08, 0xdc, 0xab, 0xcd, 0xef},		// Mac address
			                .ip 	= {192, 168, 10, 11},						// IP address
			                .sn 	= {255, 255, 255, 0},						// Subnet mask
							.dns    = {192, 168, 10, 250},						// DNS
			                .gw 	= {192, 168, 10, 250},						// Gateway address
							.dhcp   = NETINFO_STATIC 							// DHCP enable/disable
	                      };

	wizchip_setnetinfo(&netInfo);
	wizchip_getnetinfo(&netInfo);

	httpServer_init(WEBTX_BUF, WEBRX_BUF, MAX_HTTPSOCK, socknumlist);		// Tx/Rx buffers (1kB) / The number of W5500 chip H/W sockets in use
	reg_httpServer_cbfunc(NVIC_SystemReset, NULL);
	reg_httpServer_webContent((uint8_t *)"index.html",    (uint8_t *)index_html);			// index.html
  /* Infinite loop */
  for(;;)
  {
		if((ret = loopback_tcps(0, buf_rx, 5001)) < 0) {
		    logI("%d:loopback_tcps error:%d\r\n", 0, ret);
			break;
		}
//		if((ret = loopback_tcpc(0, buf_rx, dest_ip, 5001)) < 0) {
//		    logI("%d:loopback_tcpc error:%d\r\n", 0, ret);
//			break;
//		}

	  // HTTP
	  for(uint8_t i = 0; i < MAX_HTTPSOCK; i++)	{
		  httpServer_run(i); 	// HTTP Server handler
	  }
    osDelay(2);
  }
  /* USER CODE END StartTaskETH */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void cs_sel(void)
{
	HAL_GPIO_WritePin(SPI_NSS_GPIO_Port, SPI_NSS_Pin, GPIO_PIN_RESET); //CS LOW
}

void cs_desel(void)
{
	HAL_GPIO_WritePin(SPI_NSS_GPIO_Port, SPI_NSS_Pin, GPIO_PIN_SET); //CS HIGH
}

uint8_t spi_rb(void)
{
	uint8_t rbuf;
	HAL_SPI_Receive(&hspi1, &rbuf, 1, 1000);
	return rbuf;
}

void spi_wb(uint8_t b)
{
	HAL_SPI_Transmit(&hspi1, &b, 1, 1000);
}

void spi_rb_burst(uint8_t *buf, uint16_t len)
{
	HAL_SPI_Receive(&hspi1, buf, len, 1000);
}

void spi_wb_burst(uint8_t *buf, uint16_t len)
{
	HAL_SPI_Transmit(&hspi1, buf, len, 1000);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART1) {	// UART 1

	}
	if(huart->Instance==USART2) {	// UART 2 RS485
		HAL_UART_Receive_IT(&huart2, (uint8_t *)byte_rs485, 1);
	}
	if(huart->Instance==USART6) {	// UART6
		HAL_UART_Receive_IT(&huart6, (uint8_t *)byte_rs232, 1);
	}
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{

}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{

}
/* USER CODE END Application */

