      SUBROUTINE SB03QX( TRANA, UPLO, LYAPUN, N, XANORM, T, LDT, U, LDU,
     $                   R, LDR, FERR, IWORK, DWORK, LDWORK, INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 1999.
C
C     PURPOSE
C
C     To estimate a forward error bound for the solution X of a real
C     continuous-time Lyapunov matrix equation,
C
C            op(A)'*X + X*op(A) = C,
C
C     where op(A) = A or A' (A**T) and C is symmetric (C = C**T). The
C     matrix A, the right hand side C, and the solution X are N-by-N.
C     An absolute residual matrix, which takes into account the rounding
C     errors in forming it, is given in the array R.
C
C     ARGUMENTS 
C
C     Mode Parameters
C
C     TRANA   CHARACTER*1
C             Specifies the form of op(A) to be used, as follows:
C             = 'N':  op(A) = A    (No transpose);
C             = 'T':  op(A) = A**T (Transpose);
C             = 'C':  op(A) = A**T (Conjugate transpose = Transpose).
C
C     UPLO    CHARACTER*1
C             Specifies which part of the symmetric matrix R is to be
C             used, as follows:
C             = 'U':  Upper triangular part;
C             = 'L':  Lower triangular part.
C
C     LYAPUN  CHARACTER*1
C             Specifies whether or not the original Lyapunov equations 
C             should be solved, as follows:
C             = 'O':  Solve the original Lyapunov equations, updating
C                     the right-hand sides and solutions with the
C                     matrix U, e.g., X <-- U'*X*U;
C             = 'R':  Solve reduced Lyapunov equations only, without
C                     updating the right-hand sides and solutions.
C
C     Input/Output Parameters
C
C     N       (input) INTEGER
C             The order of the matrices A and R.  N >= 0.
C
C     XANORM  (input) DOUBLE PRECISION
C             The absolute (maximal) norm of the symmetric solution
C             matrix X of the Lyapunov equation.  XANORM >= 0.
C
C     T       (input) DOUBLE PRECISION array, dimension (LDT,N)
C             The leading N-by-N upper Hessenberg part of this array
C             must contain the upper quasi-triangular matrix T in Schur
C             canonical form from a Schur factorization of A.
C
C     LDT     INTEGER
C             The leading dimension of array T.  LDT >= MAX(1,N).
C
C     U       (input) DOUBLE PRECISION array, dimension (LDU,N)
C             The leading N-by-N part of this array must contain the
C             orthogonal matrix U from a real Schur factorization of A.
C             If LYAPUN = 'R', the array U is not referenced.
C
C     LDU     INTEGER
C             The leading dimension of array U.  
C             LDU >= 1,        if LYAPUN = 'R';
C             LDU >= MAX(1,N), if LYAPUN = 'O'.
C
C     R       (input/output) DOUBLE PRECISION array, dimension (LDR,N)
C             On entry, if UPLO = 'U', the leading N-by-N upper
C             triangular part of this array must contain the upper
C             triangular part of the absolute residual matrix R, with
C             bounds on rounding errors added.
C             On entry, if UPLO = 'L', the leading N-by-N lower
C             triangular part of this array must contain the lower
C             triangular part of the absolute residual matrix R, with
C             bounds on rounding errors added.
C             On exit, the leading N-by-N part of this array contains
C             the symmetric absolute residual matrix R (with bounds on
C             rounding errors added), fully stored. 
C
C     LDR     INTEGER
C             The leading dimension of array R.  LDR >= MAX(1,N).
C
C     FERR    (output) DOUBLE PRECISION
C             An estimated forward error bound for the solution X.
C             If XTRUE is the true solution, FERR bounds the magnitude
C             of the largest entry in (X - XTRUE) divided by the
C             magnitude of the largest entry in X.
C             If N = 0 or XANORM = 0, FERR is set to 0, without any
C             calculations.
C
C     Workspace
C
C     IWORK   INTEGER array, dimension (N*N)
C
C     DWORK   DOUBLE PRECISION array, dimension (LDWORK)
C
C     LDWORK  INTEGER
C             The length of the array DWORK.  LDWORK >= 2*N*N.
C
C     Error Indicator
C
C     INFO    INTEGER
C             = 0:  successful exit;
C             < 0:  if INFO = -i, the i-th argument had an illegal 
C                   value;
C             = N+1:  if the matrices T and -T' have common or very
C                   close eigenvalues; perturbed values were used to
C                   solve Lyapunov equations (but the matrix T is
C                   unchanged).
C
C     METHOD
C
C     The forward error bound is estimated using a practical error bound
C     similar to the one proposed in [1], based on the 1-norm estimator
C     in [2].
C
C     REFERENCES
C
C     [1] Higham, N.J.
C         Perturbation theory and backward error for AX-XB=C.
C         BIT, vol. 33, pp. 124-136, 1993.
C
C     [2] Higham, N.J.
C         FORTRAN codes for estimating the one-norm of a real or
C         complex matrix, with applications to condition estimation.
C         ACM Trans. Math. Softw., 14, pp. 381-396, 1988.
C
C     NUMERICAL ASPECTS
C                               3
C     The algorithm requires 0(N ) operations.
C
C     FURTHER COMMENTS
C
C     The option LYAPUN = 'R' may occasionally produce slightly worse
C     or better estimates, and it is much faster than the option 'O'.
C     The routine can be also used as a final step in estimating a
C     forward error bound for the solution of a continuous-time
C     algebraic matrix Riccati equation.
C
C     CONTRIBUTOR
C
C     V. Sima, Research Institute for Informatics, Bucharest, Romania,
C     Oct. 1998. Partly based on DGLSVX (and then SB03QD) by P. Petkov,
C     Tech. University of Sofia, March 1998 (and December 1998).
C
C     REVISIONS
C
C     February 6, 1999, V. Sima, Katholieke Univ. Leuven, Belgium.   
C
C     KEYWORDS
C
C     Lyapunov equation, orthogonal transformation, real Schur form.
C
C     ******************************************************************
C
C     .. Parameters ..
      DOUBLE PRECISION   ZERO, ONE
      PARAMETER          ( ZERO = 0.0D+0, ONE = 1.0D+0 )
C     ..
C     .. Scalar Arguments ..
      CHARACTER          LYAPUN, TRANA, UPLO
      INTEGER            INFO, LDR, LDT, LDU, LDWORK, N
      DOUBLE PRECISION   FERR, XANORM
C     ..
C     .. Array Arguments ..
      INTEGER            IWORK( * )
      DOUBLE PRECISION   DWORK( * ), R( LDR, * ), T( LDT, * ),
     $                   U( LDU, * )
C     ..
C     .. Local Scalars ..
      LOGICAL            LOWER, NOTRNA, UPDATE
      CHARACTER          TRANAT, UPLOW
      INTEGER            I, IJ, INFO2, ITMP, J, KASE, NN
      DOUBLE PRECISION   EST, SCALE, TEMP
C     ..
C     .. External Functions ..
      LOGICAL            LSAME
      DOUBLE PRECISION   DLANSY  
      EXTERNAL           DLANSY, LSAME
C     ..
C     .. External Subroutines ..
      EXTERNAL           DLACON, MA02ED, MB01RU, SB03MY, XERBLA
C     ..
C     .. Intrinsic Functions ..
      INTRINSIC          MAX
C     ..
C     .. Executable Statements ..
C
C     Decode and Test input parameters.
C
      NOTRNA = LSAME( TRANA,  'N' )
      UPDATE = LSAME( LYAPUN, 'O' )
C
      NN   = N*N
      INFO = 0
      IF( .NOT.( NOTRNA .OR. LSAME( TRANA, 'T' ) .OR.
     $                       LSAME( TRANA, 'C' ) ) ) THEN
         INFO = -1           
      ELSE IF( .NOT.( LSAME( UPLO, 'L' ) .OR. LSAME( UPLO, 'U' ) ) ) 
     $   THEN
         INFO = -2           
      ELSE IF( .NOT.( UPDATE .OR. LSAME( LYAPUN, 'R' ) ) ) THEN
         INFO = -3           
      ELSE IF( N.LT.0 ) THEN
         INFO = -4
      ELSE IF( XANORM.LT.ZERO ) THEN
         INFO = -5
      ELSE IF( LDT.LT.MAX( 1, N ) ) THEN
         INFO = -7
      ELSE IF( LDU.LT.1 .OR. ( UPDATE .AND. LDU.LT.N ) ) THEN
         INFO = -9
      ELSE IF( LDR.LT.MAX( 1, N ) ) THEN
         INFO = -11
      ELSE IF( LDWORK.LT.2*NN ) THEN
         INFO = -15
      END IF
C
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'SB03QX', -INFO )
         RETURN
      END IF
C
C     Quick return if possible.
C
      FERR = ZERO
      IF( N.EQ.0 .OR. XANORM.EQ.ZERO )
     $   RETURN
C
      ITMP = NN + 1
C
      IF( NOTRNA ) THEN
         TRANAT = 'T'
      ELSE
         TRANAT = 'N'
      END IF
C
C     Fill in the remaining triangle of the symmetric residual matrix.
C        
      CALL MA02ED( UPLO, N, R, LDR )
C
      KASE = 0
C
C     REPEAT
   10 CONTINUE
      CALL DLACON( NN, DWORK( ITMP ), DWORK, IWORK, EST, KASE )
      IF( KASE.NE.0 ) THEN
C
C        Select the triangular part of symmetric matrix to be used.
C        
         IF( DLANSY( '1-norm', 'Upper', N, DWORK, N, DWORK( ITMP ) ) 
     $       .GE.
     $       DLANSY( '1-norm', 'Lower', N, DWORK, N, DWORK( ITMP ) )
     $     ) THEN
            UPLOW = 'U'
            LOWER = .FALSE.
         ELSE
            UPLOW = 'L'
            LOWER = .TRUE.
         END IF
C        
         IF( KASE.EQ.2 ) THEN
            IJ = 0
            IF( LOWER ) THEN
C
C              Scale the lower triangular part of symmetric matrix
C              by the residual matrix.
C
               DO 30 J = 1, N
                  DO 20 I = J, N
                     IJ = IJ + 1
                     DWORK( IJ ) = DWORK( IJ )*R( I, J )
   20             CONTINUE
                  IJ = IJ + J
   30          CONTINUE
            ELSE
C
C              Scale the upper triangular part of symmetric matrix
C              by the residual matrix.
C
               DO 50 J = 1, N
                  DO 40 I = 1, J
                     IJ = IJ + 1
                     DWORK( IJ ) = DWORK( IJ )*R( I, J )
   40             CONTINUE
                  IJ = IJ + N - J
   50          CONTINUE
            END IF
         END IF
C
         IF( UPDATE ) THEN
C
C           Transform the right-hand side: RHS := U'*RHS*U.
C           
            CALL MB01RU( UPLOW, 'Transpose', N, N, ZERO, ONE, DWORK, N,
     $                   U, LDU, DWORK, N, DWORK( ITMP ), NN, INFO2 )
         END IF
         CALL MA02ED( UPLOW, N, DWORK, N )
C
         IF( KASE.EQ.2 ) THEN
C
C           Solve op(T)'*Y + Y*op(T) = scale*RHS.
C
            CALL SB03MY( TRANA, N, T, LDT, DWORK, N, SCALE, INFO2 )
         ELSE
C
C           Solve op(T)*W + W*op(T)' = scale*RHS.
C
            CALL SB03MY( TRANAT, N, T, LDT, DWORK, N, SCALE, INFO2 )
         END IF
C
         IF( INFO2.GT.0 )
     $      INFO = N + 1
C
         IF( UPDATE ) THEN
C
C           Transform back to obtain the solution: Z := U*Z*U', with
C           Z = Y or Z = W.
C           
            CALL MB01RU( UPLOW, 'No transpose', N, N, ZERO, ONE, DWORK,
     $                   N, U, LDU, DWORK, N, DWORK( ITMP ), NN, INFO2 )
         END IF
C
         IF( KASE.EQ.1 ) THEN
            IJ = 0
            IF( LOWER ) THEN
C
C              Scale the lower triangular part of symmetric matrix
C              by the residual matrix.
C
               DO 70 J = 1, N
                  DO 60 I = J, N
                     IJ = IJ + 1
                     DWORK( IJ ) = DWORK( IJ )*R( I, J )
   60             CONTINUE
                  IJ = IJ + J
   70          CONTINUE
            ELSE
C
C              Scale the upper triangular part of symmetric matrix
C              by the residual matrix.
C
               DO 90 J = 1, N
                  DO 80 I = 1, J
                     IJ = IJ + 1
                     DWORK( IJ ) = DWORK( IJ )*R( I, J )
   80             CONTINUE
                  IJ = IJ + N - J
   90          CONTINUE
            END IF
         END IF
C
C        Fill in the remaining triangle of the symmetric matrix.
C        
         CALL MA02ED( UPLOW, N, DWORK, N )
         GO TO 10
      END IF
C
C     UNTIL KASE = 0
C
C     Compute the estimate of the relative error.
C
      TEMP = XANORM*SCALE
      IF( TEMP.GT.EST ) THEN
         FERR = EST / TEMP
      ELSE
         FERR = ONE
      END IF
C
      RETURN
C
C *** Last line of SB03QX ***
      END
