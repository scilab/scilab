      LOGICAL FUNCTION SB02OU( ALPHAR, ALPHAI, BETA )
C
C     RELEASE 4.0, WGS COPYRIGHT 1999.
C
C     PURPOSE
C
C     To select the unstable generalized eigenvalues for solving the
C     continuous-time algebraic Riccati equation.
C
C     ARGUMENTS
C
C     Input/Output Parameters
C
C     ALPHAR  (input) DOUBLE PRECISION
C             The real part of the numerator of the current eigenvalue 
C             considered.
C
C     ALPHAI  (input) DOUBLE PRECISION
C             The imaginary part of the numerator of the current 
C             eigenvalue considered.
C
C     BETA    (input) DOUBLE PRECISION
C             The (real) denominator of the current eigenvalue 
C             considered. It is assumed that BETA <> 0 (regular case).
C
C     METHOD
C
C     The function value SB02OU is set to .TRUE. for an unstable
C     eigenvalue and to .FALSE., otherwise.
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
C     Release 3.0: V. Sima, Katholieke Univ. Leuven, Belgium, Sep. 1997.
C     Supersedes Release 2.0 routine SB02CW by P. Van Dooren, Philips
C     Research Laboratory, Brussels, Belgium.
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
      DOUBLE PRECISION   ZERO
      PARAMETER          ( ZERO = 0.0D0 )
C     .. Scalar Arguments ..
      DOUBLE PRECISION   ALPHAR, ALPHAI, BETA
C     .. Executable Statements ..
C
      SB02OU = ( ALPHAR.LT.ZERO .AND. BETA.LT.ZERO ) .OR.
     $         ( ALPHAR.GT.ZERO .AND. BETA.GT.ZERO ) 
C
      RETURN
C *** Last line of SB02OU ***
      END
