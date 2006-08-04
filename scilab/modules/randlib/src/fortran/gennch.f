      DOUBLE PRECISION FUNCTION gennch(df,xnonc)
C**********************************************************************
C
C     DOUBLE PRECISION FUNCTION GENNCH( DF, XNONC )
C           Generate random value of Noncentral CHIsquare variable
C
C
C                              Function
C
C

C     Generates random deviate  from the  distribution  of a  noncentral
C     chisquare with DF degrees  of freedom and noncentrality  parameter
C     XNONC.
C
C
C                              Arguments
C
C
C     DF --> Degrees of freedom of the chisquare
C            (Must be >= 1.0)
C                         DOUBLE PRECISION DF
C
C     XNONC --> Noncentrality parameter of the chisquare
C               (Must be >= 0.0)
C                         DOUBLE PRECISION XNONC
C
C
C                              Method
C
C
C     Uses fact that  noncentral chisquare  is  the  sum of a  chisquare
C     deviate with DF-1  degrees of freedom plus the  square of a normal
C     deviate with mean sqrt(XNONC) and standard deviation 1.
C
C**********************************************************************
C     .. Scalar Arguments ..
      DOUBLE PRECISION df,xnonc
C     ..
C     .. External Functions ..
C     JJV changed these to call SGAMMA and SNORM directly
C      DOUBLE PRECISION genchi,gennor
C      EXTERNAL genchi,gennor
      DOUBLE PRECISION sgamma,snorm
      EXTERNAL sgamma,snorm
C     ..
C     .. Intrinsic Functions ..
      INTRINSIC sqrt
C     ..
C     JJV changed abort to df < 1, and added case: df = 1 
C     .. Executable Statements ..
C     JJV changed this to call SGAMMA and SNORM directly
C      gennch = genchi(df-1.0) + gennor(sqrt(xnonc),1.0)**2

 10   IF (df.GE.1.000001) GO TO 20
C     JJV case DF = 1.0
      gennch = (snorm() + sqrt(xnonc))**2
      GO TO 30

C     JJV case DF > 1.0
 20   gennch = 2.0*sgamma((df-1.0)/2.0) + (snorm() + sqrt(xnonc))**2
 30   RETURN
      
      END
