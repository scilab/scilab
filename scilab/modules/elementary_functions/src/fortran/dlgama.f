CS    REAL FUNCTION ALGAMA(X)
      DOUBLE PRECISION FUNCTION DLGAMA(X)
C----------------------------------------------------------------------
C
C This routine calculates the LOG(GAMMA) function for a positive real
C   argument X.  Computation is based on an algorithm outlined in
C   references 1 and 2.  The program uses rational functions that
C   theoretically approximate LOG(GAMMA) to at least 18 significant
C   decimal digits.  The approximation for X > 12 is from reference
C   3, while approximations for X < 12.0 are similar to those in
C   reference 1, but are unpublished.  The accuracy achieved depends
C   on the arithmetic system, the compiler, the intrinsic functions,
C   and proper selection of the machine-dependent constants.
C
C
C*********************************************************************
C*********************************************************************
C
C Explanation of machine-dependent constants
C
C beta   - radix for the floating-point representation
C maxexp - the smallest positive power of beta that overflows
C XBIG   - largest argument for which LN(GAMMA(X)) is representable
C          in the machine, i.e., the solution to the equation
C                  LN(GAMMA(XBIG)) = beta**maxexp
C XINF   - largest machine representable floating-point number;
C          approximately beta**maxexp.
C EPS    - The smallest positive floating-point number such that
C          1.0+EPS .GT. 1.0
C FRTBIG - Rough estimate of the fourth root of XBIG
C
C
C     Approximate values for some important machines are:
C
C                           beta      maxexp         XBIG
C
C CRAY-1        (S.P.)        2        8191       9.62E+2461
C Cyber 180/855
C   under NOS   (S.P.)        2        1070       1.72E+319
C IEEE (IBM/XT,
C   SUN, etc.)  (S.P.)        2         128       4.08E+36
C IEEE (IBM/XT,
C   SUN, etc.)  (D.P.)        2        1024       2.55D+305
C IBM 3033      (D.P.)       16          63       4.29D+73
C VAX D-Format  (D.P.)        2         127       2.05D+36
C VAX G-Format  (D.P.)        2        1023       1.28D+305
C
C
C                           XINF        EPS        FRTBIG
C
C CRAY-1        (S.P.)   5.45E+2465   7.11E-15    3.13E+615
C Cyber 180/855
C   under NOS   (S.P.)   1.26E+322    3.55E-15    6.44E+79
C IEEE (IBM/XT,
C   SUN, etc.)  (S.P.)   3.40E+38     1.19E-7     1.42E+9
C IEEE (IBM/XT,
C   SUN, etc.)  (D.P.)   1.79D+308    2.22D-16    2.25D+76
C IBM 3033      (D.P.)   7.23D+75     2.22D-16    2.56D+18
C VAX D-Format  (D.P.)   1.70D+38     1.39D-17    1.20D+9
C VAX G-Format  (D.P.)   8.98D+307    1.11D-16    1.89D+76
C
C**************************************************************
C**************************************************************
C
C Error returns
C
C  The program returns the value XINF for X .LE. 0.0 or when
C     overflow would occur.  The computation is believed to 
C     be free of underflow and overflow.
C
C
C Intrinsic functions required are:
C
C      LOG
C
C
C References:
C
C  1) W. J. Cody and K. E. Hillstrom, 'Chebyshev Approximations for
C     the Natural Logarithm of the Gamma Function,' Math. Comp. 21,
C     1967, pp. 198-203.
C
C  2) K. E. Hillstrom, ANL/AMD Program ANLC366S, DGAMMA/DLGAMA, May,
C     1969.
C 
C  3) Hart, Et. Al., Computer Approximations, Wiley and sons, New
C     York, 1968.
C
C
C  Authors: W. J. Cody and L. Stoltz
C           Argonne National Laboratory
C
C  Latest modification: June 16, 1988
C
C----------------------------------------------------------------------
*
*  Some modifs from Bruno (25 Feb 2005):
*    - return Nan (in place of XINF) if x <= 0
*    - return Inf (in place of XINF) if x > XBIG 
*
*  In fact an error indicator should be returned in these cases to prevent
*  the user...
*
      INTEGER I
CS    REAL      
      DOUBLE PRECISION
     1    C,CORR,D1,D2,D4,EPS,FRTBIG,FOUR,HALF,ONE,PNT68,P1,P2,P4,
     2    Q1,Q2,Q4,RES,SQRTPI,THRHAL,TWELVE,TWO,X,XBIG,XDEN,XINF,
     3    XM1,XM2,XM4,XNUM,Y,YSQ,ZERO
      DIMENSION C(7),P1(8),P2(8),P4(8),Q1(8),Q2(8),Q4(8)
C----------------------------------------------------------------------
C  Mathematical constants
C----------------------------------------------------------------------
CS    DATA ONE,HALF,TWELVE,ZERO/1.0E0,0.5E0,12.0E0,0.0E0/,
CS   1     FOUR,THRHAL,TWO,PNT68/4.0E0,1.5E0,2.0E0,0.6796875E0/,
CS   2     SQRTPI/0.9189385332046727417803297E0/
      DATA ONE,HALF,TWELVE,ZERO/1.0D0,0.5D0,12.0D0,0.0D0/,
     1     FOUR,THRHAL,TWO,PNT68/4.0D0,1.5D0,2.0D0,0.6796875D0/,
     2     SQRTPI/0.9189385332046727417803297D0/
C----------------------------------------------------------------------
C  Machine dependent parameters
C----------------------------------------------------------------------
CS    DATA XBIG,XINF,EPS,FRTBIG/4.08E36,3.401E38,1.19E-7,1.42E9/
      DATA XBIG,XINF,EPS,FRTBIG/2.55D305,1.79D308,2.22D-16,2.25D76/
C----------------------------------------------------------------------
C  Numerator and denominator coefficients for rational minimax
C     approximation over (0.5,1.5).
C----------------------------------------------------------------------
CS    DATA D1/-5.772156649015328605195174E-1/
CS    DATA P1/4.945235359296727046734888E0,2.018112620856775083915565E2,
CS   1        2.290838373831346393026739E3,1.131967205903380828685045E4,
CS   2        2.855724635671635335736389E4,3.848496228443793359990269E4,
CS   3        2.637748787624195437963534E4,7.225813979700288197698961E3/
CS    DATA Q1/6.748212550303777196073036E1,1.113332393857199323513008E3,
CS   1        7.738757056935398733233834E3,2.763987074403340708898585E4,
CS   2        5.499310206226157329794414E4,6.161122180066002127833352E4,
CS   3        3.635127591501940507276287E4,8.785536302431013170870835E3/
      DATA D1/-5.772156649015328605195174D-1/
      DATA P1/4.945235359296727046734888D0,2.018112620856775083915565D2,
     1        2.290838373831346393026739D3,1.131967205903380828685045D4,
     2        2.855724635671635335736389D4,3.848496228443793359990269D4,
     3        2.637748787624195437963534D4,7.225813979700288197698961D3/
      DATA Q1/6.748212550303777196073036D1,1.113332393857199323513008D3,
     1        7.738757056935398733233834D3,2.763987074403340708898585D4,
     2        5.499310206226157329794414D4,6.161122180066002127833352D4,
     3        3.635127591501940507276287D4,8.785536302431013170870835D3/
C----------------------------------------------------------------------
C  Numerator and denominator coefficients for rational minimax
C     Approximation over (1.5,4.0).
C----------------------------------------------------------------------
CS    DATA D2/4.227843350984671393993777E-1/
CS    DATA P2/4.974607845568932035012064E0,5.424138599891070494101986E2,
CS   1        1.550693864978364947665077E4,1.847932904445632425417223E5,
CS   2        1.088204769468828767498470E6,3.338152967987029735917223E6,
CS   3        5.106661678927352456275255E6,3.074109054850539556250927E6/
CS    DATA Q2/1.830328399370592604055942E2,7.765049321445005871323047E3,
CS   1        1.331903827966074194402448E5,1.136705821321969608938755E6,
CS   2        5.267964117437946917577538E6,1.346701454311101692290052E7,
CS   3        1.782736530353274213975932E7,9.533095591844353613395747E6/
      DATA D2/4.227843350984671393993777D-1/
      DATA P2/4.974607845568932035012064D0,5.424138599891070494101986D2,
     1        1.550693864978364947665077D4,1.847932904445632425417223D5,
     2        1.088204769468828767498470D6,3.338152967987029735917223D6,
     3        5.106661678927352456275255D6,3.074109054850539556250927D6/
      DATA Q2/1.830328399370592604055942D2,7.765049321445005871323047D3,
     1        1.331903827966074194402448D5,1.136705821321969608938755D6,
     2        5.267964117437946917577538D6,1.346701454311101692290052D7,
     3        1.782736530353274213975932D7,9.533095591844353613395747D6/
C----------------------------------------------------------------------
C  Numerator and denominator coefficients for rational minimax
C     Approximation over (4.0,12.0).
C----------------------------------------------------------------------
CS    DATA D4/1.791759469228055000094023E0/
CS    DATA P4/1.474502166059939948905062E4,2.426813369486704502836312E6,
CS   1        1.214755574045093227939592E8,2.663432449630976949898078E9,
CS   2      2.940378956634553899906876E10,1.702665737765398868392998E11,
CS   3      4.926125793377430887588120E11,5.606251856223951465078242E11/
CS    DATA Q4/2.690530175870899333379843E3,6.393885654300092398984238E5,
CS   2        4.135599930241388052042842E7,1.120872109616147941376570E9,
CS   3      1.488613728678813811542398E10,1.016803586272438228077304E11,
CS   4      3.417476345507377132798597E11,4.463158187419713286462081E11/
      DATA D4/1.791759469228055000094023D0/
      DATA P4/1.474502166059939948905062D4,2.426813369486704502836312D6,
     1        1.214755574045093227939592D8,2.663432449630976949898078D9,
     2      2.940378956634553899906876D10,1.702665737765398868392998D11,
     3      4.926125793377430887588120D11,5.606251856223951465078242D11/
      DATA Q4/2.690530175870899333379843D3,6.393885654300092398984238D5,
     2        4.135599930241388052042842D7,1.120872109616147941376570D9,
     3      1.488613728678813811542398D10,1.016803586272438228077304D11,
     4      3.417476345507377132798597D11,4.463158187419713286462081D11/
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
      Y = X
      IF ((Y .GT. ZERO) .AND. (Y .LE. XBIG)) THEN
            IF (Y .LE. EPS) THEN
                  RES = -LOG(Y)
               ELSE IF (Y .LE. THRHAL) THEN
C----------------------------------------------------------------------
C  EPS .LT. X .LE. 1.5
C----------------------------------------------------------------------
                  IF (Y .LT. PNT68) THEN
                        CORR = -LOG(Y)
                        XM1 = Y
                     ELSE
                        CORR = ZERO
                        XM1 = (Y - HALF) - HALF
                  END IF
                  IF ((Y .LE. HALF) .OR. (Y .GE. PNT68)) THEN
                        XDEN = ONE
                        XNUM = ZERO
                        DO 140 I = 1, 8
                           XNUM = XNUM*XM1 + P1(I)
                           XDEN = XDEN*XM1 + Q1(I)
  140                   CONTINUE
                        RES = CORR + (XM1 * (D1 + XM1*(XNUM/XDEN)))
                     ELSE
                        XM2 = (Y - HALF) - HALF
                        XDEN = ONE
                        XNUM = ZERO
                        DO 220 I = 1, 8
                           XNUM = XNUM*XM2 + P2(I)
                           XDEN = XDEN*XM2 + Q2(I)
  220                   CONTINUE
                        RES = CORR + XM2 * (D2 + XM2*(XNUM/XDEN))
                  END IF
               ELSE IF (Y .LE. FOUR) THEN
C----------------------------------------------------------------------
C  1.5 .LT. X .LE. 4.0
C----------------------------------------------------------------------
                  XM2 = Y - TWO
                  XDEN = ONE
                  XNUM = ZERO
                  DO 240 I = 1, 8
                     XNUM = XNUM*XM2 + P2(I)
                     XDEN = XDEN*XM2 + Q2(I)
  240             CONTINUE
                  RES = XM2 * (D2 + XM2*(XNUM/XDEN))
               ELSE IF (Y .LE. TWELVE) THEN
C----------------------------------------------------------------------
C  4.0 .LT. X .LE. 12.0
C----------------------------------------------------------------------
                  XM4 = Y - FOUR
                  XDEN = -ONE
                  XNUM = ZERO
                  DO 340 I = 1, 8
                     XNUM = XNUM*XM4 + P4(I)
                     XDEN = XDEN*XM4 + Q4(I)
  340             CONTINUE
                  RES = D4 + XM4*(XNUM/XDEN)
               ELSE 
C----------------------------------------------------------------------
C  Evaluate for argument .GE. 12.0,
C----------------------------------------------------------------------
                  RES = ZERO
                  IF (Y .LE. FRTBIG) THEN
                        RES = C(7)
                        YSQ = Y * Y
                        DO 450 I = 1, 6
                           RES = RES / YSQ + C(I)
  450                   CONTINUE
                  END IF
                  RES = RES/Y
                  CORR = LOG(Y)
                  RES = RES + SQRTPI - HALF*CORR
                  RES = RES + Y*(CORR-ONE)
            END IF
         ELSE
C----------------------------------------------------------------------
C  Return for bad arguments
C----------------------------------------------------------------------
*           modif from Bruno (see comment at the beginning)
*            RES = XINF
            if (X .le. 0.d0) then
              CALL returnananfortran(RES)
            else  
C this means that X > XBIG and so that log(gamma) overflows 
C bad trick to get Inf
               RES = 2*XINF  
            endif
*           end modif
      END IF
C----------------------------------------------------------------------
C  Final adjustments and return
C----------------------------------------------------------------------
CS    ALGAMA = RES
      DLGAMA = RES
      RETURN
C ---------- Last line of DLGAMA ----------
      END


