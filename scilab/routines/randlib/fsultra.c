#include <math.h>             /* to use floor    */
#include "ultra.h" 
#include "others_generators.h"
#include "../graphics/Math.h" /* to use sciprint */


/* 
 * calling FSU - ULTRA	from scilab 
 */

unsigned long fsultra()
{
  return (unsigned long) i32bit();
}

static unsigned long s1= 1234567ul,s2=7654321ul ; 

int set_state_fsultra(double g1, double g2)
{
  if (g1 == floor(g1) && g2 == floor(g2)  &&
      0.0 <= g1 && g1 <= 4294967295.0 &&
      0.0 <= g2 && g2 <= 4294967295.0 )
    {
      s1 = (unsigned long) g1;
      s2 = (unsigned long) g2;
      rinit(s1,s2);
      return ( 1 );
    }
  else
    {
      sciprint("\n\r bad seeds for fsultra, must be integers in [0,2^32-1]\n\r");
      return ( 0 );
    }
}

void get_state_fsultra(double g[])
{
  g[0] = (double) s1;
  g[1] = (double) s2;
}

