      SUBROUTINE SB04MY( N, M, IND, A, LDA, B, LDB, C, LDC, D, IPR,
     $                   INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 1999.
C
C     PURPOSE
C
C     To construct and solve a linear algebraic system of order M whose
C     coefficient matrix is in upper Hessenberg form. Such systems
C     appear when solving Sylvester equations using the Hessenberg-Schur
C     method.
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
C             The index of the column in C to be computed.  IND >= 1.
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
C             the matrix C with column IND updated.
C
C     LDC     INTEGER
C             The leading dimension of array C.  LDC >= MAX(1,M).
C
C     Workspace
C
C     D       DOUBLE PRECISION array, dimension (M*(M+1)/2+2*M)
C
C     IPR     INTEGER array, dimension (2*M)
C
C     Error Indicator
C
C     INFO    INTEGER
C             = 0:  successful exit;
C             > 0:  if INFO = IND, a singular matrix was encountered.
C
C     METHOD
C
C     A special linear algebraic system of order M, with coefficient
C     matrix in upper Hessenberg form is constructed and solved. The
C     coefficient matrix is stored compactly, row-wise.
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
C     Supersedes Release 2.0 routine SB04AY by G. Golub, S. Nash, and
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
C     .. Scalar Arguments ..
      INTEGER           INFO, IND, LDA, LDB, LDC, M, N
C     .. Array Arguments ..
      INTEGER           IPR(*)
      DOUBLE PRECISION  A(LDA,*), B(LDB,*), C(LDC,*), D(*)
C     .. Local Scalars ..
      INTEGER           I, I2, J, K, K1, K2, M1
C     .. External Subroutines ..
      EXTERNAL          DAXPY, DCOPY, SB04MW
C     .. Intrinsic Functions ..
      INTRINSIC         MAX
C     .. Executable Statements ..
C
      DO 20 I = IND + 1, N
         CALL DAXPY( M, -B(IND,I), C(1,I), 1, C(1,IND), 1 )
   20 CONTINUE
C
      M1 = M + 1
      I2 = ( M*M1 )/2 + M1
      K2 = 1
      K  = M
C
C     Construct the linear algebraic system of order M.
C
      DO 40 I = 1, M
         J = M1 - K
         CALL DCOPY ( K, A(I,J), LDA, D(K2), 1 )
         K1 = K2
         K2 = K2 + K
         IF ( I.GT.1 ) THEN
            K1 = K1 + 1
            K  = K - 1
         END IF
         D(K1) = D(K1) + B(IND,IND)
C
C        Store the right hand side.
C
         D(I2) = C(I,IND)
         I2 = I2 + 1
   40 CONTINUE
C
C     Solve the linear algebraic system and store the solution in C.
C
      CALL SB04MW( M, D, IPR, INFO )
C
      IF ( INFO.NE.0 ) THEN
         INFO = IND
      ELSE
C
         DO 60 I = 1, M
            C(I,IND) = D(IPR(I))
   60    CONTINUE
C
      END IF
C
      RETURN
C *** Last line of SB04MY ***
      END
