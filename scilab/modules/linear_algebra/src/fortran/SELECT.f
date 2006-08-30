      LOGICAL FUNCTION SELECTC( EIG )
C     
C     RELEASE 4.0, WGS COPYRIGHT 2001.
C     
C     PURPOSE
C     
C     To select the stable eigenvalues in ordering the Schur form
C     of a matrix.
C     
C     ARGUMENTS
C     
C     Input/Output Parameters
C     
C     EIG    (input) COMPLEX*16
C     The  current eigenvalue considered.
C     
C     METHOD
C     
C     The function value SELECTC is set to .TRUE. for a stable
c     eigenvalue
C     and to .FALSE., otherwise.
C     
C     REFERENCES
C     
C     None.
C     
C     NUMERICAL ASPECTS
C     
C     None.
C     
C     CONTRIBUTOR
C     
C     
C     
C     REVISIONS
C     
C     -
C     
C     KEYWORDS
C     
C     Algebraic Riccati equation, closed loop system, continuous-time
C     system, optimal regulator, Schur form.
C     
C     ******************************************************************
C     
C     .. Parameters ..
      DOUBLE PRECISION  ZERO
      PARAMETER         ( ZERO = 0.0D0 )
C     .. Scalar Arguments ..
      COMPLEX*16        EIG
C     .. Intrinsic Functions ..
      INTRINSIC         DREAL
C     .. Executable Statements ..
C     
      SELECTC = DREAL(EIG).LT.ZERO
C     
      RETURN
C     *** Last line of SELECTC ***
      END


      LOGICAL FUNCTION SELECTD( EIG )
C     
C     RELEASE 4.0, WGS COPYRIGHT 2001.
C     
C     PURPOSE
C     
C     To select the eigenvalues inside the unit circle in ordering
C     the Schur form of a matrix.
C     
C     ARGUMENTS
C     
C     Input/Output Parameters
C     
C     EIG    (input) COMPLEX*16
C     The  current eigenvalue considered.
C     
C     METHOD
C     
C     The function value SELECTC is set to .TRUE. for an eigenvalue
c     which
C     is inside the unit circle and to .FALSE., otherwise.
C     
C     REFERENCES
C     
C     None.
C     
C     NUMERICAL ASPECTS
C     
C     None.
C     
C     CONTRIBUTOR
C     
C     
C     
C     REVISIONS
C     
C     -
C     
C     KEYWORDS
C     
C     Algebraic Riccati equation, closed loop system, continuous-time
C     system, optimal regulator, Schur form.
C     
C     ******************************************************************
C     
C     .. Parameters ..
      DOUBLE PRECISION  ONE
      PARAMETER         ( ONE = 1.0D0 )
C     .. Scalar Arguments ..
      COMPLEX*16        EIG
C     .. Intrinsic Functions ..
      INTRINSIC ABS
C     .. Executable Statements ..
C     
      SELECTD = ABS(EIG).LT.ONE
C     
      RETURN
C     *** Last line of SELECTD ***
      END


