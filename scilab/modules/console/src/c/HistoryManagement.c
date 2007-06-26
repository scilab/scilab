/** 
 * @copyright INRIA 2007
 * @author Vincent COUVERT
 */

#include "HistoryManagement.h"

/*-----------------------------------------------------------------------------------*/
char *GetNextEntry(char *beg)
{
  sci_hist *found = SearchForwardInHistory(beg);
  
  if (found==NULL)
    {
      return NULL;
    }
  else
    {
      return found->line;
    }

}
/*-----------------------------------------------------------------------------------*/
char *GetPreviousEntry(char *beg)
{
  sci_hist *found = SearchBackwardInHistory(beg);
  
  if (found==NULL)
    {
      return NULL;
    }
  else
    {
      return found->line;
    }

}
/*-----------------------------------------------------------------------------------*/
