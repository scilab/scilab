      SUBROUTINE MB03OY( M, N, A, LDA, RCOND, SVLMAX, RANK, SVAL, JPVT, 
     $                   TAU, DWORK, INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 1999.
C
C     PURPOSE
C
C     To compute a rank-revealing QR factorization of a real general 
C     M-by-N matrix  A,  which may be rank-deficient, and estimate its
C     effective rank using incremental condition estimation.  
C
C     The routine uses a truncated QR factorization with column pivoting
C                                   [ R11 R12 ]
C        A * P = Q * R,  where  R = [         ],
C                                   [  0  R22 ]
C     with R11 defined as the largest leading upper triangular submatrix
C     whose estimated condition number is less than 1/RCOND.  The order
C     of R11, RANK, is the effective rank of A.  Condition estimation is
C     performed during the QR factorization process.  Matrix R22 is full
C     (but of small norm), or empty.
C
C     MB03OY  does not perform any scaling of the matrix A.
C
C     ARGUMENTS 
C
C     Input/Output Parameters
C
C     M       (input) INTEGER
C             The number of rows of the matrix A.  M >= 0.
C            
C     N       (input) INTEGER
C             The number of columns of the matrix A.  N >= 0.
C            
C     A       (input/output) DOUBLE PRECISION array, dimension 
C             ( LDA, N )
C             On entry, the leading M-by-N part of this array must
C             contain the given matrix A.
C             On exit, the leading RANK-by-RANK upper triangular part
C             of A contains the triangular factor R11, and the elements
C             below the diagonal in the first  RANK  columns, with the
C             array TAU, represent the orthogonal matrix Q as a product
C             of  RANK  elementary reflectors. 
C             The remaining  N-RANK  columns contain the result of the 
C             QR factorization process used. 
C            
C     LDA     INTEGER
C             The leading dimension of the array A.  LDA >= max(1,M).
C            
C     RCOND   (input) DOUBLE PRECISION
C             RCOND is used to determine the effective rank of A, which
C             is defined as the order of the largest leading triangular
C             submatrix R11 in the QR factorization with pivoting of A,
C             whose estimated condition number is less than 1/RCOND.
C             0 <= RCOND <= 1.
C             NOTE that when SVLMAX > 0, the estimated rank could be
C             less than that defined above (see SVLMAX).
C            
C     SVLMAX  (input) DOUBLE PRECISION
C             If A is a submatrix of another matrix B, and the rank 
C             decision should be related to that matrix, then SVLMAX
C             should be an estimate of the largest singular value of B 
C             (for instance, the Frobenius norm of B).  If this is not 
C             the case, the input value SVLMAX = 0 should work.
C             SVLMAX >= 0.
C            
C     RANK    (output) INTEGER
C             The effective (estimated) rank of A, i.e. the order of 
C             the submatrix R11.
C            
C     SVAL    (output) DOUBLE PRECISION array, dimension ( 3 )
C             The estimates of some of the singular values of the 
C             triangular factor R:
C             SVAL(1): largest singular value of R(1:RANK,1:RANK);
C             SVAL(2): smallest singular value of R(1:RANK,1:RANK);
C             SVAL(3): smallest singular value of R(1:RANK+1,1:RANK+1),
C                      if RANK < MIN( M, N ), or of R(1:RANK,1:RANK),
C                      otherwise.
C             If the triangular factorization is a rank-revealing one
C             (which will be the case if the leading columns were well-
C             conditioned), then SVAL(1) will also be an estimate for
C             the largest singular value of A, and SVAL(2) and SVAL(3)
C             will be estimates for the RANK-th and (RANK+1)-st singular
C             values of A, respectively.
C             By examining these values, one can confirm that the rank
C             is well defined with respect to the chosen value of RCOND.
C             The ratio SVAL(1)/SVAL(2) is an estimate of the condition
C             number of R(1:RANK,1:RANK).
C            
C     JPVT    (output) INTEGER array, dimension ( N )
C             If JPVT(i) = k, then the i-th column of A*P was the k-th
C             column of A.
C            
C     TAU     (output) DOUBLE PRECISION array, dimension ( MIN( M, N ) )
C             The leading  RANK  elements of TAU contain the scalar
C             factors of the elementary reflectors.
C            
C     Workspace
C
C     DWORK   DOUBLE PRECISION array, dimension ( LDWORK )
C             where LDWORK = max( 1, 3*N ).
C            
C     Error Indicator
C
C     INFO    INTEGER
C             = 0:  successful exit
C             < 0:  if INFO = -i, the i-th argument had an illegal 
C                   value.
C
C     METHOD
C
C     The routine computes a truncated QR factorization with column
C     pivoting of A,  A * P = Q * R,  with  R  defined above, and, 
C     during this process, finds the largest leading submatrix whose
C     estimated condition number is less than 1/RCOND, taking the
C     possible positive value of SVLMAX into account.  This is performed
C     using the LAPACK incremental condition estimation scheme and a
C     slightly modified rank decision test.  The factorization process 
C     stops when  RANK  has been determined.
C
C     The matrix Q is represented as a product of elementary reflectors
C
C        Q = H(1) H(2) . . . H(k), where k = rank <= min(m,n).
C
C     Each H(i) has the form
C
C        H = I - tau * v * v'
C
C     where tau is a real scalar, and v is a real vector with
C     v(1:i-1) = 0 and v(i) = 1; v(i+1:m) is stored on exit in
C     A(i+1:m,i), and tau in TAU(i).
C
C     The matrix P is represented in jpvt as follows: If
C        jpvt(j) = i
C     then the jth column of P is the ith canonical unit vector.
C
C     REFERENCES
C
C     [1] Bischof, C.H. and P. Tang.
C         Generalizing Incremental Condition Estimation.
C         LAPACK Working Notes 32, Mathematics and Computer Science
C         Division, Argonne National Laboratory, UT, CS-91-132,
C         May 1991.
C
C     [2] Bischof, C.H. and P. Tang.
C         Robust Incremental Condition Estimation.
C         LAPACK Working Notes 33, Mathematics and Computer Science
C         Division, Argonne National Laboratory, UT, CS-91-133,
C         May 1991.
C
C     NUMERICAL ASPECTS
C
C     The algorithm is backward stable.
C
C     CONTRIBUTOR
C
C     V. Sima, Katholieke Univ. Leuven, Belgium, Feb. 1998.
C
C     REVISIONS
C
C     -
C
C     KEYWORDS
C
C     Eigenvalue problem, matrix operations, orthogonal transformation,
C     singular values.
C
C    ******************************************************************
C
C     .. Parameters ..
      INTEGER            IMAX, IMIN
      PARAMETER          ( IMAX = 1, IMIN = 2 )
      DOUBLE PRECISION   ZERO, ONE
      PARAMETER          ( ZERO = 0.0D0, ONE = 1.0D0 )
C     .. Scalar Arguments ..
      INTEGER            INFO, LDA, M, N, RANK
      DOUBLE PRECISION   RCOND, SVLMAX
C     .. Array Arguments ..
      INTEGER            JPVT( * )
      DOUBLE PRECISION   A( LDA, * ), DWORK( * ), SVAL( 3 ), TAU( * )
C     ..
C     .. Local Scalars ..
      INTEGER            I, ISMAX, ISMIN, ITEMP, J, MN, PVT
      DOUBLE PRECISION   AII, C1, C2, S1, S2, SMAX, SMAXPR, SMIN,
     $                   SMINPR, TEMP, TEMP2
C     ..
C     .. External Functions ..
      INTEGER            IDAMAX
      DOUBLE PRECISION   DNRM2
      EXTERNAL           DNRM2, IDAMAX
C     .. External Subroutines ..
      EXTERNAL           DLAIC1, DLARF, DLARFG, DSCAL, DSWAP, XERBLA
C     .. Intrinsic Functions ..
      INTRINSIC          ABS, MAX, MIN, SQRT
C     ..
C     .. Executable Statements ..
C
C     Test the input scalar arguments.
C
      INFO = 0
      IF( M.LT.0 ) THEN
         INFO = -1
      ELSE IF( N.LT.0 ) THEN
         INFO = -2
      ELSE IF( LDA.LT.MAX( 1, M ) ) THEN
         INFO = -4
      ELSE IF( RCOND.LT.ZERO .OR. RCOND.GT.ONE ) THEN
         INFO = -5
      ELSE IF( SVLMAX.LT.ZERO ) THEN
         INFO = -6
      END IF
C
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'MB03OY', -INFO )
         RETURN
      END IF
C
C     Quick return if possible.
C
      MN = MIN( M, N )
      IF( MN.EQ.0 ) THEN
         RANK = 0
         SVAL( 1 ) = ZERO
         SVAL( 2 ) = ZERO
         SVAL( 3 ) = ZERO
         RETURN
      END IF
C
      ISMIN = 1
      ISMAX = ISMIN + N
C
C     Initialize partial column norms and pivoting vector. The first n
C     elements of DWORK store the exact column norms. The already used
C     leading part is then overwritten by the condition estimator.
C     
      DO 10 I = 1, N
         DWORK( I ) = DNRM2( M, A( 1, I ), 1 )
         DWORK( N+I ) = DWORK( I )
         JPVT( I ) = I
   10 CONTINUE
C     
C     Compute factorization and determine RANK using incremental
C     condition estimation.
C     
      RANK = 0
C
   20 CONTINUE
      IF( RANK.LT.MN ) THEN
         I = RANK + 1
C     
C        Determine ith pivot column and swap if necessary.
C       
         PVT = ( I-1 ) + IDAMAX( N-I+1, DWORK( I ), 1 )
C       
         IF( PVT.NE.I ) THEN
            CALL DSWAP( M, A( 1, PVT ), 1, A( 1, I ), 1 )
            ITEMP = JPVT( PVT )
            JPVT( PVT ) = JPVT( I )
            JPVT( I )   = ITEMP
            DWORK( PVT )   = DWORK( I )
            DWORK( N+PVT ) = DWORK( N+I )
         END IF
C       
C        Save A(I,I) and generate elementary reflector H(i).
C       
         IF( I.LT.M ) THEN
            AII = A( I, I )
            CALL DLARFG( M-I+1, A( I, I ), A( I+1, I ), 1, TAU( I ) )
         ELSE
            TAU( M ) = ZERO
         END IF
C       
         IF( RANK.EQ.0 ) THEN
C     
C           Initialize; exit if matrix is zero (RANK = 0).
C           
            SMAX = ABS( A( 1, 1 ) )
            IF ( SMAX.EQ.ZERO ) THEN
               SVAL( 1 ) = ZERO
               SVAL( 2 ) = ZERO
               SVAL( 3 ) = ZERO
               RETURN
            END IF
            SMIN = SMAX
            SMAXPR = SMAX
            SMINPR = SMIN
            C1 = ONE
            C2 = ONE
         ELSE
C     
C           One step of incremental condition estimation.
C           
            CALL DLAIC1( IMIN, RANK, DWORK( ISMIN ), SMIN, A( 1, I ),
     $                   A( I, I ), SMINPR, S1, C1 )
            CALL DLAIC1( IMAX, RANK, DWORK( ISMAX ), SMAX, A( 1, I ),
     $                   A( I, I ), SMAXPR, S2, C2 )
         END IF
C           
         IF( SVLMAX*RCOND.LE.SMAXPR ) THEN
            IF( SVLMAX*RCOND.LE.SMINPR ) THEN
               IF( SMAXPR*RCOND.LE.SMINPR ) THEN
C           
C                 Continue factorization, as rank is at least RANK.
C                 
                  IF( I.LT.N ) THEN
C                 
C                    Apply H(i) to A(i:m,i+1:n) from the left.
C                 
                     AII = A( I, I )
                     A( I, I ) = ONE
                     CALL DLARF( 'Left', M-I+1, N-I, A( I, I ), 1,
     $                           TAU( I ), A( I, I+1 ), LDA, 
     $                           DWORK( 2*N+1 ) )
                     A( I, I ) = AII
                  END IF
C                 
C                 Update partial column norms.
C                 
                  DO 30 J = I + 1, N
                     IF( DWORK( J ).NE.ZERO ) THEN
                        TEMP = ONE - 
     $                            ( ABS( A( I, J ) ) / DWORK( J ) )**2
                        TEMP = MAX( TEMP, ZERO )
                        TEMP2 = ONE + 0.05D0*TEMP*
     $                            ( DWORK( J ) / DWORK( N+J ) )**2
                        IF( TEMP2.EQ.ONE ) THEN
                           IF( M-I.GT.0 ) THEN
                              DWORK( J ) = DNRM2( M-I, A( I+1, J ), 1 )
                              DWORK( N+J ) = DWORK( J )
                           ELSE
                              DWORK( J )   = ZERO
                              DWORK( N+J ) = ZERO
                           END IF
                        ELSE
                           DWORK( J ) = DWORK( J )*SQRT( TEMP )
                        END IF
                     END IF
   30             CONTINUE
C              
                  DO 40 I = 1, RANK
                     DWORK( ISMIN+I-1 ) = S1*DWORK( ISMIN+I-1 )
                     DWORK( ISMAX+I-1 ) = S2*DWORK( ISMAX+I-1 )
   40             CONTINUE
C                 
                  DWORK( ISMIN+RANK ) = C1
                  DWORK( ISMAX+RANK ) = C2
                  SMIN = SMINPR
                  SMAX = SMAXPR
                  RANK = RANK + 1
                  GO TO 20
               END IF
            END IF
         END IF
      END IF
C                 
C     Restore the changed part of the (RANK+1)-th column and set SVAL.
C     
      IF ( RANK.LT.N ) THEN
         IF ( I.LT.M ) THEN
            CALL DSCAL( M-I, -A( I, I )*TAU( I ), A( I+1, I ), 1 )
            A( I, I ) = AII
         END IF
      END IF
      IF ( RANK.EQ.0 ) THEN
         SMIN = ZERO
         SMINPR = ZERO
      END IF
      SVAL( 1 ) = SMAX
      SVAL( 2 ) = SMIN
      SVAL( 3 ) = SMINPR
C
      RETURN
C *** Last line of MB03OY ***
      END
