      SUBROUTINE IB01PX( JOB, NOBR, N, M, L, UF, LDUF, UN, LDUN, UL,
     $                   LDUL, PGAL, LDPGAL, K, LDK, R, LDR, X, B, LDB,
     $                   D, LDD, TOL, IWORK, DWORK, LDWORK, IWARN,
     $                   INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 2000.
C
C     PURPOSE
C
C     To build and solve the least squares problem  T*X = Kv,  and 
C     estimate the matrices B and D of a linear time-invariant (LTI)
C     state space model, using the solution  X,  and the singular
C     value decomposition information and other intermediate results,
C     provided by other routines. 
C
C     The matrix  T  is computed as a sum of Kronecker products,
C
C        T = T + kron(Uf(:,(i-1)*m+1:i*m),N_i),  for i = 1 : s,
C
C     (with  T  initialized by zero), where  Uf  is the triangular
C     factor of the QR factorization of the future input part (see 
C     SLICOT Library routine IB01ND),  N_i  is given by the i-th block
C     row of the matrix
C
C            [ Q_11  Q_12  ...  Q_1,s-2  Q_1,s-1  Q_1s ]   [ I_L  0  ] 
C            [ Q_12  Q_13  ...  Q_1,s-1    Q_1s    0   ]   [         ]
C        N = [ Q_13  Q_14  ...    Q_1s      0      0   ] * [         ],
C            [  :     :            :        :      :   ]   [         ]
C            [ Q_1s   0    ...     0        0      0   ]   [  0  GaL ]
C
C     and where
C
C               [   -L_1|1    ]          [ M_i-1 - L_1|i ]
C        Q_11 = [             ],  Q_1i = [               ],  i = 2:s,
C               [ I_L - L_2|1 ]          [     -L_2|i    ]
C
C     are  (n+L)-by-L  matrices, and  GaL  is built from the first  n  
C     relevant singular vectors,  GaL = Un(1:L(s-1),1:n),  computed 
C     by IB01ND.
C
C     The vector  Kv  is vec(K), with the matrix  K  defined by
C
C        K = [ K_1  K_2  K_3  ...  K_s ],
C
C     where  K_i = K(:,(i-1)*m+1:i*m),  i = 1:s,  is  (n+L)-by-m.
C     The given matrices are  Uf,  GaL,  and  
C
C            [ L_1|1  ...  L_1|s ]
C        L = [                   ],   (n+L)-by-L*s,
C            [ L_2|1  ...  L_2|s ]
C
C        M = [ M_1  ...  M_s-1 ],      n-by-L*(s-1),  and
C        K,                            (n+L)-by-m*s.
C
C     Matrix  M  is the pseudoinverse of the matrix  GaL,  computed by
C     SLICOT Library routine IB01PD.
C
C     ARGUMENTS
C
C     Mode Parameters
C
C     JOB     CHARACTER*1
C             Specifies which of the matrices B and D should be
C             computed, as follows:
C             = 'B':  compute the matrix B, but not the matrix D;
C             = 'D':  compute both matrices B and D.
C
C     Input/Output Parameters
C
C     NOBR    (input) INTEGER
C             The number of block rows,  s,  in the input and output
C             Hankel matrices processed by other routines.  NOBR > 1.
C
C     N       (input) INTEGER
C             The order of the system.  NOBR > N > 0.
C
C     M       (input) INTEGER
C             The number of system inputs.  M >= 0.
C
C     L       (input) INTEGER
C             The number of system outputs.  L > 0.
C
C     UF      (input/output) DOUBLE PRECISION array, dimension 
C             ( LDUF,M*NOBR ) 
C             On entry, the leading  M*NOBR-by-M*NOBR  upper triangular
C             part of this array must contain the upper triangular 
C             factor of the QR factorization of the future input part,
C             as computed by SLICOT Library routine IB01ND.
C             The strict lower triangle need not be set to zero. 
C             On exit, the leading  M*NOBR-by-M*NOBR  upper triangular
C             part of this array is unchanged, and the strict lower 
C             triangle is set to zero.
C
C     LDUF    INTEGER
C             The leading dimension of the array  UF.
C             LDUF >= MAX( 1, M*NOBR ).
C
C     UN      (input) DOUBLE PRECISION array, dimension ( LDUN,N ) 
C             The leading  L*(NOBR-1)-by-N  part of this array must
C             contain the matrix  GaL,  i.e., the leading part of the
C             first  N  columns of the matrix  Un  of relevant singular
C             vectors.
C
C     LDUN    INTEGER
C             The leading dimension of the array  UN.
C             LDUN >= L*(NOBR-1).
C
C     UL      (input/output) DOUBLE PRECISION array, dimension 
C             ( LDUL,L*NOBR )
C             On entry, the leading  (N+L)-by-L*NOBR  part of this array
C             must contain the given matrix  L.
C             On exit, if  M > 0,  the leading  (N+L)-by-L*NOBR  part of
C             this array is overwritten by the matrix  
C             [ Q_11  Q_12  ...  Q_1,s-2  Q_1,s-1  Q_1s ]. 
C
C     LDUL    INTEGER
C             The leading dimension of the array  UL.  LDUL >= N+L.
C
C     PGAL    (input) DOUBLE PRECISION array, dimension 
C             ( LDPGAL,L*(NOBR-1) )
C             The leading  N-by-L*(NOBR-1)  part of this array must
C             contain the pseudoinverse of the matrix  GaL,  computed by
C             SLICOT Library routine IB01PD.
C
C     LDPGAL  INTEGER
C             The leading dimension of the array  PGAL.  LDPGAL >= N.
C
C     K       (input) DOUBLE PRECISION array, dimension ( LDK,M*NOBR )
C             The leading  (N+L)-by-M*NOBR  part of this array must
C             contain the given matrix  K.
C            
C     LDK     INTEGER
C             The leading dimension of the array  K.  LDK >= N+L.
C
C     R       (output) DOUBLE PRECISION array, dimension ( LDR,M*(N+L) )
C             The leading  (N+L)*M*NOBR-by-M*(N+L)  part of this array
C             contains details of the complete orthogonal factorization
C             of the coefficient matrix  T  of the least squares problem
C             which is solved for getting the system matrices B and D.
C            
C     LDR     INTEGER
C             The leading dimension of the array  R.
C             LDR >= MAX( 1, (N+L)*M*NOBR ).
C            
C     X       (output) DOUBLE PRECISION array, dimension 
C             ( (N+L)*M*NOBR ) 
C             The leading  M*(N+L)  elements of this array contain the
C             least squares solution of the system  T*X = Kv.
C             The remaining elements are used as workspace (to store the
C             corresponding part of the vector Kv = vec(K)).
C            
C     B       (output) DOUBLE PRECISION array, dimension ( LDB,M )
C             The leading N-by-M part of this array contains the system
C             input matrix.  
C
C     LDB     INTEGER
C             The leading dimension of the array B.  LDB >= N.
C
C     D       (output) DOUBLE PRECISION array, dimension ( LDD,M )
C             If  JOB = 'D',  the leading L-by-M part of this array 
C             contains the system input-output matrix.  
C             If  JOB = 'B',  this array is not referenced.  
C
C     LDD     INTEGER
C             The leading dimension of the array D.
C             LDD >= L, if  JOB = 'D';
C             LDD >= 1, if  JOB = 'B'.
C
C     Tolerances
C
C     TOL     DOUBLE PRECISION
C             The tolerance to be used for estimating the rank of
C             matrices. If the user sets  TOL > 0,  then the given value
C             of  TOL  is used as a lower bound for the reciprocal
C             condition number;  an m-by-n matrix whose estimated
C             condition number is less than  1/TOL  is considered to  
C             be of full rank.  If the user sets  TOL <= 0,  then an 
C             implicitly computed, default tolerance, defined by 
C             TOLDEF = m*n*EPS,  is used instead, where  EPS  is the 
C             relative machine precision (see LAPACK Library routine 
C             DLAMCH).
C
C     Workspace
C
C     IWORK   INTEGER array, dimension ( M*(N+L) )
C
C     DWORK   DOUBLE PRECISION array, dimension ( LDWORK )
C             On exit, if  INFO = 0,  DWORK(1) returns the optimal value
C             of LDWORK,  and, if  M > 0,  DWORK(2)  contains the  
C             reciprocal condition number of the triangular factor of
C             the matrix  T.
C             On exit, if  INFO = -26,  DWORK(1)  returns the minimum
C             value of LDWORK.
C
C     LDWORK  INTEGER
C             The length of the array DWORK.
C             LDWORK >= MAX( (N+L)*(N+L), 4*M*(N+L)+1 ).
C             For good performance,  LDWORK  should be larger.
C
C     Warning Indicator
C
C     IWARN   INTEGER
C             = 0:  no warning;
C             = 4:  the least squares problem to be solved has a
C                   rank-deficient coefficient matrix. 
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
C     The matrix  T  is computed, evaluating the sum of Kronecker
C     products, and then the linear system  T*X = Kv  is solved in a
C     least squares sense. The matrices  B  and  D  are then directly
C     obtained from the least squares solution.
C
C     REFERENCES
C
C     [1] Verhaegen M., and Dewilde, P.
C         Subspace Model Identification. Part 1: The output-error 
C         state-space model identification class of algorithms.
C         Int. J. Control, 56, pp. 1187-1210, 1992.
C
C     [2] Van Overschee, P., and De Moor, B.
C         N4SID: Two Subspace Algorithms for the Identification
C         of Combined Deterministic-Stochastic Systems.
C         Automatica, Vol.30, No.1, pp. 75-93, 1994.
C
C     [3] Van Overschee, P.
C         Subspace Identification : Theory - Implementation -
C         Applications.
C         Ph. D. Thesis, Department of Electrical Engineering,
C         Katholieke Universiteit Leuven, Belgium, Feb. 1995.
C
C     NUMERICAL ASPECTS
C
C     The implemented method is numerically stable.
C
C     CONTRIBUTOR
C
C     V. Sima, Katholieke Universiteit Leuven, Feb. 2000.
C
C     REVISIONS
C
C     -
C
C     KEYWORDS
C
C     Identification methods; least squares solutions; multivariable
C     systems; QR decomposition; singular value decomposition.
C
C     ******************************************************************
C
C     .. Parameters ..
      DOUBLE PRECISION   ZERO, ONE
      PARAMETER          ( ZERO  = 0.0D0, ONE = 1.0D0 )
C     .. Scalar Arguments ..
      DOUBLE PRECISION   TOL
      INTEGER            INFO, IWARN, L, LDB, LDD, LDK, LDPGAL, LDR,
     $                   LDUF, LDUL, LDUN, LDWORK, M, N, NOBR
      CHARACTER          JOB
C     .. Array Arguments ..
      DOUBLE PRECISION   B(LDB, *), D(LDD, *), DWORK(*), K(LDK, *),  
     $                   PGAL(LDPGAL, *), R(LDR, *), UF(LDUF, *), 
     $                   UL(LDUL, *), UN(LDUN, *), X(*)
      INTEGER            IWORK( * )
C     .. Local Scalars ..
      DOUBLE PRECISION   RCOND, TOLL
      INTEGER            I, IERR, J, JWORK, LDUN2, LNOBR, LP1, MAXWRK,
     $                   MINWRK, MKRON, MNOBR, NKRON, NP1, NPL, RANK
      LOGICAL            WITHB, WITHD
C     .. External Functions ..
      LOGICAL            LSAME
      DOUBLE PRECISION   DLAMCH
      EXTERNAL           DLAMCH, LSAME
C     .. External Subroutines ..
      EXTERNAL           DGELSY, DGEMM, DLACPY, DLASET, DTRCON, MB01VD,
     $                   XERBLA
C     .. Intrinsic Functions ..
      INTRINSIC          MAX
C     .. Executable Statements ..
C
C     Decode the scalar input parameters.
C
      WITHD = LSAME( JOB, 'D' )
      WITHB = LSAME( JOB, 'B' ) .OR. WITHD
      MNOBR = M*NOBR
      LNOBR = L*NOBR
      LDUN2 = LNOBR - L
      LP1   = L + 1
      NP1   = N + 1
      NPL   = N + L
      IWARN = 0
      INFO  = 0
C
C     Check the scalar input parameters.
C
      IF( .NOT.WITHB ) THEN
         INFO = -1
      ELSE IF( NOBR.LE.1 ) THEN
         INFO = -2
      ELSE IF( N.GE.NOBR .OR. N.LE.0 ) THEN
         INFO = -3
      ELSE IF( M.LT.0 ) THEN
         INFO = -4
      ELSE IF( L.LE.0 ) THEN
         INFO = -5
      ELSE IF( LDUF.LT.MAX( 1, MNOBR ) ) THEN
         INFO = -7
      ELSE IF( LDUN.LT.LDUN2 ) THEN
         INFO = -9
      ELSE IF( LDUL.LT.NPL ) THEN
         INFO = -11
      ELSE IF( LDPGAL.LT.N ) THEN
         INFO = -13
      ELSE IF( LDK.LT.NPL ) THEN
         INFO = -15
      ELSE IF( LDR.LT.MAX( 1, MNOBR*NPL ) ) THEN
         INFO = -17
      ELSE IF( LDB.LT.N ) THEN
         INFO = -20
      ELSE IF( LDD.LT.1 .OR. ( WITHD .AND. LDD.LT.L ) ) THEN
         INFO = -22
      ELSE IF( LDWORK.GE.1 ) THEN
C
C        Compute workspace.
C        (Note: Comments in the code beginning "Workspace:" describe the
C         minimal amount of workspace needed at that point in the code,
C         as well as the preferred amount for good performance.
C         NB refers to the optimal block size for the immediately
C         following subroutine, as returned by ILAENV.)
C
         MINWRK = MAX( NPL*NPL, 4*M*NPL + 1 )
C
         IF ( LDWORK.LT.MINWRK ) THEN
            INFO = -26
            DWORK( 1 ) = MINWRK
         END IF
      END IF
C
C     Return if there are illegal arguments.
C
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'IB01PX', -INFO )
         RETURN
      END IF
C
C     Quick return if possible.
C
      IF( M.EQ.0 ) THEN
         DWORK(1) = ONE
         RETURN
      END IF
C
C     Construct the matrix  [ Q_11  Q_12  ...  Q_1,s-1  Q_1s ]  in  UL.
C
      DO 20 J = 1, L
C
         DO 10 I = 1, NPL
            UL(I,J) = -UL(I,J)
   10    CONTINUE
C
         UL(N+J,J) = ONE + UL(N+J,J)
   20 CONTINUE
C
      DO 50 J = LP1, LNOBR
C
         DO 30 I = 1, N
            UL(I,J) = PGAL(I,J-L) - UL(I,J)
   30    CONTINUE
C
         DO 40 I = NP1, NPL
            UL(I,J) = -UL(I,J)
   40    CONTINUE
C
   50 CONTINUE
C
C     Compute the coefficient matrix T using Kronecker products.
C     Workspace: (N+L)*(N+L).
C     In the same loop, vectorize K in X.
C
      CALL DLASET( 'Full', MNOBR*NPL, M*NPL, ZERO, ZERO, R, LDR )
      CALL DLASET( 'Lower', MNOBR-1, MNOBR-1, ZERO, ZERO, UF(2,1), 
     $             LDUF )
      JWORK = NPL*L + 1
C
      DO 60 I = 1, NOBR
         CALL DLACPY( 'Full', NPL, L, UL(1,(I-1)*L+1), LDUL, DWORK,
     $                NPL )
         IF ( I.LT.NOBR ) THEN
            CALL DGEMM ( 'NoTranspose', 'NoTranspose', NPL, N,
     $                   L*(NOBR-I), ONE, UL(1,I*L+1), LDUL, UN, LDUN,
     $                   ZERO, DWORK(JWORK), NPL )
         ELSE
            CALL DLASET( 'Full', NPL, N, ZERO, ZERO, DWORK(JWORK), NPL )
         END IF
         CALL MB01VD( 'NoTranspose', 'NoTranspose', MNOBR, M, NPL, 
     $                NPL, ONE, ONE, UF(1,(I-1)*M+1), LDUF, DWORK, 
     $                NPL, R, LDR, MKRON, NKRON, IERR )
         CALL DLACPY( 'Full', NPL, M, K(1,(I-1)*M+1), LDK, 
     $                X((I-1)*NKRON+1), NPL )
   60 CONTINUE
C
C     Compute the tolerance.
C
      TOLL = TOL
      IF( TOLL.LE.ZERO ) 
     $   TOLL = MKRON*NKRON*DLAMCH( 'Precision' )
C
C     Solve the least square problem  T*X = vec(K).
C     Workspace:  need   4*M*(N+L)+1;
C                 prefer 3*M*(N+L)+(M*(N+L)+1)*NB. 
C
      DO 70 I = 1, NKRON
         IWORK(I) = 0
   70 CONTINUE
C
      CALL DGELSY( MKRON, NKRON, 1, R, LDR, X, MKRON, IWORK, TOLL, RANK,
     $             DWORK, LDWORK, IERR )
      MAXWRK = DWORK(1)
C
C     Compute the reciprocal of the condition number of the triangular 
C     factor  R  of  T.
C     Workspace: need 3*M*(N+L).
C
      CALL DTRCON( '1-norm', 'Upper', 'NonUnit', NKRON, R, LDR, RCOND,
     $             DWORK, IWORK, IERR )
C
      IF ( RANK.LT.NKRON ) THEN
C     
C        The least squares problem is rank-deficient.
C     
         IWARN = 4
      END IF
C
C     Construct the matrix  D,  if needed.
C
      IF ( WITHD ) 
     $   CALL DLACPY( 'Full', L, M, X, NPL, D, LDD )
C
C     Construct the matrix  B.
C
      CALL DLACPY( 'Full', N, M, X(LP1), NPL, B, LDB )
C
C     Return optimal workspace in DWORK(1) and reciprocal condition
C     number in  DWORK(2).
C
      DWORK(1) = MAX( MINWRK, MAXWRK )
      DWORK(2) = RCOND
C
      RETURN
C
C *** Last line of IB01PX ***
      END
