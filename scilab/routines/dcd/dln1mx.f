      DOUBLE PRECISION FUNCTION dln1mx(x)
C**********************************************************************
C
C     DOUBLE PRECISION FUNCTION DLN1MX(X)
C               Double precision LN(1-X)
C
C
C                              Function
C
C
C     Returns ln(1-x) for small x (good accuracy if x .le. 0.1).
C     Note that the obvious code of
C               LOG(1.0-X)
C     won't work for small X because 1.0-X loses accuracy
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
C     If X > 0.1, the obvious code above is used ELSE
C     The Taylor series for 1-x is expanded to 20 terms.
C
C**********************************************************************
C     .. Scalar Arguments ..
      DOUBLE PRECISION x
C     ..
C     .. External Functions ..
      DOUBLE PRECISION dln1px
      EXTERNAL dln1px
C     ..
C     .. Executable Statements ..

      dln1mx = dln1px(-x)
      RETURN

      END
