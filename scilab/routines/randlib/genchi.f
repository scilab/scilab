      DOUBLE PRECISION FUNCTION genchi(df)
C**********************************************************************
C
C     DOUBLE PRECISION FUNCTION GENCHI( DF )
C                Generate random value of CHIsquare variable
C
C
C                              Function
C
C
C     Generates random deviate from the distribution of a chisquare
C     with DF degrees of freedom random variable.
C
C
C                              Arguments
C
C
C     DF --> Degrees of freedom of the chisquare
C            (Must be positive)
C                         DOUBLE PRECISION DF
C
C
C                              Method
C
C
C     Uses relation between chisquare and gamma.
C
C**********************************************************************
C     .. Scalar Arguments ..
      DOUBLE PRECISION df
C     ..
C     .. External Functions ..
C      DOUBLE PRECISION gengam
C      EXTERNAL gengam
      DOUBLE PRECISION sgamma
      EXTERNAL sgamma
C     ..
C     .. Executable Statements ..
C     JJV changed this to call sgamma directly
C   10 genchi = 2.0*gengam(1.0,df/2.0)
 10   genchi = 2.0*sgamma(df/2.0)
      RETURN

      END
