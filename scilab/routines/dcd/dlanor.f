      DOUBLE PRECISION FUNCTION dlanor(x)
C**********************************************************************
C
C      DOUBLE PRECISION FUNCTION DLANOR( X )
C           Double precision Logarith of the Asymptotic Normal
C
C
C                              Function
C
C
C      Computes the logarithm of the cumulative normal distribution
C      from abs( x ) to infinity for abs( x ) >= 5.
C
C
C                              Arguments
C
C
C      X --> Value at which cumulative normal to be evaluated
C                     DOUBLE PRECISION X
C
C
C                              Method
C
C
C      23 term expansion of formula 26.2.12 of Abramowitz and Stegun.
C      The relative error at X = 5 is about 0.5E-5.
C
C
C                              Note
C
C
C      ABS(X) must be >= 5 else there is an error stop.
C
C**********************************************************************
      include '../stack.h'
C     .. Parameters ..
      DOUBLE PRECISION dlsqpi
      PARAMETER (dlsqpi=0.91893853320467274177D0)
C     ..
C     .. Scalar Arguments ..
      DOUBLE PRECISION x
C     ..
C     .. Local Scalars ..
      DOUBLE PRECISION approx,correc,xx,xx2
C     ..
C     .. Local Arrays ..
      DOUBLE PRECISION coef(12)
C     ..
C     .. External Functions ..
      DOUBLE PRECISION devlpl,dln1px
      EXTERNAL devlpl,dln1px
C     ..
C     .. Intrinsic Functions ..
      INTRINSIC abs,log
C     ..
C     .. Data statements ..
      DATA coef/-1.0D0,3.0D0,-15.0D0,105.0D0,-945.0D0,10395.0D0,
     +     -135135.0D0,2027025.0D0,-34459425.0D0,654729075.0D0,
     +     -13749310575D0,316234143225.0D0/
C     ..
C     .. Executable Statements ..

      xx = abs(x)
      IF (xx.LT.5.0D0) then 
         call basout(io,wte,'Argument too small in DLANOR')
         status = -100
         dlanor=0.0d0
         return
      endif

      approx = -dlsqpi - 0.5D0*xx*xx - log(xx)

      xx2 = xx*xx
      correc = devlpl(coef,12,1.0D0/xx2)/xx2
      correc = dln1px(correc)

      dlanor = approx + correc

      RETURN

      END
