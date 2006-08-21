/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __IHMCOLOR__
#define __IHMCOLOR__


#include <windows.h>


#include "../machine.h"
#include "../stack-c.h"
#include "../version.h"
#include "wgnuplib.h"

#ifndef NULL
  #define NULL 0
#endif


BOOL SetIhmSystemDefaultTextBackgroundColor(void);
BOOL SetIhmSystemDefaultTextColor(void);
void InitIhmDefaultColor(void);

BOOL SetIhmTextBackgroundColor(int R,int G,int B,BOOL Refresh);
BOOL SetIhmTextColor(int R,int G,int B,BOOL Refresh);
DWORD GetIhmTextColor(void);
DWORD GetIhmTextBackgroundColor(void);

BOOL ChooseColorBox(int *R,int *G,int *B);


#endif /* __IHMCOLOR__ */
/*-----------------------------------------------------------------------------------*/ 
