      SUBROUTINE SB04QU( N, M, IND, A, LDA, B, LDB, C, LDC, D, IPR,
     $                   INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 2000.
C
C     PURPOSE
C
C     To construct and solve a linear algebraic system of order 2*M
C     whose coefficient matrix has zeros below the third subdiagonal,
C     and zero elements on the third subdiagonal with even column 
C     indices. Such systems appear when solving discrete-time Sylvester 
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
C     D       DOUBLE PRECISION array, dimension (2*M*M+8*M)
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
C     matrix has zeros below the third subdiagonal and zero elements on 
C     the third subdiagonal with even column indices, is constructed and
C     solved. The coefficient matrix is stored compactly, row-wise.
C
C     REFERENCES
C
C     [1] Golub, G.H., Nash, S. and Van Loan, C.F.
C         A Hessenberg-Schur method for the problem AX + XB = C.
C         IEEE Trans. Auto. Contr., AC-24, pp. 909-913, 1979.
C
C     [2] Sima, V.
C         Algorithms for Linear-quadratic Optimization.
C         Marcel Dekker, Inc., New York, 1996.
C
C     NUMERICAL ASPECTS
C     
C     None.
C
C     CONTRIBUTORS
C
C     D. Sima, University of Bucharest, May 2000.
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
      DOUBLE PRECISION  ONE, ZERO
      PARAMETER         ( ONE = 1.0D0, ZERO = 0.0D0 )
C     .. Scalar Arguments ..
      INTEGER           INFO, IND, LDA, LDB, LDC, M, N
C     .. Array Arguments ..
      INTEGER           IPR(*)
      DOUBLE PRECISION  A(LDA,*), B(LDB,*), C(LDC,*), D(*)
C     .. Local Scalars ..
      INTEGER           I, I2, IND1, J, K, K1, K2, M2
      DOUBLE PRECISION  TEMP
C     .. Local Arrays ..
      DOUBLE PRECISION  DUM(1)
C     .. External Subroutines ..
      EXTERNAL          DAXPY, DCOPY, DTRMV, SB04QR
C     .. Intrinsic Functions ..
      INTRINSIC         MAX
C     .. Executable Statements ..
C
      IND1 = IND - 1
C  
      IF ( IND.LT.N ) THEN   	
         DUM(1) = ZERO
         CALL DCOPY ( M, DUM, 0, D, 1 ) 
         DO 10 I = IND + 1, N
            CALL DAXPY ( M, B(IND1,I), C(1,I), 1, D, 1 )
   10    CONTINUE
C      
         DO 20 I = 2, M
            C(I,IND1) = C(I,IND1) - A(I,I-1)*D(I-1) 
   20    CONTINUE  
         CALL DTRMV ( 'Upper', 'No Transpose', 'Non Unit', M, A, LDA,
     $                D, 1 )
         DO 30 I = 1, M
            C(I,IND1) = C(I,IND1) - D(I) 
   30    CONTINUE  
C 
         CALL DCOPY ( M, DUM, 0, D, 1 ) 
         DO 40 I = IND + 1, N
            CALL DAXPY ( M, B(IND,I), C(1,I), 1, D, 1 )
   40    CONTINUE
C
         DO 50 I = 2, M
            C(I,IND) = C(I,IND) - A(I,I-1)*D(I-1) 
   50    CONTINUE  
         CALL DTRMV ( 'Upper', 'No Transpose', 'Non Unit', M, A, LDA,
     $                D, 1 )
         DO 60 I = 1, M
            C(I,IND) = C(I,IND) - D(I)
   60    CONTINUE  
      END IF
C
C     Construct the linear algebraic system of order 2*M.
C
      K1 = -1
      M2 = 2*M
      I2 = M2*(M + 3)
      K  = M2
C
      DO 80 I = 1, M
C
         DO 70 J = MAX( 1, I - 1 ), M
            K1 = K1 + 2
            K2 = K1 + K
            TEMP = A(I,J)
            D(K1)   = TEMP * B(IND1,IND1)
            D(K1+1) = TEMP * B(IND1,IND)
            D(K2)   = TEMP * B(IND,IND1)
            D(K2+1) = TEMP * B(IND,IND)
            IF ( I.EQ.J ) THEN
               D(K1)   = D(K1) + ONE
               D(K2+1) = D(K2+1) + ONE
            END IF
   70    CONTINUE
C
         K1 = K2
         IF ( I.GT.1 ) K = K - 2
C
C        Store the right hand side.
C
         I2 = I2 + 2
         D(I2)   = C(I,IND)
         D(I2-1) = C(I,IND1)
   80 CONTINUE
C
C     Solve the linear algebraic system and store the solution in C.
C
      CALL SB04QR( M2, D, IPR, INFO )
C
      IF ( INFO.NE.0 ) THEN
         INFO = IND
      ELSE
         I2 = 0
C
         DO 90 I = 1, M
            I2 = I2 + 2
            C(I,IND1) = D(IPR(I2-1))    
            C(I,IND)  = D(IPR(I2))
   90    CONTINUE
C
      END IF
C
      RETURN
C *** Last line of SB04QU ***
      END
