/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* @author Sylvestre LEDRU */
/*-----------------------------------------------------------------------------------*/ 

#include <stdio.h>

extern void Xputchar(int c);

void PutChar(int c)
{
  if(modeX)
    Xputchar(c);
  else
    putchar(c);
}
