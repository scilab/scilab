      DOUBLE PRECISION FUNCTION dcddexpm1(x)
C**********************************************************************
C
C      DOUBLE PRECISION FUNCTION dcddexpm1(x)
C            Evaluation of the function EXP(X) - 1
C
C
C                              Arguments
C
C
C     X --> Argument at which exp(x)-1 desired
C                    DOUBLE PRECISION X
C
C
C                              Method
C
C
C     Renaming of function rexp from code of:
C
C     DiDinato, A. R. and Morris,  A.   H.  Algorithm 708: Significant
C     Digit Computation of the Incomplete  Beta  Function Ratios.  ACM
C     Trans. Math.  Softw. 18 (1993), 360-373.
C
C**********************************************************************
C     .. Scalar Arguments ..
      DOUBLE PRECISION x
C     ..
C     .. Local Scalars ..
      DOUBLE PRECISION p1,p2,q1,q2,q3,q4,w
C     ..
C     .. Intrinsic Functions ..
      INTRINSIC abs,exp
C     ..
C     .. Data statements ..
      DATA p1/.914041914819518D-09/,p2/.238082361044469D-01/,
     +     q1/-.499999999085958D+00/,q2/.107141568980644D+00/,
     +     q3/-.119041179760821D-01/,q4/.595130811860248D-03/
C     ..
C     .. Executable Statements ..
C
      IF (abs(x).GT.0.15D0) GO TO 10
      dcddexpm1 = x* (((p2*x+p1)*x+1.0D0)/
     +         ((((q4*x+q3)*x+q2)*x+q1)*x+1.0D0))
      RETURN
C
   10 w = exp(x)
      IF (x.GT.0.0D0) GO TO 20
      dcddexpm1 = (w-0.5D0) - 0.5D0
      RETURN

   20 dcddexpm1 = w* (0.5D0+ (0.5D0-1.0D0/w))
      RETURN

      END
