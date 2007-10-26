      FUNCTION PSI(XX)
C----------------------------------------------------------------------
C
C This function program evaluates the logarithmic derivative of the
C   gamma function, 
C
C      psi(x) = d/dx (gamma(x)) / gamma(x) = d/dx (ln gamma(x))
C
C   for real x, where either
C
C          -xmax1 < x < -xmin (x not a negative integer), or
C            xmin < x.
C
C   The calling sequence for this function is 
C
C                  Y = PSI(X)
C
C   The main computation uses rational Chebyshev approximations
C   published in Math. Comp. 27, 123-127 (1973) by Cody, Strecok and
C   Thacher.  This transportable program is patterned after the
C   machine-dependent FUNPACK program PSI(X), but cannot match that
C   version for efficiency or accuracy.  This version uses rational
C   approximations that are theoretically accurate to 20 significant
C   decimal digits.  The accuracy achieved depends on the arithmetic
C   system, the compiler, the intrinsic functions, and proper selection
C   of the machine-dependent constants.
C
C*******************************************************************
C*******************************************************************
C
C Explanation of machine-dependent constants
C
C   XINF   = largest positive machine number
C   XMAX1  = beta ** (p-1), where beta is the radix for the
C            floating-point system, and p is the number of base-beta
C            digits in the floating-point significand.  This is an
C            upper bound on non-integral floating-point numbers, and
C            the negative of the lower bound on acceptable negative
C            arguments for PSI.  If rounding is necessary, round this
C            value down.
C   XMIN1  = the smallest in magnitude acceptable argument.  We
C            recommend XMIN1 = MAX(1/XINF,xmin) rounded up, where
C            xmin is the smallest positive floating-point number.
C   XSMALL = absolute argument below which  PI*COTAN(PI*X)  may be
C            represented by 1/X.  We recommend XSMALL < sqrt(3 eps)/pi,
C            where eps is the smallest positive number such that
C            1+eps > 1. 
C   XLARGE = argument beyond which PSI(X) may be represented by
C            LOG(X).  The solution to the equation
C               x*ln(x) = beta ** p
C            is a safe value.
C
C     Approximate values for some important machines are
C
C                        beta  p     eps     xmin       XINF  
C
C  CDC 7600      (S.P.)    2  48  7.11E-15  3.13E-294  1.26E+322
C  CRAY-1        (S.P.)    2  48  7.11E-15  4.58E-2467 5.45E+2465
C  IEEE (IBM/XT,
C    SUN, etc.)  (S.P.)    2  24  1.19E-07  1.18E-38   3.40E+38
C  IEEE (IBM/XT,
C    SUN, etc.)  (D.P.)    2  53  1.11D-16  2.23E-308  1.79D+308
C  IBM 3033      (D.P.)   16  14  1.11D-16  5.40D-79   7.23D+75
C  SUN 3/160     (D.P.)    2  53  1.11D-16  2.23D-308  1.79D+308
C  VAX 11/780    (S.P.)    2  24  5.96E-08  2.94E-39   1.70E+38
C                (D.P.)    2  56  1.39D-17  2.94D-39   1.70D+38
C   (G Format)   (D.P.)    2  53  1.11D-16  5.57D-309  8.98D+307
C
C                         XMIN1      XMAX1     XSMALL    XLARGE
C
C  CDC 7600      (S.P.)  3.13E-294  1.40E+14  4.64E-08  9.42E+12
C  CRAY-1        (S.P.)  1.84E-2466 1.40E+14  4.64E-08  9.42E+12
C  IEEE (IBM/XT,
C    SUN, etc.)  (S.P.)  1.18E-38   8.38E+06  1.90E-04  1.20E+06
C  IEEE (IBM/XT,
C    SUN, etc.)  (D.P.)  2.23D-308  4.50D+15  5.80D-09  2.71D+14
C  IBM 3033      (D.P.)  1.39D-76   4.50D+15  5.80D-09  2.05D+15
C  SUN 3/160     (D.P.)  2.23D-308  4.50D+15  5.80D-09  2.71D+14
C  VAX 11/780    (S.P.)  5.89E-39   8.38E+06  1.35E-04  1.20E+06
C                (D.P.)  5.89D-39   3.60D+16  2.05D-09  2.05D+15
C   (G Format)   (D.P.)  1.12D-308  4.50D+15  5.80D-09  2.71D+14
C
C*******************************************************************
C*******************************************************************
C
C Error Returns
C
C  The program returns XINF for  X < -XMAX1, for X zero or a negative
C    integer, or when X lies in (-XMIN1, 0), and returns -XINF
C    when X lies in (0, XMIN1).
C
C Intrinsic functions required are:
C
C     ABS, AINT, DBLE, INT, LOG, REAL, TAN
C
C
C  Author: W. J. Cody
C          Mathematics and Computer Science Division 
C          Argonne National Laboratory
C          Argonne, IL 60439
C
C  Latest modification: June 8, 1988
C
C----------------------------------------------------------------------
      INTEGER I,N,NQ
CS    REAL
      DOUBLE PRECISION
     1   AUG,CONV,DEN,PSI,FOUR,FOURTH,HALF,ONE,P1,P2,PIOV4,Q1,Q2,
     2   SGN,THREE,XLARGE,UPPER,W,X,XINF,XMAX1,XMIN1,XSMALL,X01,
     3   X01D,X02,XX,Z,ZERO
      DIMENSION P1(9),P2(7),Q1(8),Q2(6)
C----------------------------------------------------------------------
C  Mathematical constants.  PIOV4 = pi / 4
C----------------------------------------------------------------------
CS    DATA ZERO,FOURTH,HALF,ONE/0.0E0,0.25E0,0.5E0,1.0E0/
CS    DATA THREE,FOUR/3.0E0,4.0E0/,PIOV4/7.8539816339744830962E-01/
      DATA ZERO,FOURTH,HALF,ONE/0.0D0,0.25D0,0.5D0,1.0D0/
      DATA THREE,FOUR/3.0D0,4.0D0/,PIOV4/7.8539816339744830962D-01/
C----------------------------------------------------------------------
C  Machine-dependent constants
C----------------------------------------------------------------------
CS    DATA XINF/1.70E+38/, XMIN1/5.89E-39/, XMAX1/8.38E+06/,
CS   1     XSMALL/1.35E-04/, XLARGE/1.20E+06/
      DATA XINF/1.79D+308/, XMIN1/2.23D-308/, XMAX1/4.50D+15/,
     1     XSMALL/5.80D-09/, XLARGE/2.71D+14/
C----------------------------------------------------------------------
C  Zero of psi(x)
C----------------------------------------------------------------------
CS    DATA X01/187.0E0/,X01D/128.0E0/,X02/6.9464496836234126266E-04/
      DATA X01/187.0D0/,X01D/128.0D0/,X02/6.9464496836234126266D-04/
C----------------------------------------------------------------------
C  Coefficients for approximation to  psi(x)/(x-x0)  over [0.5, 3.0]
C----------------------------------------------------------------------
CS    DATA P1/4.5104681245762934160E-03,5.4932855833000385356E+00,
CS   1        3.7646693175929276856E+02,7.9525490849151998065E+03,
CS   2        7.1451595818951933210E+04,3.0655976301987365674E+05,
CS   3        6.3606997788964458797E+05,5.8041312783537569993E+05,
CS   4        1.6585695029761022321E+05/
CS    DATA Q1/9.6141654774222358525E+01,2.6287715790581193330E+03,
CS   1        2.9862497022250277920E+04,1.6206566091533671639E+05,
CS   2        4.3487880712768329037E+05,5.4256384537269993733E+05,
CS   3        2.4242185002017985252E+05,6.4155223783576225996E-08/
      DATA P1/4.5104681245762934160D-03,5.4932855833000385356D+00,
     1        3.7646693175929276856D+02,7.9525490849151998065D+03,
     2        7.1451595818951933210D+04,3.0655976301987365674D+05,
     3        6.3606997788964458797D+05,5.8041312783537569993D+05,
     4        1.6585695029761022321D+05/
      DATA Q1/9.6141654774222358525D+01,2.6287715790581193330D+03,
     1        2.9862497022250277920D+04,1.6206566091533671639D+05,
     2        4.3487880712768329037D+05,5.4256384537269993733D+05,
     3        2.4242185002017985252D+05,6.4155223783576225996D-08/
C----------------------------------------------------------------------
C  Coefficients for approximation to  psi(x) - ln(x) + 1/(2x) 
C     for  x > 3.0
C----------------------------------------------------------------------
CS    DATA P2/-2.7103228277757834192E+00,-1.5166271776896121383E+01,
CS   1        -1.9784554148719218667E+01,-8.8100958828312219821E+00,
CS   2        -1.4479614616899842986E+00,-7.3689600332394549911E-02,
CS   3        -6.5135387732718171306E-21/
CS    DATA Q2/ 4.4992760373789365846E+01, 2.0240955312679931159E+02,
CS   1         2.4736979003315290057E+02, 1.0742543875702278326E+02,
CS   2         1.7463965060678569906E+01, 8.8427520398873480342E-01/
      DATA P2/-2.7103228277757834192D+00,-1.5166271776896121383D+01,
     1        -1.9784554148719218667D+01,-8.8100958828312219821D+00,
     2        -1.4479614616899842986D+00,-7.3689600332394549911D-02,
     3        -6.5135387732718171306D-21/
      DATA Q2/ 4.4992760373789365846D+01, 2.0240955312679931159D+02,
     1         2.4736979003315290057D+02, 1.0742543875702278326D+02,
     2         1.7463965060678569906D+01, 8.8427520398873480342D-01/
C----------------------------------------------------------------------
CS    CONV(I) = REAL(I)
      CONV(I) = DBLE(I)
      X = XX
      W = ABS(X)
      AUG = ZERO
C----------------------------------------------------------------------
C  Check for valid arguments, then branch to appropriate algorithm
C----------------------------------------------------------------------
      IF ((-X .GE. XMAX1) .OR. (W .LT. XMIN1)) THEN
            GO TO 410
         ELSE IF (X .GE. HALF) THEN
            GO TO 200
C----------------------------------------------------------------------
C  X < 0.5, use reflection formula: psi(1-x) = psi(x) + pi * cot(pi*x)
C     Use 1/X for PI*COTAN(PI*X)  when  XMIN1 < |X| <= XSMALL.  
C----------------------------------------------------------------------
         ELSE IF (W .LE. XSMALL) THEN
            AUG = -ONE / X
            GO TO 150
      END IF
C----------------------------------------------------------------------
C  Argument reduction for cot
C----------------------------------------------------------------------
  100 IF (X .LT. ZERO) THEN
            SGN = PIOV4
         ELSE
            SGN = -PIOV4
      END IF
      W = W - AINT(W)
      NQ = INT(W * FOUR)
      W = FOUR * (W - CONV(NQ) * FOURTH)
C----------------------------------------------------------------------
C  W is now related to the fractional part of  4.0 * X.
C     Adjust argument to correspond to values in the first
C     quadrant and determine the sign.
C----------------------------------------------------------------------
      N = NQ / 2
      IF ((N+N) .NE. NQ) W = ONE - W
      Z = PIOV4 * W
      IF (MOD(N,2) .NE. 0) SGN = - SGN
C----------------------------------------------------------------------
C  determine the final value for  -pi * cotan(pi*x)
C----------------------------------------------------------------------
      N = (NQ + 1) / 2
      IF (MOD(N,2) .EQ. 0) THEN
C----------------------------------------------------------------------
C  Check for singularity
C----------------------------------------------------------------------
            IF (Z .EQ. ZERO) GO TO 410
            AUG = SGN * (FOUR / TAN(Z))
         ELSE
            AUG = SGN * (FOUR * TAN(Z))
      END IF
  150 X = ONE - X
  200 IF (X .GT. THREE) GO TO 300
C----------------------------------------------------------------------
C  0.5 <= X <= 3.0
C----------------------------------------------------------------------
      DEN = X
      UPPER = P1(1) * X
      DO 210 I = 1, 7
         DEN = (DEN + Q1(I)) * X
         UPPER = (UPPER + P1(I+1)) * X
  210 CONTINUE
      DEN = (UPPER + P1(9)) / (DEN + Q1(8))
      X = (X-X01/X01D) - X02
      PSI = DEN * X + AUG
      GO TO 500
C----------------------------------------------------------------------
C  3.0 < X 
C----------------------------------------------------------------------
  300 IF (X .LT. XLARGE) THEN
         W = ONE / (X * X)
         DEN = W
         UPPER = P2(1) * W
         DO 310 I = 1, 5
            DEN = (DEN + Q2(I)) * W
            UPPER = (UPPER + P2(I+1)) * W
  310    CONTINUE
         AUG = (UPPER + P2(7)) / (DEN + Q2(6)) - HALF / X + AUG
      END IF
      PSI = AUG + LOG(X)
      GO TO 500
C----------------------------------------------------------------------
C  Error return
C----------------------------------------------------------------------
  410 PSI = XINF
      IF (X .GT. ZERO) PSI = -XINF
  500 RETURN
C---------- Last card of PSI ----------
      END
