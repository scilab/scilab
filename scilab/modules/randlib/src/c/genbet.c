#include <math.h>
#include "grand.h"
#include "core_math.h"

double C2F(genbet)(double *aa, double *bb)
/*
**********************************************************************
This source code was taken in the project "freemat"(BSD license)
This source code was modified by Gaüzère Sabine according to the
modifications done by JJV

     float genbet(float aa,float bb)
               GeNerate BETa random deviate
                              Function
     Returns a single random deviate from the beta distribution with
     parameters A and B.  The density of the beta is
               x^(a-1) * (1-x)^(b-1) / B(a,b) for 0 < x < 1

                              Arguments

     A --> First parameter of the beta distribution
                         DOUBLE PRECISION A
     JJV                 (A > 1.0E-37)

     B --> Second parameter of the beta distribution
                         DOUBLE PRECISION B
     JJV                 (B > 1.0E-37)

                              Method
     R. C. H. Cheng
     Generating Beta Variatew with Nonintegral Shape Parameters
     Communications of the ACM, 21:317-322  (1978)
     (Algorithms BB and BC)
**********************************************************************
*/
{
#define expmax 87.498222998046875
#define infnty 1.0E38
#define minlog 1.0E-37

    static double olda = -1.0E37;
    static double oldb = -1.0E37;
    static double genbet, a, alpha, b, beta, delta, gamma, k1, k2, r, s, t, u1, u2, v, w, y, z;
    static int qsame;

    qsame = olda == *aa && oldb == *bb;
    if (qsame)
    {
        goto S20;
    }
    //if(!(aa <= 0.0 || bb <= 0.0)) goto S10;
    //    Scierror(999, "RANLIB Error: AA or BB <= 0 in GENBET - Abort!");
    //  JJV added small minimum for small log problem in calc of W
    //  in Rand.c
#ifndef _MSC_VER
    // warning C4102: 'S10' : unreferenced label
S10:
#endif
    olda = *aa;
    oldb = *bb;

S20:
    if (!(Min(*aa, *bb) > 1.0))
    {
        goto S100;
    }
    /*
         Alborithm BB
         Initialize
    */
    if (qsame)
    {
        goto S30;
    }
    a = Min(*aa, *bb);
    b = Max(*aa, *bb);
    alpha = a + b;
    beta = sqrt((alpha - 2.0) / (2.0 * a * b - alpha));
    gamma = a + 1.0 / beta;
S30:
S40:
    u1 = C2F(ranf)();
    /*
         Step 1
    */
    u2 = C2F(ranf)();
    v = beta * log(u1 / (1.0 - u1));
    //  JJV altered this
    if (v > expmax)
    {
        goto S55;
    }
    //  JJV added checker to see if a*exp(v) will overflow
    //  JJV 50 _was_ w = a*exp(v); also note here a > 1.0
    //    if(!(v > expmax)) goto S50;
    //    w = infnty;
    //    goto S60;
#ifndef _MSC_VER
    // warning C4102: 'S50' : unreferenced label
S50:
#endif
    w = exp(v);
    if (w > (infnty / a))
    {
        goto S55;
    }
    w *= a;
    goto S60;

S55:
    w = infnty;
S60:
    z = pow(u1, 2.0f) * u2;
    r = gamma * v - 1.3862943649291992188;
    s = a + r - w;
    /*
         Step 2
    */
    if (s + 2.6094379425048828125 >= 5.0 * z)
    {
        goto S70;
    }
    /*
         Step 3
    */
    t = log(z);
    if (s > t)
    {
        goto S70;
    }
    /*
         Step 4
    */
    //    JJV added checker to see if log(alpha/(b+w)) will
    //    JJV overflow.  If so, we count the log as -INF, and
    //    JJV consequently evaluate conditional as true, i.e.
    //    JJV the algorithm rejects the trial and starts over
    //    JJV May not need this here since ALPHA > 2.0
    if (alpha / (b + w) < minlog)
    {
        goto S40;
    }
    if (r + alpha * log(alpha / (b + w)) < t)
    {
        goto S40;
    }
S70:
    /*
         Step 5
    */
    if (!(*aa == a))
    {
        goto S80;
    }
    genbet = w / (b + w);
    goto S90;
S80:
    genbet = b / (b + w);
S90:
    goto S230;
S100:
    /*
         Algorithm BC
         Initialize
    */
    if (qsame)
    {
        goto S110;
    }
    a = Max(*aa, *bb);
    b = Min(*aa, *bb);
    alpha = a + b;
    beta = 1.0 / b;
    delta = 1.0 + a - b;
    k1 = delta * (1.3888900168240070343E-2 + 4.1666701436042785645E-2 * b) / (a * beta - 0.77777802944183349609);
    k2 = 0.25 + (0.5 + 0.25 / delta) * b;
S110:
S120:
    u1 = C2F(ranf)();
    /*
         Step 1
    */
    u2 = C2F(ranf)();
    if (u1 >= 0.5)
    {
        goto S130;
    }
    /*
         Step 2
    */
    y = u1 * u2;
    z = u1 * y;
    if (0.25 * u2 + z - y >= k1)
    {
        goto S120;
    }
    goto S170;
S130:
    /*
         Step 3
    */
    z = pow(u1, 2.0) * u2;
    if (!(z <= 0.25))
    {
        goto S160;
    }
    v = beta * log(u1 / (1.0 - u1));

    //  JJV instead of checking v > expmax at top, I will check
    //  JJV if a < 1, then check the appropriate values

    if (a > 1.0)
    {
        goto S135;
    }
    //  JJV A < 1 so it can help out if EXP(V) would overflow
    if (v > expmax)
    {
        goto S132;
    }
    w = a * exp(v);
    goto S200;
S132:
    w = v + log(a);
    if (w > expmax)
    {
        goto S140;
    }
    w = exp(w);
    goto S200;

    //  JJV in this case A > 1
S135:
    if (v > expmax)
    {
        goto S140;
    }
    w = exp(v);
    if (w > infnty / a)
    {
        goto S140;
    }
    w *= a;
    goto S200;
S140:
    w = infnty;
    goto S200;
    //S150:
    //    goto S200;
S160:
    if (z >= k2)
    {
        goto S120;
    }
S170:
    /*
         Step 4
         Step 5
    */
    v = beta * log(u1 / (1.0 - u1));
    //  JJV same kind of checking as above
    if (a > 1.0)
    {
        goto S175;
    }
    //  JJV A < 1 so it can help out if EXP(V) would overflow
    if (v > expmax)
    {
        goto S172;
    }
    w = a * exp(v);
    goto S190;
S172:
    w = v + log(a);
    if (w > expmax)
    {
        goto S180;
    }
    w = exp(w);
    goto S190;
    //  JJV in this case A > 1
S175:
    if (v > expmax)
    {
        goto S180;
    }
    w = exp(v);
    if (w > infnty / a)
    {
        goto S180;
    }
    w *= a;
    goto S190;

S180:
    w = infnty;

    //  JJV here we also check to see if log overlows; if so, we treat it
    //  JJV as -INF, which means condition is true, i.e. restart
S190:
    if (alpha / (b + w) < minlog)
    {
        goto S120;
    }
    if (alpha * (log(alpha / (b + w)) + v) - 1.3862943649291992188 < log(z))
    {
        goto S120;
    }
S200:
    /*
         Step 6
    */
    if (!(a == *aa))
    {
        goto S210;
    }
    genbet = w / (b + w);
    goto S220;
S210:
    genbet = b / (b + w);
S220:
S230:
    return genbet;
#undef expmax
#undef infnty
#undef minlog
}
