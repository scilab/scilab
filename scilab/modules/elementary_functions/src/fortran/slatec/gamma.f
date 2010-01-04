CS    REAL FUNCTION GAMMA(X)
      DOUBLE PRECISION FUNCTION DGAMMACODY(X)
C----------------------------------------------------------------------
C
C This routine calculates the GAMMA function for a real argument X.
C   Computation is based on an algorithm outlined in reference 1.
C   The program uses rational functions that approximate the GAMMA
C   function to at least 20 significant decimal digits.  Coefficients
C   for the approximation over the interval (1,2) are unpublished.
C   Those for the approximation for X .GE. 12 are from reference 2.
C   The accuracy achieved depends on the arithmetic system, the
C   compiler, the intrinsic functions, and proper selection of the
C   machine-dependent constants.
C
C
C*******************************************************************
C*******************************************************************
C
C Explanation of machine-dependent constants
C
C beta   - radix for the floating-point representation
C maxexp - the smallest positive power of beta that overflows
C XBIG   - the largest argument for which GAMMA(X) is representable
C          in the machine, i.e., the solution to the equation
C                  GAMMA(XBIG) = beta**maxexp
C XINF   - the largest machine representable floating-point number;
C          approximately beta**maxexp
C EPS    - the smallest positive floating-point number such that
C          1.0+EPS .GT. 1.0
C XMININ - the smallest positive floating-point number such that
C          1/XMININ is machine representable
C
C     Approximate values for some important machines are:
C
C                            beta       maxexp        XBIG
C
C CRAY-1         (S.P.)        2         8191        966.961
C Cyber 180/855
C   under NOS    (S.P.)        2         1070        177.803
C IEEE (IBM/XT,
C   SUN, etc.)   (S.P.)        2          128        35.040
C IEEE (IBM/XT,
C   SUN, etc.)   (D.P.)        2         1024        171.624
C IBM 3033       (D.P.)       16           63        57.574
C VAX D-Format   (D.P.)        2          127        34.844
C VAX G-Format   (D.P.)        2         1023        171.489
C
C                            XINF         EPS        XMININ
C
C CRAY-1         (S.P.)   5.45E+2465   7.11E-15    1.84E-2466
C Cyber 180/855
C   under NOS    (S.P.)   1.26E+322    3.55E-15    3.14E-294
C IEEE (IBM/XT,
C   SUN, etc.)   (S.P.)   3.40E+38     1.19E-7     1.18E-38
C IEEE (IBM/XT,
C   SUN, etc.)   (D.P.)   1.79D+308    2.22D-16    2.23D-308
C IBM 3033       (D.P.)   7.23D+75     2.22D-16    1.39D-76
C VAX D-Format   (D.P.)   1.70D+38     1.39D-17    5.88D-39
C VAX G-Format   (D.P.)   8.98D+307    1.11D-16    1.12D-308
C
C*******************************************************************
C*******************************************************************
C
C Error returns
C
C  The program returns the value XINF for singularities or
C     when overflow would occur.  The computation is believed
C     to be free of underflow and overflow.
C
C
C  Intrinsic functions required are:
C
C     INT, DBLE, EXP, LOG, REAL, SIN
C
C
C References: "An Overview of Software Development for Special
C              Functions", W. J. Cody, Lecture Notes in Mathematics,
C              506, Numerical Analysis Dundee, 1975, G. A. Watson
C              (ed.), Springer Verlag, Berlin, 1976.
C
C              Computer Approximations, Hart, Et. Al., Wiley and
C              sons, New York, 1968.
C
C  Latest modification: October 12, 1989
C
C  Authors: W. J. Cody and L. Stoltz
C           Applied Mathematics Division
C           Argonne National Laboratory
C           Argonne, IL 60439
C
C----------------------------------------------------------------------
*
*  A few modifs from Bruno (25 Feb 2005) from a Serge 's request:
*
*     - change the name of this function (DGAMMA ->  DGAMMACODY) to avoid conflict with 
*       the gamma Slatec (file dgamma.f). (in fact thare was no conflict as this
*       function was retrieved in the makefile...)
*    
*     - modify to get "better" values in some cases:
*
*         1/ when x is small we used more completly the equivalent gamma(x) ~ 1/x
*            (the original code uses it only if XMININ <= x < eps)
*            this lets to get  +-Inf for x = +-0
*         2/ when x is a negative integer return Nan (in place of XINF)
*         3/ when the gamma overflow return Inf (in place of XINF)
*       Serge asks me to change this in the Slatec gamma function but I try
*       first to do it in the Cody 's gamma... In fact to do a real job an
*       exception may be returned (with an integer flag say IERR) in theses
*       cases so as to prevent the user depending on the ieee scilab var.
*
      INTEGER I,N
      LOGICAL PARITY
CS    REAL 
      DOUBLE PRECISION 
     1    C,CONV,EPS,FACT,HALF,ONE,P,PI,Q,RES,SQRTPI,SUM,TWELVE,
     2    TWO,X,XBIG,XDEN,XINF,XNUM,Y,Y1,YSQ,Z,ZERO, XMININ
      DIMENSION C(7),P(8),Q(8)
C----------------------------------------------------------------------
C  Mathematical constants
C----------------------------------------------------------------------
CS    DATA ONE,HALF,TWELVE,TWO,ZERO/1.0E0,0.5E0,12.0E0,2.0E0,0.0E0/,
CS   1     SQRTPI/0.9189385332046727417803297E0/,
CS   2     PI/3.1415926535897932384626434E0/
      DATA ONE,HALF,TWELVE,TWO,ZERO/1.0D0,0.5D0,12.0D0,2.0D0,0.0D0/,
     1     SQRTPI/0.9189385332046727417803297D0/,
     2     PI/3.1415926535897932384626434D0/
C----------------------------------------------------------------------
C  Machine dependent parameters
C----------------------------------------------------------------------
CS    DATA XBIG,XMININ,EPS/35.040E0,1.18E-38,1.19E-7/,
CS   1     XINF/3.4E38/
      DATA XBIG,XMININ,EPS/171.624D0,2.23D-308,2.22D-16/,
     1     XINF/1.79D308/
C----------------------------------------------------------------------
C  Numerator and denominator coefficients for rational minimax
C     approximation over (1,2).
C----------------------------------------------------------------------
CS    DATA P/-1.71618513886549492533811E+0,2.47656508055759199108314E+1,
CS   1       -3.79804256470945635097577E+2,6.29331155312818442661052E+2,
CS   2       8.66966202790413211295064E+2,-3.14512729688483675254357E+4,
CS   3       -3.61444134186911729807069E+4,6.64561438202405440627855E+4/
CS    DATA Q/-3.08402300119738975254353E+1,3.15350626979604161529144E+2,
CS   1      -1.01515636749021914166146E+3,-3.10777167157231109440444E+3,
CS   2        2.25381184209801510330112E+4,4.75584627752788110767815E+3,
CS   3      -1.34659959864969306392456E+5,-1.15132259675553483497211E+5/
      DATA P/-1.71618513886549492533811D+0,2.47656508055759199108314D+1,
     1       -3.79804256470945635097577D+2,6.29331155312818442661052D+2,
     2       8.66966202790413211295064D+2,-3.14512729688483675254357D+4,
     3       -3.61444134186911729807069D+4,6.64561438202405440627855D+4/
      DATA Q/-3.08402300119738975254353D+1,3.15350626979604161529144D+2,
     1      -1.01515636749021914166146D+3,-3.10777167157231109440444D+3,
     2        2.25381184209801510330112D+4,4.75584627752788110767815D+3,
     3      -1.34659959864969306392456D+5,-1.15132259675553483497211D+5/
C----------------------------------------------------------------------
C  Coefficients for minimax approximation over (12, INF).
C----------------------------------------------------------------------
CS    DATA C/-1.910444077728E-03,8.4171387781295E-04,
CS   1     -5.952379913043012E-04,7.93650793500350248E-04,
CS   2     -2.777777777777681622553E-03,8.333333333333333331554247E-02,
CS   3      5.7083835261E-03/
      DATA C/-1.910444077728D-03,8.4171387781295D-04,
     1     -5.952379913043012D-04,7.93650793500350248D-04,
     2     -2.777777777777681622553D-03,8.333333333333333331554247D-02,
     3      5.7083835261D-03/
C----------------------------------------------------------------------
C  Statement functions for conversion between integer and float
C----------------------------------------------------------------------
CS    CONV(I) = REAL(I)
      CONV(I) = DBLE(I)
      PARITY = .FALSE.
      FACT = ONE
      N = 0
      Y = X

      if (abs(Y) .LT. EPS) then
*        argument is enough small (to use the equivalent 1/x)
         RES = ONE / Y
         goto 900
 
      ELSE IF (Y .LE. ZERO) THEN
C----------------------------------------------------------------------
C  Argument is negative
C----------------------------------------------------------------------
         Y = -X
         Y1 = AINT(Y)
         RES = Y - Y1
         IF (RES .NE. ZERO) THEN
            IF (Y1 .NE. AINT(Y1*HALF)*TWO) PARITY = .TRUE.
            FACT = -PI / SIN(PI*RES)
            Y = Y + ONE
         ELSE
*          RES = XINF
* modif Bruno: return Nan (Y is a negative integer)
            CALL returnananfortran(RES)
            GO TO 900
         END IF
      END IF
C----------------------------------------------------------------------
C  Argument is positive
C----------------------------------------------------------------------
      IF (Y .LT. EPS) THEN
C----------------------------------------------------------------------
C  Argument .LT. EPS
C----------------------------------------------------------------------
*         IF (Y .GE. XMININ) THEN
         RES = ONE / Y
*         ELSE
*            RES = XINF
*            GO TO 900
*         END IF
      ELSE IF (Y .LT. TWELVE) THEN
         Y1 = Y
         IF (Y .LT. ONE) THEN
C----------------------------------------------------------------------
C  0.0 .LT. argument .LT. 1.0
C----------------------------------------------------------------------
            Z = Y
            Y = Y + ONE
         ELSE
C----------------------------------------------------------------------
C  1.0 .LT. argument .LT. 12.0, reduce argument if necessary
C----------------------------------------------------------------------
            N = INT(Y) - 1
            Y = Y - CONV(N)
            Z = Y - ONE
         END IF
C----------------------------------------------------------------------
C  Evaluate approximation for 1.0 .LT. argument .LT. 2.0
C----------------------------------------------------------------------
         XNUM = ZERO
         XDEN = ONE
         DO 260 I = 1, 8
            XNUM = (XNUM + P(I)) * Z
            XDEN = XDEN * Z + Q(I)
 260     CONTINUE
         RES = XNUM / XDEN + ONE
         IF (Y1 .LT. Y) THEN
C----------------------------------------------------------------------
C  Adjust result for case  0.0 .LT. argument .LT. 1.0
C----------------------------------------------------------------------
            RES = RES / Y1
         ELSE IF (Y1 .GT. Y) THEN
C----------------------------------------------------------------------
C  Adjust result for case  2.0 .LT. argument .LT. 12.0
C----------------------------------------------------------------------
            DO 290 I = 1, N
               RES = RES * Y
               Y = Y + ONE
 290        CONTINUE
         END IF
      ELSE
C----------------------------------------------------------------------
C  Evaluate for argument .GE. 12.0,
C----------------------------------------------------------------------
         IF (Y .LE. XBIG) THEN
            YSQ = Y * Y
            SUM = C(7)
            DO 350 I = 1, 6
               SUM = SUM / YSQ + C(I)
 350        CONTINUE
            SUM = SUM/Y - Y + SQRTPI
            SUM = SUM + (Y-HALF)*LOG(Y)
            RES = EXP(SUM)
         ELSE
*                  RES = XINF
* modif bruno : return an Inf
            xfinf = 0.d0
            RES = 1 / xfinf
* end modif bruno
            GO TO 900
         END IF
      END IF
C----------------------------------------------------------------------
C  Final adjustments and return
C----------------------------------------------------------------------
      IF (PARITY) RES = -RES
      IF (FACT .NE. ONE) RES = FACT / RES
CS900 GAMMA = RES
  900 DGAMMACODY = RES
      RETURN
C ---------- Last line of GAMMA ----------
      END
