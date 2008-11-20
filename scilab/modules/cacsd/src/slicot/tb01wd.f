      SUBROUTINE TB01WD( N, M, P, A, LDA, B, LDB, C, LDC, U, LDU,
     $                   WR, WI, DWORK, LDWORK, INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 1999.
C
C     PURPOSE
C
C     To reduce the system state matrix A to an upper real Schur form
C     by using an orthogonal similarity transformation A <-- U'*A*U and
C     to apply the transformation to the matrices B and C: B <-- U'*B
C     and C <-- C*U.
C
C     ARGUMENTS
C
C     Input/Output Parameters
C
C     N       (input) INTEGER
C             The order of the original state-space representation,
C             i.e. the order of the matrix A.  N >= 0.
C
C     M       (input) INTEGER
C             The number of system inputs, or of columns of B.  M >= 0.
C             
C     P       (input) INTEGER
C             The number of system outputs, or of rows of C.  P >= 0.
C             
C     A       (input/output) DOUBLE PRECISION array, dimension (LDA,N)
C             On entry, the leading N-by-N part of this array must  
C             contain the original state dynamics matrix A.
C             On exit, the leading N-by-N part of this array contains
C             the matrix U' * A * U in real Schur form. The elements
C             below the first subdiagonal are set to zero.
C             Note:  A matrix is in real Schur form if it is upper
C                    quasi-triangular with 1-by-1 and 2-by-2 blocks. 
C                    2-by-2 blocks are standardized in the form
C                             [  a  b  ]
C                             [  c  a  ]
C                    where b*c < 0. The eigenvalues of such a block 
C                    are a +- sqrt(bc).
C     
C     LDA     INTEGER
C             The leading dimension of array A.  LDA >= MAX(1,N).
C
C     B       (input/output) DOUBLE PRECISION array, dimension (LDB,M)
C             On entry, the leading N-by-M part of this array must 
C             contain the input matrix B.
C             On exit, the leading N-by-M part of this array contains 
C             the transformed input matrix U' * B.
C              
C     LDB     INTEGER
C             The leading dimension of array B.  LDB >= MAX(1,N).
C
C     C       (input/output) DOUBLE PRECISION array, dimension (LDC,N)
C             On entry, the leading P-by-N part of this array must 
C             contain the output matrix C.
C             On exit, the leading P-by-N part of this array contains 
C             the transformed output matrix C * U.
C              
C     LDC     INTEGER
C             The leading dimension of array C.  LDC >= MAX(1,P).
C
C     U       (output) DOUBLE PRECISION array, dimension (LDU,N)
C             The leading N-by-N part of this array contains the
C             orthogonal transformation matrix used to reduce A to the
C             real Schur form. The columns of U are the Schur vectors of
C             matrix A.
C
C     LDU     INTEGER
C             The leading dimension of array U.  LDU >= max(1,N).
C
C     WR, WI  (output) DOUBLE PRECISION arrays, dimension (N)
C             WR and WI contain the real and imaginary parts,
C             respectively, of the computed eigenvalues of A. The
C             eigenvalues will be in the same order that they appear on
C             the diagonal of the output real Schur form of A. Complex
C             conjugate pairs of eigenvalues will appear consecutively
C             with the eigenvalue having the positive imaginary part
C             first.
C
C     Workspace
C
C     DWORK   DOUBLE PRECISION array, dimension (LDWORK)
C             On exit, if INFO = 0, DWORK(1) returns the optimal value
C             of LDWORK.
C
C     LDWORK  INTEGER
C             The dimension of working array DWORK.  LWORK >= 3*N.
C             For optimum performance LDWORK should be larger.
C
C     Error Indicator
C
C     INFO    INTEGER
C             = 0:  successful exit;
C             < 0:  if INFO = -i, the i-th argument had an illegal 
C                   value;
C             > 0:  if INFO = i, the QR algorithm failed to compute 
C                   all the eigenvalues; elements i+1:N of WR and WI
C                   contain those eigenvalues which have converged; 
C                   U contains the matrix which reduces A to its
C                   partially converged Schur form.
C
C     METHOD
C
C     Matrix A is reduced to a real Schur form using an orthogonal
C     similarity transformation A <- U'*A*U. Then, the transformation 
C     is applied to the matrices B and C: B <-- U'*B and C <-- C*U.
C
C     NUMERICAL ASPECTS
C                                     3
C     The algorithm requires about 10N  floating point operations.
C
C     CONTRIBUTOR
C
C     A. Varga, German Aerospace Center,
C     DLR Oberpfaffenhofen, March 1998.
C     Based on the RASP routine SRSFDC.
C
C     REVISIONS
C
C     -   
C
C     KEYWORDS
C
C     Orthogonal transformation, real Schur form, similarity
C     transformation.
C
C     ******************************************************************
C
C     .. Parameters ..
      DOUBLE PRECISION ZERO, ONE
      PARAMETER        ( ZERO = 0.0D0, ONE = 1.0D0 )
C     .. Scalar Arguments ..
      INTEGER          INFO, LDA, LDB, LDC, LDU, LDWORK, M, N, P
C     .. Array Arguments ..
      DOUBLE PRECISION A(LDA,*), B(LDB,*), C(LDC,*), DWORK(*), U(LDU,*),
     $                 WI(*), WR(*)
C     .. Local Scalars ..
      INTEGER          I, LDWP, SDIM
      DOUBLE PRECISION WRKOPT
C     .. Local Arrays ..
      LOGICAL          BWORK( 1 )
C     .. External Functions ..
      LOGICAL          SELECT1
      EXTERNAL         SELECT1
C     .. External Subroutines ..
      EXTERNAL         DCOPY, DGEES, DGEMM, DGEMV, DLACPY, XERBLA
C     .. Intrinsic Functions ..
      INTRINSIC        DBLE, MAX
C     
C     .. Executable Statements ..
C
      INFO = 0
C
C     Check input parameters.
C
      IF( N.LT.0 ) THEN
         INFO = -1
      ELSE IF( M.LT.0 ) THEN
         INFO = -2
      ELSE IF( P.LT.0 ) THEN
         INFO = -3
      ELSE IF( LDA.LT.MAX( 1, N ) ) THEN
         INFO = -5
      ELSE IF( LDB.LT.MAX( 1, N ) ) THEN
         INFO = -7
      ELSE IF( LDC.LT.MAX( 1, P ) ) THEN
         INFO = -9
      ELSE IF( LDU.LT.MAX( 1, N ) ) THEN
         INFO = -11
      ELSE IF( LDWORK.LT.3*N ) THEN
         INFO = -15
      END IF
C
      IF( INFO.NE.0 ) THEN
C
C        Error return.
C
         CALL XERBLA( 'TB01WD', -INFO )
         RETURN
      END IF
C
C     Quick return if possible.
C
      IF( N.EQ.0 )
     $   RETURN
C
C     Reduce A to real Schur form using an orthogonal similarity
C     transformation A <- U'*A*U, accumulate the transformation in U
C     and compute the eigenvalues of A in (WR,WI).
C
C     Workspace:  need   3*N;
C                 prefer larger.
C
      CALL DGEES( 'Vectors', 'Not ordered', SELECT1, N, A, LDA, SDIM,
     $            WR, WI, U, LDU, DWORK, LDWORK, BWORK, INFO )
      WRKOPT = DWORK( 1 )
      IF( INFO.NE.0 )
     $    RETURN
C
C     Apply the transformation: B <-- U'*B.
C
      IF( LDWORK.LT.N*M ) THEN
C
C        Not enough working space for using DGEMM.
C     
         DO 10 I = 1, M
            CALL DCOPY( N, B(1,I), 1, DWORK, 1 )
            CALL DGEMV( 'Transpose', N, N, ONE, U, LDU, DWORK, 1, ZERO,
     $                  B(1,I), 1 )
   10    CONTINUE
C
      ELSE
         CALL DLACPY( 'Full', N, M, B, LDB, DWORK, N )
         CALL DGEMM( 'Transpose', 'No transpose', N, M, N, ONE, U, LDU,
     $               DWORK, N, ZERO, B, LDB )
         WRKOPT = MAX( WRKOPT, DBLE( N*M ) )
      END IF
C
C     Apply the transformation: C <-- C*U.
C
      IF( LDWORK.LT.N*P ) THEN
C
C        Not enough working space for using DGEMM.
C     
         DO 20 I = 1, P
            CALL DCOPY( N, C(I,1), LDC, DWORK, 1 )
            CALL DGEMV( 'Transpose', N, N, ONE, U, LDU, DWORK, 1, ZERO,
     $                  C(I,1), LDC )
   20    CONTINUE
C
      ELSE
         LDWP = MAX( 1, P )
         CALL DLACPY( 'Full', P, N, C, LDC, DWORK, LDWP )
         CALL DGEMM( 'No transpose', 'No transpose', P, N, N, ONE,
     $               DWORK, LDWP, U, LDU, ZERO, C, LDC )
         WRKOPT = MAX( WRKOPT, DBLE( N*P ) )
      END IF
C
      DWORK( 1 ) = WRKOPT
C
      RETURN
C *** Last line of TB01WD ***
      END
