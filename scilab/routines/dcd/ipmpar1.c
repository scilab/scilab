
/*
 *  MACHINE CONSTANTS
 *  These numbers must be updated when the program is ported to a new machine.
 *  Using spConfig.h to get the largest int 
 */

#define spINSIDE_SPARSE
#include "../sparse/spConfig.h"
#include "../machine.h"


int C2F(largestint)()
{
  static int first=0,large;
  if ( first == 0) 
    {
      if (sizeof(int)==sizeof(long))
	large = LARGEST_LONG_INTEGER ;
      else if (sizeof(int)==sizeof(short))
	large = LARGEST_SHORT_INTEGER;
      else 
	large = 2147483647 ; /** using default value **/
      first++;
      return large ;
    }
  else 
    return large;
}
