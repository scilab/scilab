/* 
 *   PURPOSE
 *      the kiss generator of G. Marsaglia
 *      generate random integers (uint) in [0, 2^32 - 1]
 *      the state is given by 4 integers (z, w, jsr, jcong)
 *
 *   NOTES
 *      The code was given by G. Marsaglia at the end of  a
 *      thread  concerning  RNG  in C in several newsgroups
 *      (whom sci.math.num-analysis) "My offer of RNG's for
 *      C  was an invitation to dance..."
 *
 *      Slight modifications by Bruno Pincon for inclusion in
 *      Scilab (added set/get state routines)
 *
 *      kiss is made of combinaison of severals  others but  
 *      they  are not interfaced at the scilab level.
 *
 *      Need that it is assumed that the 
 *         unsigned long arithmetic is the classic 32 bits 
 *         unsigned arithmetic modulo 2^32 (ie all is exact
 *         modulo 2^32) 
 *
 */

#include "../graphics/Math.h" /* to use sciprint */
#include <math.h>             /* to use floor    */

/* The Marsaglia 's macros : */
#define znew  (z=36969*(z&65535)+(z>>16))
#define wnew  (w=18000*(w&65535)+(w>>16))
#define MWC   ((znew<<16)+wnew )
#define CONG  (jcong=69069*jcong+1234567)
#define SHR3  (jsr^=(jsr<<17), jsr^=(jsr>>13), jsr^=(jsr<<5))
#define KISS  ((MWC^CONG)+SHR3)

/*  the kiss 's state  (any int in [0,2^32-1] are OK ?) */
static unsigned long z=362436069, w=521288629, jsr=123456789, jcong=380116160;

unsigned long kiss()
{
  return ( KISS );
}

int set_state_kiss(double g1, double g2, double g3, double g4)
{
  if (g1 == floor(g1) && g2 == floor(g2) && 
      g3 == floor(g3) && g4 == floor(g4) &&  
      0.0 <= g1 && g1 <= 4294967295.0 &&
      0.0 <= g2 && g2 <= 4294967295.0 &&
      0.0 <= g3 && g3 <= 4294967295.0 &&
      0.0 <= g4 && g4 <= 4294967295.0 )
    {
          z = (unsigned long) g1;
          w = (unsigned long) g2;
        jsr = (unsigned long) g3;
      jcong = (unsigned long) g4;
      return ( 1 );
    }
  else
    {
      sciprint("\n\r bad seeds for kiss, must be integers in [0,2^32-1]\n\r");
      return ( 0 );
    }
}

void get_state_kiss(double g[])
{
  g[0] = (double) z;
  g[1] = (double) w;
  g[2] = (double) jsr;
  g[3] = (double) jcong;
}

