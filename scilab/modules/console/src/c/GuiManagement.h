/** 
 * @copyright INRIA 2007
 * @author Vincent COUVERT
 */

#include "machine.h"
extern int C2F (scilines) __PARAMS((int *nl, int *nc));

/**
  * Sets Scilab parameters used to display data according the size of the console
  * @param nbRows number of rows that can be used to display data
  * @param nbCols number of columns that can be used to display data
  */
void setScilabLines(int nbRows, int nbCols);
