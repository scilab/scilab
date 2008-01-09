/*--------------------------------------------------------------------------*/
/** 
 * @copyright INRIA 2007
 * @author Vincent COUVERT
 */
/*--------------------------------------------------------------------------*/
#include "GuiManagement.h"
#include "../../../shell/includes/scilines.h"
/*--------------------------------------------------------------------------*/
void setScilabLines(int nbRows, int nbCols)
{
  if (getLinesSize() == 0) 
    {
      /* The user do not want to be asked for more display */
      /* Modify only the number of columns used to format displayed data */

      scilines(getLinesSize(), nbCols);
    }
  else 
    {
      /* Modify number of lines and columns used to format displayed data */

      scilines(nbRows, nbCols);
    }
}
/*--------------------------------------------------------------------------*/
