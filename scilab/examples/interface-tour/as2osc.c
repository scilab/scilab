#include <string.h> 
#include <stdio.h>
#include "stack-c.h"

/*************************************************************
 * Example of using a character string sent by the interface.
 * All 'a' are replaced by 'o'.
 *************************************************************/

int as2osc(thechain)
     char *thechain;
{
  static int k, l;
  l = strlen(thechain);
  sciprint("character string length %d\r\n",l);
  for (k = 0 ; k < l; k++) 
    {
      if ( thechain[k] == 'a' ) thechain[k]='o';
    }
  return 0;
} 


























