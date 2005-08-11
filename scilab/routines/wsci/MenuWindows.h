/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#ifndef __MENUWINDOWS__
#define __MENUWINDOWS__
/*-----------------------------------------------------------------------------------*/ 
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include <windows.h>

#include "wgnuplib.h"
#include "Messages.h"
#include "Warnings.h"
#include "Errors.h"

/*-----------------------------------------------------------------------------------*/ 
#ifndef NULL
  #define NULL 0
#endif

#define TRUE  1
#define FALSE 0

MW InitMWStruct(void);
LPMW GetMENUWinScilab(void);
LPSTR GetszMenuName(void);
LPSTR GetszGraphMenuName(void);
void InitszMenuName(char *ScilabDirectory);
void InitszGraphMenuName(char *ScilabDirectory);
#endif /* __MENUWINDOWS__ */
/*-----------------------------------------------------------------------------------*/ 
