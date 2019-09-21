/**
 * @file    app_wfi.c
 * @author  Jorge Guzman
 * @date    May 25, 2019
 * @version 0.1.0.0 (beta)
 * @brief   Lib que configura e usa o modulo wifi
 */

//==============================================================================
// INCLUDE FILES
//==============================================================================
#include "app_wifi.h"
#include "app_sensores.h"

#include "wifi.h"
#include "setup_hw.h"

#include <stdio.h>
#include <string.h>

//==============================================================================
// PRIVATE DEFINITIONS
//==============================================================================

#define SSID     			"LHC"
#define PASSWORD 			"tijolo22"
#define PORT          		80

#define WIFI_WRITE_TIMEOUT 	10000
#define WIFI_READ_TIMEOUT  	10000
#define SOCKET              0

//==============================================================================
// PRIVATE TYPEDEFS
//==============================================================================

typedef struct
{
	uint8_t ssid[50];
	uint8_t password[50];
} ClientConnection_t;

//==============================================================================
// PRIVATE VARIABLES
//==============================================================================

static uint8_t http[1024];
static uint8_t IP_Addr[4];
static GPIO_PinState LedState = GPIO_PIN_RESET;

ClientConnection_t clientConn = { 0 };

//==============================================================================
// PRIVATE FUNCTIONS
//==============================================================================

static WIFI_Status_t SendWebPage(GPIO_PinState ledIsOn, Sensors_t *sens);
static  int wifi_start(void);
static  int wifi_connect(void);
static  bool WebServerProcess(void);

//==============================================================================
// SOURCE CODE
//==============================================================================

static int wifi_start(void)
{
	uint8_t MAC_Addr[6];

	/*Initialize and use WIFI module */
	if (WIFI_Init() == WIFI_STATUS_OK)
	{
		DBG("ES-WIFI Initialized.\n");

		if (WIFI_GetMAC_Address(MAC_Addr) == WIFI_STATUS_OK)
		{
			DBG("> es-wifi module MAC Address : %X:%X:%X:%X:%X:%X\n", MAC_Addr[0], MAC_Addr[1], MAC_Addr[2], MAC_Addr[3], MAC_Addr[4], MAC_Addr[5]);
		}
		else
		{
			DBG("> ERROR : CANNOT get MAC address\n");
			return -1;
		}
	}
	else
	{
		return -1;
	}
	return 0;
}

int wifi_connect(void)
{
	wifi_start();

	DBG("\nConnecting to %s , %s\n", SSID, PASSWORD);

	if (WIFI_Connect(SSID, PASSWORD, WIFI_ECN_WPA2_PSK) == WIFI_STATUS_OK)
	{
		if (WIFI_GetIP_Address(IP_Addr) == WIFI_STATUS_OK)
		{
			DBG("> es-wifi module connected: got IP Address : %d.%d.%d.%d\n", IP_Addr[0], IP_Addr[1], IP_Addr[2], IP_Addr[3]);
		}
		else
		{
			DBG(" ERROR : es-wifi module CANNOT get IP address\n");
			return -1;
		}
	}
	else
	{
		DBG("ERROR : es-wifi module NOT connected\n");
		return -1;
	}
	return 0;
}

int Wifi_Server(void)
{
	bool StopServer = false;

	DBG("\nRunning HTML Server test\n");

	if (wifi_connect() != 0)
	{
		return -1;
	}

	if (WIFI_STATUS_OK != WIFI_StartServer(SOCKET, WIFI_TCP_PROTOCOL, 1, "", PORT))
	{
		DBG("ERROR: Cannot start server.\n");
	}

	DBG("Server is running and waiting for an HTTP  Client connection to %d.%d.%d.%d\n", IP_Addr[0], IP_Addr[1], IP_Addr[2], IP_Addr[3]);

	do
	{
		uint8_t RemoteIP[4];
		uint16_t RemotePort;

		while (WIFI_STATUS_OK != WIFI_WaitServerConnection(SOCKET, 1000, RemoteIP, &RemotePort))
		{
			DBG("Waiting connection to  %d.%d.%d.%d\n", IP_Addr[0], IP_Addr[1], IP_Addr[2], IP_Addr[3]);
		}

		DBG("Client connected %d.%d.%d.%d:%d\n", RemoteIP[0], RemoteIP[1], RemoteIP[2], RemoteIP[3], RemotePort);

		StopServer = WebServerProcess();

		if (WIFI_CloseServerConnection(SOCKET) != WIFI_STATUS_OK)
		{
			DBG("ERROR: failed to close current Server connection\n");
			return -1;
		}

	} while (StopServer == false);

	if (WIFI_STATUS_OK != WIFI_StopServer(SOCKET))
	{
		DBG("ERROR: Cannot stop server.\n");
	}

	DBG("Server is stop\n");
	return 0;
}


static bool WebServerProcess(void)
{
	bool stopserver=false;
	uint16_t respLen;
	static uint8_t resp[1024];
	static Sensors_t sensor = {0};

	if (WIFI_STATUS_OK == WIFI_ReceiveData(SOCKET, resp, 1000, &respLen, WIFI_READ_TIMEOUT))
	{
		DBG("get %d byte from server\n",respLen);

		if( respLen > 0)
		{
			if(strstr((char *)resp, "GET")) /* GET: put web page */
			{
				Sensores_Read(&sensor);

				if(SendWebPage(LedState, &sensor) != WIFI_STATUS_OK)
				{
					DBG("> ERROR : Cannot send web page\n");
				}
				else
				{
					DBG("Send page after  GET command\n");
				}
			}
			else if(strstr((char *)resp, "POST"))/* POST: received info */
			{
				DBG("Post request\n");

				if(strstr((char *)resp, "radio"))
				{
					if(strstr((char *)resp, "radio=0"))
					{
						LedState = GPIO_PIN_RESET;
						Led_Set(LedState);
					}
					else if(strstr((char *)resp, "radio=1"))
					{
						LedState = GPIO_PIN_SET;
						Led_Set(LedState);
					}
					Sensores_Read(&sensor);
				}
				if(strstr((char *)resp, "stop_server"))
				{
					if(strstr((char *)resp, "stop_server=0"))
					{
						stopserver = false;
					}
					else if(strstr((char *)resp, "stop_server=1"))
					{
						stopserver = true;
					}
				}

				Sensores_Read(&sensor);

				if(SendWebPage(LedState, &sensor) != WIFI_STATUS_OK)
				{
					DBG("> ERROR : Cannot send web page\n");
				}
				else
				{
					DBG("Send Page after POST command\n");
				}
			}
		}
	}
	else
	{
		DBG("Client close connection\n");
	}
	return stopserver;

}


/**
 * @brief  Send HTML page
 * @param  None
 * @retval None
 */
static WIFI_Status_t SendWebPage(GPIO_PinState ledIsOn, Sensors_t *sens)
{
	uint8_t string[50];
	uint16_t SentDataLength;
	WIFI_Status_t ret;

	/* construct web page content */
	strcpy((char *)http, (char *)"HTTP/1.0 200 OK\r\nContent-Type: text/html\r\nPragma: no-cache\r\n\r\n");
	strcat((char *)http, (char *)"<html>\r\n<body>\r\n");
	strcat((char *)http, (char *)"<title>STM32 Web Server</title>\r\n");
	strcat((char *)http, (char *)"<h2>InventekSys : Web Server using Es-Wifi with STM32</h2>\r\n");

	//==============================================================================
	// Sensores
	//==============================================================================


	strcat((char *)http, (char *)"<br /><hr>\r\n");
	strcat((char *)http, (char *)"<p><form method=\"POST\"><strong>Temp: <input type=\"text\" value=\"");
	snprintf((char *)string, sizeof(string), "%.2f", sens->HTS221_temp);
	strcat((char *)http, (char *)string);
	strcat((char *)http, (char *)"\"> <sup>O</sup>C");


	//==============================================================================

	if (ledIsOn == GPIO_PIN_SET)
	{
		strcat((char *)http, (char *)"<p><input type=\"radio\" name=\"radio\" value=\"0\" >LED off");
		strcat((char *)http, (char *)"<br><input type=\"radio\" name=\"radio\" value=\"1\" checked>LED on");
	}
	else
	{
		strcat((char *)http, (char *)"<p><input type=\"radio\" name=\"radio\" value=\"0\" checked>LED off");
		strcat((char *)http, (char *)"<br><input type=\"radio\" name=\"radio\" value=\"1\" >LED on");
	}

	strcat((char *)http, (char *)"</strong><p><input type=\"submit\"></form></span>");
	strcat((char *)http, (char *)"</body>\r\n</html>\r\n");

	ret = WIFI_SendData(0, (uint8_t *)http, strlen((char *)http), &SentDataLength, WIFI_WRITE_TIMEOUT);

	if((ret == WIFI_STATUS_OK) && (SentDataLength != strlen((char *)http)))
	{
		ret = WIFI_STATUS_ERROR;
	}

	return ret;
}


/**
 * @brief  EXTI line detection callback.
 * @param  GPIO_Pin: Specifies the port pin connected to corresponding EXTI line.
 * @retval None
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch (GPIO_Pin)
	{
	case GPIO_PIN_1:
		SPI_WIFI_ISR();
		break;

	default:
		break;
	}
}


void Wifi_SetConnection(uint8_t *ssid, uint8_t *password)
{
	snprintf((char *) clientConn.ssid, sizeof(clientConn.ssid), "%s", ssid);
	snprintf((char *) clientConn.password, sizeof(clientConn.password), "%s", password);
}

