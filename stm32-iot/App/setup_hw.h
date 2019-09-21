/**
 * @file    setup_hw.h
 * @author  Jorge Guzman,
 * @date    Fev 2, 2018
 * @version 0.1.0
 * @brief   Biblioteca que contem as rotinas de configuracao e inicializacao do projeto
 * @details
 */

#ifndef _SETUP_HW_
#define	_SETUP_HW_

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

//==============================================================================
// INCLUDE FIRST FILES
//==============================================================================

#include "main.h"
#include "stm32l4xx_hal.h"
#include <stdint.h>

//==============================================================================
// PUBLIC DEFINICTIONS
//==============================================================================

#define ANSI_COLOR_RED     		"\x1b[31m"
#define ANSI_COLOR_GREEN   		"\x1b[32m"
#define ANSI_COLOR_YELLOW  		"\x1b[33m"
#define ANSI_COLOR_BLUE    		"\x1b[34m"
#define ANSI_COLOR_MAGENTA 		"\x1b[35m"
#define ANSI_COLOR_CYAN    		"\x1b[36m"
#define ANSI_COLOR_RESET   		"\x1b[0m"

#define DEF_CONSOLE_CLEAR     	"\033[2J\033[1;1H"
#define DEF_CONSOLE_DEFAULT   	"\033[0m"

#define DEF_CONSOLE_RED       	"\033[91m"
#define DEF_CONSOLE_GREEN     	"\033[92m"
#define DEF_CONSOLE_YELLOW    	"\033[93m"
#define DEF_CONSOLE_BLUE    	"\033[94m"
#define DEF_CONSOLE_WHITE     	"\033[97m"
#define DEF_CONSOLE_BOLD_RED  	"\033[1;31m"
#define DEF_CONSOLE_BOLD_YELLOW "\033[1;33m"
#define DEF_CONSOLE_BOLD_BLUE 	"\033[1;34m"
#define DEF_CONSOLE_BOLD_GREEN  "\033[1;32m"
#define DEF_CONSOLE_BOLD_WHITE  "\033[1;37m"

#define DEF_BACKGROUND_BLACK    "\033[40;1;37m"
#define DEF_BACKGROUND_RED     	"\033[41;1;37m"
#define DEF_BACKGROUND_GREEN    "\033[42;1;37m"
#define DEF_BACKGROUND_BROWN    "\033[43;1;37m"
#define DEF_BACKGROUND_BLUE     "\033[44;1;37m"
#define DEF_BACKGROUND_PURBLE  	"\033[45;1;37m"
#define DEF_BACKGROUND_CYAN 	"\033[46;1;37m"
#define DEF_BACKGROUND_GRAY 	"\033[47;1;37m"

#define DBG(fmt, ...) 					Debug_Printf(fmt"\r\n", ##__VA_ARGS__)

//==============================================================================
// PUBLIC FUNCTIONS
//==============================================================================

void Setup_Init(void);
HAL_StatusTypeDef Debug_Printf(char * format, ...);


/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif
