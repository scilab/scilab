      DOUBLE PRECISION FUNCTION gennf(dfn,dfd,xnonc)

C**********************************************************************
C
C     DOUBLE PRECISION FUNCTION GENNF( DFN, DFD, XNONC )
C           GENerate random deviate from the Noncentral F distribution
C
C
C                              Function
C
C
C     Generates a random deviate from the  noncentral F (variance ratio)
C     distribution with DFN degrees of freedom in the numerator, and DFD
C     degrees of freedom in the denominator, and noncentrality parameter
C     XNONC.
C
C
C                              Arguments
C
C
C     DFN --> Numerator degrees of freedom
C             (Must be >= 1.0)
C                              DOUBLE PRECISION DFN
C      DFD --> Denominator degrees of freedom
C             (Must be positive)
C                              DOUBLE PRECISION DFD
C
C     XNONC --> Noncentrality parameter
C               (Must be nonnegative)
C                              DOUBLE PRECISION XNONC
C
C
C                              Method
C
C
C     Directly generates ratio of noncentral numerator chisquare variate
C     to central denominator chisquare variate.
C
C**********************************************************************
C     .. Scalar Arguments ..
      DOUBLE PRECISION dfd,dfn,xnonc
C     ..
C     .. Local Scalars ..
      DOUBLE PRECISION xden,xnum
C     ..
C     .. External Functions ..
C     JJV changed the code to call SGAMMA and SNORM directly
C      DOUBLE PRECISION genchi,gennch
C      EXTERNAL genchi,gennch
      DOUBLE PRECISION sgamma,snorm
      EXTERNAL sgamma,snorm
C     ..
C     .. Executable Statements ..
C     JJV changed the argument checker to allow DFN = 1.0
C     JJV in the same way as GENNCH was changed.
C      GENNF = ( GENNCH( DFN, XNONC ) / DFN ) / ( GENCHI( DFD ) / DFD )
C     JJV changed this to call SGAMMA and SNORM directly
C     xnum = gennch(dfn,xnonc)/dfn
 10   IF (dfn.GE.1.000001) GO TO 20
C     JJV case dfn = 1.0 - here I am treating dfn as exactly 1.0
      xnum = (snorm() + sqrt(xnonc))**2
      GO TO 30

C     JJV case dfn > 1.0
 20   xnum = (2.0*sgamma((dfn-1.0)/2.0) + (snorm()+sqrt(xnonc))**2)/dfn

C     xden = genchi(dfd)/dfd
 30   xden = 2.0*sgamma(dfd/2.0)/dfd

C     JJV changed constant so that it will not underflow at compile time
C     JJV while not slowing generator by using double precision or logs.
C      IF (.NOT. (xden.LE. (1.0E-38*xnum))) GO TO 40
      IF (.NOT. (xden.LE. (1.0E-37*xnum))) GO TO 40
      call basout(io,6,'nf: Generated numbers would cause overflow')
C     JJV next 2 lines changed to maintain truncation of large deviates.
C      WRITE (*,*) ' GENNF returning 1.0E38'
C      gennf = 1.0E38
       call basout(io,6,' returning 1.0E37')
      gennf = 1.0E37
      GO TO 50

   40 gennf = xnum/xden
   50 RETURN

      END
