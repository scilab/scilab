      SUBROUTINE MB04KD( UPLO, N, M, P, R, LDR, A, LDA, B, LDB, C, LDC,
     $                   TAU, DWORK )
C
C     RELEASE 3.0, WGS COPYRIGHT 1997.
C
C     PURPOSE
C
C     To calculate a QR factorization of the first block column and
C     apply the orthogonal transformations (from the left) also to the
C     second block column of a structured matrix, as follows
C                          _
C            [ R   0 ]   [ R   C ]
C       Q' * [       ] = [       ]
C            [ A   B ]   [ 0   D ]
C                 _
C     where R and R are upper triangular. The matrix A can be full or
C     upper trapezoidal/triangular. The problem structure is exploited.
C     This computation is useful, for instance, in combined measurement
C     and time update of one iteration of the Kalman filter (square 
C     root information filter).
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
C             The number of columns of the matrices B, C and D.  M >= 0.
C
C     P       (input) INTEGER
C             The number of rows of the matrices A, B and D.  P >= 0.
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
C             On entry, the leading P-by-M part of this array must 
C             contain the matrix B. 
C             On exit, the leading P-by-M part of this array contains
C             the computed matrix D.
C
C     LDB     INTEGER
C             The leading dimension of array B.  LDB >= MAX(1,P).
C
C     C       (output) DOUBLE PRECISION array, dimension (LDC,M)
C             The leading N-by-M part of this array contains the
C             computed matrix C.
C
C     LDC     INTEGER
C             The leading dimension of array C.  LDC >= MAX(1,N).
C
C     TAU     (output) DOUBLE PRECISION array, dimension (N)
C             The scalar factors of the elementary reflectors used.
C
C     Workspace
C
C     DWORK   DOUBLE PRECISION array, dimension (N)
C
C     METHOD
C
C     The routine uses N Householder transformations exploiting the zero
C     pattern of the block matrix.  A Householder matrix has the form
C
C                                     ( 1 ),
C        H  = I - tau *u *u',    u  = ( v ) 
C         i          i  i  i      i   (  i)  
C                                     
C     where v  is a P-vector, if UPLO = 'F', or an min(i,P)-vector, if 
C            i
C     UPLO = 'U'.  The components of v  are stored in the i-th column
C                                     i
C     of A, and tau  is stored in TAU(i).
C                  i
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
C     -
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
      EXTERNAL          DAXPY, DCOPY, DGEMV, DGER, DLARFG, DSCAL
C     .. Intrinsic Functions ..
      INTRINSIC         MIN
C     .. Executable Statements ..
C
      IF( MIN( N, P ).EQ.0 )
     $   RETURN
C
      LUPLO = LSAME( UPLO, 'U' ) 
      IM = P
C
      DO 10 I = 1, N
C
C        Annihilate the I-th column of A and apply the transformations
C        to the entire block matrix, exploiting its structure.
C     
         IF( LUPLO ) IM = MIN( I, P )
         CALL DLARFG( IM+1, R(I,I), A(1,I), 1, TAU(I) )
         IF( TAU(I).NE.ZERO ) THEN
C              
C                                      [ R(I,I+1:N)        0     ]
C           [ w C(I,:) ] := [ 1 v' ] * [                         ]
C                                      [ A(1:IM,I+1:N) B(1:IM,:) ]
C           
            IF( I.LT.N ) THEN
               CALL DCOPY( N-I, R(I,I+1), LDR, DWORK, 1 )
               CALL DGEMV( 'Transpose', IM, N-I, ONE, A(1,I+1), LDA,
     $                     A(1,I), 1, ONE, DWORK, 1 )
            END IF
            CALL DGEMV( 'Transpose', IM, M, ONE, B, LDB, A(1,I), 1,
     $                  ZERO, C(I,1), LDC )
C           
C           [ R(I,I+1:N)      C(I,:)  ]    [ R(I,I+1:N)        0     ]
C           [                         ] := [                         ]
C           [ A(1:IM,I+1:N) D(1:IM,:) ]    [ A(1:IM,I+1:N) B(1:IM,:) ]
C           
C                                                 [ 1 ]
C                                         - tau * [   ] * [ w C(I,:) ]
C                                                 [ v ]
C           
            IF( I.LT.N ) THEN
               CALL DAXPY( N-I, -TAU(I), DWORK, 1, R(I,I+1), LDR )
               CALL DGER( IM, N-I, -TAU(I), A(1,I), 1, DWORK, 1, 
     $                    A(1,I+1), LDA )
            END IF
            CALL DSCAL( M, -TAU(I), C(I,1), LDC )
            CALL DGER( IM, M, ONE, A(1,I), 1, C(I,1), LDC, B, LDB )
         END IF
   10 CONTINUE
C
      RETURN
C *** Last line of MB04KD ***
      END
