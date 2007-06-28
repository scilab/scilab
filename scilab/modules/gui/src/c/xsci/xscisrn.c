/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* @author Ledru Sylvestre */
/* @author Others */
/*-----------------------------------------------------------------------------------*/
#include "xscisrn.h"
#include "x_charproc.h"
#include "../shell/includes/PutChar.h"
void C2F(xscisncr)(char *str,integer *n,integer dummy)
{
  int i;
  for ( i =0 ; i < *n; i++) {
    PutChar(str[i]);
  }
}

/** print str on Scilab window \n are changed to \r\n **/

void C2F(xscisrn)(char *str,integer *n,integer dummy)
{
  int i;
  for ( i =0 ; i < *n; i++) {
    if ( str[i] == '\n' ) 
      {
		  PutChar('\r');
      }
    PutChar(str[i]);
  }
}
