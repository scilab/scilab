      SUBROUTINE MA02FD( X1, X2, C, S, INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 2000.
C
C     PURPOSE
C
C     To compute the coefficients c and s (c^2 + s^2 = 1) for a modified
C     hyperbolic plane rotation, such that, 
C
C         y1 := 1/c * x1 - s/c * x2 = sqrt(x1^2 - x2^2),
C         y2 :=  -s * y1 +  c  * x2 = 0,
C
C     given two real numbers x1 and x2, satisfying either x1 = x2 = 0,
C     or abs(x2) < abs(x1).
C
C     ARGUMENTS 
C
C     Input/Output Parameters
C
C     X1      (input/output) DOUBLE PRECISION
C             On entry, the real number x1.
C             On exit, the real number y1.
C
C     X2      (input) DOUBLE PRECISION
C             The real number x2.
C             The values x1 and x2 should satisfy either x1 = x2 = 0, or
C             abs(x2) < abs(x1).
C
C     C       (output) DOUBLE PRECISION
C             The cosines c of the modified hyperbolic plane rotation.
C
C     S       (output) DOUBLE PRECISION
C             The sines s of the modified hyperbolic plane rotation. 
C
C     Error Indicator
C
C     INFO    INTEGER
C             = 0:  succesful exit;
C             = 1:  if abs(x2) >= abs(x1) and either x1 <> 0 or x2 <> 0.
C
C     CONTRIBUTOR
C
C     D. Kressner, Technical Univ. Chemnitz, Germany, June 2000.
C
C     REVISIONS
C
C     V. Sima, Katholieke Univ. Leuven, Belgium, June 2000.
C
C     KEYWORDS
C
C     Orthogonal transformation, plane rotation.
C
C     *****************************************************************
C
C     .. Parameters ..
      DOUBLE PRECISION  ONE, ZERO
      PARAMETER         ( ONE = 1.0D0, ZERO = 0.0D0 )
C     .. Scalar Arguments ..
      DOUBLE PRECISION  X1, X2, C, S
      INTEGER           INFO
C     .. Intrinsic Functions ..
      INTRINSIC         ABS, SIGN, SQRT
C     .. Executable Statements ..
C
      IF ( ( X1.NE.ZERO .OR. X2.NE.ZERO ) .AND. 
     $     ABS( X2 ).GE.ABS( X1 ) ) THEN
         INFO = 1
      ELSE
         INFO = 0
         IF ( X1.EQ.ZERO ) THEN
            S = ZERO
            C = ONE
         ELSE
            S = X2 / X1
C
C           No overflows could appear in the next statement; underflows
C           are possible if X2 is tiny and X1 is huge, but then 
C              abs(C) = ONE - delta,
C           where delta is much less than machine precision.
C
            C  = SIGN( SQRT( ONE - S ) * SQRT( ONE + S ), X1 )
            X1 = C * X1
         END IF
      END IF
C
      RETURN
C *** Last line of MA02FD ***
      END
