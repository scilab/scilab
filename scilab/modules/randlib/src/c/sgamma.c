#include <math.h>
#include "grand.h"
#include "core_math.h"


double fsign1 (double x, double y)
{
    if (y >= 0.0)
    {
        return Abs(x);
    }
    else
    {
        return -Abs(x);
    }
}

double C2F(sgamma)(double *a)
/*
**********************************************************************


     (STANDARD-)  G A M M A  DISTRIBUTION


**********************************************************************
**********************************************************************

               PARAMETER  A >= 1.0  !

**********************************************************************
This source code was taken in the project "freemat"(BSD license)
This source code was modified by Gaüzère Sabine according to the
modifications done by JJV

     FOR DETAILS SEE:

               AHRENS, J.H. AND DIETER, U.
               GENERATING GAMMA VARIATES BY A
               MODIFIED REJECTION TECHNIQUE.
               COMM. ACM, 25,1 (JAN. 1982), 47 - 54.

     STEP NUMBERS CORRESPOND TO ALGORITHM 'GD' IN THE ABOVE PAPER
                                 (STRAIGHTFORWARD IMPLEMENTATION)

     Modified by Barry W. Brown, Feb 3, 1988 to use RANF instead of
     SUNIF.  The argument IR thus goes away.

**********************************************************************

               PARAMETER  0.0 < A < 1.0  !

**********************************************************************

     FOR DETAILS SEE:

               AHRENS, J.H. AND DIETER, U.
               COMPUTER METHODS FOR SAMPLING FROM GAMMA,
               BETA, POISSON AND BINOMIAL DISTRIBUTIONS.
               COMPUTING, 12 (1974), 223 - 246.

     (ADAPTED IMPLEMENTATION OF ALGORITHM 'GS' IN THE ABOVE PAPER)

**********************************************************************
     INPUT: A =PARAMETER (MEAN) OF THE STANDARD GAMMA DISTRIBUTION
     OUTPUT: SGAMMA = SAMPLE FROM THE GAMMA-(A)-DISTRIBUTION
     COEFFICIENTS Q(K) - FOR Q0 = SUM(Q(K)*A**(-K))
     COEFFICIENTS A(K) - FOR Q = Q0+(T*T/2)*SUM(A(K)*V**K)
     COEFFICIENTS E(K) - FOR EXP(Q)-1 = SUM(E(K)*Q**K)
     PREVIOUS A PRE-SET TO ZERO - AA IS A', AAA IS A"
     SQRT32 IS THE SQUAREROOT OF 32 = 5.656854249492380
*/
{
    //extern float sign( float num, float sign );
    static double q1 = 4.166669026017189026E-2;
    static double q2 = 2.083148062229156494E-2;
    static double q3 = 8.01191013306379318E-3;
    static double q4 = 1.44121004268527031E-3;
    static double q5 = -7.388000085484236E-5;
    static double q6 = 2.4510998628102243E-4;
    static double q7 = 2.4239999765995890E-4;
    static double a1 = 0.33333331346511840820;
    static double a2 = -0.25000301003456115723;
    static double a3 = 0.20000620186328887939;
    static double a4 = -0.16629210114479064941;
    static double a5 = 0.14236569404602050781;
    static double a6 = -0.13671770691871643066;
    static double a7 = 0.12337949872016906738;
    static double e1 = 1.0;
    static double e2 = 0.49998968839645385742;
    static double e3 = 0.16682900488376617432;
    static double e4 = 4.077529907226562500E-2;
    static double e5 = 1.029300037771463394E-2;
    static double aa = 0.0;
    static double aaa = 0.0;
    static double sqrt32 = 5.65685415267944335938;
    static double sgamma, s2, s, d, t, x, u, r, q0, b, si, c, v, q, e, w, p, b0;
    if (*a == aa)
    {
        goto S10;
    }
    if (*a < 1.0)
    {
        goto S130;
    }
    /*
         STEP  1:  RECALCULATIONS OF S2,S,D IF A HAS CHANGED
    */
    aa = *a;
    s2 = *a - 0.5;
    s = sqrt(s2);
    d = sqrt32 - 12.0 * s;
S10:
    /*
         STEP  2:  T=STANDARD NORMAL DEVIATE,
                   X=(S,1/2)-NORMAL DEVIATE.
                   IMMEDIATE ACCEPTANCE (I)
    */
    t = C2F(snorm)();
    x = s + 0.5 * t;
    sgamma = x * x;
    if (t >= 0.0)
    {
        return sgamma;
    }
    /*
         STEP  3:  U= 0,1 -UNIFORM SAMPLE. SQUEEZE ACCEPTANCE (S)
    */
    u = C2F(ranf)();
    if (d * u <= t * t * t)
    {
        return sgamma;
    }
    /*
         STEP  4:  RECALCULATIONS OF Q0,B,SI,C IF NECESSARY
    */
    if (*a == aaa)
    {
        goto S40;
    }
    aaa = *a;
    r = 1.0 / *a;
    q0 = ((((((q7 * r + q6) * r + q5) * r + q4) * r + q3) * r + q2) * r + q1) * r;
    /*
                   APPROXIMATION DEPENDING ON SIZE OF PARAMETER A
                   THE CONSTANTS IN THE EXPRESSIONS FOR B, SI AND
                   C WERE ESTABLISHED BY NUMERICAL EXPERIMENTS
    */
    if (*a <= 3.686)
    {
        goto S30;    //3.68600010871887207031
    }
    if (*a <= 13.022)
    {
        goto S20;    //13.02200031280517578125
    }
    /*
                   CASE 3:  A .GT. 13.022
    */
    b = 1.76999998092651367188;
    si = 0.75;
    c = 0.15150000154972076416 / s;
    goto S40;
S20:
    /*
                   CASE 2:  3.686 .LT. A .LE. 13.022
    */
    b = 1.65400004386901855469 + 7.60000012814998627E-3 * s2;
    si = 1.67999994754791259766 / s + 0.27500000596046447754;
    c = 6.199999898672103882E-2 / s + 2.400000020861625671E-2;
    goto S40;
S30:
    /*
                   CASE 1:  A .LE. 3.686
    */
    b = 0.46299999952316284180 + s + 0.17800000309944152832 * s2;
    si = 1.23500001430511474609;
    c = 0.19499999284744262695 / s - 7.900000363588333130E-2 + 1.5999999642372131348E-1 * s;
S40:
    /*
         STEP  5:  NO QUOTIENT TEST IF X NOT POSITIVE
    */
    if (x <= 0.0)
    {
        goto S70;
    }
    /*
         STEP  6:  CALCULATION OF V AND QUOTIENT Q
    */
    v = t / (s + s);
    if (Abs(v) <= 0.25)
    {
        goto S50;
    }
    q = q0 - s * t + 0.25 * t * t + (s2 + s2) * log(1.0 + v);
    goto S60;
S50:
    q = q0 + 0.5 * t * t * ((((((a7 * v + a6) * v + a5) * v + a4) * v + a3) * v + a2) * v + a1) * v;
S60:
    /*
         STEP  7:  QUOTIENT ACCEPTANCE (Q)
    */
    if (log(1.0 - u) <= q)
    {
        return sgamma;
    }
S70:
    /*
         STEP  8:  E=STANDARD EXPONENTIAL DEVIATE
                   U= 0,1 -UNIFORM DEVIATE
                   T=(B,SI)-DOUBLE EXPONENTIAL (LAPLACE) SAMPLE
    */
    e = C2F(sexpo)();
    u = C2F(ranf)();
    u += (u - 1.0);
    t = b + fsign1(si * e, u);
    /*
         STEP  9:  REJECTION IF T .LT. TAU(1) = -.71874483771719
    */
    if (t < -0.7187449)
    {
        goto S70;    //-0.71874487400054931641
    }
    /*
         STEP 10:  CALCULATION OF V AND QUOTIENT Q
    */
    v = t / (s + s);
    if (Abs(v) <= 0.25)
    {
        goto S90;
    }
    q = q0 - s * t + 0.25 * t * t + (s2 + s2) * log(1.0 + v);
    goto S100;
S90:
    q = q0 + 0.5 * t * t * ((((((a7 * v + a6) * v + a5) * v + a4) * v + a3) * v + a2) * v + a1) * v;
S100:
    /*
         STEP 11:  HAT ACCEPTANCE (H) (IF Q NOT POSITIVE GO TO STEP 8)
    */
    if (q <= 0.0)
    {
        goto S70;
    }
    if (q <= 0.5)
    {
        goto S110;
    }
    //  JJV modified the code through line 125 to handle large Q case
    if (q < 15.0)
    {
        goto S105;
    }
    //  JJV Here Q is large enough that Q = log(exp(Q) - 1.0) (for DOUBLE PRECISION Q)
    //  JJV so reformulate test at 120 in terms of one EXP, if not too big
    //  JJV 87.49823 is close to the largest DOUBLE PRECISION which can be
    //  JJV exponentiated (87.49823 = log(1.0E38))
    if ((q + e - 0.5 * t * t) > 87.49823)
    {
        goto S125;    //87.498222998046875
    }
    if (c * Abs(u) > exp(q + e - 0.5 * t * t))
    {
        goto S70;
    }
    goto S125;
S105:
    w = exp(q) - 1.0;
    goto S120;
S110:
    w = ((((e5 * q + e4) * q + e3) * q + e2) * q + e1) * q;
S120:
    /*
                   IF T IS REJECTED, SAMPLE AGAIN AT STEP 8
    */
    if (c * Abs(u) > w * exp(e - 0.5 * t * t))
    {
        goto S70;
    }
S125:
    x = s + 0.5 * t;
    sgamma = x * x;
    return sgamma;
    /*
         ALTERNATE METHOD FOR PARAMETERS A BELOW 1  (.3678794=EXP(-1.))
    */
    //     JJV changed B to B0 (which was added to declarations for this)
    //     JJV in 130 to END to fix rare and subtle bug.
    //     JJV Line: '130 aa = 0.0' was removed (unnecessary, wasteful).
    //     JJV Reasons: the state of AA only serves to tell the A .GE. 1.0
    //     JJV case if certain A-dependant constants need to be recalculated.
    //     JJV The A .LT. 1.0 case (here) no longer changes any of these, and
    //     JJV the recalculation of B (which used to change with an
    //     JJV A .LT. 1.0 call) is governed by the state of AAA anyway.
S130:
    b0 = 1.0 + 0.3678794 * (*a); //0.36787939071655273438
S140:
    p = b0 * C2F(ranf)();
    if (p >= 1.0)
    {
        goto S150;
    }
    sgamma = exp(log(p) / *a);
    if (C2F(sexpo)() < sgamma)
    {
        goto S140;
    }
    return sgamma;
S150:
    sgamma = -log((b0 - p) / *a);
    if (C2F(sexpo)() < (1.0 - *a)*log(sgamma))
    {
        goto S140;
    }
    return sgamma;
}
