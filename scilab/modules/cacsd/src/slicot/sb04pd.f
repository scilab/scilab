      SUBROUTINE SB04PD( DICO, FACTA, FACTB, TRANA, TRANB, ISGN, M, N,
     $                   A, LDA, U, LDU, B, LDB, V, LDV, C, LDC, SCALE,
     $                   DWORK, LDWORK, INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 2000.
C
C     PURPOSE
C
C     To solve for X either the real continuous-time Sylvester equation
C
C        op(A)*X + ISGN*X*op(B) = scale*C,                           (1)
C
C     or the real discrete-time Sylvester equation
C
C        op(A)*X*op(B) + ISGN*X = scale*C,                           (2)
C
C     where op(M) = M or M**T, and ISGN = 1 or -1. A is M-by-M and
C     B is N-by-N; the right hand side C and the solution X are M-by-N;
C     and scale is an output scale factor, set less than or equal to 1
C     to avoid overflow in X. The solution matrix X is overwritten
C     onto C.
C
C     If A and/or B are not (upper) quasi-triangular, that is, block
C     upper triangular with 1-by-1 and 2-by-2 diagonal blocks, they are
C     reduced to Schur canonical form, that is, quasi-triangular with
C     each 2-by-2 diagonal block having its diagonal elements equal and
C     its off-diagonal elements of opposite sign.
C
C     ARGUMENTS
C
C     Mode Parameters
C
C     DICO    CHARACTER*1
C             Specifies the equation from which X is to be determined
C             as follows:
C             = 'C':  Equation (1), continuous-time case;
C             = 'D':  Equation (2), discrete-time case.
C
C     FACTA   CHARACTER*1
C             Specifies whether or not the real Schur factorization
C             of the matrix A is supplied on entry, as follows:
C             = 'F':  On entry, A and U contain the factors from the
C                     real Schur factorization of the matrix A;
C             = 'N':  The Schur factorization of A will be computed
C                     and the factors will be stored in A and U;
C             = 'S':  The matrix A is quasi-triangular (or Schur).
C
C     FACTB   CHARACTER*1
C             Specifies whether or not the real Schur factorization
C             of the matrix B is supplied on entry, as follows:
C             = 'F':  On entry, B and V contain the factors from the
C                     real Schur factorization of the matrix B;
C             = 'N':  The Schur factorization of B will be computed
C                     and the factors will be stored in B and V;
C             = 'S':  The matrix B is quasi-triangular (or Schur).
C
C     TRANA   CHARACTER*1
C             Specifies the form of op(A) to be used, as follows:
C             = 'N':  op(A) = A    (No transpose);
C             = 'T':  op(A) = A**T (Transpose);
C             = 'C':  op(A) = A**T (Conjugate transpose = Transpose).
C
C     TRANB   CHARACTER*1
C             Specifies the form of op(B) to be used, as follows:
C             = 'N':  op(B) = B    (No transpose);
C             = 'T':  op(B) = B**T (Transpose);
C             = 'C':  op(B) = B**T (Conjugate transpose = Transpose).
C
C     ISGN    INTEGER
C             Specifies the sign of the equation as described before.
C             ISGN may only be 1 or -1.
C
C     Input/Output Parameters
C
C     M       (input) INTEGER
C             The order of the matrix A, and the number of rows in the
C             matrices X and C.  M >= 0.
C
C     N       (input) INTEGER
C             The order of the matrix B, and the number of columns in
C             the matrices X and C.  N >= 0.
C
C     A       (input or input/output) DOUBLE PRECISION array, 
C             dimension (LDA,M)
C             On entry, the leading M-by-M part of this array must
C             contain the matrix A. If FACTA = 'S', then A contains
C             a quasi-triangular matrix, and if FACTA = 'F', then A
C             is in Schur canonical form; the elements below the upper
C             Hessenberg part of the array A are not referenced.
C             On exit, if FACTA = 'N', and INFO = 0 or INFO >= M+1, the
C             leading M-by-M upper Hessenberg part of this array 
C             contains the upper quasi-triangular matrix in Schur
C             canonical form from the Schur factorization of A. The 
C             contents of array A is not modified if FACTA = 'F' or 'S'.
C
C     LDA     INTEGER
C             The leading dimension of array A.  LDA >= MAX(1,M).
C
C     U       (input or output) DOUBLE PRECISION array, dimension
C             (LDU,M)
C             If FACTA = 'F', then U is an input argument and on entry
C             the leading M-by-M part of this array must contain the
C             orthogonal matrix U of the real Schur factorization of A.
C             If FACTA = 'N', then U is an output argument and on exit,
C             if INFO = 0 or INFO >= M+1, it contains the orthogonal
C             M-by-M matrix from the real Schur factorization of A.
C             If FACTA = 'S', the array U is not referenced.
C
C     LDU     INTEGER
C             The leading dimension of array U.
C             LDU >= MAX(1,M), if FACTA = 'F' or 'N';
C             LDU >= 1,        if FACTA = 'S'.
C
C     B       (input or input/output) DOUBLE PRECISION array, 
C             dimension (LDB,N)
C             On entry, the leading N-by-N part of this array must
C             contain the matrix B. If FACTB = 'S', then B contains
C             a quasi-triangular matrix, and if FACTB = 'F', then B
C             is in Schur canonical form; the elements below the upper
C             Hessenberg part of the array B are not referenced.
C             On exit, if FACTB = 'N', and INFO = 0 or INFO = M+N+1,
C             the leading N-by-N upper Hessenberg part of this array 
C             contains the upper quasi-triangular matrix in Schur
C             canonical form from the Schur factorization of B. The 
C             contents of array B is not modified if FACTB = 'F' or 'S'.
C
C     LDB     (input) INTEGER
C             The leading dimension of the array B.  LDB >= max(1,N).
C
C     V       (input or output) DOUBLE PRECISION array, dimension
C             (LDV,N)
C             If FACTB = 'F', then V is an input argument and on entry
C             the leading N-by-N part of this array must contain the
C             orthogonal matrix V of the real Schur factorization of B.
C             If FACTB = 'N', then V is an output argument and on exit,
C             if INFO = 0 or INFO = M+N+1, it contains the orthogonal
C             N-by-N matrix from the real Schur factorization of B.
C             If FACTB = 'S', the array V is not referenced.
C
C     LDV     INTEGER
C             The leading dimension of array V.
C             LDV >= MAX(1,N), if FACTB = 'F' or 'N';
C             LDV >= 1,        if FACTB = 'S'.
C
C     C       (input/output) DOUBLE PRECISION array, dimension (LDC,N)
C             On entry, the leading M-by-N part of this array must  
C             contain the right hand side matrix C.
C             On exit, if INFO = 0 or INFO = M+N+1, the leading M-by-N
C             part of this array contains the solution matrix X.
C
C     LDC     INTEGER
C             The leading dimension of array C.  LDC >= MAX(1,M).
C
C     SCALE   (output) DOUBLE PRECISION
C             The scale factor, scale, set less than or equal to 1 to
C             prevent the solution overflowing.
C
C     Workspace
C
C     DWORK   DOUBLE PRECISION array, dimension (LDWORK)
C             On exit, if INFO = 0 or M+N+1, then: DWORK(1) returns the
C             optimal value of LDWORK; if FACTA = 'N', DWORK(1+i) and
C             DWORK(1+M+i), i = 1,...,M, contain the real and imaginary
C             parts, respectively, of the eigenvalues of A; and, if
C             FACTB = 'N', DWORK(1+f+j) and DWORK(1+f+N+j), j = 1,...,N,
C             with f = 2*M if FACTA = 'N', and f = 0, otherwise, contain
C             the real and imaginary parts, respectively, of the
C             eigenvalues of B.
C
C     LDWORK  INTEGER
C             The length of the array DWORK.
C             LDWORK >= MAX( 1, a+MAX( c, b+d, b+e ) ), 
C             where a = 1+2*M, if FACTA =  'N',
C                   a = 0,     if FACTA <> 'N',
C                   b = 2*N,   if FACTB =  'N', FACTA =  'N',
C                   b = 1+2*N, if FACTB =  'N', FACTA <> 'N',
C                   b = 0,     if FACTB <> 'N',
C                   c = 3*M,   if FACTA =  'N',
C                   c = M,     if FACTA =  'F',
C                   c = 0,     if FACTA =  'S',
C                   d = 3*N,   if FACTB =  'N',
C                   d = N,     if FACTB =  'F',
C                   d = 0,     if FACTB =  'S',
C                   e = M,     if DICO  =  'C', FACTA <> 'S',
C                   e = 0,     if DICO  =  'C', FACTA =  'S',
C                   e = 2*M,   if DICO  =  'D'.
C             An upper bound is  
C             LDWORK = 1+2*M+MAX( 3*M, 5*N, 2*N+2*M ). 
C             For good performance, LDWORK should be larger, e.g.,
C             LDWORK = 1+2*M+MAX( 3*M, 5*N, 2*N+2*M, 2*N+M*N ). 
C
C     Error Indicator
C
C     INFO    INTEGER
C             = 0:  successful exit;
C             < 0:  if INFO = -i, the i-th argument had an illegal
C                   value;
C             = i:  if INFO = i, i = 1,...,M, the QR algorithm failed
C                   to compute all the eigenvalues of the matrix A
C                   (see LAPACK Library routine DGEES); the elements
C                   2+i:1+M and 2+i+M:1+2*M of DWORK contain the real
C                   and imaginary parts, respectively, of the
C                   eigenvalues of A which have converged, and the
C                   array A contains the partially converged Schur form;
C             = M+j:  if INFO = M+j, j = 1,...,N, the QR algorithm 
C                   failed to compute all the eigenvalues of the matrix
C                   B (see LAPACK Library routine DGEES); the elements
C                   2+f+j:1+f+N and 2+f+j+N:1+f+2*N of DWORK contain the
C                   real and imaginary parts, respectively, of the
C                   eigenvalues of B which have converged, and the
C                   array B contains the partially converged Schur form;
C                   as defined for the parameter DWORK, 
C                   f = 2*M, if FACTA =  'N',
C                   f = 0,   if FACTA <> 'N';
C             = M+N+1:  if DICO = 'C', and the matrices A and -ISGN*B
C                   have common or very close eigenvalues, or
C                   if DICO = 'D', and the matrices A and -ISGN*B have 
C                   almost reciprocal eigenvalues (that is, if lambda(i)
C                   and mu(j) are eigenvalues of A and -ISGN*B, then
C                   lambda(i) = 1/mu(j) for some i and j);
C                   perturbed values were used to solve the equation
C                   (but the matrices A and B are unchanged).
C
C     METHOD
C
C     An extension and refinement of the algorithms in [1,2] is used.
C     If the matrices A and/or B are not quasi-triangular (see PURPOSE),
C     they are reduced to Schur canonical form
C
C        A = U*S*U',  B = V*T*V',
C
C     where U, V are orthogonal, and S, T are block upper triangular 
C     with 1-by-1 and 2-by-2 blocks on their diagonal. The right hand
C     side matrix C is updated accordingly,
C
C        C = U'*C*V;
C
C     then, the solution matrix X of the "reduced" Sylvester equation
C     (with A and B in (1) or (2) replaced by S and T, respectively),
C     is computed column-wise via a back substitution scheme. A set of
C     equivalent linear algebraic systems of equations of order at most
C     four are formed and solved using Gaussian elimination with
C     complete pivoting. Finally, the solution X of the original 
C     equation is obtained from the updating formula
C
C        X = U*X*V'.
C
C     If A and/or B are already quasi-triangular (or in Schur form), the
C     initial factorizations and the corresponding updating steps are 
C     omitted.
C
C     REFERENCES
C
C     [1] Bartels, R.H. and Stewart, G.W.  T
C         Solution of the matrix equation A X + XB = C.
C         Comm. A.C.M., 15, pp. 820-826, 1972.
C
C     [2] Anderson, E., Bai, Z., Bischof, C., Demmel, J., Dongarra, J.,
C         Du Croz, J., Greenbaum, A., Hammarling, S., McKenney, A.,
C         Ostrouchov, S., and Sorensen, D.
C         LAPACK Users' Guide: Second Edition.
C         SIAM, Philadelphia, 1995.
C
C     NUMERICAL ASPECTS
C
C     The algorithm is stable and reliable, since orthogonal
C     transformations and Gaussian elimination with complete pivoting 
C     are used. If INFO = M+N+1, the Sylvester equation is numerically
C     singular.
C
C     CONTRIBUTORS
C
C     D. Sima, University of Bucharest, April 2000.
C     V. Sima, Research Institute for Informatics, Bucharest, Apr. 2000.
C
C     REVISIONS
C
C     -
C
C     KEYWORDS
C
C     Matrix algebra, orthogonal transformation, real Schur form, 
C     Sylvester equation.
C
C     ******************************************************************
C
C     .. Parameters ..
      DOUBLE PRECISION   ZERO, ONE
      PARAMETER          ( ZERO = 0.0D+0, ONE = 1.0D+0 )
C     ..
C     .. Scalar Arguments ..
      CHARACTER          DICO, FACTA, FACTB, TRANA, TRANB
      INTEGER            INFO, ISGN, LDA, LDB, LDC, LDU, LDV, LDWORK, M,
     $                   N
      DOUBLE PRECISION   SCALE
C     ..
C     .. Array Arguments ..
      DOUBLE PRECISION   A( LDA, * ), B( LDB, * ), C( LDC, * ),
     $                   DWORK( * ),  U( LDU, * ), V( LDV, * )
C     ..
C     .. Local Scalars ..
      LOGICAL            BLAS3A, BLAS3B, BLOCKA, BLOCKB, CONT, NOFACA,
     $                   NOFACB, NOTRNA, NOTRNB, SCHURA, SCHURB
      INTEGER            AVAILW, BL, CHUNKA, CHUNKB, I, IA, IB, IERR, J,
     $                   JWORK, MAXWRK, MINWRK, SDIM
C     ..
C     .. Local Arrays ..
      LOGICAL            BWORK( 1 )
C     ..
C     .. External Functions ..
      LOGICAL            LSAME, SELECT1
      EXTERNAL           LSAME, SELECT1
C     ..
C     .. External Subroutines ..
      EXTERNAL           DCOPY, DGEES, DGEMM, DGEMV, DLACPY, DTRSYL,
     $                   SB04PY, XERBLA
C     ..
C     .. Intrinsic Functions ..
      INTRINSIC          DBLE, INT, MAX, MIN
C     ..
C     .. Executable Statements ..
C
C     Decode and Test input parameters
C
      CONT   = LSAME( DICO,  'C' )
      NOFACA = LSAME( FACTA, 'N' )
      NOFACB = LSAME( FACTB, 'N' )
      SCHURA = LSAME( FACTA, 'S' )
      SCHURB = LSAME( FACTB, 'S' )
      NOTRNA = LSAME( TRANA, 'N' )
      NOTRNB = LSAME( TRANB, 'N' )
C
      INFO = 0
      IF( .NOT.CONT .AND. .NOT.LSAME( DICO, 'D' ) ) THEN
         INFO = -1
      ELSE IF( .NOT.NOFACA .AND. .NOT.LSAME( FACTA, 'F' ) .AND. 
     $         .NOT.SCHURA ) THEN
         INFO = -2
      ELSE IF( .NOT.NOFACB .AND. .NOT.LSAME( FACTB, 'F' ) .AND. 
     $         .NOT.SCHURB ) THEN
         INFO = -3
      ELSE IF( .NOT.NOTRNA .AND. .NOT.LSAME( TRANA, 'T' ) .AND.
     $         .NOT.LSAME( TRANA, 'C' ) ) THEN
         INFO = -4
      ELSE IF( .NOT.NOTRNB .AND. .NOT.LSAME( TRANB, 'T' ) .AND.
     $         .NOT.LSAME( TRANB, 'C' ) ) THEN
         INFO = -5
      ELSE IF( ISGN.NE.1 .AND. ISGN.NE.-1 ) THEN
         INFO = -6
      ELSE IF( M.LT.0 ) THEN
         INFO = -7
      ELSE IF( N.LT.0 ) THEN
         INFO = -8
      ELSE IF( LDA.LT.MAX( 1, M ) ) THEN
         INFO = -10
      ELSE IF( LDU.LT.1 .OR. ( .NOT.SCHURA .AND. LDU.LT.M ) ) THEN
         INFO = -12
      ELSE IF( LDB.LT.MAX( 1, N ) ) THEN
         INFO = -14
      ELSE IF( LDV.LT.1 .OR. ( .NOT.SCHURB .AND. LDV.LT.N ) ) THEN
         INFO = -16
      ELSE IF( LDC.LT.MAX( 1, M ) ) THEN
         INFO = -18
      ELSE 
         IF ( NOFACA ) THEN
            IA = 1 + 2*M 
            MINWRK = 3*M 
         ELSE
            IA = 0 
         END IF
         IF ( SCHURA ) THEN
            MINWRK = 0
         ELSE IF ( .NOT.NOFACA ) THEN
            MINWRK = M
         END IF
         IB = 0
         IF ( NOFACB ) THEN
            IB = 2*N
            IF ( .NOT.NOFACA )
     $         IB = IB + 1
            MINWRK = MAX( MINWRK, IB + 3*N ) 
         ELSE IF ( .NOT.SCHURB ) THEN
            MINWRK = MAX( MINWRK, N ) 
         END IF
         IF ( CONT ) THEN
            IF ( .NOT.SCHURA ) 
     $         MINWRK = MAX( MINWRK, IB + M ) 
         ELSE
            MINWRK = MAX( MINWRK, IB + 2*M ) 
         END IF
         MINWRK = MAX( 1, IA + MINWRK )
         IF( LDWORK.LT.MINWRK )
     $      INFO = -21
      END IF
C
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'SB04PD', -INFO )
         RETURN
      END IF
C
C     Quick return if possible.
C
      IF( M.EQ.0 .OR. N.EQ.0 ) THEN
         SCALE = ONE
         DWORK( 1 ) = ONE
         RETURN
      END IF
      MAXWRK = MINWRK
C
      IF( NOFACA ) THEN
C
C        Compute the Schur factorization of A.
C        Workspace:  need   1+5*M;
C                    prefer larger.
C        (Note: Comments in the code beginning "Workspace:" describe the
C        minimal amount of real workspace needed at that point in the
C        code, as well as the preferred amount for good performance.
C        NB refers to the optimal block size for the immediately
C        following subroutine, as returned by ILAENV.)
C
         JWORK  = 2*M + 2
         IA     = JWORK
         AVAILW = LDWORK - JWORK + 1
         CALL DGEES( 'Vectors', 'Not ordered', SELECT1, M, A, LDA, SDIM,
     $               DWORK( 2 ), DWORK( M+2 ), U, LDU, DWORK( JWORK ),
     $               AVAILW, BWORK, IERR )
         IF( IERR.GT.0 ) THEN
            INFO = IERR
            RETURN
         END IF
         MAXWRK = MAX( MAXWRK, INT( DWORK( JWORK ) ) + JWORK - 1 )
      ELSE
         JWORK  = 1
         IA     = 2
         AVAILW = LDWORK
      END IF
C
      IF( .NOT.SCHURA ) THEN
C
C        Transform the right-hand side:  C <-- U'*C.
C        Workspace:  need   a+M,
C                    prefer a+M*N,
C                    where  a = 1+2*M, if FACTA =  'N',
C                           a = 0,     if FACTA <> 'N'.
C
         CHUNKA = AVAILW / M
         BLOCKA = MIN( CHUNKA, N ).GT.1
         BLAS3A = CHUNKA.GE.N .AND. BLOCKA
C
         IF ( BLAS3A ) THEN
C
C           Enough workspace for a fast BLAS 3 algorithm. 
C
            CALL DLACPY( 'Full', M, N, C, LDC, DWORK( JWORK ), M )
            CALL DGEMM( 'Transpose', 'NoTranspose', M, N, M, ONE,
     $                  U, LDU, DWORK( JWORK ), M, ZERO, C, LDC )
         ELSE IF ( BLOCKA ) THEN
C
C           Use as many columns of C as possible.
C
            DO 10 J = 1, N, CHUNKA
               BL = MIN( N-J+1, CHUNKA )
               CALL DLACPY( 'Full', M, BL, C( 1, J ), LDC,
     $                      DWORK( JWORK ), M )
               CALL DGEMM( 'Transpose', 'NoTranspose', M, BL, M, ONE,
     $                     U, LDU, DWORK( JWORK ), M, ZERO, C( 1, J ),
     $                     LDC )
   10       CONTINUE
C
         ELSE
C
C           Use a BLAS 2 algorithm.
C
            DO 20 J = 1, N
               CALL DCOPY( M, C( 1, J ), 1, DWORK( JWORK ), 1 )
               CALL DGEMV( 'Transpose', M, M, ONE, U, LDU,
     $                     DWORK( JWORK ), 1, ZERO, C( 1, J ), 1 )
   20       CONTINUE
C
         END IF
         MAXWRK = MAX( MAXWRK, JWORK + M*N - 1 )
      END IF
C
      IF( NOFACB ) THEN
C
C        Compute the Schur factorization of B.
C        Workspace:  need   1+MAX(a-1,0)+5*N,
C                    prefer larger.
C
         JWORK  = IA + 2*N
         AVAILW = LDWORK - JWORK + 1
         CALL DGEES( 'Vectors', 'Not ordered', SELECT1, N, B, LDB, SDIM,
     $               DWORK( IA ), DWORK( N+IA ), V, LDV, DWORK( JWORK ),
     $               AVAILW, BWORK, IERR )
         IF( IERR.GT.0 ) THEN
            INFO = IERR + M
            RETURN
         END IF
         MAXWRK = MAX( MAXWRK, INT( DWORK( JWORK ) ) + JWORK - 1 )
C
         IF( .NOT.SCHURA ) THEN
C
C           Recompute the blocking parameters.
C
            CHUNKA = AVAILW / M
            BLOCKA = MIN( CHUNKA, N ).GT.1
            BLAS3A = CHUNKA.GE.N .AND. BLOCKA
         END IF
      END IF
C
      IF( .NOT.SCHURB ) THEN
C
C        Transform the right-hand side:  C <-- C*V.
C        Workspace:  need   a+b+N,
C                    prefer a+b+M*N,
C                    where  b = 2*N,   if FACTB =  'N', FACTA =  'N',
C                           b = 1+2*N, if FACTB =  'N', FACTA <> 'N',
C                           b = 0,     if FACTB <> 'N'.
C
         CHUNKB = AVAILW / N
         BLOCKB = MIN( CHUNKB, M ).GT.1
         BLAS3B = CHUNKB.GE.M .AND. BLOCKB
C
         IF ( BLAS3B ) THEN
C
C           Enough workspace for a fast BLAS 3 algorithm. 
C
            CALL DLACPY( 'Full', M, N, C, LDC, DWORK( JWORK ), M )
            CALL DGEMM( 'NoTranspose', 'NoTranspose', M, N, N, ONE,
     $                  DWORK( JWORK ), M, V, LDV, ZERO, C, LDC )
         ELSE IF ( BLOCKB ) THEN
C
C           Use as many rows of C as possible.
C
            DO 30 I = 1, M, CHUNKB
               BL = MIN( M-I+1, CHUNKB )
               CALL DLACPY( 'Full', BL, N, C( I, 1 ), LDC,
     $                      DWORK( JWORK ), BL )
               CALL DGEMM( 'NoTranspose', 'NoTranspose', BL, N, N, ONE,
     $                     DWORK( JWORK ), BL, V, LDV, ZERO, C( I, 1 ),
     $                     LDC )
   30       CONTINUE
C
         ELSE
C
C           Use a BLAS 2 algorithm.
C
            DO 40 I = 1, M
               CALL DCOPY( N, C( I, 1 ), LDC, DWORK( JWORK ), 1 )
               CALL DGEMV( 'Transpose', N, N, ONE, V, LDV,
     $                     DWORK( JWORK ), 1, ZERO, C( I, 1 ), LDC )
   40       CONTINUE
C
         END IF
         MAXWRK = MAX( MAXWRK, JWORK + M*N - 1 )
      END IF
C
C     Solve the (transformed) equation.
C     Workspace for DICO = 'D':  a+b+2*M.
C
      IF ( CONT ) THEN
         CALL DTRSYL( TRANA, TRANB, ISGN, M, N, A, LDA, B, LDB, C, LDC,
     $                SCALE, IERR )
      ELSE                   
         CALL SB04PY( TRANA, TRANB, ISGN, M, N, A, LDA, B, LDB, C, LDC,
     $                SCALE, DWORK( JWORK ), IERR )
         MAXWRK = MAX( MAXWRK, JWORK + 2*M - 1 )
      END IF
      IF( IERR.GT.0 )
     $   INFO = M + N + 1
C
C     Transform back the solution, if needed.
C
      IF( .NOT.SCHURA ) THEN
C
C        Transform the right-hand side:  C <-- U*C.
C        Workspace:  need   a+b+M;
C                    prefer a+b+M*N.
C
         IF ( BLAS3A ) THEN
C
C           Enough workspace for a fast BLAS 3 algorithm. 
C
            CALL DLACPY( 'Full', M, N, C, LDC, DWORK( JWORK ), M )
            CALL DGEMM( 'NoTranspose', 'NoTranspose', M, N, M, ONE,
     $                  U, LDU, DWORK( JWORK ), M, ZERO, C, LDC )
         ELSE IF ( BLOCKA ) THEN
C
C           Use as many columns of C as possible.
C
            DO 50 J = 1, N, CHUNKA
               BL = MIN( N-J+1, CHUNKA )
               CALL DLACPY( 'Full', M, BL, C( 1, J ), LDC,
     $                      DWORK( JWORK ), M )
               CALL DGEMM( 'NoTranspose', 'NoTranspose', M, BL, M, ONE,
     $                     U, LDU, DWORK( JWORK ), M, ZERO, C( 1, J ),
     $                     LDC )
   50       CONTINUE
C
         ELSE
C
C           Use a BLAS 2 algorithm.
C
            DO 60 J = 1, N
               CALL DCOPY( M, C( 1, J ), 1, DWORK( JWORK ), 1 )
               CALL DGEMV( 'NoTranspose', M, M, ONE, U, LDU,
     $                     DWORK( JWORK ), 1, ZERO, C( 1, J ), 1 )
   60       CONTINUE
C
         END IF
      END IF
C
      IF( .NOT.SCHURB ) THEN
C
C        Transform the right-hand side:  C <-- C*V'.
C        Workspace:  need   a+b+N;
C                    prefer a+b+M*N.
C
         IF ( BLAS3B ) THEN
C
C           Enough workspace for a fast BLAS 3 algorithm. 
C
            CALL DLACPY( 'Full', M, N, C, LDC, DWORK( JWORK ), M )
            CALL DGEMM( 'NoTranspose', 'Transpose', M, N, N, ONE,
     $                  DWORK( JWORK ), M, V, LDV, ZERO, C, LDC )
         ELSE IF ( BLOCKB ) THEN
C
C           Use as many rows of C as possible.
C
            DO 70 I = 1, M, CHUNKB
               BL = MIN( M-I+1, CHUNKB )
               CALL DLACPY( 'Full', BL, N, C( I, 1 ), LDC,
     $                      DWORK( JWORK ), BL )
               CALL DGEMM( 'NoTranspose', 'Transpose', BL, N, N, ONE,
     $                     DWORK( JWORK ), BL, V, LDV, ZERO, C( I, 1 ),
     $                     LDC )
   70       CONTINUE
C
         ELSE
C
C           Use a BLAS 2 algorithm.
C
            DO 80 I = 1, M
               CALL DCOPY( N, C( I, 1 ), LDC, DWORK( JWORK ), 1 )
               CALL DGEMV( 'NoTranspose', N, N, ONE, V, LDV,
     $                     DWORK( JWORK ), 1, ZERO, C( I, 1 ), LDC )
   80       CONTINUE
C
         END IF
      END IF
C
      DWORK( 1 ) = DBLE( MAXWRK )
C
      RETURN
C *** Last line of SB04PD ***
      END
