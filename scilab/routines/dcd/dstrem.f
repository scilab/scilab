      DOUBLE PRECISION FUNCTION dstrem(z)
      IMPLICIT DOUBLE PRECISION (a-h,o-p,r-z),INTEGER (i-n),LOGICAL (q)
C**********************************************************************
C
C     DOUBLE PRECISION FUNCTION DSTREM( Z )
C             Double precision Sterling Remainder
C
C
C                              Function
C
C
C     Returns   Log(Gamma(Z))  -  Sterling(Z)  where   Sterling(Z)  is
C     Sterling's Approximation to Log(Gamma(Z))
C
C     Sterling(Z) = LOG( SQRT( 2*PI ) ) + ( Z-0.5 ) * LOG( Z ) - Z
C
C
C                              Arguments
C
C
C     Z --> Value at which Sterling remainder calculated
C           Must be positive.
C                  DOUBLE PRECISION Z
C
C
C                              Method
C
C
C
C     If Z >= 6 uses 9 terms of series in Bernoulli numbers
C     (Values calculated using Maple)
C     Otherwise computes difference explicitly
C
C**********************************************************************
      include '../stack.h'
C     .. Parameters ..
      DOUBLE PRECISION hln2pi
      PARAMETER (hln2pi=0.91893853320467274178D0)
      INTEGER ncoef
      PARAMETER (ncoef=10)
C     ..
C     .. Scalar Arguments ..
      DOUBLE PRECISION z
C     ..
C     .. Local Scalars ..
      DOUBLE PRECISION sterl
C     ..
C     .. Local Arrays ..
      DOUBLE PRECISION coef(ncoef)
C     ..
C     .. External Functions ..
      DOUBLE PRECISION devlpl,dlngam
      EXTERNAL devlpl,dlngam
C     ..
C     .. Intrinsic Functions ..
      INTRINSIC log
C     ..
C     .. Data statements ..
      DATA coef/0.0D0,0.0833333333333333333333333333333D0,
     +     -0.00277777777777777777777777777778D0,
     +     0.000793650793650793650793650793651D0,
     +     -0.000595238095238095238095238095238D0,
     +     0.000841750841750841750841750841751D0,
     +     -0.00191752691752691752691752691753D0,
     +     0.00641025641025641025641025641026D0,
     +     -0.0295506535947712418300653594771D0,
     +     0.179644372368830573164938490016D0/
C     ..
C     .. Executable Statements ..

C    For information, here are the next 11 coefficients of the
C    remainder term in Sterling's formula
C            -1.39243221690590111642743221691
C            13.4028640441683919944789510007
C            -156.848284626002017306365132452
C            2193.10333333333333333333333333
C            -36108.7712537249893571732652192
C            691472.268851313067108395250776
C            -0.152382215394074161922833649589D8
C            0.382900751391414141414141414141D9
C            -0.108822660357843910890151491655D11
C            0.347320283765002252252252252252D12
C            -0.123696021422692744542517103493D14
C
C     --->(jpc) status is not used 
      IF (z.LE.0.0D0) then 
         call basout(io,wte,'Zero or negative argument in DSTREM')
         status=-100
         dstrem=0.0d0
         return
      endif
      IF (.NOT. (z.GT.6.0D0)) GO TO 10
      dstrem = devlpl(coef,10,1.0D0/z**2)*z
      GO TO 20

   10 sterl = hln2pi + (z-0.5D0)*log(z) - z
      dstrem = dlngam(z) - sterl
   20 RETURN

      END
