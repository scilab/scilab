      SUBROUTINE MB02UD( FACT, SIDE, TRANS, JOBP, M, N, ALPHA, RCOND,
     $                   RANK, R, LDR, Q, LDQ, SV, B, LDB, RP, LDRP,
     $                   DWORK, LDWORK, INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 1999.
C
C     PURPOSE
C
C     To compute the minimum norm least squares solution of one of the 
C     following linear systems
C
C        op(R)*X = alpha*B,                                          (1)
C        X*op(R) = alpha*B,                                          (2)
C
C     where alpha is a real scalar, op(R) is either R or its transpose,
C     R', R is an L-by-L real upper triangular matrix, B is an M-by-N 
C     real matrix, and L = M for (1), or L = N for (2). Singular value 
C     decomposition, R = Q*S*P', is used, assuming that R is rank
C     deficient.
C
C     ARGUMENTS
C
C     Mode Parameters
C
C     FACT    CHARACTER*1
C             Specifies whether R has been previously factored or not,
C             as follows:
C             = 'F':  R has been factored and its rank and singular 
C                     value decomposition, R = Q*S*P', are available;
C             = 'N':  R has not been factored and its singular value 
C                     decomposition, R = Q*S*P', should be computed.
C
C     SIDE    CHARACTER*1
C             Specifies whether op(R) appears on the left or right
C             of X as follows:
C             = 'L':  Solve op(R)*X = alpha*B  (op(R) is on the left);
C             = 'R':  Solve X*op(R) = alpha*B  (op(R) is on the right).
C
C     TRANS   CHARACTER*1
C             Specifies the form of op(R) to be used as follows:
C             = 'N':  op(R) = R;
C             = 'T':  op(R) = R';
C             = 'C':  op(R) = R'.
C            
C     JOBP    CHARACTER*1
C             Specifies whether or not the pseudoinverse of R is to be 
C             computed or it is available as follows:
C             = 'P':  Compute pinv(R), if FACT = 'N', or 
C                     use pinv(R),     if FACT = 'F';
C             = 'N':  Do not compute or use pinv(R).
C
C     Input/Output Parameters
C
C     M       (input) INTEGER
C             The number of rows of the matrix B.  M >= 0.
C
C     N       (input) INTEGER
C             The number of columns of the matrix B.  N >= 0.
C
C     ALPHA   (input) DOUBLE PRECISION
C             The scalar alpha. When alpha is zero then B need not be
C             set before entry.
C            
C     RCOND   (input) DOUBLE PRECISION
C             RCOND is used to determine the effective rank of R.
C             Singular values of R satisfying Sv(i) <= RCOND*Sv(1) are
C             treated as zero. If RCOND <= 0, then EPS is used instead,
C             where EPS is the relative machine precision (see LAPACK
C             Library routine DLAMCH).  RCOND <= 1.
C             RCOND is not used if FACT = 'F'.
C
C     RANK    (input or output) INTEGER
C             The rank of matrix R.
C             RANK is an input parameter when FACT = 'F', and an output 
C             parameter when FACT = 'N'.  L >= RANK >= 0.
C
C     R       (input/output) DOUBLE PRECISION array, dimension (LDR,L)
C             On entry, if FACT = 'F', the leading L-by-L part of this
C             array must contain the L-by-L orthogonal matrix P' from 
C             singular value decomposition, R = Q*S*P', of the matrix R;
C             if JOBP = 'P', the first RANK rows of P' are assumed to be 
C             scaled by inv(S(1:RANK,1:RANK)). 
C             On entry, if FACT = 'N', the leading L-by-L upper
C             triangular part of this array must contain the upper 
C             triangular matrix R.
C             On exit, if INFO = 0, the leading L-by-L part of this
C             array contains the L-by-L orthogonal matrix P', with its
C             first RANK rows scaled by inv(S(1:RANK,1:RANK)), when
C             JOBP = 'P'. 
C
C     LDR     INTEGER
C             The leading dimension of array R.  LDR >= MAX(1,L).
C
C     Q       (input or output) DOUBLE PRECISION array, dimension 
C             (LDQ,L)
C             On entry, if FACT = 'F', the leading L-by-L part of this
C             array must contain the L-by-L orthogonal matrix Q from 
C             singular value decomposition, R = Q*S*P', of the matrix R.
C             If FACT = 'N', this array need not be set on entry, and 
C             on exit, if INFO = 0, the leading L-by-L part of this
C             array contains the orthogonal matrix Q.
C
C     LDQ     INTEGER
C             The leading dimension of array Q.  LDQ >= MAX(1,L).
C
C     SV      (input or output) DOUBLE PRECISION array, dimension (L)
C             On entry, if FACT = 'F', the first RANK entries of this
C             array must contain the reciprocal of the largest RANK
C             singular values of the matrix R, and the last L-RANK
C             entries of this array must contain the remaining singular
C             values of R sorted in descending order.
C             If FACT = 'N', this array need not be set on input, and 
C             on exit, if INFO = 0, the first RANK entries of this array
C             contain the reciprocal of the largest RANK singular values
C             of the matrix R, and the last L-RANK entries of this array
C             contain the remaining singular values of R sorted in
C             descending order.
C
C     B       (input/output) DOUBLE PRECISION array, dimension (LDB,N)
C             On entry, if ALPHA <> 0, the leading M-by-N part of this
C             array must contain the matrix B.
C             On exit, if INFO = 0 and RANK > 0, the leading M-by-N part
C             of this array contains the M-by-N solution matrix X.
C
C     LDB     INTEGER
C             The leading dimension of array B.  LDB >= MAX(1,M).
C
C     RP      (input or output) DOUBLE PRECISION array, dimension 
C             (LDRP,L)
C             On entry, if FACT = 'F', JOBP = 'P', and RANK > 0, the
C             leading L-by-L part of this array must contain the L-by-L
C             matrix pinv(R), the Moore-Penrose pseudoinverse of R.
C             On exit, if FACT = 'N', JOBP = 'P', and RANK > 0, the
C             leading L-by-L part of this array contains the L-by-L
C             matrix pinv(R), the Moore-Penrose pseudoinverse of R.
C             If JOBP = 'N', this array is not referenced.
C
C     LDRP    INTEGER
C             The leading dimension of array RP.
C             LDRP >= MAX(1,L), if JOBP = 'P'.
C             LDRP >= 1,        if JOBP = 'N'.
C
C     Workspace
C
C     DWORK   DOUBLE PRECISION array, dimension (LDWORK)
C             On exit, if INFO = 0, DWORK(1) returns the optimal LDWORK;
C             if INFO = i, 1 <= i <= L, then DWORK(2:L) contain the
C             unconverged superdiagonal elements of an upper bidiagonal
C             matrix D whose diagonal is in SV (not necessarily sorted).
C             D satisfies R = Q*D*P', so it has the same singular
C             values as R, and singular vectors related by Q and P'.
C
C     LDWORK  INTEGER
C             The length of the array DWORK. 
C             LDWORK >= MAX(1,L),   if FACT = 'F';
C             LDWORK >= MAX(1,5*L), if FACT = 'N'.
C             For optimum performance LDWORK should be larger than
C             MAX(1,L,M*N),   if FACT = 'F';
C             MAX(1,5*L,M*N), if FACT = 'N'.
C
C     Error Indicator
C
C     INFO    INTEGER
C             = 0:  successful exit;
C             < 0:  if INFO = -i, the i-th argument had an illegal
C                   value;
C             > 0:  if INFO = i, i = 1:L, the SVD algorithm has failed 
C                   to converge. In this case INFO specifies how many 
C                   superdiagonals did not converge (see the description
C                   of DWORK); this failure is not likely to occur.
C
C     METHOD
C
C     The L-by-L upper triangular matrix R is factored as  R = Q*S*P',
C     if FACT = 'N', using SLICOT Library routine MB03UD, where Q and P
C     are L-by-L orthogonal matrices and S is an L-by-L diagonal matrix
C     with non-negative diagonal elements, SV(1), SV(2), ..., SV(L),
C     ordered decreasingly. Then, the effective rank of R is estimated,
C     and matrix (or matrix-vector) products and scalings are used to 
C     compute X. If FACT = 'F', only matrix (or matrix-vector) products
C     and scalings are performed.
C
C     FURTHER COMMENTS
C
C     Option JOBP = 'P' should be used only if the pseudoinverse is
C     really needed. Usually, it is possible to avoid the use of
C     pseudoinverse, by computing least squares solutions.
C     The routine uses BLAS 3 calculations if LDWORK >= M*N, and BLAS 2 
C     calculations, otherwise. No advantage of any additional workspace
C     larger than L is taken for matrix products, but the routine can
C     be called repeatedly for chunks of columns of B, if LDWORK < M*N.
C
C     CONTRIBUTOR
C
C     V. Sima, Research Institute of Informatics, Bucharest, Oct. 1999.
C
C     REVISIONS
C
C     V. Sima, Feb. 2000.   
C
C     KEYWORDS
C
C     Bidiagonalization, orthogonal transformation, singular value
C     decomposition, singular values, triangular form.
C
C    ******************************************************************
C
C     .. Parameters ..
      DOUBLE PRECISION  ONE, ZERO
      PARAMETER         ( ONE = 1.0D0, ZERO = 0.0D0 )
C     .. Scalar Arguments ..
      CHARACTER         FACT, JOBP, SIDE, TRANS
      INTEGER           INFO, LDB, LDQ, LDR, LDRP, LDWORK, M, N, RANK
      DOUBLE PRECISION  ALPHA, RCOND
C     .. Array Arguments ..
      DOUBLE PRECISION  B(LDB,*), DWORK(*), Q(LDQ,*), R(LDR,*), 
     $                  RP(LDRP,*), SV(*)
C     .. Local Scalars ..
      LOGICAL           LEFT, NFCT, PINV, TRAN
      CHARACTER*1       NTRAN
      INTEGER           I, L, MAXWRK, MINWRK, MN
      DOUBLE PRECISION  TOLL
C     .. External Functions ..
      LOGICAL           LSAME
      INTEGER           ILAENV
      DOUBLE PRECISION  DLAMCH
      EXTERNAL          DLAMCH, ILAENV, LSAME
C     .. External Subroutines ..
      EXTERNAL          DCOPY, DGEMM, DGEMV, DLACPY, DLASET, MB01SD, 
     $                  MB03UD, XERBLA
C     .. Intrinsic Functions ..
      INTRINSIC         MAX, MIN
C     .. Executable Statements ..
C
C     Check the input scalar arguments.
C
      INFO = 0
      NFCT = LSAME( FACT,  'N' )
      LEFT = LSAME( SIDE,  'L' )
      PINV = LSAME( JOBP,  'P' )
      TRAN = LSAME( TRANS, 'T' ) .OR. LSAME( TRANS, 'C' )
      IF( LEFT ) THEN
         L = M
      ELSE
         L = N
      END IF
      MN = M*N
      IF( .NOT.NFCT .AND. .NOT.LSAME( FACT, 'F' ) ) THEN
         INFO = -1
      ELSE IF( .NOT.LEFT .AND. .NOT.LSAME( SIDE,  'R' ) ) THEN
         INFO = -2
      ELSE IF( .NOT.TRAN .AND. .NOT.LSAME( TRANS, 'N' ) ) THEN
         INFO = -3
      ELSE IF( .NOT.PINV .AND. .NOT.LSAME( JOBP,  'N' ) ) THEN
         INFO = -4
      ELSE IF( M.LT.0 ) THEN
         INFO = -5
      ELSE IF( N.LT.0 ) THEN
         INFO = -6
      ELSE IF( NFCT .AND. RCOND.GT.ONE ) THEN
         INFO = -8
      ELSE IF( .NOT.NFCT .AND. ( RANK.LT.ZERO .OR. RANK.GT.L ) ) THEN
         INFO = -9
      ELSE IF( LDR.LT.MAX( 1, L ) ) THEN
         INFO = -11
      ELSE IF( LDQ.LT.MAX( 1, L ) ) THEN
         INFO = -13
      ELSE IF( LDB.LT.MAX( 1, M ) ) THEN
         INFO = -16
      ELSE IF( LDRP.LT.1 .OR. ( PINV .AND. LDRP.LT.L ) ) THEN
         INFO = -18
      END IF
C
C     Compute workspace
C     (Note: Comments in the code beginning "Workspace:" describe the
C     minimal amount of workspace needed at that point in the code,
C     as well as the preferred amount for good performance.
C     NB refers to the optimal block size for the immediately following 
C     subroutine, as returned by ILAENV.)
C
      MINWRK = 1
      IF( INFO.EQ.0 .AND. LDWORK.GE.1 .AND. L.GT.0 ) THEN
         MINWRK = MAX( 1, L )
         MAXWRK = MAX( MINWRK, MN )
         IF( NFCT ) THEN
            MAXWRK = MAX( MAXWRK, 3*L+2*L*
     $                    ILAENV( 1, 'DGEBRD', ' ', L, L, -1, -1 ) )
            MAXWRK = MAX( MAXWRK, 3*L+L*
     $                    ILAENV( 1, 'DORGBR', 'Q', L, L, L, -1 ) )
            MAXWRK = MAX( MAXWRK, 3*L+L*
     $                    ILAENV( 1, 'DORGBR', 'P', L, L, L, -1 ) )
            MINWRK = MAX( 1, 5*L )
            MAXWRK = MAX( MAXWRK, MINWRK )
         END IF
      END IF
C
      IF( LDWORK.LT.MINWRK ) THEN
         INFO = -20
      END IF
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'MB02UD', -INFO )
         RETURN
      END IF
C
C     Quick return if possible.
C
      IF( L.EQ.0 ) THEN
         IF( NFCT )
     $      RANK = 0
         DWORK(1) = ONE
         RETURN
      END IF
C
      IF( NFCT ) THEN
C
C        Compute the SVD of R, R = Q*S*P'.
C        Matrix Q is computed in the array Q, and P' overwrites R.
C        Workspace: need   5*L;
C                   prefer larger.
C
         CALL MB03UD( 'Vectors', 'Vectors', L, R, LDR, Q, LDQ, SV,
     $                DWORK, LDWORK, INFO )
         IF ( INFO.NE.0 )
     $      RETURN
C
C        Use the default tolerance, if required.
C
         TOLL = RCOND
         IF( TOLL.LE.ZERO ) 
     $      TOLL = DLAMCH( 'Precision' )
         TOLL = MAX( TOLL*SV(1), DLAMCH( 'Safe minimum' ) )
C
C        Estimate the rank of R.
C
         DO 10 I = 1, L
            IF ( TOLL.GT.SV(I) )
     $         GO TO 20
   10    CONTINUE
C
         I = L + 1
   20    CONTINUE
         RANK = I - 1
C
         DO 30 I = 1, RANK
            SV(I) = ONE / SV(I)
   30    CONTINUE
C
         IF( PINV .AND. RANK.GT.0 ) THEN
C
C           Compute  pinv(S)'*P'  in R.
C
            CALL MB01SD( 'Row scaling', RANK, L, R, LDR, SV, SV )
C
C           Compute  pinv(R) = P*pinv(S)*Q'  in  RP.
C
            CALL DGEMM( 'Transpose', 'Transpose', L, L, RANK, ONE, R,
     $                  LDR, Q, LDQ, ZERO, RP, LDRP )
         END IF
      END IF
C
C     Return if min(M,N) = 0 or RANK = 0.
C
      IF( MIN( M, N ).EQ.0 .OR. RANK.EQ.0 ) THEN
         DWORK(1) = MAXWRK
         RETURN
      END IF
C
C     Set X = 0 if alpha = 0.
C
      IF( ALPHA.EQ.ZERO ) THEN
         CALL DLASET( 'Full', M, N, ZERO, ZERO, B, LDB )            
         DWORK(1) = MAXWRK
         RETURN
      END IF
C
      IF( PINV ) THEN
C
         IF( LEFT ) THEN
C
C           Compute  alpha*op(pinv(R))*B  in workspace and save it in B.
C           Workspace:  need   M   (BLAS 2); 
C                       prefer M*N (BLAS 3). 
C
            IF( LDWORK.GE.MN ) THEN
               CALL DGEMM( TRANS, 'NoTranspose', M, N, M, ALPHA,
     $                     RP, LDRP, B, LDB, ZERO, DWORK, M )
               CALL DLACPY( 'Full', M, N, DWORK, M, B, LDB )
            ELSE
C
               DO 40 I = 1, N
                  CALL DGEMV( TRANS, M, M, ALPHA, RP, LDRP, B(1,I), 1,
     $                        ZERO, DWORK, 1 )
                  CALL DCOPY( M, DWORK, 1, B(1,I), 1 )
   40          CONTINUE
C
            END IF
         ELSE
C
C           Compute  alpha*B*op(pinv(R))  in workspace and save it in B.
C           Workspace:  need   N   (BLAS 2); 
C                       prefer M*N (BLAS 3). 
C
            IF( LDWORK.GE.MN ) THEN
               CALL DGEMM( 'NoTranspose', TRANS, M, N, N, ALPHA, B, LDB,
     $                     RP, LDRP, ZERO, DWORK, M )
               CALL DLACPY( 'Full', M, N, DWORK, M, B, LDB )
            ELSE
C
               IF( TRAN ) THEN
                  NTRAN = 'N'
               ELSE
                  NTRAN = 'T'
               END IF
C
               DO 50 I = 1, M
                  CALL DGEMV( NTRAN, N, N, ALPHA, RP, LDRP, B(I,1), LDB,
     $                        ZERO, DWORK, 1 )
                  CALL DCOPY( N, DWORK, 1, B(I,1), LDB )
   50          CONTINUE
C
            END IF
         END IF
C
      ELSE
C
         IF( LEFT ) THEN
C
C           Compute  alpha*P*pinv(S)*Q'*B  or  alpha*Q*pinv(S)'*P'*B.
C           Workspace:  need   M   (BLAS 2); 
C                       prefer M*N (BLAS 3). 
C
            IF( LDWORK.GE.MN ) THEN
               IF( TRAN ) THEN
C
C                 Compute  alpha*P'*B  in workspace.
C
                  CALL DGEMM( 'NoTranspose', 'NoTranspose', M, N, M, 
     $                        ALPHA, R, LDR, B, LDB, ZERO, DWORK, M )
C
C                 Compute  alpha*pinv(S)'*P'*B.
C
                  CALL MB01SD( 'Row scaling', RANK, N, DWORK, M, SV, 
     $                         SV )
C
C                 Compute  alpha*Q*pinv(S)'*P'*B.
C
                  CALL DGEMM( 'NoTranspose', 'NoTranspose', M, N, RANK,
     $                        ONE, Q, LDQ, DWORK, M, ZERO, B, LDB )
               ELSE
C
C                 Compute  alpha*Q'*B  in workspace.
C
                  CALL DGEMM( 'Transpose', 'NoTranspose', M, N, M,
     $                        ALPHA, Q, LDQ, B, LDB, ZERO, DWORK, M )
C
C                 Compute  alpha*pinv(S)*Q'*B.
C
                  CALL MB01SD( 'Row scaling', RANK, N, DWORK, M, SV, 
     $                         SV )
C
C                 Compute  alpha*P*pinv(S)*Q'*B.
C
                  CALL DGEMM( 'Transpose', 'NoTranspose', M, N, RANK,
     $                        ONE, R, LDR, DWORK, M, ZERO, B, LDB )
               END IF
            ELSE
               IF( TRAN ) THEN
C
C                 Compute  alpha*P'*B  in B using workspace.
C
                  DO 60 I = 1, N
                     CALL DGEMV( 'NoTranspose', M, M, ALPHA, R, LDR,
     $                           B(1,I), 1, ZERO, DWORK, 1 )
                     CALL DCOPY( M, DWORK, 1, B(1,I), 1 )
   60             CONTINUE
C
C                 Compute  alpha*pinv(S)'*P'*B.
C
                  CALL MB01SD( 'Row scaling', RANK, N, B, LDB, SV, SV )
C
C                 Compute  alpha*Q*pinv(S)'*P'*B  in B using workspace.
C
                  DO 70 I = 1, N
                     CALL DGEMV( 'NoTranspose', M, RANK, ONE, Q, LDQ,
     $                           B(1,I), 1, ZERO, DWORK, 1 )
                     CALL DCOPY( M, DWORK, 1, B(1,I), 1 )
   70             CONTINUE
               ELSE
C
C                 Compute  alpha*Q'*B  in B using workspace.
C
                  DO 80 I = 1, N
                     CALL DGEMV( 'Transpose', M, M, ALPHA, Q, LDQ,
     $                           B(1,I), 1, ZERO, DWORK, 1 )
                     CALL DCOPY( M, DWORK, 1, B(1,I), 1 )
   80             CONTINUE
C
C                 Compute  alpha*pinv(S)*Q'*B.
C
                  CALL MB01SD( 'Row scaling', RANK, N, B, LDB, SV, SV )
C
C                 Compute  alpha*P*pinv(S)*Q'*B  in B using workspace.
C
                  DO 90 I = 1, N
                     CALL DGEMV( 'Transpose', RANK, M, ONE, R, LDR,
     $                           B(1,I), 1, ZERO, DWORK, 1 )
                     CALL DCOPY( M, DWORK, 1, B(1,I), 1 )
   90             CONTINUE
               END IF
            END IF
         ELSE
C
C           Compute  alpha*B*P*pinv(S)*Q'  or  alpha*B*Q*pinv(S)'*P'.
C           Workspace:  need   N   (BLAS 2); 
C                       prefer M*N (BLAS 3). 
C
            IF( LDWORK.GE.MN ) THEN
               IF( TRAN ) THEN
C
C                 Compute  alpha*B*Q  in workspace.
C
                  CALL DGEMM( 'NoTranspose', 'NoTranspose', M, N, N,
     $                        ALPHA, B, LDB, Q, LDQ, ZERO, DWORK, M )
C
C                 Compute  alpha*B*Q*pinv(S)'.
C
                  CALL MB01SD( 'Column scaling', M, RANK, DWORK, M, SV,
     $                         SV )
C
C                 Compute  alpha*B*Q*pinv(S)'*P' in B.
C
                  CALL DGEMM( 'NoTranspose', 'NoTranspose', M, N, RANK,
     $                        ONE, DWORK, M, R, LDR, ZERO, B, LDB )
               ELSE
C
C                 Compute  alpha*B*P  in workspace.
C
                  CALL DGEMM( 'NoTranspose', 'Transpose', M, N, N,
     $                        ALPHA, B, LDB, R, LDR, ZERO, DWORK, M )
C
C                 Compute  alpha*B*P*pinv(S).
C
                  CALL MB01SD( 'Column scaling', M, RANK, DWORK, M, SV,
     $                         SV )
C
C                 Compute  alpha*B*P*pinv(S)*Q' in B.
C
                  CALL DGEMM( 'NoTranspose', 'Transpose', M, N, RANK,
     $                        ONE, DWORK, M, Q, LDQ, ZERO, B, LDB )
               END IF
            ELSE
               IF( TRAN ) THEN
C
C                 Compute  alpha*B*Q  in B using workspace.
C
                  DO 100 I = 1, M
                     CALL DGEMV( 'Transpose', N, N, ALPHA, Q, LDQ,
     $                           B(I,1), LDB, ZERO, DWORK, 1 )
                     CALL DCOPY( N, DWORK, 1, B(I,1), LDB )
  100             CONTINUE
C
C                 Compute  alpha*B*Q*pinv(S)'.
C
                  CALL MB01SD( 'Column scaling', M, RANK, B, LDB, SV,
     $                         SV )
C
C                 Compute  alpha*B*Q*pinv(S)'*P' in B using workspace.
C
                  DO 110 I = 1, M
                     CALL DGEMV( 'Transpose', RANK, N, ONE, R, LDR,
     $                           B(I,1), LDB, ZERO, DWORK, 1 )
                     CALL DCOPY( N, DWORK, 1, B(I,1), LDB )
  110             CONTINUE
C
               ELSE
C
C                 Compute  alpha*B*P  in B using workspace.
C
                  DO 120 I = 1, M
                     CALL DGEMV( 'NoTranspose', N, N, ALPHA, R, LDR,
     $                           B(I,1), LDB, ZERO, DWORK, 1 )
                     CALL DCOPY( N, DWORK, 1, B(I,1), LDB )
  120             CONTINUE
C
C                 Compute  alpha*B*P*pinv(S).
C
                  CALL MB01SD( 'Column scaling', M, RANK, B, LDB, SV,
     $                         SV )
C
C                 Compute  alpha*B*P*pinv(S)*Q' in B using workspace.
C
                  DO 130 I = 1, M
                     CALL DGEMV( 'NoTranspose', N, RANK, ONE, Q, LDQ,
     $                           B(I,1), LDB, ZERO, DWORK, 1 )
                     CALL DCOPY( N, DWORK, 1, B(I,1), LDB )
  130             CONTINUE
               END IF
            END IF
         END IF
      END IF
C
C     Return optimal workspace in DWORK(1).
C
      DWORK(1) = MAXWRK
C
      RETURN
C *** Last line of MB02UD ***
      END
