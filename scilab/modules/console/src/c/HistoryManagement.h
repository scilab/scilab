/** 
 * @copyright INRIA 2007
 * @author Vincent COUVERT
 */

#include "../../../shell/src/c/history.h"

/** Get next entry in Scilab history beginning with string beg
 *
 * @param beg the pattern that the entry beginning must match
 * return the entry found or NULL
 */
char *getNextEntry(char *beg);

/** Get previous entry in Scilab history beginning with string beg
 *
 * @param beg the pattern that the entry beginning must match
 * return the entiy found or NULL
 */
char *getPreviousEntry(char *beg);

/** Get previous entry in Scilab history beginning with string beg
 * Used when entering commands beginning by a !
 * @param beg the pattern that the entry beginning must match
 * return the entry found or NULL
 */
char *searchBackward(char *beg);
