/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* @author Sylvestre LEDRU */
/*-----------------------------------------------------------------------------------*/ 

#include <stdio.h>
#include "PutChar.h"
#include "scilabmode.h"
#include "../../../gui/src/c/xsci/x_charproc.h"

void PutChar(int c)
{
  if(getScilabMode() == SCILAB_STD)
    Xputchar(c);
  else
    putchar(c);
}
