      SUBROUTINE SB04MU( N, M, IND, A, LDA, B, LDB, C, LDC, D, IPR,
     $                   INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 1999.
C
C     PURPOSE
C
C     To construct and solve a linear algebraic system of order 2*M
C     whose coefficient matrix has zeros below the second subdiagonal.
C     Such systems appear when solving continuous-time Sylvester 
C     equations using the Hessenberg-Schur method.
C
C     ARGUMENTS
C
C     Input/Output Parameters
C
C     N       (input) INTEGER
C             The order of the matrix B.  N >= 0.
C
C     M       (input) INTEGER
C             The order of the matrix A.  M >= 0.
C
C     IND     (input) INTEGER
C             IND and IND - 1 specify the indices of the columns in C
C             to be computed.  IND > 1.
C
C     A       (input) DOUBLE PRECISION array, dimension (LDA,M)
C             The leading M-by-M part of this array must contain an
C             upper Hessenberg matrix.
C
C     LDA     INTEGER
C             The leading dimension of array A.  LDA >= MAX(1,M).
C
C     B       (input) DOUBLE PRECISION array, dimension (LDB,N)
C             The leading N-by-N part of this array must contain a
C             matrix in real Schur form.
C
C     LDB     INTEGER
C             The leading dimension of array B.  LDB >= MAX(1,N).
C
C     C       (input/output) DOUBLE PRECISION array, dimension (LDC,N)
C             On entry, the leading M-by-N part of this array must
C             contain the coefficient matrix C of the equation.
C             On exit, the leading M-by-N part of this array contains
C             the matrix C with columns IND-1 and IND updated.
C
C     LDC     INTEGER
C             The leading dimension of array C.  LDC >= MAX(1,M).
C
C     Workspace
C
C     D       DOUBLE PRECISION array, dimension (2*M*M+7*M)
C
C     IPR     INTEGER array, dimension (4*M)
C
C     Error Indicator
C
C     INFO    INTEGER
C             = 0:  successful exit;
C             > 0:  if INFO = IND, a singular matrix was encountered.
C
C     METHOD
C
C     A special linear algebraic system of order 2*M, whose coefficient
C     matrix has zeros below the second subdiagonal is constructed and
C     solved. The coefficient matrix is stored compactly, row-wise.
C
C     REFERENCES
C
C     [1] Golub, G.H., Nash, S. and Van Loan, C.F.
C         A Hessenberg-Schur method for the problem AX + XB = C.
C         IEEE Trans. Auto. Contr., AC-24, pp. 909-913, 1979.
C
C     NUMERICAL ASPECTS
C     
C     None.
C
C     CONTRIBUTORS
C
C     Release 3.0: V. Sima, Katholieke Univ. Leuven, Belgium, Sep. 1997.
C     Supersedes Release 2.0 routine SB04AU by G. Golub, S. Nash, and
C     C. Van Loan, Stanford University, California, United States of
C     America, January 1982.
C
C     REVISIONS
C
C     -
C
C     KEYWORDS
C
C     Hessenberg form, orthogonal transformation, real Schur form,
C     Sylvester equation.
C
C     ******************************************************************
C
      DOUBLE PRECISION  ZERO
      PARAMETER         ( ZERO = 0.0D0 )
C     .. Scalar Arguments ..
      INTEGER           INFO, IND, LDA, LDB, LDC, M, N
C     .. Array Arguments ..
      INTEGER           IPR(*)
      DOUBLE PRECISION  A(LDA,*), B(LDB,*), C(LDC,*), D(*)
C     .. Local Scalars ..
      INTEGER           I, I2, IND1, J, K, K1, K2, M2
      DOUBLE PRECISION  TEMP
C     .. External Subroutines ..
      EXTERNAL          DAXPY, SB04MR
C     .. Intrinsic Functions ..
      INTRINSIC         MAX, MIN
C     .. Executable Statements ..
C
      IND1 = IND - 1
C     
      DO 20 I = IND + 1, N
         CALL DAXPY( M, -B(IND1,I), C(1,I), 1, C(1,IND1), 1 )
         CALL DAXPY( M, -B(IND,I),  C(1,I), 1, C(1,IND),  1 )
   20 CONTINUE
C
C     Construct the linear algebraic system of order 2*M.
C
      K1 = -1
      M2 = 2*M
      I2 = M*(M2 + 5)
      K  = M2
C
      DO 60 I = 1, M
C
         DO 40 J = MAX( 1, I - 1 ), M
            K1 = K1 + 2
            K2 = K1 + K
            TEMP = A(I,J)
            IF ( I.NE.J ) THEN
               D(K1)   = TEMP
               D(K1+1) = ZERO
               IF ( J.GT.I ) D(K2) = ZERO
               D(K2+1) = TEMP
            ELSE
               D(K1)   = TEMP + B(IND1,IND1)
               D(K1+1) = B(IND1,IND)
               D(K2)   = B(IND,IND1)
               D(K2+1) = TEMP + B(IND,IND)
            END IF
   40    CONTINUE
C
         K1 = K2
         K = K - MIN( 2, I )
C
C        Store the right hand side.
C
         I2 = I2 + 2
         D(I2)   = C(I,IND)
         D(I2-1) = C(I,IND1)
   60 CONTINUE
C
C     Solve the linear algebraic system and store the solution in C.
C
      CALL SB04MR( M2, D, IPR, INFO )
C
      IF ( INFO.NE.0 ) THEN
         INFO = IND
      ELSE
         I2 = 0
C
         DO 80 I = 1, M
            I2 = I2 + 2
            C(I,IND1) = D(IPR(I2-1))    
            C(I,IND)  = D(IPR(I2))
   80    CONTINUE
C
      END IF
C
      RETURN
C *** Last line of SB04MU ***
      END
