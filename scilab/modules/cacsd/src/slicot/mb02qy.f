      SUBROUTINE MB02QY( M, N, NRHS, RANK, A, LDA, JPVT, B, LDB, TAU,
     $                   DWORK, LDWORK, INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 1999.
C
C     PURPOSE
C
C     To determine the minimum-norm solution to a real linear least
C     squares problem:
C
C         minimize || A * X - B ||,
C
C     using the rank-revealing QR factorization of a real general
C     M-by-N matrix  A,  computed by SLICOT Library routine  MB03OD.
C
C     ARGUMENTS
C
C     Input/Output Parameters
C
C     M       (input) INTEGER
C             The number of rows of the matrices A and B.  M >= 0.
C
C     N       (input) INTEGER
C             The number of columns of the matrix A.  N >= 0.
C
C     NRHS    (input) INTEGER
C             The number of columns of the matrix B.  NRHS >= 0.
C
C     RANK    (input) INTEGER
C             The effective rank of  A,  as returned by SLICOT Library   
C             routine  MB03OD.  min(M,N) >= RANK >= 0.
C
C     A       (input/output) DOUBLE PRECISION array, dimension 
C             ( LDA, N )
C             On entry, the leading min(M,N)-by-N upper trapezoidal
C             part of this array contains the triangular factor  R,  as
C             returned by SLICOT Library routine  MB03OD.  The strict
C             lower trapezoidal part of  A  is not referenced.
C             On exit, if  RANK < N,  the leading  RANK-by-RANK  upper 
C             triangular part of this array contains the upper
C             triangular matrix  R  of the complete orthogonal 
C             factorization of  A,  and the submatrix  (1:RANK,RANK+1:N)
C             of this array, with the array  TAU,  represent the
C             orthogonal matrix  Z  (of the complete orthogonal 
C             factorization of  A),  as a product of  RANK  elementary 
C             reflectors. 
C             On exit, if  RANK = N,  this array is unchanged. 
C
C     LDA     INTEGER
C             The leading dimension of the array A.  LDA >= max(1,M).
C
C     JPVT    (input) INTEGER array, dimension ( N )
C             The recorded permutations performed by SLICOT Library
C             routine  MB03OD;  if  JPVT(i) = k,  then the i-th column
C             of  A*P  was the k-th column of the original matrix  A.
C
C     B       (input/output) DOUBLE PRECISION array, dimension 
C             ( LDB, NRHS )
C             On entry, if  NRHS > 0,  the leading M-by-NRHS part of
C             this array must contain the matrix  B  (corresponding to
C             the transformed matrix  A,  returned by SLICOT Library 
C             routine  MB03OD).
C             On exit, if  NRHS > 0,  the leading N-by-NRHS part of this
C             array contains the solution matrix X.
C             If  M >= N  and  RANK = N,  the residual sum-of-squares
C             for the solution in the i-th column is given by the sum
C             of squares of elements  N+1:M  in that column.
C             If  NRHS = 0,  the array  B  is not referenced.
C
C     LDB     INTEGER
C             The leading dimension of the array B.
C             LDB >= max(1,M,N),  if  NRHS > 0.
C             LDB >= 1,           if  NRHS = 0.
C
C     TAU     (output) DOUBLE PRECISION array, dimension ( min(M,N) )
C             The scalar factors of the elementary reflectors.
C             If  RANK = N,  the array  TAU  is not referenced. 
C
C     Workspace
C
C     DWORK   DOUBLE PRECISION array, dimension ( LDWORK )
C             On exit, if  INFO = 0,  DWORK(1) returns the optimal value
C             of LDWORK.
C
C     LDWORK  INTEGER
C             The length of the array DWORK.
C             LDWORK >= max( 1, N, NRHS ).
C             For good performance,  LDWORK  should sometimes be larger.
C
C     Error Indicator
C
C     INFO    INTEGER
C             = 0:  successful exit;
C             < 0:  if INFO = -i, the i-th argument had an illegal
C                   value.
C
C     METHOD
C
C     The routine uses a QR factorization with column pivoting:
C
C        A * P = Q * R = Q * [ R11 R12 ],
C                            [  0  R22 ]
C
C     where  R11  is an upper triangular submatrix of estimated rank
C     RANK,  the effective rank of  A.  The submatrix  R22  can be
C     considered as negligible.
C
C     If  RANK < N,  then  R12  is annihilated by orthogonal
C     transformations from the right, arriving at the complete
C     orthogonal factorization:
C
C        A * P = Q * [ T11 0 ] * Z.
C                    [  0  0 ]
C
C     The minimum-norm solution is then
C
C        X = P * Z' [ inv(T11)*Q1'*B ],
C                   [        0       ]
C
C     where Q1 consists of the first  RANK  columns of Q.
C
C     The input data for  MB02QY  are the transformed matrices  Q' * A
C     (returned by SLICOT Library routine  MB03OD)  and  Q' * B.  
C     Matrix  Q  is not needed.
C
C     NUMERICAL ASPECTS
C
C     The implemented method is numerically stable.
C
C     CONTRIBUTOR
C
C     V. Sima, Research Institute for Informatics, Bucharest, Aug. 1999.
C
C     REVISIONS
C
C     -
C
C     KEYWORDS
C
C     Least squares solutions; QR decomposition.
C
C    ******************************************************************
C
C     .. Parameters ..
      DOUBLE PRECISION   ZERO, ONE
      PARAMETER          ( ZERO = 0.0D0, ONE = 1.0D0 )
C     .. Scalar Arguments ..
      INTEGER            INFO, LDA, LDB, LDWORK, M, N, NRHS, RANK
C     .. Array Arguments ..
      INTEGER            JPVT( * )
      DOUBLE PRECISION   A( LDA, * ), B( LDB, * ), DWORK( * ), TAU( * )
C     .. Local Scalars ..
      INTEGER            I, IASCL, IBSCL, J, MN
      DOUBLE PRECISION   ANRM, BIGNUM, BNRM, MAXWRK, SMLNUM
C     .. External Functions ..
      DOUBLE PRECISION   DLAMCH, DLANGE, DLANTR
      EXTERNAL           DLAMCH, DLANGE, DLANTR
C     .. External Subroutines ..
      EXTERNAL           DCOPY, DLABAD, DLASCL, DLASET, DORMRZ, DTRSM,
     $                   DTZRZF, XERBLA
C     .. Intrinsic Functions ..
      INTRINSIC          DBLE, MAX, MIN
C     ..
C     .. Executable Statements ..
C
      MN = MIN( M, N )
C
C     Test the input scalar arguments.
C
      INFO = 0
      IF( M.LT.0 ) THEN
         INFO = -1
      ELSE IF( N.LT.0 ) THEN
         INFO = -2
      ELSE IF( NRHS.LT.0 ) THEN
         INFO = -3
      ELSE IF( RANK.LT.0 .OR. RANK.GT.MN ) THEN
         INFO = -4
      ELSE IF( LDA.LT.MAX( 1, M ) ) THEN
         INFO = -6
      ELSE IF( LDB.LT.1 .OR. ( NRHS.GT.0 .AND. LDB.LT.MAX( M, N ) ) )
     $      THEN
         INFO = -9
      ELSE IF( LDWORK.LT.MAX( 1, N, NRHS ) ) THEN
         INFO = -12
      END IF
C
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'MB02QY', -INFO )
         RETURN
      END IF
C
C     Quick return if possible.
C
      IF( MIN( MN, NRHS ).EQ.0 ) THEN
         DWORK( 1 ) = ONE
         RETURN
      END IF
C
C     Logically partition R = [ R11 R12 ],
C                             [  0  R22 ]
C
C     where R11 = R(1:RANK,1:RANK).  If  RANK = N,  let  T11 = R11.
C
      MAXWRK = DBLE( N )
      IF( RANK.LT.N ) THEN
C
C        Get machine parameters.
C
         SMLNUM = DLAMCH( 'Safe minimum' ) / DLAMCH( 'Precision' )
         BIGNUM = ONE / SMLNUM
         CALL DLABAD( SMLNUM, BIGNUM )
C
C        Scale A, B if max entries outside range [SMLNUM,BIGNUM].
C
         ANRM = DLANTR( 'MaxNorm', 'Upper', 'Non-unit', RANK, N, A, LDA,
     $                  DWORK )
         IASCL = 0
         IF( ANRM.GT.ZERO .AND. ANRM.LT.SMLNUM ) THEN
C
C           Scale matrix norm up to SMLNUM.
C
            CALL DLASCL( 'Upper', 0, 0, ANRM, SMLNUM, RANK, N, A, LDA,
     $                   INFO )
            IASCL = 1
         ELSE IF( ANRM.GT.BIGNUM ) THEN
C
C           Scale matrix norm down to BIGNUM.
C
            CALL DLASCL( 'Upper', 0, 0, ANRM, BIGNUM, RANK, N, A, LDA,
     $                   INFO )
            IASCL = 2
         ELSE IF( ANRM.EQ.ZERO ) THEN
C
C           Matrix all zero. Return zero solution.
C
            CALL DLASET( 'Full', N, NRHS, ZERO, ZERO, B, LDB )
            DWORK( 1 ) = ONE
            RETURN
         END IF
C
         BNRM = DLANGE( 'MaxNorm', M, NRHS, B, LDB, DWORK )
         IBSCL = 0
         IF( BNRM.GT.ZERO .AND. BNRM.LT.SMLNUM ) THEN
C
C           Scale matrix norm up to SMLNUM.
C
            CALL DLASCL( 'General', 0, 0, BNRM, SMLNUM, M, NRHS, B, LDB,
     $                   INFO )
            IBSCL = 1
         ELSE IF( BNRM.GT.BIGNUM ) THEN
C
C           Scale matrix norm down to BIGNUM.
C
            CALL DLASCL( 'General', 0, 0, BNRM, BIGNUM, M, NRHS, B, LDB,
     $                   INFO )
            IBSCL = 2
         END IF
C
C        [R11,R12] = [ T11, 0 ] * Z.
C        Details of Householder rotations are stored in TAU.
C        Workspace need RANK, prefer RANK*NB.
C
         CALL DTZRZF( RANK, N, A, LDA, TAU, DWORK, LDWORK, INFO )
         MAXWRK = MAX( MAXWRK, DWORK( 1 ) )
      END IF
C
C     B(1:RANK,1:NRHS) := inv(T11) * B(1:RANK,1:NRHS).
C
      CALL DTRSM( 'Left', 'Upper', 'No transpose', 'Non-unit', RANK,
     $            NRHS, ONE, A, LDA, B, LDB )
C
      IF( RANK.LT.N ) THEN
C
         CALL DLASET( 'Full', N-RANK, NRHS, ZERO, ZERO, B( RANK+1, 1 ),
     $                LDB )
C
C        B(1:N,1:NRHS) := Z' * B(1:N,1:NRHS).
C        Workspace need NRHS, prefer NRHS*NB.
C
         CALL DORMRZ( 'Left', 'Transpose', N, NRHS, RANK, N-RANK, A,
     $                LDA, TAU, B, LDB, DWORK, LDWORK, INFO )
         MAXWRK = MAX( MAXWRK, DWORK( 1 ) )
C
C        Undo scaling.
C
         IF( IASCL.EQ.1 ) THEN
            CALL DLASCL( 'General', 0, 0, ANRM, SMLNUM, N, NRHS, B, LDB,
     $                   INFO )
            CALL DLASCL( 'Upper', 0, 0, SMLNUM, ANRM, RANK, RANK, A,
     $                   LDA, INFO )
         ELSE IF( IASCL.EQ.2 ) THEN
            CALL DLASCL( 'General', 0, 0, ANRM, BIGNUM, N, NRHS, B, LDB,
     $                   INFO )
            CALL DLASCL( 'Upper', 0, 0, BIGNUM, ANRM, RANK, RANK, A,
     $                   LDA, INFO )
         END IF
         IF( IBSCL.EQ.1 ) THEN
            CALL DLASCL( 'General', 0, 0, SMLNUM, BNRM, N, NRHS, B, LDB,
     $                   INFO )
         ELSE IF( IBSCL.EQ.2 ) THEN
            CALL DLASCL( 'General', 0, 0, BIGNUM, BNRM, N, NRHS, B, LDB,
     $                   INFO )
         END IF
      END IF
C
C     B(1:N,1:NRHS) := P * B(1:N,1:NRHS).
C     Workspace N.
C
      DO 20 J = 1, NRHS
C
         DO 10 I = 1, N
            DWORK( JPVT( I ) ) = B( I, J )
   10    CONTINUE
C
         CALL DCOPY( N, DWORK, 1, B( 1, J ), 1 )
   20 CONTINUE
C
      DWORK( 1 ) = MAXWRK
      RETURN
C
C *** Last line of MB02QY ***
      END
