/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* @author Sylvestre LEDRU */
/*-----------------------------------------------------------------------------------*/ 

#include <stdio.h>
#include "PutChar.h"
#include "zzledt_Linux.h"
#include "../../../gui/src/c/xsci/x_charproc.h"

void PutChar(int c)
{
  if(isModeX())
    Xputchar(c);
  else
    putchar(c);
}
