      DOUBLE PRECISION FUNCTION genunf(low,high)
C**********************************************************************
C
C     DOUBLE PRECISION FUNCTION GENUNF( LOW, HIGH )
C
C               GeNerate Uniform DOUBLE PRECISION between LOW and HIGH
C
C
C                              Function
C
C
C     Generates a DOUBLE PRECISION uniformly distributed between LOW and HIGH.
C
C
C                              Arguments
C
C
C     LOW --> Low bound (exclusive) on DOUBLE PRECISION value to be generated
C                         DOUBLE PRECISION LOW
C
C     HIGH --> High bound (exclusive) on DOUBLE PRECISION value to be generated
C                         DOUBLE PRECISION HIGH
C
C**********************************************************************
C     .. Scalar Arguments ..
      DOUBLE PRECISION high,low
C     ..
C     .. External Functions ..
      DOUBLE PRECISION ranf
      EXTERNAL ranf
C     ..
C     .. Executable Statements ..
   10 genunf = low + (high-low)*ranf()

      RETURN

      END
