#include "../machine.h" 
#include "grand.h" 


/*------------------------------------------------------------------
 *   SUBROUTINE GENPRM( IARRAY, LARRAY ) 
 *              GENerate random PeRMutation of iarray 
 *                             Arguments 
 *    IARRAY <--> On output IARRAY is a random permutation of its 
 *                value on input 
 *                        INTEGER IARRAY( LARRAY ) 
 *    LARRAY <--> Length of IARRAY 
 *                        INTEGER LARRAY 
 ------------------------------------------------------------------*/

int C2F(genprm)(integer * iarray,integer * larray)
{
  static integer itmp, i, iwhich;
  for (i= 1; i <= *larray ; ++i) {
    iwhich = C2F(ignuin)(i,*larray);
    itmp = iarray[iwhich-1];
    iarray[iwhich-1] = iarray[i-1];
    iarray[i-1] = itmp;
  }
  return 0;
}


