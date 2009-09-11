*DECK SPOFA
      SUBROUTINE spofa(a,lda,n,info)
      INTEGER lda,n,info
      DOUBLE PRECISION a(lda,1)
C
C     SPOFA FACTORS A DOUBLE PRECISION SYMMETRIC POSITIVE DEFINITE MATRIX.
C
C     SPOFA IS USUALLY CALLED BY SPOCO, BUT IT CAN BE CALLED
C     DIRECTLY WITH A SAVING IN TIME IF  RCOND  IS NOT NEEDED.
C     (TIME FOR SPOCO) = (1 + 18/N)*(TIME FOR SPOFA) .
C
C     ON ENTRY
C
C        A       DOUBLE PRECISION(LDA, N)
C                THE SYMMETRIC MATRIX TO BE FACTORED.  ONLY THE
C                DIAGONAL AND UPPER TRIANGLE ARE USED.
C
C        LDA     INTEGER
C                THE LEADING DIMENSION OF THE ARRAY  A .
C
C        N       INTEGER
C                THE ORDER OF THE MATRIX  A .
C
C     ON RETURN
C
C        A       AN UPPER TRIANGULAR MATRIX  R  SO THAT  A = TRANS(R)*R
C                WHERE  TRANS(R)  IS THE TRANSPOSE.
C                THE STRICT LOWER TRIANGLE IS UNALTERED.
C                IF  INFO .NE. 0 , THE FACTORIZATION IS NOT COMPLETE.
C
C        INFO    INTEGER
C                = 0  FOR NORMAL RETURN.
C                = K  SIGNALS AN ERROR CONDITION.  THE LEADING MINOR
C                     OF ORDER  K  IS NOT POSITIVE DEFINITE.
C
C     LINPACK.  THIS VERSION DATED 08/14/78 .
C     CLEVE MOLER, UNIVERSITY OF NEW MEXICO, ARGONNE NATIONAL LAB.
C
C     SUBROUTINES AND FUNCTIONS
C
C     BLAS SDOT
C     FORTRAN SQRT
C
C     INTERNAL VARIABLES
C
      DOUBLE PRECISION sdot,t
      DOUBLE PRECISION s
      INTEGER j,jm1,k
C     BEGIN BLOCK WITH ...EXITS TO 40
C
C
      DO 30 j = 1,n
          info = j
          s = 0.0E0
          jm1 = j - 1
          IF (jm1.LT.1) GO TO 20
          DO 10 k = 1,jm1
              t = a(k,j) - sdot(k-1,a(1,k),1,a(1,j),1)
              t = t/a(k,k)
              a(k,j) = t
              s = s + t*t
   10     CONTINUE
   20     CONTINUE
          s = a(j,j) - s
C     ......EXIT
          IF (s.LE.0.0E0) GO TO 40
          a(j,j) = sqrt(s)
   30 CONTINUE
      info = 0
   40 CONTINUE
      RETURN

      END
