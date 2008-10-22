      SUBROUTINE SB02RU( DICO, HINV, TRANA, UPLO, N, A, LDA, G, LDG, Q,
     $                   LDQ, S, LDS, IWORK, DWORK, LDWORK, INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 1999.
C
C     PURPOSE
C
C     To construct the 2n-by-2n Hamiltonian or symplectic matrix S
C     associated to the linear-quadratic optimization problem, used to
C     solve the continuous- or discrete-time algebraic Riccati equation,
C     respectively.
C
C     For a continuous-time problem, S is defined by
C
C             ( op(A)   -G    )
C         S = (               ),                                     (1)
C             (  -Q   -op(A)' )
C
C     and for a discrete-time problem by
C
C                     -1              -1
C             (  op(A)           op(A)  *G       )
C         S = (        -1                   -1   ),                  (2)
C             ( Q*op(A)     op(A)' + Q*op(A)  *G )
C
C     or
C                              -T             -T
C             ( op(A) + G*op(A)  *Q   -G*op(A)   )
C         S = (           -T                 -T  ),                  (3)
C             (     -op(A)  *Q          op(A)    )
C
C     where op(A) = A or A' (A**T), A, G, and Q are n-by-n matrices,
C     with G and Q symmetric. Matrix A must be nonsingular in the
C     discrete-time case.
C
C     ARGUMENTS
C
C     Mode Parameters
C
C     DICO    CHARACTER*1
C             Specifies the type of the system as follows:
C             = 'C':  Continuous-time system;
C             = 'D':  Discrete-time system.
C
C     HINV    CHARACTER*1
C             If DICO = 'D', specifies which of the matrices (2) or (3)
C             is constructed, as follows:
C             = 'D':  The matrix S in (2) is constructed;
C             = 'I':  The (inverse) matrix S in (3) is constructed.
C             HINV is not referenced if DICO = 'C'.
C
C     TRANA   CHARACTER*1
C             Specifies the form of op(A) to be used, as follows:
C             = 'N':  op(A) = A    (No transpose);
C             = 'T':  op(A) = A**T (Transpose);
C             = 'C':  op(A) = A**T (Conjugate transpose = Transpose).
C
C     UPLO    CHARACTER*1
C             Specifies which triangle of the matrices G and Q is
C             stored, as follows:
C             = 'U':  Upper triangle is stored;
C             = 'L':  Lower triangle is stored.
C
C     Input/Output Parameters
C
C     N       (input) INTEGER
C             The order of the matrices A, G, and Q.  N >= 0.
C
C     A       (input) DOUBLE PRECISION array, dimension (LDA,N)
C             The leading N-by-N part of this array must contain the
C             matrix A.
C
C     LDA     INTEGER
C             The leading dimension of the array A.  LDA >= MAX(1,N).
C
C     G       (input/output) DOUBLE PRECISION array, dimension (LDG,N)
C             On entry, the leading N-by-N upper triangular part (if
C             UPLO = 'U') or lower triangular part (if UPLO = 'L') of
C             this array must contain the upper triangular part or lower
C             triangular part, respectively, of the symmetric matrix G.
C             On exit, if DICO = 'D', the leading N-by-N part of this
C             array contains the symmetric matrix G fully stored.
C             If DICO = 'C', this array is not modified on exit, and the
C             strictly lower triangular part (if UPLO = 'U') or strictly
C             upper triangular part (if UPLO = 'L') is not referenced.
C
C     LDG     INTEGER
C             The leading dimension of the array G.  LDG >= MAX(1,N).
C
C     Q       (input/output) DOUBLE PRECISION array, dimension (LDQ,N)
C             On entry, the leading N-by-N upper triangular part (if
C             UPLO = 'U') or lower triangular part (if UPLO = 'L') of
C             this array must contain the upper triangular part or lower
C             triangular part, respectively, of the symmetric matrix Q.
C             On exit, if DICO = 'D', the leading N-by-N part of this
C             array contains the symmetric matrix Q fully stored.
C             If DICO = 'C', this array is not modified on exit, and the
C             strictly lower triangular part (if UPLO = 'U') or strictly
C             upper triangular part (if UPLO = 'L') is not referenced.
C
C     LDQ     INTEGER
C             The leading dimension of the array Q.  LDQ >= MAX(1,N).
C
C     S       (output) DOUBLE PRECISION array, dimension (LDS,2*N)
C             If INFO = 0, the leading 2N-by-2N part of this array
C             contains the Hamiltonian or symplectic matrix of the
C             problem.
C
C     LDS     INTEGER
C             The leading dimension of the array S.  LDS >= MAX(1,2*N).
C
C     Workspace
C
C     IWORK   INTEGER array, dimension (LIWORK), where
C             LIWORK >= 0,   if DICO = 'C';
C             LIWORK >= 2*N, if DICO = 'D'.
C
C     DWORK   DOUBLE PRECISION array, dimension (LDWORK)
C             On exit, if DICO = 'D', DWORK(1) returns the reciprocal
C             condition number  RCOND  of the given matrix  A,  and
C             DWORK(2) returns the reciprocal pivot growth factor
C             norm(A)/norm(U) (see SLICOT Library routine MB02PD).
C             If DWORK(2) is much less than 1, then the computed  S
C             and  RCOND  could be unreliable. If 0 < INFO <= N, then
C             DWORK(2) contains the reciprocal pivot growth factor for
C             the leading INFO columns of  A.
C
C     LDWORK  INTEGER
C             The length of the array DWORK.
C             LDWORK >= 0,          if DICO = 'C';
C             LDWORK >= MAX(2,6*N), if DICO = 'D'.
C
C     Error Indicator
C
C     INFO    INTEGER
C             = 0:  successful exit;
C             < 0:  if INFO = -i, the i-th argument had an illegal
C                   value;
C             = i:  if the leading i-by-i (1 <= i <= N) upper triangular
C                   submatrix of A is singular in discrete-time case;
C             = N+1:  if matrix A is numerically singular in discrete-
C                   time case.
C
C     METHOD
C
C     For a continuous-time problem, the 2n-by-2n Hamiltonian matrix (1)
C     is constructed.
C     For a discrete-time problem, the 2n-by-2n symplectic matrix (2) or
C     (3) - the inverse of the matrix in (2) - is constructed.
C
C     NUMERICAL ASPECTS
C
C     The discrete-time case needs the inverse of the matrix A, hence
C     the routine should not be used when A is ill-conditioned.
C                               3  
C     The algorithm requires 0(n ) floating point operations in the
C     discrete-time case.
C
C     FURTHER COMMENTS
C
C     This routine is a functionally extended and with improved accuracy
C     version of the SLICOT Library routine SB02MU. Transposed problems
C     can be dealt with as well. The LU factorization of  op(A)  (with
C     no equilibration) and iterative refinement are used for solving
C     the various linear algebraic systems involved.
C
C     CONTRIBUTOR
C
C     V. Sima, Research Institute for Informatics, Bucharest, Apr. 1999.
C
C     REVISIONS
C
C     -
C
C     KEYWORDS
C
C     Algebraic Riccati equation, closed loop system, continuous-time
C     system, discrete-time system, optimal regulator, Schur form.
C
C     ******************************************************************
C
C     .. Parameters ..
      DOUBLE PRECISION  ZERO, ONE
      PARAMETER         ( ZERO = 0.0D0, ONE = 1.0D0 )
C     .. Scalar Arguments ..
      CHARACTER         DICO, HINV, TRANA, UPLO
      INTEGER           INFO, LDA, LDG, LDQ, LDS, LDWORK, N
C     .. Array Arguments ..
      INTEGER           IWORK(*)
      DOUBLE PRECISION  A(LDA,*), DWORK(*), G(LDG,*), Q(LDQ,*),
     $                  S(LDS,*)
C     .. Local Scalars ..
      CHARACTER         EQUED, TRANAT
      LOGICAL           DISCR, LHINV, LUPLO, NOTRNA
      INTEGER           I, J, N2, NJ, NP1
      DOUBLE PRECISION  PIVOTG, RCOND, RCONDA, TEMP
C     .. External Functions ..
      LOGICAL           LSAME
      EXTERNAL          LSAME
C     .. External Subroutines ..
      EXTERNAL          DCOPY, DGEMM, DLACPY, DLASET, DSWAP, MA02AD,
     $                  MA02ED, MB02PD, XERBLA
C     .. Intrinsic Functions ..
      INTRINSIC         MAX
C     .. Executable Statements ..
C
      N2 = N + N
      INFO = 0
      DISCR  = LSAME( DICO,  'D' )
      LUPLO  = LSAME( UPLO,  'U' )
      NOTRNA = LSAME( TRANA, 'N' )
      IF( DISCR )
     $   LHINV = LSAME( HINV, 'D' )
C
C     Test the input scalar arguments.
C
      IF( .NOT.DISCR .AND. .NOT.LSAME( DICO, 'C' ) ) THEN
         INFO = -1
      ELSE IF( DISCR ) THEN
         IF( .NOT.LHINV .AND. .NOT.LSAME( HINV, 'I' ) )
     $      INFO = -2
      ELSE IF( INFO.EQ.0 ) THEN
         IF( .NOT.NOTRNA .AND. .NOT.LSAME( TRANA, 'T' )
     $                   .AND. .NOT.LSAME( TRANA, 'C' ) ) THEN
            INFO = -3
         ELSE IF( .NOT.LUPLO .AND. .NOT.LSAME( UPLO, 'L' ) ) THEN
            INFO = -4
         ELSE IF( N.LT.0 ) THEN
            INFO = -5
         ELSE IF( LDA.LT.MAX( 1, N ) ) THEN
            INFO = -7
         ELSE IF( LDG.LT.MAX( 1, N ) ) THEN
            INFO = -9
         ELSE IF( LDQ.LT.MAX( 1, N ) ) THEN
            INFO = -11
         ELSE IF( LDS.LT.MAX( 1, N2 ) ) THEN
            INFO = -13
         ELSE IF( ( LDWORK.LT.0 ) .OR.
     $            ( DISCR .AND. LDWORK.LT.MAX( 2, 6*N ) ) ) THEN
            INFO = -16
         END IF
      END IF
C
      IF ( INFO.NE.0 ) THEN
C
C        Error return.
C
         CALL XERBLA( 'SB02RU', -INFO )
         RETURN
      END IF
C
C     Quick return if possible.
C
      IF ( N.EQ.0 ) THEN
         IF ( DISCR ) THEN
            DWORK(1) = ONE
            DWORK(2) = ONE
         END IF
         RETURN
      END IF
C
C     The code tries to exploit data locality as much as possible,
C     assuming that LDS is greater than LDA, LDQ, and/or LDG.
C
      IF ( .NOT.DISCR ) THEN
C
C        Continuous-time case: Construct Hamiltonian matrix column-wise.
C
C        Copy op(A) in S(1:N,1:N), and construct full Q
C        in S(N+1:2*N,1:N) and change the sign.
C        
         DO 100 J = 1, N
            IF ( NOTRNA ) THEN
               CALL DCOPY( N, A(1,J), 1, S(1,J), 1 )
            ELSE
               CALL DCOPY( N, A(J,1), LDA, S(1,J), 1 )
            END IF
C        
            IF ( LUPLO ) THEN
C
               DO 20 I = 1, J
                  S(N+I,J) = -Q(I,J)
   20          CONTINUE
C
               DO 40 I = J + 1, N
                  S(N+I,J) = -Q(J,I)
   40          CONTINUE
C
            ELSE
C
               DO 60 I = 1, J - 1
                  S(N+I,J) = -Q(J,I)
   60          CONTINUE
C
               DO 80 I = J, N
                  S(N+I,J) = -Q(I,J)
   80          CONTINUE
C
            END IF
  100    CONTINUE
C
C        Construct full G in S(1:N,N+1:2*N) and change the sign, and
C        construct -op(A)' in S(N+1:2*N,N+1:2*N).
C        
         DO 240 J = 1, N
            NJ = N + J
            IF ( LUPLO ) THEN
C
               DO 120 I = 1, J
                  S(I,NJ) = -G(I,J)
  120          CONTINUE
C
               DO 140 I = J + 1, N
                  S(I,NJ) = -G(J,I)
  140          CONTINUE
C
            ELSE
C
               DO 160 I = 1, J - 1
                  S(I,NJ) = -G(J,I)
  160          CONTINUE
C
               DO 180 I = J, N
                  S(I,NJ) = -G(I,J)
  180          CONTINUE
C
            END IF
C
            IF ( NOTRNA ) THEN
C
               DO 200 I = 1, N
                  S(N+I,NJ) = -A(J,I)
  200          CONTINUE
C
            ELSE
C
               DO 220 I = 1, N
                  S(N+I,NJ) = -A(I,J)
  220          CONTINUE
C
            END IF
  240    CONTINUE
C
      ELSE
C
C        Discrete-time case: Construct the symplectic matrix (2) or (3).
C
C        Fill in the remaining triangles of the symmetric matrices Q
C        and G.
C
         CALL MA02ED( UPLO, N, Q, LDQ )
         CALL MA02ED( UPLO, N, G, LDG )
C
C        Prepare the construction of S in (2) or (3).
C
         NP1 = N + 1
         IF ( NOTRNA ) THEN
            TRANAT = 'T'
         ELSE
            TRANAT = 'N'
         END IF
C
C        Solve  op(A)'*X = Q  in  S(N+1:2*N,1:N),  using the LU
C        factorization of  op(A),  obtained in  S(1:N,1:N),  and
C        iterative refinement. No equilibration of  A  is used.
C        Workspace:  6*N.
C
         CALL MB02PD( 'No equilibration', TRANAT, N, N, A, LDA, S,
     $                LDS, IWORK, EQUED, DWORK, DWORK, Q, LDQ,
     $                S(NP1,1), LDS, RCOND, DWORK, DWORK(NP1),
     $                IWORK(NP1), DWORK(N2+1), INFO )
C
C        Return if the matrix is exactly singular or singular to
C        working precision.
C
         IF( INFO.GT.0 ) THEN
            DWORK(1) = RCOND
            DWORK(2) = DWORK(N2+1)
            RETURN
         END IF
C
         RCONDA = RCOND
         PIVOTG = DWORK(N2+1)
C
         IF ( LHINV ) THEN
C
C           Complete the construction of S in (2).
C
C           Transpose  X  in-situ.
C
            DO 260 J = 1, N - 1
               CALL DSWAP( N-J, S(NP1+J,J), 1, S(N+J,J+1), LDS )
  260       CONTINUE
C
C           Solve  op(A)*X = I_n  in  S(N+1:2*N,N+1:2*N),  using the LU
C           factorization of  op(A),  computed in  S(1:N,1:N),  and
C           iterative refinement.
C
            CALL DLASET( 'Full', N, N, ZERO, ONE, S(1,NP1), LDS )
            CALL MB02PD( 'Factored', TRANA, N, N, A, LDA, S, LDS, IWORK,
     $                   EQUED, DWORK, DWORK, S(1,NP1), LDS, S(NP1,NP1),
     $                   LDS, RCOND, DWORK, DWORK(NP1), IWORK(NP1),
     $                   DWORK(N2+1), INFO )
C
C           Solve  op(A)*X = G  in  S(1:N,N+1:2*N),  using the LU
C           factorization of  op(A),  computed in  S(1:N,1:N),  and
C           iterative refinement.
C
            CALL MB02PD( 'Factored', TRANA, N, N, A, LDA, S, LDS, IWORK,
     $                   EQUED, DWORK, DWORK, G, LDG, S(1,NP1), LDS,
     $                   RCOND, DWORK, DWORK(NP1), IWORK(NP1),
     $                   DWORK(N2+1), INFO )
C
C                      -1
C           Copy  op(A)    from  S(N+1:2*N,N+1:2*N)  in  S(1:N,1:N).
C
            CALL DLACPY( 'Full', N, N, S(NP1,NP1), LDS, S, LDS )
C
C                                    -1
C           Compute  op(A)' + Q*op(A)  *G  in  S(N+1:2*N,N+1:2*N).
C
            IF ( NOTRNA ) THEN
               CALL MA02AD( 'Full', N, N, A, LDA, S(NP1,NP1), LDS )
            ELSE
               CALL DLACPY( 'Full', N, N, A, LDA, S(NP1,NP1), LDS )
            END IF
            CALL DGEMM( 'No transpose', 'No transpose', N, N, N, ONE,
     $                  Q, LDQ, S(1,NP1), LDS, ONE, S(NP1,NP1), LDS )
C
         ELSE
C
C           Complete the construction of S in (3).
C
C           Change the sign of  X.
C
            DO 300 J = 1, N
C
               DO 280 I = NP1, N2
                  S(I,J) = -S(I,J)
  280          CONTINUE
C
  300       CONTINUE
C
C           Solve  op(A)'*X = I_n  in  S(N+1:2*N,N+1:2*N),  using the LU
C           factorization of  op(A),  computed in  S(1:N,1:N),  and
C           iterative refinement.
C
            CALL DLASET( 'Full', N, N, ZERO, ONE, S(1,NP1), LDS )
            CALL MB02PD( 'Factored', TRANAT, N, N, A, LDA, S, LDS,
     $                   IWORK, EQUED, DWORK, DWORK, S(1,NP1), LDS,
     $                   S(NP1,NP1), LDS, RCOND, DWORK, DWORK(NP1),
     $                   IWORK(NP1), DWORK(N2+1), INFO )
C
C           Solve  op(A)*X' = -G  in  S(1:N,N+1:2*N),  using the LU
C           factorization of  op(A),  obtained in  S(1:N,1:N),  and
C           iterative refinement.
C
            CALL MB02PD( 'Factored', TRANA, N, N, A, LDA, S, LDS, IWORK,
     $                   EQUED, DWORK, DWORK, G, LDG, S(1,NP1), LDS,
     $                   RCOND, DWORK, DWORK(NP1), IWORK(NP1),
     $                   DWORK(N2+1), INFO )
C
C           Change the sign of  X  and transpose it in-situ.
C
            DO 340 J = NP1, N2
C
               DO 320 I = 1, N
                  TEMP   = -S(I,J)
                  S(I,J) = -S(J-N,I+N)
                  S(J-N,I+N) = TEMP
  320          CONTINUE
C
  340       CONTINUE
C                                   -T
C           Compute  op(A) + G*op(A)  *Q  in  S(1:N,1:N).
C
            IF ( NOTRNA ) THEN
               CALL DLACPY( 'Full', N, N, A, LDA, S, LDS )
            ELSE
               CALL MA02AD( 'Full', N, N, A, LDA, S, LDS )
            END IF
            CALL DGEMM( 'No transpose', 'No transpose', N, N, N, -ONE,
     $                  G, LDG, S(NP1,1), LDS, ONE, S, LDS )
C
         END IF
         DWORK(1) = RCONDA
         DWORK(2) = PIVOTG
      END IF
      RETURN
C
C *** Last line of SB02RU ***
      END
