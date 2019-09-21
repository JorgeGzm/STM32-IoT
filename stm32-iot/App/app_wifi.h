/**
 * @file    app_wfi.c
 * @author  Jorge Guzman
 * @date    May 25, 2019
 * @version 0.1.0.0 (beta)
 * @brief   Lib que configura e usa o modulo wifi
 */


#ifndef _APP_WIFI_H_
#define _APP_WIFI_H_

//==============================================================================
// INCLUDE FILES
//==============================================================================

#include "setup_hw.h"

//==============================================================================
// PUBLIC FUNCTIONS
//==============================================================================

void Wifi_SetConnection(uint8_t *ssid, uint8_t *password);
int Wifi_Server(void);

#endif
