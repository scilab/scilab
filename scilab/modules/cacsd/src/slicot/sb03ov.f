      SUBROUTINE SB03OV( A, B, C, S )
C
C     RELEASE 4.0, WGS COPYRIGHT 1999.
C
C     PURPOSE
C
C     To construct a complex plane rotation such that, for a complex
C     number  a  and a real number  b,
C
C        ( conjg( c )  s )*( a ) = ( d ),
C        (       -s    c ) ( b )   ( 0 )
C
C     where  d  is always real and is overwritten on  a,  so that on
C     return the imaginary part of  a  is zero.  b  is unaltered.
C
C     This routine has A and C declared as REAL, because it is intended
C     for use within a real Lyapunov solver and the REAL declarations
C     mean that a standard Fortran DOUBLE PRECISION version may be
C     readily constructed.  However A and C could safely be declared
C     COMPLEX in the calling program, although some systems may give a
C     type mismatch warning.
C
C     ARGUMENTS 
C
C     Input/Output Parameters
C
C     A       (input/output) DOUBLE PRECISION array, dimension (2)
C             On entry, A(1) and A(2) must contain the real and
C             imaginary part, respectively, of the complex number a.
C             On exit, A(1) contains the real part of d, and A(2) is
C             set to zero.
C
C     B       (input) DOUBLE PRECISION
C             The real number b.
C
C     C       (output) DOUBLE PRECISION array, dimension (2)
C             C(1) and C(2) contain the real and imaginary part,
C             respectively, of the complex number c, the cosines of
C             the plane rotation.
C
C     S       (output) DOUBLE PRECISION
C             The real number s, the sines of the plane rotation.
C
C     CONTRIBUTOR
C
C     Release 3.0: V. Sima, Katholieke Univ. Leuven, Belgium, Aug. 1997.
C     Supersedes Release 2.0 routine SB03CV by Sven Hammarling,
C     NAG Ltd., United Kingdom, May 1985.
C
C     REVISIONS
C
C     Dec. 1997.
C
C     KEYWORDS
C
C     Lyapunov equation, orthogonal transformation.
C
C     *****************************************************************
C
C     .. Parameters ..
      DOUBLE PRECISION  ONE, ZERO
      PARAMETER         ( ONE = 1.0D0, ZERO = 0.0D0 )
C     .. Scalar Arguments ..
      DOUBLE PRECISION  B, S
C     .. Array Arguments ..
      DOUBLE PRECISION  A(2), C(2)
C     .. Local Scalars ..
      DOUBLE PRECISION  D
C     .. External Functions ..
      DOUBLE PRECISION  DLAPY3
      EXTERNAL          DLAPY3
C     .. Executable Statements ..
C
      D = DLAPY3( A(1), A(2), B )
      IF ( D.EQ.ZERO ) THEN
         C(1) = ONE
         C(2) = ZERO
         S = ZERO
      ELSE
         C(1) = A(1)/D
         C(2) = A(2)/D
         S = B/D
         A(1) = D
         A(2) = ZERO
      END IF
C
      RETURN
C *** Last line of SB03OV ***
      END
