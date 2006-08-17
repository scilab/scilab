      SUBROUTINE MB04OD( UPLO, N, M, P, R, LDR, A, LDA, B, LDB, C, LDC,
     $                   TAU, DWORK )
C
C     RELEASE 4.0, WGS COPYRIGHT 1999.
C
C     PURPOSE
C
C     To calculate a QR factorization of the first block column and
C     apply the orthogonal transformations (from the left) also to the
C     second block column of a structured matrix, as follows
C                          _   _
C            [ R   B ]   [ R   B ]
C       Q' * [       ] = [     _ ]
C            [ A   C ]   [ 0   C ]
C                 _
C     where R and R are upper triangular. The matrix A can be full or
C     upper trapezoidal/triangular. The problem structure is exploited.
C
C     ARGUMENTS
C
C     Mode Parameters
C
C     UPLO    CHARACTER*1
C             Indicates if the matrix A is or not triangular as follows:
C             = 'U':  Matrix A is upper trapezoidal/triangular;
C             = 'F':  Matrix A is full.
C
C     Input/Output Parameters
C
C     N       (input) INTEGER                 _
C             The order of the matrices R and R.  N >= 0.
C
C     M       (input) INTEGER
C             The number of columns of the matrices B and C.  M >= 0.
C
C     P       (input) INTEGER
C             The number of rows of the matrices A and C.  P >= 0.
C
C     R       (input/output) DOUBLE PRECISION array, dimension (LDR,N)
C             On entry, the leading N-by-N upper triangular part of this
C             array must contain the upper triangular matrix R.
C             On exit, the leading N-by-N upper triangular part of this
C                                                        _
C             array contains the upper triangular matrix R.
C             The strict lower triangular part of this array is not
C             referenced.
C
C     LDR     INTEGER
C             The leading dimension of array R.  LDR >= MAX(1,N).
C
C     A       (input/output) DOUBLE PRECISION array, dimension (LDA,N)
C             On entry, if UPLO = 'F', the leading P-by-N part of this 
C             array must contain the matrix A. If UPLO = 'U', the 
C             leading MIN(P,N)-by-N part of this array must contain the
C             upper trapezoidal (upper triangular if P >= N) matrix A,
C             and the elements below the diagonal are not referenced. 
C             On exit, the leading P-by-N part (upper trapezoidal or
C             triangular, if UPLO = 'U') of this array contains the
C             trailing components (the vectors v, see Method) of the
C             elementary reflectors used in the factorization.
C
C     LDA     INTEGER
C             The leading dimension of array A.  LDA >= MAX(1,P).
C
C     B       (input/output) DOUBLE PRECISION array, dimension (LDB,M)
C             On entry, the leading N-by-M part of this array must
C             contain the matrix B. 
C             On exit, the leading N-by-M part of this array contains
C                                 _
C             the computed matrix B.
C
C     LDB     INTEGER
C             The leading dimension of array B.  LDB >= MAX(1,N).
C
C     C       (input/output) DOUBLE PRECISION array, dimension (LDC,M)
C             On entry, the leading P-by-M part of this array must 
C             contain the matrix C.
C             On exit, the leading P-by-M part of this array contains
C                                 _
C             the computed matrix C.
C
C     LDC     INTEGER
C             The leading dimension of array C.  LDC >= MAX(1,P).
C
C     TAU     (output) DOUBLE PRECISION array, dimension (N)
C             The scalar factors of the elementary reflectors used.
C
C     Workspace
C
C     DWORK   DOUBLE PRECISION array, dimension (MAX(N-1,M))
C
C     METHOD
C
C     The routine uses N Householder transformations exploiting the zero
C     pattern of the block matrix.  A Householder matrix has the form
C
C                                     ( 1 )
C        H  = I - tau *u *u',    u  = ( v ),
C         i          i  i  i      i   (  i)  
C                                     
C     where v  is a P-vector, if UPLO = 'F', or a min(i,P)-vector, if 
C            i
C     UPLO = 'U'.  The components of v  are stored in the i-th column
C                                     i
C     of A, and tau  is stored in TAU(i).
C                  i
C     In-line code for applying Householder transformations is used
C     whenever possible (see MB04OY routine).
C  
C     NUMERICAL ASPECTS
C
C     The algorithm is backward stable.
C
C     CONTRIBUTORS
C
C     V. Sima, Katholieke Univ. Leuven, Belgium, Feb. 1997.
C
C     REVISIONS
C
C     Dec. 1997.
C
C     KEYWORDS
C
C     Elementary reflector, QR factorization, orthogonal transformation.
C
C     ******************************************************************
C
C     .. Parameters ..
      DOUBLE PRECISION  ZERO, ONE
      PARAMETER         ( ZERO = 0.0D0, ONE = 1.0D0 )
C     .. Scalar Arguments ..
      CHARACTER*1       UPLO
      INTEGER           LDA, LDB, LDC, LDR, M, N, P
C     .. Array Arguments ..
      DOUBLE PRECISION  A(LDA,*), B(LDB,*), C(LDC,*), DWORK(*),
     $                  R(LDR,*), TAU(*)
C     .. Local Scalars ..
      LOGICAL           LUPLO
      INTEGER           I, IM
C     .. External Functions ..
      LOGICAL           LSAME
      EXTERNAL          LSAME
C     .. External Subroutines ..
      EXTERNAL          DLARFG, MB04OY
C     .. Intrinsic Functions ..
      INTRINSIC         MIN
C     .. Executable Statements ..
C
C     For efficiency reasons, the parameters are not checked.
C
      IF( MIN( N, P ).EQ.0 )
     $   RETURN
C
      LUPLO = LSAME( UPLO, 'U' ) 
      IF ( LUPLO ) THEN
C
         DO 10 I = 1, N
C
C           Annihilate the I-th column of A and apply the
C           transformations to the entire block matrix, exploiting
C           its structure.
C
            IM = MIN( I, P )
            CALL DLARFG( IM+1, R(I,I), A(1,I), 1, TAU(I) )
C
C           Compute
C                           [ R(I,I+1:N)    ]
C           w := [ 1 v' ] * [               ],
C                           [ A(1:IM,I+1:N) ]
C
C           [ R(I,I+1:N)    ]    [ R(I,I+1:N)    ]         [ 1 ]
C           [               ] := [               ] - tau * [   ] * w .
C           [ A(1:IM,I+1:N) ]    [ A(1:IM,I+1:N) ]         [ v ]
C
            IF ( N-I.GT.0 )
     $         CALL MB04OY( IM, N-I, A(1,I), TAU(I), R(I,I+1), LDR,
     $                      A(1,I+1), LDA, DWORK )
C
C           Compute
C                           [  B(I,:)   ]
C           w := [ 1 v' ] * [           ],
C                           [ C(1:IM,:) ]
C
C           [   B(I,:)  ]    [  B(I,:)   ]         [ 1 ]
C           [           ] := [           ] - tau * [   ] * w.
C           [ C(1:IM,:) ]    [ C(1:IM,:) ]         [ v ]
C
C
            IF ( M.GT.0 )
     $         CALL MB04OY( IM, M, A(1,I), TAU(I), B(I,1), LDB, C, LDC,
     $                      DWORK )
   10    CONTINUE
C
      ELSE
C
         DO 20 I = 1, N - 1
C
C           Annihilate the I-th column of A and apply the
C           transformations to the first block column, exploiting its
C           structure.
C
            CALL DLARFG( P+1, R(I,I), A(1,I), 1, TAU(I) )
C
C           Compute
C                           [ R(I,I+1:N) ]
C           w := [ 1 v' ] * [            ],
C                           [ A(:,I+1:N) ]
C
C           [ R(I,I+1:N) ]    [ R(I,I+1:N) ]         [ 1 ]
C           [            ] := [            ] - tau * [   ] * w .
C           [ A(:,I+1:N) ]    [ A(:,I+1:N) ]         [ v ]
C
            CALL MB04OY( P, N-I, A(1,I), TAU(I), R(I,I+1), LDR,
     $                   A(1,I+1), LDA, DWORK )
   20    CONTINUE
C
         CALL DLARFG( P+1, R(N,N), A(1,N), 1, TAU(N) )
         IF ( M.GT.0 ) THEN
C
C           Apply the transformations to the second block column.
C
            DO 30 I = 1, N
C
C              Compute
C                              [ B(I,:) ]
C              w := [ 1 v' ] * [        ],
C                              [   C    ]
C
C              [ B(I,:) ]    [ B(I,:) ]         [ 1 ]
C              [        ] := [        ] - tau * [   ] * w.
C              [   C    ]    [   C    ]         [ v ]
C
               CALL MB04OY( P, M, A(1,I), TAU(I), B(I,1), LDB, C, LDC,
     $                      DWORK )
   30       CONTINUE
C
         END IF
      END IF
      RETURN
C *** Last line of MB04OD ***
      END
