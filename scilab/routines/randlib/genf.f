      DOUBLE PRECISION FUNCTION genf(dfn,dfd)
C**********************************************************************
C
C     DOUBLE PRECISION FUNCTION GENF( DFN, DFD )
C                GENerate random deviate from the F distribution
C
C
C                              Function
C
C
C     Generates a random deviate from the F (variance ratio)
C     distribution with DFN degrees of freedom in the numerator
C     and DFD degrees of freedom in the denominator.
C
C
C                              Arguments
C
C
C     DFN --> Numerator degrees of freedom
C             (Must be positive)
C                              DOUBLE PRECISION DFN
C      DFD --> Denominator degrees of freedom
C             (Must be positive)
C                              DOUBLE PRECISION DFD
C
C
C                              Method
C
C
C     Directly generates ratio of chisquare variates
C
C**********************************************************************
      include '../stack.h'
C     .. Scalar Arguments ..
      DOUBLE PRECISION dfd,dfn
C     ..
C     .. Local Scalars ..
      DOUBLE PRECISION xden,xnum
C     ..
C     JJV changed this code to call sgamma directly
C     .. External Functions ..
C      DOUBLE PRECISION genchi
C      EXTERNAL genchi
      DOUBLE PRECISION sgamma
      EXTERNAL sgamma
C     ..
C     .. Executable Statements ..
 10   xnum = 2.0*sgamma(dfn/2.0)/dfn

C      GENF = ( GENCHI( DFN ) / DFN ) / ( GENCHI( DFD ) / DFD )
      xden = 2.0*sgamma(dfd/2.0)/dfd
C     JJV changed constant so that it will not underflow at compile time
C     JJV while not slowing generator by using double precision or logs.
C      IF (.NOT. (xden.LE. (1.0E-38*xnum))) GO TO 20
      IF (.NOT. (xden.LE. (1.0E-37*xnum))) GO TO 20
      call basout(io,wte,'F: generated numbers would cause overflow')
c      WRITE (*,*) ' Numerator ',xnum,' Denominator ',xden
C     JJV next 2 lines changed to maintain truncation of large deviates.
C      WRITE (*,*) ' GENF returning 1.0E38'
C      genf = 1.0E38
       call basout(io,wte,' GENF returning 1.0E37')
      genf = 1.0E37
      GO TO 30

   20 genf = xnum/xden
   30 RETURN

      END
