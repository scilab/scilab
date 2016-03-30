#include <math.h>
#include "grand.h"
#include "core_math.h"

double fsign (double x, double y)
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

int C2F(ignpoi)(double *mu)
/*
**********************************************************************
This source code was taken in the project "freemat"(BSD license)
This source code was modified by Gaüzère Sabine according to the
modifications done by JJV

     long ignpoi(float mu)
                    GENerate POIsson random deviate
                              Function
     Generates a single random deviate from a Poisson
     distribution with mean AV.
                              Arguments
     av --> The mean of the Poisson distribution from which
            a random deviate is to be generated.
     genexp <-- The random deviate.
                              Method
     Renames KPOIS from TOMS as slightly modified by BWB to use RANF
     instead of SUNIF.
     For details see:
               Ahrens, J.H. and Dieter, U.
               Computer Generation of Poisson Deviates
               From Modified Normal Distributions.
               ACM Trans. Math. Software, 8, 2
               (June 1982),163-179
**********************************************************************
**********************************************************************


     P O I S S O N  DISTRIBUTION


**********************************************************************
**********************************************************************

     FOR DETAILS SEE:

               AHRENS, J.H. AND DIETER, U.
               COMPUTER GENERATION OF POISSON DEVIATES
               FROM MODIFIED NORMAL DISTRIBUTIONS.
               ACM TRANS. MATH. SOFTWARE, 8,2 (JUNE 1982), 163 - 179.

     (SLIGHTLY MODIFIED VERSION OF THE PROGRAM IN THE ABOVE ARTICLE)

**********************************************************************
      INTEGER FUNCTION IGNPOI(IR,MU)
     INPUT:  IR=CURRENT STATE OF BASIC RANDOM NUMBER GENERATOR
             MU=MEAN MU OF THE POISSON DISTRIBUTION
     OUTPUT: IGNPOI=SAMPLE FROM THE POISSON-(MU)-DISTRIBUTION
     MUPREV=PREVIOUS MU, MUOLD=MU AT LAST EXECUTION OF STEP P OR B.
     TABLES: COEFFICIENTS A0-A7 FOR STEP F. FACTORIALS FACT
     COEFFICIENTS A(K) - FOR PX = FK*V*V*SUM(A(K)*V**K)-DEL
     SEPARATION OF CASES A AND B
*/
{
    //extern float sign( float num, float sign );
    static double a0 = -0.5;
    static double a1 = 0.3333333;
    static double a2 = -0.2500068;
    static double a3 = 0.2000118;
    static double a4 = -0.1661269;
    static double a5 = 0.1421878;
    static double a6 = -0.1384794;
    static double a7 = 0.125006;
    static double muold = -1.0E37;
    static double muprev = -1.0E37;
    static double fact[10] =
    {
        1.0, 1.0, 2.0, 6.0, 24.0, 120.0, 720.0, 5040.0, 40320.0, 362880.0
    };
    static int ignpoi, j, k, kflag, l, m, ll;
    // JJV I added a variable 'll' here - it is the 'l' for CASE A
    static double b1, b2, c, c0, c1, c2, c3, d, del, difmuk, e, fk, fx, fy, g, omega, p, p0, px, py, q, s,
           t, u, v, x, xx, pp[35];

    if (*mu == muprev)
    {
        goto S10;
    }
    if (*mu < 10.0)
    {
        goto S120;
    }
    /*
         C A S E  A. (RECALCULATION OF S,D,L IF MU HAS CHANGED)
    */
    //   JJV This is the case where I changed 'l' to 'll'
    //   JJV Here 'll' is set once and used in a comparison once
    muprev = *mu;
    s = sqrt(*mu);
    d = 6.0 * *mu * *mu;
    /*
                 THE POISSON PROBABILITIES PK EXCEED THE DISCRETE NORMAL
                 PROBABILITIES FK WHENEVER K >= M(MU). L=IFIX(MU-1.1484)
                 IS AN UPPER BOUND TO M(MU) FOR ALL MU >= 10 .
    */
    ll = (int) (*mu - 1.1484);
S10:
    /*
         STEP N. NORMAL SAMPLE - SNORM(IR) FOR STANDARD NORMAL DEVIATE
    */
    g = *mu + s * C2F(snorm)();
    if (g < 0.0)
    {
        goto S20;
    }
    ignpoi = (int) (g);
    /*
         STEP I. IMMEDIATE ACCEPTANCE IF IGNPOI IS LARGE ENOUGH
    */
    if (ignpoi >= ll)
    {
        return ignpoi;
    }
    /*
         STEP S. SQUEEZE ACCEPTANCE - SUNIF(IR) FOR (0,1)-SAMPLE U
    */
    fk = (double)ignpoi;
    difmuk = *mu - fk;
    u = C2F(ranf)();
    if (d * u >= difmuk * difmuk * difmuk)
    {
        return ignpoi;
    }
S20:
    /*
         STEP P. PREPARATIONS FOR STEPS Q AND H.
                 (RECALCULATIONS OF PARAMETERS IF NECESSARY)
                 .3989423=(2*PI)**(-.5)  .416667E-1=1./24.  .1428571=1./7.
                 THE QUANTITIES B1, B2, C3, C2, C1, C0 ARE FOR THE HERMITE
                 APPROXIMATIONS TO THE DISCRETE NORMAL PROBABILITIES FK.
                 C=.1069/MU GUARANTEES MAJORIZATION BY THE 'HAT'-FUNCTION.
    */
    if (*mu == muold)
    {
        goto S30;
    }
    muold = *mu;
    omega = 0.3989423 / s;
    b1 = 4.166667E-2 / *mu;
    b2 = 0.3 * b1 * b1;
    c3 = 0.1428571 * b1 * b2;
    c2 = b2 - 15.0 * c3;
    c1 = b1 - 6.0 * b2 + 45.0 * c3;
    c0 = 1.0 - b1 + 3.0 * b2 - 15.0 * c3;
    c = 0.1069 / *mu;
S30:
    if (g < 0.0)
    {
        goto S50;
    }
    /*
                 'SUBROUTINE' F IS CALLED (KFLAG=0 FOR CORRECT RETURN)
    */
    kflag = 0;
    goto S70;
S40:
    /*
         STEP Q. QUOTIENT ACCEPTANCE (RARE CASE)
    */
    if (fy - u * fy <= py * exp(px - fx))
    {
        return ignpoi;
    }
S50:
    /*
         STEP E. EXPONENTIAL SAMPLE - SEXPO(IR) FOR STANDARD EXPONENTIAL
                 DEVIATE E AND SAMPLE T FROM THE LAPLACE 'HAT'
                 (IF T <= -.6744 THEN PK < FK FOR ALL MU >= 10.)
    */
    e = C2F(sexpo)();
    u = C2F(ranf)();
    u += (u - 1.0);
    t = 1.8 + fsign(e, u);
    if (t <= -0.6744)
    {
        goto S50;
    }
    ignpoi = (int) (*mu + s * t);
    fk = (double)ignpoi;
    difmuk = *mu - fk;
    /*
                 'SUBROUTINE' F IS CALLED (KFLAG=1 FOR CORRECT RETURN)
    */
    kflag = 1;
    goto S70;
S60:
    /*
         STEP H. HAT ACCEPTANCE (E IS REPEATED ON REJECTION)
    */
    if (c * Abs(u) > py * exp(px + e) - fy * exp(fx + e))
    {
        goto S50;
    }
    return ignpoi;
S70:
    /*
         STEP F. 'SUBROUTINE' F. CALCULATION OF PX,PY,FX,FY.
                 CASE IGNPOI .LT. 10 USES FACTORIALS FROM TABLE FACT
    */
    if (ignpoi >= 10)
    {
        goto S80;
    }
    px = - *mu;
    py = pow((double) * mu, (double)ignpoi) / *(fact + ignpoi);
    goto S110;
S80:
    /*
                 CASE IGNPOI .GE. 10 USES POLYNOMIAL APPROXIMATION
                 A0-A7 FOR ACCURACY WHEN ADVISABLE
                 .8333333E-1=1./12.  .3989423=(2*PI)**(-.5)
    */
    del = 8.333333E-2 / fk;
    del -= (4.8 * del * del * del);
    v = difmuk / fk;
    if (Abs(v) <= 0.25)
    {
        goto S90;
    }
    px = fk * log(1.0 + v) - difmuk - del;
    goto S100;
S90:
    px = fk * v * v * (((((((a7 * v + a6) * v + a5) * v + a4) * v + a3) * v + a2) * v + a1) * v + a0) - del;
S100:
    py = 0.3989423 / sqrt(fk);
S110:
    x = (0.5 - difmuk) / s;
    xx = x * x;
    fx = -0.5 * xx;
    fy = omega * (((c3 * xx + c2) * xx + c1) * xx + c0);
    if (kflag <= 0)
    {
        goto S40;
    }
    goto S60;
S120:
    /*
         C A S E  B. (START NEW TABLE AND CALCULATE P0 IF NECESSARY)
    */
    //  JJV changed MUPREV assignment from 0.0 to initial value
    muprev = -1.0E37;
    //     Jpc 1999: the next lines seams to produce a bug
    //     and I finaly commented them out
    //     IF (mu.EQ.muold) GO TO 130
    //     JJV added argument checker here
    //     JJV added line label here
    // 125  muold = mu
    //    if(mu == muold) goto S130;
    //    muold = mu;
    m = Max(1L, (int) (*mu));
    l = 0;
    p = exp(- *mu);
    q = p0 = p;
S130:
    /*
         STEP U. UNIFORM SAMPLE FOR INVERSION METHOD
    */
    u = C2F(ranf)();
    ignpoi = 0;
    if (u <= p0)
    {
        return ignpoi;
    }
    /*
         STEP T. TABLE COMPARISON UNTIL THE END PP(L) OF THE
                 PP-TABLE OF CUMULATIVE POISSON PROBABILITIES
                 (0.458=PP(9) FOR MU=10)
    */
    if (l == 0)
    {
        goto S150;
    }
    j = 1;
    if (u > 0.458)
    {
        j = Min(l, m);
    }
    for (k = j; k <= l; k++)
    {
        if (u <= *(pp + k - 1))
        {
            goto S180;
        }
    }
    if (l == 35)
    {
        goto S130;
    }
S150:
    /*
         STEP C. CREATION OF NEW POISSON PROBABILITIES P
                 AND THEIR CUMULATIVES Q=PP(K)
    */
    l += 1;
    for (k = l; k <= 35; k++)
    {
        p = p * *mu / (double)k;
        q += p;
        *(pp + k - 1) = q;
        if (u <= q)
        {
            goto S170;
        }
    }
    l = 35;
    goto S130;
S170:
    l = k;
S180:
    ignpoi = k;
    return ignpoi;
}
