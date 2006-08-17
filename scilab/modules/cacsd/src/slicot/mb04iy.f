      SUBROUTINE MB04IY( SIDE, TRANS, N, M, K, P, A, LDA, TAU, C, LDC,
     $                   DWORK, LDWORK, INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 1999.
C
C     PURPOSE
C
C     To overwrite the real n-by-m matrix  C  with  Q' * C,  Q * C,
C     C * Q',  or  C * Q,  according to the following table
C
C                     SIDE = 'L'     SIDE = 'R'
C     TRANS = 'N':      Q * C          C * Q
C     TRANS = 'T':      Q'* C          C * Q'
C
C     where  Q  is a real orthogonal matrix defined as the product of
C     k elementary reflectors
C
C        Q = H(1) H(2) . . . H(k)
C
C     as returned by SLICOT Library routine MB04ID.  Q  is of order n
C     if  SIDE = 'L'  and of order m if  SIDE = 'R'.
C
C     ARGUMENTS
C
C     Mode Parameters
C
C     SIDE    CHARACTER*1
C             Specify if  Q  or  Q'  is applied from the left or right,
C             as follows:
C             = 'L':  apply  Q  or  Q'  from the left;
C             = 'R':  apply  Q  or  Q'  from the right.
C
C     TRANS   CHARACTER*1
C             Specify if  Q  or  Q'  is to be applied, as follows:
C             = 'N':  apply  Q   (No transpose);
C             = 'T':  apply  Q'  (Transpose).
C
C     Input/Output Parameters
C
C     N       (input) INTEGER
C             The number of rows of the matrix C.  N >= 0.
C
C     M       (input) INTEGER
C             The number of columns of the matrix C.  M >= 0.
C
C     K       (input) INTEGER
C             The number of elementary reflectors whose product defines
C             the matrix Q.
C             N >= K >= 0,  if  SIDE = 'L';
C             M >= K >= 0,  if  SIDE = 'R'.
C
C     P       (input) INTEGER
C             The order of the zero triagle (or the number of rows of 
C             the zero trapezoid) in the matrix triangularized by SLICOT
C             Library routine MB04ID.  P >= 0.
C
C     A       (input) DOUBLE PRECISION array, dimension (LDA,K)
C             On input, the elements in the rows  i+1:min(n,n-p-1+i)  of
C             the  i-th  column, and  TAU(i),  represent the orthogonal
C             reflector  H(i),  so that matrix  Q  is the product of
C             elementary reflectors:  Q = H(1) H(2) . . . H(k).
C             A is modified by the routine but restored on exit.
C
C     LDA     INTEGER
C             The leading dimension of the array  A.
C             LDA >= max(1,N),  if  SIDE = 'L';
C             LDA >= max(1,M),  if  SIDE = 'R'.
C
C     TAU     (input) DOUBLE PRECISION array, dimension (K)
C             The scalar factors of the elementary reflectors.
C
C     C       (input/output) DOUBLE PRECISION array, dimension (LDC,M)
C             On entry, the leading N-by-M part of this array must 
C             contain the matrix  C.
C             On exit, the leading N-by-M part of this array contains
C             the updated matrix C.
C
C     LDC     INTEGER
C             The leading dimension of the array  C.  LDC >= max(1,N).
C
C     Workspace
C
C     DWORK   DOUBLE PRECISION array, dimension (LDWORK)
C             On exit, if INFO = 0, DWORK(1) returns the optimal value
C             of LDWORK.
C
C     LDWORK  INTEGER
C             The length of the array DWORK.
C             LDWORK >= MAX(1,M),  if  SIDE = 'L';
C             LDWORK >= MAX(1,N),  if  SIDE = 'R'.
C             For optimum performance LDWORK >= M*NB if SIDE = 'L',
C             or LDWORK >= N*NB if SIDE = 'R', where NB is the optimal
C             block size.
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
C     If  SIDE = 'L',  each elementary reflector  H(i)  modifies  
C     n-p  elements of each column of  C,  for  i = 1:p+1,  and
C     n-i+1  elements, for  i = p+2:k.
C     If  SIDE = 'R',  each elementary reflector  H(i)  modifies  
C     m-p  elements of each row of  C,  for  i = 1:p+1,  and
C     m-i+1  elements, for  i = p+2:k.
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
C     Matrix operations, QR decomposition.
C
C     ******************************************************************
C
C     .. Parameters ..
      DOUBLE PRECISION   ONE
      PARAMETER          ( ONE = 1.0D+0 )
C     .. Scalar Arguments ..
      INTEGER            INFO, K, LDA, LDC, LDWORK, M, N, P
      CHARACTER          SIDE, TRANS
C     .. Array Arguments ..
      DOUBLE PRECISION   A( LDA, * ), C( LDC, * ), DWORK( * ), TAU( * )
C     .. Local Scalars ..
      LOGICAL            LEFT, TRAN
      INTEGER            I
      DOUBLE PRECISION   AII, WRKOPT
C     .. External Functions ..
      LOGICAL            LSAME
      EXTERNAL           LSAME
C     .. External Subroutines ..
      EXTERNAL           DLARF, DORMQR, XERBLA
C     .. Intrinsic Functions ..
      INTRINSIC          DBLE, MAX, MIN
C     .. Executable Statements ..
C
C     Check the scalar input arguments.
C
      INFO = 0
      LEFT = LSAME( SIDE,  'L' )
      TRAN = LSAME( TRANS, 'T' )
C
      IF( .NOT.LEFT .AND. .NOT.LSAME( SIDE, 'R' ) ) THEN
         INFO = -1
      ELSE IF( .NOT.TRAN .AND. .NOT.LSAME( TRANS, 'N' ) ) THEN
         INFO = -2
      ELSE IF( N.LT.0 ) THEN
         INFO = -3
      ELSE IF( M.LT.0 ) THEN
         INFO = -4
      ELSE IF( K.LT.0 .OR. ( LEFT .AND. K.GT.N ) .OR.
     $                ( .NOT.LEFT .AND. K.GT.M ) ) THEN
         INFO = -5
      ELSE IF( P.LT.0 ) THEN
         INFO = -6
      ELSE IF( ( LEFT .AND. LDA.LT.MAX( 1, N ) ) .OR. 
     $    ( .NOT.LEFT .AND. LDA.LT.MAX( 1, M ) ) ) THEN
         INFO = -8
      ELSE IF( LDC.LT.MAX( 1, N ) ) THEN
         INFO = -11
      ELSE IF( ( LEFT .AND. LDWORK.LT.MAX( 1, M ) ) .OR. 
     $    ( .NOT.LEFT .AND. LDWORK.LT.MAX( 1, N ) ) ) THEN
         INFO = -13
      END IF
C
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'MB04IY', -INFO )
         RETURN
      END IF
C
C     Quick return if possible.
C
      IF( M.EQ.0 .OR. N.EQ.0 .OR. K.EQ.0 .OR. ( LEFT .AND. N.LT.P )
     $           .OR. ( .NOT.LEFT .AND. M.LT.P ) ) THEN
         DWORK(1) = ONE
         RETURN
      END IF
C
C     (Note: Comments in the code beginning "Workspace:" describe the
C     minimal amount of real workspace needed at that point in the
C     code, as well as the preferred amount for good performance.
C     NB refers to the optimal block size for the immediately
C     following subroutine, as returned by ILAENV.)
C     
      IF( LEFT ) THEN
         WRKOPT = DBLE( M )
         IF( TRAN ) THEN
C
            DO 10 I = 1, MIN( K, P )
C
C              Apply H(i) to C(i:i+n-p-1,1:m), from the left.
C              Workspace: need M. 
C
               AII = A( I, I )
               A( I, I ) = ONE
               CALL DLARF( SIDE, N-P, M, A( I, I ), 1, TAU( I ),
     $                     C( I, 1 ), LDC, DWORK )
               A( I, I ) = AII
   10       CONTINUE
C
            IF ( P.LE.MIN( N, K ) ) THEN
C
C              Apply H(i) to C, i = p+1:k, from the left.
C              Workspace: need M;  prefer M*NB. 
C
               CALL DORMQR( SIDE, TRANS, N-P, M, K-P, A( P+1, P+1 ),
     $                      LDA, TAU( P+1 ), C( P+1, 1 ), LDC, DWORK,
     $                      LDWORK, I )
               WRKOPT = MAX( WRKOPT, DWORK( 1 ) )
            END IF
C
         ELSE
C
            IF ( P.LE.MIN( N, K ) ) THEN
C
C              Apply H(i) to C, i = k:p+1:-1, from the left.
C              Workspace: need M;  prefer M*NB. 
C
               CALL DORMQR( SIDE, TRANS, N-P, M, K-P, A( P+1, P+1 ),
     $                      LDA, TAU( P+1 ), C( P+1, 1 ), LDC, DWORK,
     $                      LDWORK, I )
               WRKOPT = MAX( WRKOPT, DWORK( 1 ) )
            END IF
C
            DO 20 I = MIN( K, P ), 1, -1
C
C              Apply H(i) to C(i:i+n-p-1,1:m), from the left.
C              Workspace: need M. 
C
               AII = A( I, I )
               A( I, I ) = ONE
               CALL DLARF( SIDE, N-P, M, A( I, I ), 1, TAU( I ),
     $                     C( I, 1 ), LDC, DWORK )
               A( I, I ) = AII
   20       CONTINUE
         END IF
C
      ELSE
C
         WRKOPT = DBLE( N )
         IF( TRAN ) THEN
C
            IF ( P.LE.MIN( M, K ) ) THEN
C
C              Apply H(i) to C, i = k:p+1:-1, from the right.
C              Workspace: need N;  prefer N*NB. 
C
               CALL DORMQR( SIDE, TRANS, N, M-P, K-P, A( P+1, P+1 ),
     $                      LDA, TAU( P+1 ), C( 1, P+1 ), LDC, DWORK,
     $                      LDWORK, I )
               WRKOPT = MAX( WRKOPT, DWORK( 1 ) )
            END IF
C
            DO 30 I = MIN( K, P ), 1, -1
C
C              Apply H(i) to C(1:n,i:i+m-p-1), from the right.
C              Workspace: need N. 
C
               AII = A( I, I )
               A( I, I ) = ONE
               CALL DLARF( SIDE, N, M-P, A( I, I ), 1, TAU( I ),
     $                     C( 1, I ), LDC, DWORK )
               A( I, I ) = AII
   30       CONTINUE
C
         ELSE
C
            DO 40 I = 1, MIN( K, P )
C
C              Apply H(i) to C(1:n,i:i+m-p-1), from the right.
C              Workspace: need N. 
C
               AII = A( I, I )
               A( I, I ) = ONE
               CALL DLARF( SIDE, N, M-P, A( I, I ), 1, TAU( I ),
     $                     C( 1, I ), LDC, DWORK )
               A( I, I ) = AII
   40       CONTINUE
C
            IF ( P.LE.MIN( M, K ) ) THEN
C
C              Apply H(i) to C, i = p+1:k, from the right.
C              Workspace: need N;  prefer N*NB. 
C
               CALL DORMQR( SIDE, TRANS, N, M-P, K-P, A( P+1, P+1 ),
     $                      LDA, TAU( P+1 ), C( 1, P+1 ), LDC, DWORK,
     $                      LDWORK, I )
               WRKOPT = MAX( WRKOPT, DWORK( 1 ) )
            END IF
C
         END IF
      END IF
C
      DWORK( 1 ) = WRKOPT 
      RETURN
C
C *** Last line of MB04IY ***
      END
