      DOUBLE PRECISION FUNCTION dln1px(a)
C**********************************************************************
C
C     DOUBLE PRECISION FUNCTION DLN1PX(X)
C               Double precision LN(1+X)
C
C
C                              Function
C
C
C     Returns ln(1+x)
C     Note that the obvious code of
C               LOG(1.0+X)
C     won't work for small X because 1.0+X loses accuracy
C
C
C                              Arguments
C
C
C     X --> Value for which ln(1-x) is desired.
C                                        X is DOUBLE PRECISION
C
C
C                              Method
C
C
C     Renames ALNREL from:
C     DiDinato, A. R. and Morris,  A.   H.  Algorithm 708: Significant
C     Digit Computation of the Incomplete  Beta  Function Ratios.  ACM
C     Trans. Math.  Softw. 18 (1993), 360-373.
C
C**********************************************************************
C-----------------------------------------------------------------------
C            EVALUATION OF THE FUNCTION LN(1 + A)
C-----------------------------------------------------------------------
C     .. Scalar Arguments ..
      DOUBLE PRECISION a
C     ..
C     .. Local Scalars ..
      DOUBLE PRECISION p1,p2,p3,q1,q2,q3,t,t2,w,x
C     ..
C     .. Intrinsic Functions ..
      INTRINSIC abs,dble,dlog
C     ..
C     .. Data statements ..
      DATA p1/-.129418923021993D+01/,p2/.405303492862024D+00/,
     +     p3/-.178874546012214D-01/
      DATA q1/-.162752256355323D+01/,q2/.747811014037616D+00/,
     +     q3/-.845104217945565D-01/
C     ..
C     .. Executable Statements ..
C--------------------------
      IF (abs(a).GT.0.375D0) GO TO 10
      t = a/ (a+2.0D0)
      t2 = t*t
      w = (((p3*t2+p2)*t2+p1)*t2+1.0D0)/ (((q3*t2+q2)*t2+q1)*t2+1.0D0)
      dln1px = 2.0D0*t*w
      RETURN
C
   10 x = 1.D0 + dble(a)
      dln1px = dlog(x)
      RETURN

      END
