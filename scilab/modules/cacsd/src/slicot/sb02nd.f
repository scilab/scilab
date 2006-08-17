      SUBROUTINE SB02ND( DICO, FACT, UPLO, JOBL, N, M, P, A, LDA, B,
     $                   LDB, R, LDR, IPIV, L, LDL, X, LDX, RNORM, F,
     $                   LDF, OUFACT, IWORK, DWORK, LDWORK, INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 1999.
C
C     PURPOSE
C
C     To compute the optimal feedback matrix F for the problem of
C     optimal control given by
C
C                        -1
C          F = (R + B'XB)  (B'XA + L')                           (1)
C
C     in the discrete-time case and
C
C               -1
C          F = R  (B'X + L')                                     (2)
C
C     in the continuous-time case, where A, B and L are N-by-N, N-by-M
C     and N-by-M matrices respectively; R and X are M-by-M and N-by-N 
C     symmetric matrices respectively.
C
C     Optionally, matrix R may be specified in a factored form, and L
C     may be zero.
C
C     ARGUMENTS
C
C     Mode Parameters
C
C     DICO    CHARACTER*1
C             Specifies the equation from which F is to be determined,
C             as follows:
C             = 'D':  Equation (1), discrete-time case;
C             = 'C':  Equation (2), continuous-time case.
C
C     FACT    CHARACTER*1
C             Specifies how the matrix R is given (factored or not), as 
C             follows:
C             = 'N':  Array R contains the matrix R;
C             = 'D':  Array R contains a P-by-M matrix D, where R = D'D;
C             = 'C':  Array R contains the Cholesky factor of R;
C             = 'U':  Array R contains the symmetric indefinite UdU' or
C                     LdL' factorization of R. This option is not
C                     available for DICO = 'D'.
C
C     UPLO    CHARACTER*1
C             Specifies which triangle of the possibly factored matrix R
C             (or R + B'XB, on exit) is or should be stored, as follows:
C             = 'U':  Upper triangle is stored;
C             = 'L':  Lower triangle is stored.
C
C     JOBL    CHARACTER*1
C             Specifies whether or not the matrix L is zero, as follows:
C             = 'Z':  L is zero;
C             = 'N':  L is nonzero.
C
C     Input/Output Parameters
C
C     N       (input) INTEGER
C             The order of the matrices A and X.  N >= 0.
C
C     M       (input) INTEGER
C             The number of system inputs.  M >= 0.
C
C     P       (input) INTEGER
C             The number of system outputs.  P >= 0.
C             This parameter must be specified only for FACT = 'D'.
C
C     A       (input) DOUBLE PRECISION array, dimension (LDA,N)
C             If DICO = 'D', the leading N-by-N part of this array must
C             contain the state matrix A of the system.
C             If DICO = 'C', this array is not referenced.
C
C     LDA     INTEGER
C             The leading dimension of array A.  
C             LDA >= MAX(1,N) if DICO = 'D';
C             LDA >= 1        if DICO = 'C'.
C
C     B       (input) DOUBLE PRECISION array, dimension (LDB,M)
C             The leading N-by-M part of this array must contain the
C             input matrix B of the system.
C             If DICO = 'D' and FACT = 'D' or 'C', the contents of this
C             array is destroyed.
C             Otherwise, B is unchanged on exit.
C
C     LDB     INTEGER
C             The leading dimension of array B.  LDB >= MAX(1,N).
C
C     R       (input/output) DOUBLE PRECISION array, dimension (LDR,M)
C             On entry, if FACT = 'N', the leading M-by-M upper
C             triangular part (if UPLO = 'U') or lower triangular part
C             (if UPLO = 'L') of this array must contain the upper
C             triangular part or lower triangular part, respectively,
C             of the symmetric input weighting matrix R.
C             On entry, if FACT = 'D', the leading P-by-M part of this 
C             array must contain the direct transmission matrix D of the
C             system.
C             On entry, if FACT = 'C', the leading M-by-M upper
C             triangular part (if UPLO = 'U') or lower triangular part
C             (if UPLO = 'L') of this array must contain the Cholesky
C             factor of the positive definite input weighting matrix R
C             (as produced by LAPACK routine DPOTRF).
C             On entry, if DICO = 'C' and FACT = 'U', the leading M-by-M
C             upper triangular part (if UPLO = 'U') or lower triangular
C             part (if UPLO = 'L') of this array must contain the
C             factors of the UdU' or LdL' factorization, respectively, 
C             of the symmetric indefinite input weighting matrix R (as
C             produced by LAPACK routine DSYTRF).
C             The stricly lower triangular part (if UPLO = 'U') or
C             stricly upper triangular part (if UPLO = 'L') of this
C             array is used as workspace.
C             On exit, if OUFACT(1) = 1, and INFO = 0 (or INFO = M+1),
C             the leading M-by-M upper triangular part (if UPLO = 'U')
C             or lower triangular part (if UPLO = 'L') of this array 
C             contains the Cholesky factor of the given input weighting
C             matrix (for DICO = 'C'), or that of the matrix R + B'XB 
C             (for DICO = 'D'). 
C             On exit, if OUFACT(1) = 2, and INFO = 0 (or INFO = M+1),
C             the leading M-by-M upper triangular part (if UPLO = 'U')
C             or lower triangular part (if UPLO = 'L') of this array 
C             contains the factors of the UdU' or LdL' factorization, 
C             respectively, of the given input weighting matrix
C             (for DICO = 'C'), or that of the matrix R + B'XB 
C             (for DICO = 'D'). 
C             On exit R is unchanged if FACT = 'U'.
C             
C     LDR     INTEGER.
C             The leading dimension of the array R.
C             LDR >= MAX(1,M)   if FACT <> 'D';
C             LDR >= MAX(1,M,P) if FACT =  'D'.
C
C     IPIV    (input/output) INTEGER array, dimension (M)
C             On entry, if FACT = 'U', this array must contain details
C             of the interchanges performed and the block structure of
C             the d factor in the UdU' or LdL' factorization of matrix R
C             (as produced by LAPACK routine DSYTRF).
C             On exit, if OUFACT(1) = 2, this array contains details of
C             the interchanges performed and the block structure of the
C             d factor in the UdU' or LdL' factorization of matrix R (or
C             D'D) or R + B'XB (or D'D + B'XB), as produced by LAPACK 
C             routine DSYTRF.
C             This array is not referenced for DICO = 'D' or FACT = 'D',
C             or 'C'.
C
C     L       (input) DOUBLE PRECISION array, dimension (LDL,M)
C             If JOBL = 'N', the leading N-by-M part of this array must
C             contain the cross weighting matrix L.
C             If JOBL = 'Z', this array is not referenced.
C
C     LDL     INTEGER
C             The leading dimension of array L.
C             LDL >= MAX(1,N) if JOBL = 'N';
C             LDL >= 1        if JOBL = 'Z'.
C
C     X       (input/output) DOUBLE PRECISION array, dimension (LDX,N)
C             On entry, the leading N-by-N part of this array must
C             contain the solution matrix X of the algebraic Riccati
C             equation as produced by SLICOT Library routines SB02MD or
C             SB02OD. Matrix X is assumed non-negative definite.
C             On exit, if DICO = 'D', FACT = 'D' or 'C', OUFACT(2) = 1,
C             and INFO = 0, the N-by-N upper triangular part of this
C             array contains the Cholesky factor of the given matrix X,
C             which is found to be positive definite.
C             On exit, if DICO = 'D', FACT = 'D' or 'C', OUFACT(2) = 2,
C             and INFO = 0, the leading N-by-N part of this array 
C             contains the matrix of orthonormal eigenvectors of X. 
C             On exit X is unchanged if DICO = 'C' or FACT = 'N'.
C
C     LDX     INTEGER
C             The leading dimension of array X.  LDX >= MAX(1,N).
C
C     RNORM   (input) DOUBLE PRECISION
C             If FACT = 'U', this parameter must contain the 1-norm of
C             the original matrix R (before factoring it).
C             Otherwise, this parameter is not used.
C
C     F       (output) DOUBLE PRECISION array, dimension (LDF,N)
C             The leading M-by-N part of this array contains the
C             optimal feedback matrix F.
C
C     LDF     INTEGER
C             The leading dimension of array F.  LDF >= MAX(1,M).
C
C     OUFACT  (output) INTEGER array, dimension (2) 
C             Information about the factorization finally used.
C             OUFACT(1) = 1:  Cholesky factorization of R (or R + B'XB)
C                             has been used;
C             OUFACT(1) = 2:  UdU' (if UPLO = 'U') or LdL' (if UPLO =  
C                             'L') factorization of R (or R + B'XB)
C                             has been used;
C             OUFACT(2) = 1:  Cholesky factorization of X has been used;
C             OUFACT(2) = 2:  Spectral factorization of X has been used.
C             The value of OUFACT(2) is not set for DICO = 'C' or for 
C             DICO = 'D' and FACT = 'N'.
C
C     Workspace
C
C     IWORK   INTEGER array, dimension (M)
C
C     DWORK   DOUBLE PRECISION array, dimension (LDWORK)
C             On exit, if INFO = 0, DWORK(1) returns the optimal value
C             of LDWORK, and DWORK(2) contains the reciprocal condition 
C             number of the matrix R (for DICO = 'C') or of R + B'XB
C             (for DICO = 'D').
C             If on exit INFO = 0, and OUFACT(2) = 2, then DWORK(3),...,
C             DWORK(N+2) contain the eigenvalues of X, in ascending 
C             order. 
C
C     LDWORK  INTEGER
C             Dimension of working array DWORK.
C             LDWORK >= max(2,3*M)         if FACT = 'N';
C             LDWORK >= max(2,2*M)         if FACT = 'U';
C             LDWORK >= max(2,3*M)         if FACT = 'C', DICO = 'C';
C             LDWORK >= N+3*M+2            if FACT = 'C', DICO = 'D';
C             LDWORK >= max(2,min(P,M)+M)  if FACT = 'D', DICO = 'C';
C             LDWORK >= max(N+3*M+2,4*N+1) if FACT = 'D', DICO = 'D'.
C             For optimum performance LDWORK should be larger.
C
C     Error Indicator
C
C     INFO    INTEGER
C             = 0:  successful exit;
C             < 0:  if INFO = -i, the i-th argument had an illegal
C                   value;
C             = i:  if the i-th element of the d factor is exactly zero;
C                   the UdU' (or LdL') factorization has been completed,
C                   but the block diagonal matrix d is exactly singular;
C             = M+1:  if the matrix R (if DICO = 'C'), or R + B'XB  
C                   (if DICO = 'D') is numerically singular (to working 
C                   precision);
C             = M+2:  if one or more of the eigenvalues of X has not 
C                   converged.
C
C     METHOD
C
C     The optimal feedback matrix F is obtained as the solution to the
C     system of linear equations
C
C        (R + B'XB) * F = B'XA + L'
C
C     in the discrete-time case and
C
C        R * F = B'X + L'
C
C     in the continuous-time case, with R replaced by D'D if FACT = 'D'.
C     The factored form of R, specified by FACT <> 'N', is taken into 
C     account. If FACT = 'N', Cholesky factorization is tried first, but
C     if the coefficient matrix is not positive definite, then UdU' (or
C     LdL') factorization is used. The discrete-time case involves 
C     updating of a triangular factorization of R (or D'D); Cholesky or
C     symmetric spectral factorization of X is employed to avoid 
C     squaring of the condition number of the matrix. When D is given,
C     its QR factorization is determined, and the triangular factor is 
C     used as described above.
C
C     NUMERICAL ASPECTS
C
C     The algorithm consists of numerically stable steps.
C                                    3     2
C     For DICO = 'C', it requires O(m  + mn ) floating point operations
C                           2
C     if FACT = 'N' and O(mn ) floating point operations, otherwise.
C     For DICO = 'D', the operation counts are similar, but additional
C        3
C     O(n ) floating point operations may be needed in the worst case.
C
C     CONTRIBUTORS
C
C     Release 3.0: V. Sima, Katholieke Univ. Leuven, Belgium, Sep. 1997.
C     Supersedes Release 2.0 routine SB02BD by M. Vanbegin, and
C     P. Van Dooren, Philips Research Laboratory, Brussels, Belgium.
C
C     REVISIONS
C
C     -
C
C     KEYWORDS
C
C     Algebraic Riccati equation, closed loop system, continuous-time
C     system, discrete-time system, matrix algebra, optimal control,
C     optimal regulator.
C
C     ******************************************************************
C
C     .. Parameters ..
      DOUBLE PRECISION  ZERO, ONE
      PARAMETER         ( ZERO = 0.0D0, ONE = 1.0D0 )
C     .. Scalar Arguments ..
      CHARACTER*1       DICO, FACT, JOBL, UPLO
      INTEGER           INFO, LDA, LDB, LDF, LDL, LDR, LDWORK, LDX, M,
     $                  N, P
      DOUBLE PRECISION  RNORM
C     .. Array Arguments ..
      INTEGER           IPIV(*), IWORK(*), OUFACT(2) 
      DOUBLE PRECISION  A(LDA,*), B(LDB,*), DWORK(*), F(LDF,*), 
     $                  L(LDL,*), R(LDR,*), X(LDX,*)
C     .. Local Scalars ..
      LOGICAL           DISCR, LFACTA, LFACTC, LFACTD, LFACTU, LUPLOU, 
     $                  WITHL
      INTEGER           I, IFAIL, ITAU, J, JW, JWORK, JZ, WRKOPT
      DOUBLE PRECISION  EPS, RCOND, RNORMP, TEMP
C     .. Local Arrays ..
      DOUBLE PRECISION  DUMMY(1)
C     .. External Functions ..
      LOGICAL           LSAME
      DOUBLE PRECISION  DLAMCH, DLANSY
      EXTERNAL          DLAMCH, DLANSY, LSAME
C     .. External Subroutines ..
      EXTERNAL          DCOPY, DGEMM, DGEMV, DGEQRF, DLASET, DPOCON, 
     $                  DPOTRF, DPOTRS, DSCAL, DSYCON, DSYEV, DSYTRF, 
     $                  DSYTRS, DTRCON, DTRMM, MB04KD, XERBLA
C     .. Intrinsic Functions ..
      INTRINSIC         ABS, INT, MAX, MIN, SQRT
C     .. Executable Statements ..
C
      INFO = 0
      DISCR  = LSAME( DICO, 'D' )
      LFACTC = LSAME( FACT, 'C' )
      LFACTD = LSAME( FACT, 'D' )
      LFACTU = LSAME( FACT, 'U' )
      LUPLOU = LSAME( UPLO, 'U' )
      WITHL  = LSAME( JOBL, 'N' )
      LFACTA = LFACTC.OR.LFACTD.OR.LFACTU
C
C     Test the input scalar arguments.
C
      IF( .NOT.DISCR .AND. .NOT.LSAME( DICO, 'C' ) ) THEN
         INFO = -1
      ELSE IF( ( .NOT.LFACTA .AND. .NOT.LSAME( FACT, 'N' ) ) .OR.
     $         ( DISCR .AND. LFACTU ) ) THEN
         INFO = -2
      ELSE IF( .NOT.LUPLOU .AND. .NOT.LSAME( UPLO, 'L' ) ) THEN
         INFO = -3
      ELSE IF( .NOT.WITHL .AND. .NOT.LSAME( JOBL, 'Z' ) ) THEN
         INFO = -4
      ELSE IF( N.LT.0 ) THEN
         INFO = -5
      ELSE IF( M.LT.0 ) THEN
         INFO = -6
      ELSE IF( P.LT.0 ) THEN
         INFO = -7
      ELSE IF( ( .NOT.DISCR .AND. LDA.LT.1 )             .OR.
     $         (      DISCR .AND. LDA.LT.MAX( 1, N ) ) ) THEN
         INFO = -9
      ELSE IF( LDB.LT.MAX( 1, N ) ) THEN
         INFO = -11
      ELSE IF( (              LDR.LT.MAX( 1, M ) )   .OR.
     $         ( LFACTD .AND. LDR.LT.MAX( 1, P ) ) ) THEN
         INFO = -13
      ELSE IF( ( .NOT.WITHL .AND. LDL.LT.1 )             .OR.
     $         (      WITHL .AND. LDL.LT.MAX( 1, N ) ) ) THEN
         INFO = -16
      ELSE IF( LDX.LT.MAX( 1, N ) ) THEN
         INFO = -18
      ELSE IF( LFACTU ) THEN
         IF( RNORM.LT.ZERO ) 
     $      INFO = -19
      END IF
      IF( LDF.LT.MAX( 1, M ) ) THEN
         INFO = -21
      ELSE IF( ( ( .NOT.LFACTA .OR. ( LFACTC .AND. .NOT.DISCR ) ) 
     $                         .AND. LDWORK.LT.MAX( 2, 3*M ) ) .OR.
     $         (        LFACTU .AND. LDWORK.LT.MAX( 2, 2*M ) ) .OR. 
     $    ( DISCR .AND. LFACTC .AND. LDWORK.LT.N + 3*M + 2 )   .OR.
     $(.NOT.DISCR .AND. LFACTD .AND. LDWORK.LT.MAX( 2, MIN(P,M) + M ) )
     $                                                         .OR.
     $    ( DISCR .AND. LFACTD .AND. LDWORK.LT.MAX( N + 3*M + 2, 
     $                                              4*N + 1 ) ) ) THEN
         INFO = -25
      END IF
C
      IF ( INFO.NE.0 ) THEN
C
C        Error return.
C
         CALL XERBLA( 'SB02ND', -INFO )
         RETURN
      END IF
C
C     Quick return if possible.
C
      IF ( N.EQ.0 .OR. M.EQ.0 .OR. ( LFACTD .AND. P.EQ.0 ) ) THEN
        DWORK(1) = ONE
        DWORK(2) = ONE
        RETURN
      END IF
C
      WRKOPT = 1
      EPS = DLAMCH( 'Epsilon' )
C
C     Determine the right-hand side of the matrix equation.
C     Compute  B'X  in F.
C
C     (Note: Comments in the code beginning "Workspace:" describe the
C     minimal amount of real workspace needed at that point in the
C     code, as well as the preferred amount for good performance.
C     NB refers to the optimal block size for the immediately
C     following subroutine, as returned by ILAENV.)
C     
      CALL DGEMM( 'Transpose', 'No transpose', M, N, N, ONE, B, LDB, X,
     $            LDX, ZERO, F, LDF )
C
      IF ( .NOT.LFACTA ) THEN
         IF ( DISCR ) THEN
C
C           Discrete-time case with R not factored. Compute R + B'XB.
C
            IF ( LUPLOU ) THEN
C
               DO 10 J = 1, M
                  CALL DGEMV( 'No transpose', J, N, ONE, F, LDF, B(1,J),
     $                        1, ONE, R(1,J), 1 )
   10          CONTINUE
C
            ELSE
C           
               DO 20 J = 1, M
                  CALL DGEMV( 'Transpose', N, J, ONE, B, LDB, F(J,1),
     $                        LDF, ONE, R(J,1), LDR )
   20          CONTINUE
C
            END IF
         END IF
C
C        Compute the 1-norm of the matrix  R  or  R + B'XB.
C        Workspace: need M.
C
         RNORMP = DLANSY( '1-norm', UPLO, M, R, LDR, DWORK )
         WRKOPT = MAX( WRKOPT, M )
      END IF
C
      IF ( DISCR ) THEN
C
C        For discrete-time case, postmultiply B'X by A.
C        Workspace: need N.
C
         DO 30 I = 1, M
            CALL DCOPY( N, F(I,1), LDF, DWORK, 1 )
            CALL DGEMV( 'Transpose', N, N, ONE, A, LDA, DWORK, 1, ZERO,
     $                  F(I,1), LDF )
   30    CONTINUE
C
         WRKOPT = MAX( WRKOPT, N )
      END IF
C
      IF( WITHL ) THEN
C
C        Add L'.
C
         DO 50 I = 1, M
C
            DO 40 J = 1, N
               F(I,J) = F(I,J) + L(J,I)
   40       CONTINUE
C
   50    CONTINUE
C
      END IF
C
C     Solve the matrix equation.
C
      IF ( LFACTA ) THEN
C
C        Case 1: Matrix R is given in a factored form.
C
         IF ( LFACTD ) THEN
C        
C           Use QR factorization of D.
C           Workspace: need   min(P,M) + M,
C                      prefer min(P,M) + M*NB.
C        
            ITAU = 1
            JWORK = ITAU + MIN( P, M )
            CALL DGEQRF( P, M, R, LDR, DWORK(ITAU), DWORK(JWORK),
     $                   LDWORK-JWORK+1, IFAIL )
            WRKOPT = MAX( WRKOPT, INT( DWORK(JWORK) )+JWORK-1 )
C        
C           Make positive the diagonal elements of the triangular
C           factor. Construct the strictly lower triangle, if requested.
C        
            DO 70 I = 1, M
               IF ( R(I,I).LT.ZERO ) THEN
C        
                  DO 60 J = I, M
                     R(I,J) = -R(I,J)
   60             CONTINUE
C        
               END IF
               IF ( .NOT.LUPLOU ) 
     $            CALL DCOPY( I-1, R(1,I), 1, R(I,1), LDR ) 
   70       CONTINUE
C        
            IF ( P.LT.M ) THEN
               CALL DLASET( 'Full', M-P, M, ZERO, ZERO, R(P+1,1), LDR ) 
               IF ( .NOT.DISCR ) THEN
                  DWORK(2) = ZERO 
                  INFO = M + 1 
                  RETURN 
               END IF
            END IF
         END IF
C
         JW = 1
         IF ( DISCR ) THEN
C
C           Discrete-time case. Update the factorization for B'XB.
C           Try first the Cholesky factorization of X, saving the 
C           diagonal of X, in order to recover it, if X is not positive
C           definite. In the later case, use spectral factorization.
C           Workspace: need N. 
C           Define     JW = 1   for Cholesky factorization of X,
C                      JW = N+3 for spectral factorization of X.
C
            CALL DCOPY( N, X, LDX+1, DWORK, 1 )
            CALL DPOTRF( 'Upper', N, X, LDX, IFAIL )
            IF ( IFAIL.EQ.0 ) THEN
C
C              Use Cholesky factorization of X to compute chol(X)*B.
C
               OUFACT(2) = 1
               CALL DTRMM( 'Left', 'Upper', 'No transpose', 'Non unit',
     $                     N, M, ONE, X, LDX, B, LDB )
            ELSE
C
C              Use spectral factorization of X, X = UVU'.
C              Workspace: need   4*N+1,
C                         prefer N*(NB+2)+N+2.
C
               JW = N + 3
               OUFACT(2) = 2
               CALL DCOPY( N, DWORK, 1, X, LDX+1 )
               CALL DSYEV( 'Vectors', 'Lower', N, X, LDX, DWORK(3), 
     $                     DWORK(JW), LDWORK-JW+1, IFAIL )
               IF ( IFAIL.GT.0 ) THEN
                  INFO = M + 2
                  RETURN
               END IF
               WRKOPT = MAX( WRKOPT, INT( DWORK(JW) )+JW-1 )
               TEMP = ABS( DWORK(N+2) )*EPS 
C
C              Count the negligible eigenvalues and compute sqrt(V)U'B.
C              Workspace: need 2*N+2.
C
               JZ = 0
C
   80          CONTINUE
               IF ( ABS( DWORK(JZ+3) ).LE.TEMP ) THEN
                  JZ = JZ + 1
                  IF ( JZ.LT.N) GO TO 80
               END IF 
C
               DO 90 J = 1, M
                  CALL DCOPY( N, B(1,J), 1, DWORK(JW), 1 )
                  CALL DGEMV( 'Transpose', N, N, ONE, X, LDX, DWORK(JW),
     $                        1, ZERO, B(1,J), 1 )
   90          CONTINUE
C
               DO 100 I = JZ + 1, N
                  CALL DSCAL( M, SQRT( ABS( DWORK(I+2) ) ), B(I,1), LDB
     $                      )
  100          CONTINUE
C
               IF ( JZ.GT.0 ) 
     $            CALL DLASET( 'Full', JZ, M, ZERO, ZERO, B, LDB )
            END IF
C
C           Update the triangular factorization. 
C
            IF ( .NOT.LUPLOU ) THEN
C           
C              For efficiency, use the transposed of the lower triangle.
C           
               DO 110 I = 2, M
                  CALL DCOPY( I-1, R(I,1), LDR, R(1,I), 1 )
  110          CONTINUE
C
            END IF
C
C           Workspace: need JW+2*M-1.
C
            CALL MB04KD( 'Full', M, 0, N, R, LDR, B, LDB, DUMMY, N,
     $                   DUMMY, M, DWORK(JW), DWORK(JW+N) )
            WRKOPT = MAX( WRKOPT, JW + 2*M - 1 )
C           
C           Make positive the diagonal elements of the triangular
C           factor.
C           
            DO 130 I = 1, M
               IF ( R(I,I).LT.ZERO ) THEN
C           
                  DO 120 J = I, M
                     R(I,J) = -R(I,J)
  120             CONTINUE
C           
               END IF
  130       CONTINUE
C        
            IF ( .NOT.LUPLOU ) THEN
C
C              Construct the lower triangle.
C              
               DO 140 I = 2, M
                  CALL DCOPY( I-1, R(1,I), 1, R(I,1), LDR )
  140          CONTINUE
C
            END IF
         END IF
C        
C        Compute the condition number of the coefficient matrix.
C           
         IF ( .NOT.LFACTU ) THEN
C        
C           Workspace: need JW+3*M-1.
C           
            CALL DTRCON( '1-norm', UPLO, 'Non unit', M, R, LDR, RCOND, 
     $                    DWORK(JW), IWORK, IFAIL )
            OUFACT(1) = 1
            WRKOPT = MAX( WRKOPT, JW + 3*M - 1 )
         ELSE
C        
C           Workspace: need 2*M.
C           
            CALL DSYCON( UPLO, M, R, LDR, IPIV, RNORM, RCOND, DWORK,
     $                   IWORK, INFO )
            OUFACT(1) = 2
            WRKOPT = MAX( WRKOPT, 2*M )
         END IF
         DWORK(2) = RCOND
         IF( RCOND.LT.EPS ) THEN
            INFO = M + 1
            RETURN
         END IF
C
      ELSE
C
C        Case 2: Matrix R is given in an unfactored form.
C
C        Save the given triangle of  R  or  R + B'XB  in the other
C        strict triangle and the diagonal in the workspace, and try 
C        Cholesky factorization.
C        Workspace: need M.
C        
         CALL DCOPY( M, R, LDR+1, DWORK, 1 )
         IF( LUPLOU ) THEN
C        
            DO 150 J = 2, M
               CALL DCOPY( J-1, R(1,J), 1, R(J,1), LDR )
  150       CONTINUE
C        
         ELSE
C        
            DO 160 J = 2, M
               CALL DCOPY( J-1, R(J,1), LDR, R(1,J), 1 )
  160       CONTINUE
C        
         END IF
         CALL DPOTRF( UPLO, M, R, LDR, INFO )
         OUFACT(1) = 1
         IF( INFO.EQ.0 ) THEN
C        
C           Compute the reciprocal of the condition number of R.
C           Workspace: need 3*M.
C           
            CALL DPOCON( UPLO, M, R, LDR, RNORMP, RCOND, DWORK, IWORK,
     $                   INFO )
C           
C           Return if the matrix is singular to working precision.
C           
            DWORK(2) = RCOND
            IF( RCOND.LT.EPS ) THEN
               INFO = M + 1
               RETURN
            END IF
            WRKOPT = MAX( WRKOPT, 3*M )
         ELSE
C        
C           Use UdU' or LdL' factorization, first restoring the saved
C           triangle.
C        
            CALL DCOPY( M, DWORK, 1, R, LDR+1 )
            IF( LUPLOU ) THEN
C           
               DO 170 J = 2, M
                  CALL DCOPY( J-1, R(J,1), LDR, R(1,J), 1 )
  170          CONTINUE
C           
            ELSE
C           
               DO 180 J = 2, M
                  CALL DCOPY( J-1, R(1,J), 1, R(J,1), LDR )
  180          CONTINUE
C           
            END IF
C        
C           Workspace: need   1,
C                      prefer M*NB.
C        
            CALL DSYTRF( UPLO, M, R, LDR, IPIV, DWORK, LDWORK, INFO ) 
            OUFACT(1) = 2
            IF( INFO.GT.0 ) 
     $         RETURN
            WRKOPT = MAX( WRKOPT, INT( DWORK(1) ) )
C        
C           Compute the reciprocal of the condition number of R.
C           Workspace: need   2*M.
C           
            CALL DSYCON( UPLO, M, R, LDR, IPIV, RNORMP, RCOND, DWORK,
     $                   IWORK, INFO )
C           
C           Return if the matrix is singular to working precision.
C           
            DWORK(2) = RCOND
            IF( RCOND.LT.EPS ) THEN
               INFO = M + 1
               RETURN
            END IF
         END IF
      END IF
C
      IF (OUFACT(1).EQ.1 ) THEN
C           
C        Solve the positive definite linear system.
C              
         CALL DPOTRS( UPLO, M, N, R, LDR, F, LDF, INFO )
      ELSE
C           
C        Solve the indefinite linear system.
C              
         CALL DSYTRS( UPLO, M, N, R, LDR, IPIV, F, LDF, INFO )
      END IF
C
C     Set the optimal workspace.
C
      DWORK(1) = WRKOPT
C
      RETURN
C *** Last line of SB02ND ***
      END
