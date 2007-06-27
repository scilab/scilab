/** 
 * @copyright INRIA 2007
 * @author Vincent COUVERT
 */

#include "HistoryManagement.h"

/*-----------------------------------------------------------------------------------*/
char *GetNextEntry(char *beg)
{
  if (cur_entry != NULL)
    {
      cur_entry = cur_entry->next;
      if (cur_entry != NULL)
        return cur_entry->line;
      else
        return NULL;
    }
  else
    return NULL;
}
/*-----------------------------------------------------------------------------------*/
char *GetPreviousEntry(char *beg)
{
  if (history != NULL)
    {
      if (cur_entry == NULL)
        {
          cur_entry = history;
          return cur_entry->line;
        }
      else if (cur_entry->prev != NULL)
        {
          cur_entry = cur_entry->prev;
          return cur_entry->line;
        }
      else
        return NULL;
    }
  else
    return NULL;
}
/*-----------------------------------------------------------------------------------*/
