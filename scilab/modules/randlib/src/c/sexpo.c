#include <math.h>
#include "grand.h"
#include "core_math.h"

double C2F(sexpo)(void)
/*
**********************************************************************


     (STANDARD-)  E X P O N E N T I A L   DISTRIBUTION


**********************************************************************
**********************************************************************

This source code was taken in the project "freemat"(BSD license)
This source code was modified by Gaüzère Sabine according to the
modifications done by JJV

     FOR DETAILS SEE:

               AHRENS, J.H. AND DIETER, U.
               COMPUTER METHODS FOR SAMPLING FROM THE
               EXPONENTIAL AND NORMAL DISTRIBUTIONS.
               COMM. ACM, 15,10 (OCT. 1972), 873 - 882.

     ALL STATEMENT NUMBERS CORRESPOND TO THE STEPS OF ALGORITHM
     'SA' IN THE ABOVE PAPER (SLIGHTLY MODIFIED IMPLEMENTATION)

     Modified by Barry W. Brown, Feb 3, 1988 to use RANF instead of
     SUNIF.  The argument IR thus goes away.

**********************************************************************
     Q(N) = SUM(ALOG(2.0)**K/K!)    K=1,..,N ,      THE HIGHEST N
     (HERE 8) IS DETERMINED BY Q(N)=1.0 WITHIN STANDARD PRECISION
*/
{
    /* ** OLD VALUES
    static double q[8] = {
        0.69314718055995, 0.93337368751905, 0.98887779618387, 0.99849592529150,
        0.99982928110614, 0.99998331641007, 0.99999856914388, 0.99999989069256
    //0.6931472,0.9333737,0.9888778,0.9984959,0.9998293,0.9999833,0.9999986,0.9999999
    };
    */
    static double q[8] =
    {
        0.69314718246459960938,
        0.93337368965148925781,
        0.98887777328491210938,
        0.99849587678909301758,
        0.99982929229736328125,
        0.99998331069946289062,
        0.99999862909317016602,
        0.99999988079071044922
    };
    static int i;
    static double sexpo, a, u, ustar, umin;

    a = 0.0;
    u = C2F(ranf)();
    goto S30;
S20:
    a += q[0];
S30:
    u += u;
    //  JJV changed the following to reflect the true algorithm and
    //  JJV prevent unpredictable behavior if U is initially 0.5.
    //  IF (u.LE.1.0) GO TO 20
    if (u < 1.0)
    {
        goto S20;
    }
    u -= 1.0;
    if (u > q[0])
    {
        goto S60;
    }
    sexpo = a + u;
    return sexpo;
S60:
    i = 1;
    ustar = C2F(ranf)();
    umin = ustar;
S70:
    ustar = C2F(ranf)();
    if (ustar < umin)
    {
        umin = ustar;
    }
    i += 1;
    if (u > q[i - 1])
    {
        goto S70;
    }
    sexpo = a + umin * q[0];
    return sexpo;
}
