/** 
 * @copyright INRIA 2007
 * @author Vincent COUVERT
 */

#include "GuiManagement.h"

void setScilabLines(int nbRows, int nbCols)
{
  C2F(scilines)(&nbRows, &nbCols);
}
