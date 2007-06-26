/** 
 * @copyright INRIA 2007
 * @author Vincent COUVERT
 */

#include "../../../shell/src/c/history.h"

/** Get next entry in Scilab history beginning with string beg
 *
 * @param beg the pattern that the entry beginning must match
 * return the entriy found or NULL
 */
char *GetNextEntry(char *beg);

/** Get previous entry in Scilab history beginning with string beg
 *
 * @param beg the pattern that the entry beginning must match
 * return the entriy found or NULL
 */
char *GetPreviousEntry(char *beg);
