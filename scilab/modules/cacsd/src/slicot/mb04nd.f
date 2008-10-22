      SUBROUTINE MB04ND( UPLO, N, M, P, R, LDR, A, LDA, B, LDB, C, LDC,
     $                   TAU, DWORK )
C
C     RELEASE 4.0, WGS COPYRIGHT 1999.
C
C     PURPOSE
C
C     To calculate an RQ factorization of the first block row and
C     apply the orthogonal transformations (from the right) also to the
C     second block row of a structured matrix, as follows
C                              _
C       [ A   R ]        [ 0   R ]
C       [       ] * Q' = [ _   _ ]
C       [ C   B ]        [ C   B ]
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
C             The number of rows of the matrices B and C.  M >= 0.
C
C     P       (input) INTEGER
C             The number of columns of the matrices A and C.  P >= 0.
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
C     A       (input/output) DOUBLE PRECISION array, dimension (LDA,P)
C             On entry, if UPLO = 'F', the leading N-by-P part of this
C             array must contain the matrix A. For UPLO = 'U', if
C             N <= P, the upper triangle of the subarray A(1:N,P-N+1:P)
C             must contain the N-by-N upper triangular matrix A, and if
C             N >= P, the elements on and above the (N-P)-th subdiagonal
C             must contain the N-by-P upper trapezoidal matrix A.
C             On exit, if UPLO = 'F', the leading N-by-P part of this
C             array contains the trailing components (the vectors v, see
C             METHOD) of the elementary reflectors used in the
C             factorization. If UPLO = 'U', the upper triangle of the
C             subarray A(1:N,P-N+1:P) (if N <= P), or the elements on
C             and above the (N-P)-th subdiagonal (if N >= P), contain
C             the trailing components (the vectors v, see METHOD) of the
C             elementary reflectors used in the factorization.
C             The remaining elements are not referenced.
C
C     LDA     INTEGER
C             The leading dimension of array A.  LDA >= MAX(1,N).
C
C     B       (input/output) DOUBLE PRECISION array, dimension (LDB,N)
C             On entry, the leading M-by-N part of this array must
C             contain the matrix B. 
C             On exit, the leading M-by-N part of this array contains
C                                 _
C             the computed matrix B.
C
C     LDB     INTEGER
C             The leading dimension of array B.  LDB >= MAX(1,M).
C
C     C       (input/output) DOUBLE PRECISION array, dimension (LDC,P)
C             On entry, the leading M-by-P part of this array must
C             contain the matrix C.
C             On exit, the leading M-by-P part of this array contains
C                                 _
C             the computed matrix C.
C
C     LDC     INTEGER
C             The leading dimension of array C.  LDC >= MAX(1,M).
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
C     where v  is a P-vector, if UPLO = 'F', or a min(N-i+1,P)-vector, 
C            i
C     if UPLO = 'U'.  The components of v  are stored in the i-th row
C                                        i
C     of A, and tau  is stored in TAU(i), i = N,N-1,...,1.
C                  i
C     In-line code for applying Householder transformations is used
C     whenever possible (see MB04NY routine).
C  
C     NUMERICAL ASPECTS
C
C     The algorithm is backward stable.
C
C     CONTRIBUTORS
C
C     V. Sima, Katholieke Univ. Leuven, Belgium, Apr. 1998.
C
C     REVISIONS
C
C     -
C
C     KEYWORDS
C
C     Elementary reflector, RQ factorization, orthogonal transformation.
C
C     ******************************************************************
C
C     .. Parameters ..
      DOUBLE PRECISION  ZERO, ONE
      PARAMETER         ( ZERO = 0.0D0, ONE = 1.0D0 )
C     .. Scalar Arguments ..
      CHARACTER         UPLO
      INTEGER           LDA, LDB, LDC, LDR, M, N, P
C     .. Array Arguments ..
      DOUBLE PRECISION  A(LDA,*), B(LDB,*), C(LDC,*), DWORK(*),
     $                  R(LDR,*), TAU(*)
C     .. Local Scalars ..
      LOGICAL           LUPLO
      INTEGER           I, IM, IP
C     .. External Functions ..
      LOGICAL           LSAME
      EXTERNAL          LSAME
C     .. External Subroutines ..
      EXTERNAL          DLARFG, MB04NY
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
         DO 10 I = N, 1, -1
C
C           Annihilate the I-th row of A and apply the transformations
C           to the entire block matrix, exploiting its structure.
C
            IM = MIN( N-I+1, P )
            IP = MAX( P-N+I, 1 )
            CALL DLARFG( IM+1, R(I,I), A(I,IP), LDA, TAU(I) )
C
C           Compute
C                                                [ 1 ]
C           w := [ R(1:I-1,I)  A(1:I-1,IP:P) ] * [   ],
C                                                [ v ]
C
C           [ R(1:I-1,I)  A(1:I-1,IP:P) ] =
C           [ R(1:I-1,I)  A(1:I-1,IP:P) ] - tau * w * [ 1 v' ].
C
            IF ( I.GT.0 )
C
     $         CALL MB04NY( I-1, IM, A(I,IP), LDA, TAU(I), R(1,I), LDR,
     $                      A(1,IP), LDA, DWORK )
C
C           Compute
C                                        [ 1 ]
C           w := [ B(:,I)  C(:,IP:P) ] * [   ],
C                                        [ v ]
C
C           [ B(:,I)  C(:,IP:P) ] = [ B(:,I)  C(:,IP:P) ] -
C                                   tau * w * [ 1 v' ].
C
            IF ( M.GT.0 )
     $         CALL MB04NY( M, IM, A(I,IP), LDA, TAU(I), B(1,I), LDB,
     $                      C(1,IP), LDC, DWORK )
   10    CONTINUE
C
      ELSE
C
         DO 20 I = N, 2 , -1
C
C           Annihilate the I-th row of A and apply the transformations
C           to the first block row, exploiting its structure.
C
            CALL DLARFG( P+1, R(I,I), A(I,1), LDA, TAU(I) )
C
C           Compute
C                                             [ 1 ]
C           w := [ R(1:I-1,I)  A(1:I-1,:) ] * [   ],
C                                             [ v ]
C
C           [ R(1:I-1,I)  A(1:I-1,:) ] = [ R(1:I-1,I)  A(1:I-1,:) ] -
C                                        tau * w * [ 1 v' ].
C
            CALL MB04NY( I-1, P, A(I,1), LDA, TAU(I), R(1,I), LDR, A,
     $                   LDA, DWORK )
   20    CONTINUE
C
         CALL DLARFG( P+1, R(1,1), A(1,1), LDA, TAU(1) )
         IF ( M.GT.0 ) THEN
C
C           Apply the transformations to the second block row.
C
            DO 30 I = N, 1, -1
C
C              Compute
C                                   [ 1 ]
C              w := [ B(:,I)  C ] * [   ],
C                                   [ v ]
C
C              [ B(:,I)  C ] = [ B(:,I)  C ] - tau * w * [ 1 v' ].
C
               CALL MB04NY( M, P, A(I,1), LDA, TAU(I), B(1,I), LDB, C,
     $                      LDC, DWORK )
   30       CONTINUE
C
         END IF
      END IF
      RETURN
C *** Last line of MB04ND ***
      END
