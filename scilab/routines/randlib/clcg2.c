/*  uniform [0,1[ random number generator
 *  developed by Pierre Lecuyer based on a clever
 *  and tested combination of two linear congruential
 *  sequences 
 *
 *  modif bruno pour retrouver exactement la version utilisee
 *  par Scilab
 *
 *  nb entiers generes dans [1, 2147483562], on se ramene a
 *  [0,1[ (en fait (]0,1[) en multipliant par :
 *
 *       1/2147483563 = 4.6566130573917691e-10
 *
 *   s1 and s2 are the seeds (nonnegative integers) : on doit avoir
 *
 *       1 <= s1 <=  2147483562
 *       1 <= s2 <=  2147483398
 */

#include "../graphics/Math.h" /* to use sciprint */
#include <math.h>


static long s1 = 1234567890 ;
static long s2 = 123456789  ;

double clcg2()
{
  register long k,z;
  k= s1 /53668;
  s1 =40014*(s1%53668)-k*12211;
  if (s1 < 0) s1 += 2147483563;
  k=s2/52774;
  s2=40692*(s2%52774)-k*3791;
  if (s2 < 0) s2 += 2147483399;
  z = s1 - s2;
  if (z < 1) z += 2147483562;
  
  return( (double) z * 4.6566130573917691e-10);
}

int set_state_clcg2(double g1, double g2)
{
  
  if ( g1 == floor(g1) && g2 == floor(g2)  && 
       1 <= g1 && g1 <= 2147483562    &&
       1 <= g2 && g2 <= 2147483398 )
    {
      s1 = (long) g1;
      s2 = (long) g2;
      return ( 1 );
    }
  else
    {
      sciprint("\n\r bad seeds for clcg2 : must be integers with : 1 <= s1 <= 2147483562");
      sciprint("\n\r                                               1 <= s2 <= 2147483398 \n\r");
      return ( 0 );
    }
}

void get_state_clcg2(double g[])
{
  g[0] = (double) s1;
  g[1] = (double) s2;
}

