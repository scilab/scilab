/* 
 *   PURPOSE
 *      the basic rand generator of Scilab : s <- (a*s + c) mod m
 *      with :
 *             m = 2^{31} 
 *             a = 843314861
 *             c = 453816693
 *      
 *      s must be in [0,m-1] when user changes seed with set_state_urand
 *      period = m
 *
 *   NOTES
 *      a/ Rewritten (in C) so as to output integers like all the others 
 *         generators (and also to have the same manner to set/get the state)
 *      b/ unsigned long int arithmetic must be the classic 32 bits unsigned
 *         arithmetic (ie also is exact modulo 2^32).
 * 
 */

#include "../graphics/Math.h" /* to use sciprint */
#include <math.h>             /* to use floor    */

static unsigned long s = 0;

unsigned long urandc()
{
  s = 843314861ul * s + 453816693ul;  /* => on obtient ici un resultat modulo 2^32 */

  /* il suffit du test suivant pour obtenir le modulo 2^31 */
  if (s >= 2147483648ul) s -= 2147483648ul;

  return ( s );
}

int set_state_urand(double g)
{
  if ( g == floor(g) &&  0 <= g && g <= 2147483647 )
    {
      s = (unsigned long) g;
      return ( 1 );
    }
  else
    {
      sciprint("\n\r bad seed for urand, must be an integer in [0,  2147483647]\n\r");
      return ( 0 );
    }
}

void get_state_urand(double g[])
{
  g[0] = (double) s;
}


  
