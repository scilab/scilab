      DOUBLE PRECISION FUNCTION dbetrm(a,b)
C**********************************************************************
C
C     DOUBLE PRECISION FUNCTION DBETRM( A, B )
C          Double Precision Sterling Remainder for Complete
C                    Beta Function
C
C
C                              Function
C
C
C     Log(Beta(A,B)) = Lgamma(A) + Lgamma(B) - Lgamma(A+B)
C     where Lgamma is the log of the (complete) gamma function
C
C     Let ZZ be approximation obtained if each log gamma is approximated
C     by Sterling's formula, i.e.,
C     Sterling(Z) = LOG( SQRT( 2*PI ) ) + ( Z-0.5 ) * LOG( Z ) - Z
C
C     Returns Log(Beta(A,B)) - ZZ
C
C
C                              Arguments
C
C
C     A --> One argument of the Beta
C                    DOUBLE PRECISION A
C
C     B --> The other argument of the Beta
C                    DOUBLE PRECISION B
C
C**********************************************************************
C     .. Scalar Arguments ..
      DOUBLE PRECISION a,b
C     ..
C     .. External Functions ..
      DOUBLE PRECISION dstrem
      EXTERNAL dstrem
C     ..
C     .. Intrinsic Functions ..
      INTRINSIC max,min
C     ..
C     .. Executable Statements ..

C     Try to sum from smallest to largest
      dbetrm = -dstrem(a+b)
      dbetrm = dbetrm + dstrem(max(a,b))
      dbetrm = dbetrm + dstrem(min(a,b))
      RETURN

      END
