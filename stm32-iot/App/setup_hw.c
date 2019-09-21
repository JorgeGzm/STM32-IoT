/**
 * @file    setup_hw.h
 * @author  Jorge Guzman,
 * @date    Fev 2, 2018
 * @version 0.1.0
 * @brief   Biblioteca que contem as rotinas de configuracao e inicializacao do projeto
 * @details
 */

//==============================================================================
// INCLUDE FILES
//==============================================================================

#include "setup_hw.h"
#include "app_wifi.h"
#include "app_sensores.h"

#include "hts221/hts221.h"
#include "lps22hb/lps22hb.h"
#include "lsm6dsl/lsm6dsl.h"
#include "lis3mdl/lis3mdl.h"
#include "es_wifi_io.h"

#include <stdarg.h>
#include <stdio.h>

//==============================================================================
// PRIVATE DEFINITIONS
//==============================================================================

#define SERIAL_BUFFER_SIZE			500

//==============================================================================
// EXTERN VARIABLES
//==============================================================================

extern UART_HandleTypeDef huart1; /* Serial Debug */
extern I2C_HandleTypeDef hi2c2;
extern SPI_HandleTypeDef hspi3;

//==============================================================================
// PRIVATE VARIABLES
//==============================================================================

static uint8_t bufferSerial[SERIAL_BUFFER_SIZE];

//==============================================================================
// PRIVATE FUNCTIONS
//==============================================================================

/** @brief Start libraries of middlware */
static void Setup_InitMiddlware(void);

/** @brief Start Apps of application */
static void Setup_InitApps(void);

//==============================================================================
// SOURCE CODE
//==============================================================================

HAL_StatusTypeDef Debug_Printf(char * format, ...)
{
	va_list args;
	uint16_t len;

	HAL_StatusTypeDef resp = HAL_ERROR;

	va_start(args, format);
	len = vsnprintf((char *) bufferSerial, SERIAL_BUFFER_SIZE, format, args);
	va_end(args);

	resp = HAL_UART_Transmit(&huart1, bufferSerial, len, 1000);

	return resp;
}

static void Setup_InitMiddlware(void)
{
	HTS221_attach(&hi2c2);
	HTS221_Init(HTS221_I2C_ADDRESS);

	LPS22HB_attach(&hi2c2);
	LPS22HB_Init(LPS22HB_I2C_ADDRESS);

	LSM6DSL_attach(&hi2c2);
	LSM6DSL_myInit();

	LIS3MDL_attach(&hi2c2);
	LIS3MDL_myInit();

	SPI_WIFI_attach(&hspi3,
			WIFI_RESET_GPIO_Port, WIFI_RESET_Pin,
			ISM43362_SPI3_CSN_GPIO_Port, ISM43362_SPI3_CSN_Pin,
			ISM43362_DRDY_EXTI1_GPIO_Port, ISM43362_DRDY_EXTI1_Pin);

}

static void Setup_InitApps(void)
{
	Sensors_t sens = {0};

	DBG("%s[ OK ]%s\t %s\r\n", ANSI_COLOR_GREEN, DEF_CONSOLE_DEFAULT, "Mensagem Colorida");

	Sensores_Read(&sens);
	Sensor_Print(&sens);
	Sensor_Print_SerialPlot(&sens);

	Wifi_Server();
}

void Setup_Init(void)
{
	//==============================================================================
	// Inicializa Drivers usados pelas Libs
	//==============================================================================

	Setup_InitMiddlware();

	//==============================================================================
	// Inicializa Tasks da Aplicacao
	//==============================================================================

	Setup_InitApps();

	/* Envia mensagem de start do sistema */
	DBG("Init Program...");
}

//==============================================================================


