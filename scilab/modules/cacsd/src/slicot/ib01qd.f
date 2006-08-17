      SUBROUTINE IB01QD( JOBX0, JOB, N, M, L, NSMP, A, LDA, C, LDC, U,
     $                   LDU, Y, LDY, X0, B, LDB, D, LDD, TOL, IWORK,
     $                   DWORK, LDWORK, IWARN, INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 2000.
C
C     PURPOSE
C
C     To estimate the initial state and the system matrices  B  and  D
C     of a linear time-invariant (LTI) discrete-time system, given the
C     matrix pair  (A,C)  and the input and output trajectories of the
C     system. The model structure is :
C
C           x(k+1) = Ax(k) + Bu(k),   k >= 0,
C           y(k)   = Cx(k) + Du(k),
C
C     where  x(k)  is the  n-dimensional state vector (at time k),
C            u(k)  is the  m-dimensional input vector,
C            y(k)  is the  l-dimensional output vector,
C     and  A, B, C, and D  are real matrices of appropriate dimensions.
C     Matrix  A  is assumed to be in a real Schur form.
C
C     ARGUMENTS
C
C     Mode Parameters
C
C     JOBX0   CHARACTER*1
C             Specifies whether or not the initial state should be 
C             computed, as follows:
C             = 'X':  compute the initial state x(0);
C             = 'N':  do not compute the initial state (x(0) is known
C                     to be zero).
C
C     JOB     CHARACTER*1
C             Specifies which matrices should be computed, as follows:
C             = 'B':  compute the matrix B only (D is known to be zero);
C             = 'D':  compute the matrices B and D.
C
C     Input/Output Parameters
C
C     N       (input) INTEGER
C             The order of the system.  N >= 0.
C
C     M       (input) INTEGER
C             The number of system inputs.  M >= 0.
C
C     L       (input) INTEGER
C             The number of system outputs.  L > 0.
C
C     NSMP    (input) INTEGER
C             The number of rows of matrices  U  and  Y  (number of
C             samples,  t).
C             NSMP >= N*M + a + e,  where
C             a = 0,  if  JOBX0 = 'N';
C             a = N,  if  JOBX0 = 'X';
C             e = 0,  if  JOBX0 = 'X'  and  JOB = 'B';
C             e = 1,  if  JOBX0 = 'N'  and  JOB = 'B';
C             e = M,  if  JOB   = 'D'.
C
C     A       (input) DOUBLE PRECISION array, dimension (LDA,N)
C             The leading N-by-N part of this array must contain the
C             system state matrix  A  in a real Schur form. 
C
C     LDA     INTEGER
C             The leading dimension of the array A.  LDA >= MAX(1,N).
C
C     C       (input) DOUBLE PRECISION array, dimension (LDC,N)
C             The leading L-by-N part of this array must contain the
C             system output matrix  C  (corresponding to the real Schur
C             form of  A).
C
C     LDC     INTEGER
C             The leading dimension of the array C.  LDC >= L.
C
C     U       (input/output) DOUBLE PRECISION array, dimension (LDU,M)
C             On entry, the leading NSMP-by-M part of this array must
C             contain the t-by-m input-data sequence matrix  U,
C             U = [u_1 u_2 ... u_m].  Column  j  of  U  contains the
C             NSMP  values of the j-th input component for consecutive
C             time increments.
C             On exit, if  JOB = 'D',  the leading NSMP-by-M part of
C             this array contains details of the QR factorization of
C             the t-by-m matrix  U, possibly computed sequentially
C             (see METHOD).
C             If  JOB = 'B',  this array is unchanged on exit.
C             If M = 0, this array is not referenced.
C
C     LDU     INTEGER
C             The leading dimension of the array U.
C             LDU >= MAX(1,NSMP),  if M > 0;
C             LDU >= 1,            if M = 0.
C
C     Y       (input) DOUBLE PRECISION array, dimension (LDY,L)
C             The leading NSMP-by-L part of this array must contain the
C             t-by-l output-data sequence matrix  Y,
C             Y = [y_1 y_2 ... y_l].  Column  j  of  Y  contains the
C             NSMP  values of the j-th output component for consecutive
C             time increments.
C
C     LDY     INTEGER
C             The leading dimension of the array Y.  LDY >= MAX(1,NSMP).
C
C     X0      (output) DOUBLE PRECISION array, dimension (N)
C             If  JOBX0 = 'X',  the estimated initial state of the
C             system,  x(0).
C             If  JOBX0 = 'N',  x(0)  is set to zero without any
C             calculations.
C
C     B       (output) DOUBLE PRECISION array, dimension (LDB,M)
C             If  N > 0,  M > 0,  and  INFO = 0,  the leading N-by-M
C             part of this array contains the system input matrix  B
C             in the coordinates corresponding to the real Schur form
C             of  A.
C             If  N = 0  or  M = 0,  this array is not referenced.
C
C     LDB     INTEGER
C             The leading dimension of the array B.
C             LDB >= N,  if  N > 0  and  M > 0;
C             LDB >= 1,  if  N = 0  or   M = 0.
C
C     D       (output) DOUBLE PRECISION array, dimension (LDD,M)
C             If  M > 0,  JOB = 'D',  and  INFO = 0,  the leading
C             L-by-M part of this array contains the system input-output
C             matrix  D.
C             If  M = 0  or  JOB = 'B',  this array is not referenced.  
C
C     LDD     INTEGER
C             The leading dimension of the array D.
C             LDD >= L,  if  M > 0  and  JOB = 'D';
C             LDD >= 1,  if  M = 0  or   JOB = 'B'.
C
C     Tolerances
C
C     TOL     DOUBLE PRECISION
C             The tolerance to be used for estimating the rank of
C             matrices. If the user sets  TOL > 0,  then the given value
C             of  TOL  is used as a lower bound for the reciprocal
C             condition number;  a matrix whose estimated condition
C             number is less than  1/TOL  is considered to be of full  
C             rank.  If the user sets  TOL <= 0,  then  EPS  is used
C             instead, where  EPS  is the relative machine precision
C             (see LAPACK Library routine DLAMCH).  TOL <= 1.
C
C     Workspace
C
C     IWORK   INTEGER array, dimension (LIWORK), where
C             LIWORK >= N*M + a,            if  JOB = 'B',
C             LIWORK >= max( N*M + a, M ),  if  JOB = 'D',
C             with  a = 0,  if  JOBX0 = 'N';
C                   a = N,  if  JOBX0 = 'X'.
C
C     DWORK   DOUBLE PRECISION array, dimension (LDWORK)
C             On exit, if  INFO = 0,  DWORK(1) returns the optimal value
C             of LDWORK;  DWORK(2)  contains the reciprocal condition
C             number of the triangular factor of the QR factorization of
C             the matrix  W2  (see METHOD); if  M > 0  and  JOB = 'D', 
C             DWORK(3)  contains the reciprocal condition number of the 
C             triangular factor of the QR factorization of  U.   
C             On exit, if  INFO = -23,  DWORK(1)  returns the minimum
C             value of LDWORK.
C
C     LDWORK  INTEGER
C             The length of the array DWORK.
C             LDWORK >= max( LDW1, min( LDW2, LDW3 ) ),  where
C             LDW1 = 2,          if  M = 0  or   JOB = 'B',
C             LDW1 = 3,          if  M > 0  and  JOB = 'D',
C             LDWa = t*L*(r + 1) + max( N + max( d, f ), 6*r ),
C             LDW2 = LDWa,       if  M = 0  or  JOB = 'B',
C             LDW2 = max( LDWa, t*L*(r + 1) + 2*M*M + 6*M ),  
C                                if  M > 0  and JOB = 'D',
C             LDWb = (b + r)*(r + 1) + 
C                     max( q*(r + 1) + N*N*M + c + max( d, f ), 6*r ),
C             LDW3 = LDWb,       if  M = 0  or  JOB = 'B',
C             LDW3 = max( LDWb, (b + r)*(r + 1) + 2*M*M + 6*M ),  
C                                if  M > 0  and JOB = 'D',
C                r = N*M + a,
C                a = 0,                  if  JOBX0 = 'N',
C                a = N,                  if  JOBX0 = 'X';
C                b = 0,                  if  JOB   = 'B',
C                b = L*M,                if  JOB   = 'D';
C                c = 0,                  if  JOBX0 = 'N',
C                c = L*N,                if  JOBX0 = 'X';
C                d = 0,                  if  JOBX0 = 'N',
C                d = 2*N*N + N,          if  JOBX0 = 'X';
C                f = 2*r,                if  JOB   = 'B'   or  M = 0,
C                f = M + max( 2*r, M ),  if  JOB   = 'D'  and  M > 0;
C                q = b + r*L.
C             For good performance,  LDWORK  should be larger.
C             If  LDWORK >= LDW2  or  
C                 LDWORK >= t*L*(r + 1) + (b + r)*(r + 1) + N*N*M + c +
C                           max( d, f ),
C             then standard QR factorizations of the matrices  U  and/or
C             W2  (see METHOD) are used. 
C             Otherwise, the QR factorizations are computed sequentially
C             by performing  NCYCLE  cycles, each cycle (except possibly
C             the last one) processing  s < t  samples, where  s  is
C             chosen from the equation
C               LDWORK = s*L*(r + 1) + (b + r)*(r + 1) + N*N*M + c +
C                        max( d, f ).
C             (s  is at least  N*M+a+e,  the minimum value of  NSMP.)
C             The computational effort may increase and the accuracy may
C             decrease with the decrease of  s.  Recommended value is
C             LDWORK = LDW2,  assuming a large enough cache size, to
C             also accommodate  A,  C,  U,  and  Y.
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
C                   value;
C             = 2:  the singular value decomposition (SVD) algorithm did
C                   not converge.
C
C     METHOD
C
C     An extension and refinement of the method in [1,2] is used. 
C     Specifically, denoting
C
C           X = [ vec(D')' vec(B)' x0' ]',
C
C     where  vec(M)  is the vector obtained by stacking the columns of
C     the matrix  M,  then  X  is the least squares solution of the 
C     system  S*X = vec(Y),  with the matrix  S = [ diag(U)  W ], 
C     defined by 
C
C           ( U         |     | ... |     |     | ... |     |         )
C           (   U       |  11 | ... |  n1 |  12 | ... |  nm |         )
C       S = (     :     | y   | ... | y   | y   | ... | y   | P*Gamma ),
C           (       :   |     | ... |     |     | ... |     |         )
C           (         U |     | ... |     |     | ... |     |         )
C                                                                     ij
C     diag(U)  having  L  block rows and columns.  In this formula,  y 
C     are the outputs of the system for zero initial state computed 
C     using the following model, for j = 1:m, and for i = 1:n,
C            ij          ij                    ij
C           x  (k+1) = Ax  (k) + e_i u_j(k),  x  (0) = 0,
C
C            ij          ij
C           y  (k)   = Cx  (k),
C
C     where  e_i  is the i-th n-dimensional unit vector,  Gamma  is
C     given by
C
C                (     C     )
C                (    C*A    )
C        Gamma = (   C*A^2   ),
C                (     :     )
C                ( C*A^(t-1) )
C
C     and  P  is a permutation matrix that groups together the rows of
C     Gamma  depending on the same row of  C,  namely  
C     [ c_j;  c_j*A;  c_j*A^2; ...  c_j*A^(t-1) ],  for j = 1:L.
C     The first block column,  diag(U),  is not explicitly constructed,
C     but its structure is exploited. The last block column is evaluated
C     using powers of A with exponents 2^k. No interchanges are applied.
C     A special QR decomposition of the matrix  S  is computed. Let 
C     U = q*[ r' 0 ]'  be the QR decomposition of  U,  if  M > 0,  where
C     r  is  M-by-M.   Then,  diag(q')  is applied to  W  and  vec(Y).  
C     The block-rows of  S  and  vec(Y)  are implicitly permuted so that
C     matrix  S  becomes
C
C        ( diag(r)  W1 )
C        (    0     W2 ),
C        
C     where  W1  has L*M rows. Then, the QR decomposition of  W2 is 
C     computed (sequentially, if  M > 0) and used to obtain  B  and  x0.
C     The intermediate results and the QR decomposition of  U  are 
C     needed to find  D.  If a triangular factor is too ill conditioned,
C     then singular value decomposition (SVD) is employed. SVD is not 
C     generally needed if the input sequence is sufficiently
C     persistently exciting and  NSMP  is large enough. 
C     If the matrix  W  cannot be stored in the workspace (i.e.,  
C     LDWORK < LDW2),  the QR decompositions of  W2  and  U  are 
C     computed sequentially.
C
C     REFERENCES
C
C     [1] Verhaegen M., and Varga, A.
C         Some Experience with the MOESP Class of Subspace Model
C         Identification Methods in Identifying the BO105 Helicopter.
C         Report TR R165-94, DLR Oberpfaffenhofen, 1994.
C
C     [2] Sima, V., and Varga, A.
C         RASP-IDENT : Subspace Model Identification Programs.
C         Deutsche Forschungsanstalt fur Luft- und Raumfahrt e. V., 
C         Report TR R888-94, DLR Oberpfaffenhofen, Oct. 1994.
C
C     NUMERICAL ASPECTS
C
C     The implemented method is numerically stable.
C
C     FURTHER COMMENTS
C
C     The algorithm for computing the system matrices  B  and  D  is 
C     less efficient than the MOESP or N4SID algorithms implemented in
C     SLICOT Library routine IB01PD, because a large least squares 
C     problem has to be solved, but the accuracy is better, as the 
C     computed matrices  B  and  D  are fitted to the input and output
C     trajectories. However, if matrix  A  is unstable, the computed
C     matrices  B  and  D  could be inaccurate.
C
C     CONTRIBUTOR
C
C     V. Sima, Research Institute for Informatics, Bucharest, Apr. 2000.
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
      DOUBLE PRECISION   ZERO, ONE, TWO, THREE
      PARAMETER          ( ZERO  = 0.0D0, ONE = 1.0D0, TWO = 2.0D0, 
     $                     THREE = 3.0D0 )
C     .. Scalar Arguments ..
      DOUBLE PRECISION   TOL
      INTEGER            INFO, IWARN, L, LDA, LDB, LDC, LDD, LDU,
     $                   LDWORK, LDY, M, N, NSMP
      CHARACTER          JOB, JOBX0
C     .. Array Arguments ..
      DOUBLE PRECISION   A(LDA, *), B(LDB, *), C(LDC, *), D(LDD, *),
     $                   DWORK(*),  U(LDU, *), X0(*), Y(LDY, *)
      INTEGER            IWORK(*)
C     .. Local Scalars ..
      DOUBLE PRECISION   RCOND, RCONDU, TOLL
      INTEGER            I, I2, IA, IAS, IC, ICYCLE, IE, IERR, IEXPON,
     $                   IG, IGAM, IGS, INI, INIH, INIR, INIS, INY,
     $                   INYGAM, IQ, IREM, IRHS, ISIZE, ISV, ITAU,
     $                   ITAUU, IUPNT, IX, IXINIT, IXSAVE, IY, IYPNT, J,
     $                   JWORK, K, LDDW, LDR, LDW2, LDW3, LM, LN, LNOB,
     $                   MAXWRK, MINSMP, MINWLS, MINWRK, N2M, NCOL,
     $                   NCP1, NCYCLE, NM, NN, NOBS, NROW, NSMPL, RANK
      LOGICAL            FIRST, NCYC, POWER2, WITHB, WITHD, WITHX0
C     .. Local Arrays ..
      DOUBLE PRECISION   DUM(1)
C     .. External Functions ..
      LOGICAL            LSAME
      INTEGER            ILAENV
      DOUBLE PRECISION   DLAMCH
      EXTERNAL           DLAMCH, ILAENV, LSAME
C     .. External Subroutines ..
      EXTERNAL           DAXPY, DCOPY, DGELSS, DGEMV, DGEQRF, DLACPY,
     $                   DLASET, DORMQR, DTRCON, DTRMM, DTRMV, DTRSM, 
     $                   MA02AD, MB01TD, MB02UD, MB04OD, MB04OY, XERBLA
C     .. Intrinsic Functions ..
      INTRINSIC          DBLE, INT, LOG, MAX, MIN, MOD
C     .. Executable Statements ..
C
C     Check the input parameters.
C
      WITHD  = LSAME( JOB,   'D' )
      WITHB  = LSAME( JOB,   'B' ) .OR. WITHD
      WITHX0 = LSAME( JOBX0, 'X' )
C
      IWARN = 0
      INFO  = 0
      LM    = L*M
      LN    = L*N
      NN    = N*N
      NM    = N*M
      N2M   = N*NM
      NCOL  = NM
      IF( WITHX0 )
     $   NCOL = NCOL + N
      MINSMP  = NCOL
      IF( WITHD ) THEN
         MINSMP = MINSMP + M
         IQ     = MINSMP
      ELSE IF ( .NOT.WITHX0 ) THEN
         IQ     = MINSMP
         MINSMP = MINSMP + 1
      ELSE
         IQ     = MINSMP
      END IF
C
      IF( .NOT.( WITHX0 .OR. LSAME( JOBX0, 'N' ) ) ) THEN
         INFO = -1
      ELSE IF( .NOT.WITHB ) THEN
         INFO = -2
      ELSE IF( N.LT.0 ) THEN
         INFO = -3
      ELSE IF( M.LT.0 ) THEN
         INFO = -4
      ELSE IF( L.LE.0 ) THEN
         INFO = -5
      ELSE IF( NSMP.LT.MINSMP ) THEN
         INFO = -6
      ELSE IF( LDA.LT.MAX( 1, N ) ) THEN
         INFO = -8
      ELSE IF( LDC.LT.L ) THEN
         INFO = -10
      ELSE IF( LDU.LT.1 .OR. ( M.GT.0 .AND. LDU.LT.NSMP ) ) THEN
         INFO = -12
      ELSE IF( LDY.LT.MAX( 1, NSMP ) ) THEN
         INFO = -14
      ELSE IF( LDB.LT.1 .OR. ( LDB.LT.N .AND. M.GT.0 ) )
     $      THEN
         INFO = -17
      ELSE IF( LDD.LT.1 .OR. ( WITHD .AND. LDD.LT.L .AND. M.GT.0 ) )
     $      THEN
         INFO = -19
      ELSE IF( TOL.GT.ONE ) THEN
         INFO = -20
      END IF
C
C     Compute workspace.
C      (Note: Comments in the code beginning "Workspace:" describe the
C       minimal amount of workspace needed at that point in the code,
C       as well as the preferred amount for good performance.
C       NB refers to the optimal block size for the immediately
C       following subroutine, as returned by ILAENV.)
C
      NSMPL = NSMP*L
      IQ    = IQ*L
      NCP1  = NCOL + 1
      ISIZE = NSMPL*NCP1
      IF ( N.GT.0 .AND. WITHX0 ) THEN
         IC = 2*NN + N
      ELSE
         IC = 0
      END IF
      MINWLS = NCOL*NCP1
      IF ( WITHD ) 
     $   MINWLS = MINWLS + LM*NCP1
      IF ( M.GT.0 .AND. WITHD ) THEN
         IA = M + MAX( 2*NCOL, M )
      ELSE
         IA = 2*NCOL
      END IF
      ITAU = N2M + MAX( IC, IA )
      IF ( WITHX0 )
     $   ITAU = ITAU + LN
      LDW2 = ISIZE  + MAX( N + MAX( IC, IA ), 6*NCOL )
      LDW3 = MINWLS + MAX( IQ*NCP1 + ITAU, 6*NCOL )
      IF ( M.GT.0 .AND. WITHD ) THEN
         LDW2 = MAX( LDW2, ISIZE  + 2*M*M + 6*M )
         LDW3 = MAX( LDW3, MINWLS + 2*M*M + 6*M )
      END IF
      MINWRK = MIN( LDW2, LDW3 )
      MINWRK = MAX( MINWRK, 2 )
      IF ( M.GT.0 .AND. WITHD ) 
     $   MINWRK = MAX( MINWRK, 3 )
      IF ( INFO.EQ.0 .AND. LDWORK.GE.MINWRK ) THEN
         IF ( M.GT.0 .AND. WITHD ) THEN
            MAXWRK = ISIZE + N + M + 
     $               MAX( M*ILAENV( 1, 'DGEQRF', ' ', NSMP, M, -1, -1 ),
     $                    NCOL + NCOL*ILAENV( 1, 'DGEQRF', ' ', NSMP-M,
     $                                        NCOL, -1, -1 ) )
            MAXWRK = MAX( MAXWRK, ISIZE + N + M + 
     $                    MAX( NCP1*ILAENV( 1, 'DORMQR', 'LT', NSMP,
     $                                      NCP1, M, -1 ),
     $                         NCOL + ILAENV( 1, 'DORMQR', 'LT',
     $                                        NSMP-M, 1, NCOL, -1 ) ) )
         ELSE
            MAXWRK = ISIZE + N + NCOL + 
     $               MAX( NCOL*ILAENV( 1, 'DGEQRF', ' ', NSMPL, NCOL,
     $                                 -1, -1 ),
     $                         ILAENV( 1, 'DORMQR', 'LT',NSMPL, 1, NCOL,
     $                                 -1 ) )
         END IF
         MAXWRK = MAX( MAXWRK, MINWRK )
      END IF
C
      IF ( INFO.EQ.0 .AND. LDWORK.LT.MINWRK ) THEN
         INFO = -23
         DWORK(1) = MINWRK
      END IF
C
C     Return if there are illegal arguments.
C
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'IB01QD', -INFO )
         RETURN
      END IF
C
C     Quick return if possible.
C
      IF ( MAX( N, M ).EQ.0 ) THEN
         DWORK(2) = ONE
         IF ( M.GT.0 .AND. WITHD ) THEN
            DWORK(1) = THREE
            DWORK(3) = ONE
         ELSE
            DWORK(1) = TWO
         END IF
         RETURN
      END IF
C
C     Set up the least squares problem, either directly, if enough
C     workspace, or sequentially, otherwise.
C
      IYPNT = 1
      IUPNT = 1
      LDDW  = ( LDWORK - MINWLS - ITAU )/NCP1
      NOBS  = MIN( NSMP, LDDW/L )
C
      IF ( LDWORK.GE.LDW2 .OR. NSMP.LE.NOBS ) THEN
C
C        Enough workspace for solving the problem directly.
C
         NCYCLE = 1
         NOBS   = NSMP
         LDDW   = MAX( 1, NSMPL )
         IF ( WITHD ) THEN
            INIR = M + 1
         ELSE
            INIR = 1
         END IF
         INY  = 1
         INIS = 1
      ELSE
C
C        NCYCLE > 1  cycles are needed for solving the problem
C        sequentially, taking  NOBS  samples in each cycle (or the
C        remaining samples in the last cycle).
C
         LNOB   = L*NOBS
         LDDW   = MAX( 1, LNOB )
         NCYCLE = NSMP/NOBS
         IF ( MOD( NSMP, NOBS ).NE.0 ) 
     $      NCYCLE = NCYCLE + 1
         INIR = 1
         INIH = INIR + NCOL*NCOL
         INIS = INIH + NCOL
         IF ( WITHD ) THEN
            INY = INIS + LM*NCP1
         ELSE
            INY = INIS
         END IF
      END IF
C
      NCYC   = NCYCLE.GT.1
      INYGAM = INY  + LDDW*NM
      IRHS   = INY  + LDDW*NCOL
      IXINIT = IRHS + LDDW
      IF( NCYC ) THEN
         IC = IXINIT + N2M
         IF ( WITHX0 ) THEN
            IA = IC + LN
         ELSE
            IA = IC
         END IF
         LDR = MAX( 1, NCOL )
         IE  = INY
      ELSE
         IF ( WITHD ) THEN
            INIH = IRHS + M
         ELSE
            INIH = IRHS
         END IF
         IA  = IXINIT + N
         LDR = LDDW
         IE  = IXINIT
      END IF
      IF ( N.GT.0 .AND. WITHX0 )
     $   IAS = IA + NN
C
      ITAUU = IA
      IF ( WITHD ) THEN
         ITAU = ITAUU + M
      ELSE
         ITAU = ITAUU
      END IF
      DUM(1) = ZERO
C
      DO 190 ICYCLE = 1, NCYCLE
         FIRST = ICYCLE.EQ.1
         IF ( .NOT.FIRST ) THEN
            IF ( ICYCLE.EQ.NCYCLE ) THEN
               NOBS = NSMP - ( NCYCLE - 1 )*NOBS
               LNOB = L*NOBS
            END IF
         END IF
C
         IY     = INY
         IXSAVE = IXINIT
C
C        Compute the  M*N  output trajectories for zero initial state
C        or for the saved final state value of the previous cycle.
C        This can be performed in parallel.
C        Workspace: need  s*L*(r + 1) + b + w, 
C                   where r = M*N + a,  s = NOBS,
C                         a = 0,             if JOBX0 = 'N';
C                         a = N,             if JOBX0 = 'X';
C                         b = N,             if NCYCLE = 1;
C                         b = N*N*M,         if NCYCLE > 1;
C                         w = 0,             if NCYCLE = 1;
C                         w = r*(r+1),       if NCYCLE > 1,  JOB = 'B';
C                         w = (M*L+r)*(r+1), if NCYCLE > 1,  JOB = 'D'.
C
         DO 40 J = 1, M
            DO 30 I = 1, N
C                            ij
C              Compute the  y    trajectory and put the vectorized form
C              of it in an appropriate column of  DWORK.  To gain in
C              efficiency, a specialization of SLICOT Library routine
C              TF01ND is used.
C
               IF ( FIRST )
     $            CALL DCOPY( N, DUM, 0, DWORK(IXSAVE), 1 )
               CALL DCOPY( N, DWORK(IXSAVE), 1, X0, 1 )
               INI = IY
C
               DO 20 K = 1, NOBS
                  CALL DGEMV( 'No transpose', L, N, ONE, C, LDC, X0, 1,
     $                        ZERO, DWORK(IY), NOBS )
                  IY = IY + 1
                  CALL DTRMV( 'Upper', 'No transpose', 'Non-unit', N,
     $                        A, LDA, X0, 1 )
C
                  DO 10 IX = 2, N
                     X0(IX) = X0(IX) + A(IX,IX-1)*DWORK(IXSAVE+IX-2)
   10             CONTINUE
C
                  X0(I) = X0(I) + U(IUPNT+K-1,J)
                  CALL DCOPY( N, X0, 1, DWORK(IXSAVE), 1 )
   20          CONTINUE
C
               IF ( NCYC )
     $            IXSAVE = IXSAVE + N
               IY = INI + LDDW
   30       CONTINUE
C
   40    CONTINUE
C
         IF ( N.GT.0 .AND. WITHX0 ) THEN
C
C           Compute the permuted extended observability matrix  Gamma
C                                                                ij
C           in the following  N  columns of  DWORK  (after the  y  
C           trajectories).  Gamma  is directly constructed in the
C           required row structure.
C           Workspace: need  s*L*(r + 1) + 2*N*N + N + b + c + w, 
C                      where c = 0,   if NCYCLE = 1;
C                            c = L*N, if NCYCLE > 1. 
C
            JWORK  = IAS + NN
            IG     = INYGAM
            IEXPON = INT( LOG( DBLE( NOBS ) )/LOG( TWO ) )
            IREM   = NOBS - 2**IEXPON
            POWER2 = IREM.EQ.0
            IF ( .NOT.POWER2 ) 
     $         IEXPON = IEXPON + 1
C
            IF ( FIRST ) THEN
C
               DO 50 I = 1, N 
                  CALL DCOPY( L, C(1,I), 1, DWORK(IG), NOBS )
                  IG = IG + LDDW
   50          CONTINUE
C
            ELSE
C
               DO 60 I = IC, IC + LN - 1, L
                  CALL DCOPY( L, DWORK(I), 1, DWORK(IG), NOBS )
                  IG = IG + LDDW
   60          CONTINUE
C
            END IF
C                                          p
C           Use powers of the matrix  A:  A ,  p = 2**(J-1).
C
            CALL DLACPY( 'Upper', N, N, A, LDA, DWORK(IA), N )
            IF( N.GT.1 )
     $         CALL DCOPY( N-1, A(2,1), LDA+1, DWORK(IA+1), N+1 )
            I2   = 1
            NROW = 0
C
            DO 90 J = 1, IEXPON
               IGAM = INYGAM
               IF ( J.LT.IEXPON .OR. POWER2 ) THEN
                  NROW = I2
               ELSE 
                  NROW = IREM
               END IF
C
               DO 80 I = 1, L
                  CALL DLACPY( 'Full', NROW, N, DWORK(IGAM), LDDW,
     $                         DWORK(IGAM+I2), LDDW )
                  CALL DTRMM(  'Right', 'Upper', 'No Transpose',
     $                         'Non Unit', NROW, N, ONE, DWORK(IA), N,
     $                         DWORK(IGAM+I2), LDDW )
                  IG = IGAM
C                                                                  p
C                 Compute the contribution of the subdiagonal of  A 
C                 to the product.
C
                  DO 70 IX = 1, N - 1
                     CALL DAXPY( NROW, DWORK(IA+(IX-1)*N+IX),
     $                           DWORK(IG+LDDW), 1, DWORK(IG+I2), 1 )
                     IG = IG + LDDW
   70             CONTINUE
C
                  IGAM = IGAM + NOBS
   80          CONTINUE
C  
               IF ( J.LT.IEXPON ) THEN
                  CALL DLACPY( 'Upper', N, N, DWORK(IA), N, DWORK(IAS),
     $                         N )
                  IF( N.GT.1 )
     $               CALL DCOPY( N-1, DWORK(IA+1), N+1, DWORK(IAS+1),
     $                           N+1 )
                  CALL MB01TD( N, DWORK(IAS), N, DWORK(IA), N,
     $                         DWORK(JWORK), IERR )
                  I2 = I2*2
               END IF
   90       CONTINUE
C
            IF ( NCYC .AND. ICYCLE.LT.NCYCLE ) THEN
               IG  = INYGAM + I2 + NROW - 1
               IGS = IG
C
               DO 100 I = IC, IC + LN - 1, L
                  CALL DCOPY( L, DWORK(IG), NOBS, DWORK(I), 1 )
                  IG = IG + LDDW
  100          CONTINUE
C
               CALL DTRMM( 'Right', 'Upper', 'No Transpose', 'Non Unit',
     $                     L, N, ONE, A, LDA, DWORK(IC), L )
               IG = IGS
C
C              Compute the contribution of the subdiagonal of  A  to the
C              product.
C
               DO 110 IX = 1, N - 1
                  CALL DAXPY( L, A(IX+1,IX), DWORK(IG+LDDW), NOBS,  
     $                        DWORK(IC+(IX-1)*L), 1 )
                  IG = IG + LDDW
  110          CONTINUE
C
            END IF
         END IF
C
C        Setup (part of) the right hand side of the least squares
C        problem.
C
         IY = IRHS
C
         DO 120 K = 1, L
            CALL DCOPY( NOBS, Y(IYPNT,K), 1, DWORK(IY), 1 )
            IY = IY + NOBS
  120    CONTINUE
C
C        Compress the data using a special QR factorization.
C        Workspace: need   v + y, 
C                   where  v = s*L*(r + 1) + b + c + w + x,
C                          x = M,  y = max( 2*r, M ),
C                                             if  JOB = 'D'  and  M > 0,
C                          x = 0,  y = 2*r,   if  JOB = 'B'  or   M = 0.
C
         IF ( M.GT.0 .AND. WITHD ) THEN
C
C           Case 1:  D  is requested.
C
            JWORK = ITAU
            IF ( FIRST ) THEN
               INI = INY + M
C
C              Compress the first or single segment of  U,  U1 = Q1*R1.
C              Workspace: need   v + M; 
C                         prefer v + M*NB.
C
               CALL DGEQRF( NOBS, M, U, LDU, DWORK(ITAUU), DWORK(JWORK),
     $                      LDWORK-JWORK+1, IERR )
C                                                  ij
C              Apply  diag(Q1')  to the matrix  [ y   Gamma Y ].
C              Workspace: need   v + r + 1, 
C                         prefer v + (r + 1)*NB.
C
               DO 130 K = 1, L
                  CALL DORMQR( 'Left', 'Transpose', NOBS, NCP1, M, U, 
     $                         LDU, DWORK(ITAUU), DWORK(INY+(K-1)*NOBS),
     $                         LDDW, DWORK(JWORK), LDWORK-JWORK+1,
     $                         IERR )
  130          CONTINUE
C
               IF ( NCOL.GT.0 ) THEN
C
C                 Compress the first part of the first data segment of
C                    ij
C                 [ y   Gamma ].
C                 Workspace: need   v + 2*r, 
C                            prefer v + r + r*NB.
C
                  JWORK = ITAU + NCOL
                  CALL DGEQRF( NOBS-M, NCOL, DWORK(INI), LDDW,
     $                         DWORK(ITAU), DWORK(JWORK),
     $                         LDWORK-JWORK+1, IERR )
C
C                 Apply the transformation to the corresponding right
C                 hand side part.
C                 Workspace: need   v + r + 1, 
C                            prefer v + r + NB.
C
                  CALL DORMQR( 'Left', 'Transpose', NOBS-M, 1, NCOL,
     $                         DWORK(INI), LDDW, DWORK(ITAU),
     $                         DWORK(IRHS+M), LDDW, DWORK(JWORK),
     $                         LDWORK-JWORK+1, IERR )
C
C                 Compress the remaining parts of the first data segment
C                        ij
C                 of  [ y   Gamma ].
C                 Workspace: need   v + r - 1.
C
                  DO 140 K = 2, L
                     CALL MB04OD( 'Full', NCOL, 1, NOBS-M, DWORK(INI),
     $                            LDDW, DWORK(INI+(K-1)*NOBS), LDDW,
     $                            DWORK(IRHS+M), LDDW, 
     $                            DWORK(IRHS+M+(K-1)*NOBS), LDDW,
     $                            DWORK(ITAU), DWORK(JWORK) )
  140             CONTINUE
C
               END IF
C
               IF ( NCYC ) THEN
C                                                   ij
C                 Save the triangular factor of  [ y   Gamma ],  the 
C                 corresponding right hand side, and the first  M  rows
C                 in each  NOBS  group of rows. 
C                 Workspace: need   v.
C
                  CALL DLACPY( 'Upper', NCOL, NCP1, DWORK(INI), LDDW, 
     $                         DWORK(INIR), LDR )
C
                  DO 150 K = 1, L
                     CALL DLACPY( 'Full', M, NCP1, 
     $                            DWORK(INY +(K-1)*NOBS), LDDW, 
     $                            DWORK(INIS+(K-1)*M), LM )
  150             CONTINUE
C
               END IF
            ELSE
C
C              Compress the current data segment of  U,  Ui = Qi*Ri,
C              i = ICYCLE.
C              Workspace: need   v + r + 1.
C
               CALL MB04OD( 'Full', M, NCP1, NOBS, U, LDU, U(IUPNT,1),
     $                      LDU, DWORK(INIS), LM, DWORK(INY), LDDW,
     $                      DWORK(ITAUU), DWORK(JWORK) )
C
C              Apply  diag(Qi')  to the appropriate part of the matrix  
C                 ij
C              [ y   Gamma Y ].
C              Workspace: need   v + r + 1.
C
               DO 170 K = 2, L
C
                  DO 160 IX = 1, M
                     CALL MB04OY( NOBS, NCP1, U(IUPNT,IX),
     $                            DWORK(ITAUU+IX-1),
     $                            DWORK(INIS+(K-1)*M+IX-1), LM, 
     $                            DWORK(INY+(K-1)*NOBS), LDDW,
     $                            DWORK(JWORK) )
  160             CONTINUE
C
  170          CONTINUE
C
               IF ( NCOL.GT.0 ) THEN
C
                  JWORK = ITAU + NCOL
C
C                 Compress the current (but not the first) data segment
C                        ij
C                 of  [ y   Gamma ].
C                 Workspace: need   v + r - 1.
C
                  DO 180 K = 1, L
                     CALL MB04OD( 'Full', NCOL, 1, NOBS, DWORK(INIR),
     $                            LDR, DWORK(INY+(K-1)*NOBS), LDDW,
     $                            DWORK(INIH), LDR, 
     $                            DWORK(IRHS+(K-1)*NOBS), LDDW,
     $                            DWORK(ITAU), DWORK(JWORK) )
  180             CONTINUE
C
               END IF
            END IF
C
         ELSE IF ( NCOL.GT.0 ) THEN
C
C           Case 2:  D  is known to be zero.
C
            JWORK = ITAU + NCOL
            IF ( FIRST ) THEN
C
C              Compress the first or single data segment of
C                 ij
C              [ y   Gamma ].
C              Workspace: need   v + 2*r, 
C                         prefer v + r + r*NB.
C
               CALL DGEQRF( LDDW, NCOL, DWORK(INY), LDDW, DWORK(ITAU),
     $                      DWORK(JWORK), LDWORK-JWORK+1, IERR )
C
C              Apply the transformation to the right hand side.
C              Workspace: need   v + r + 1, 
C                         prefer v + r + NB.
C
               CALL DORMQR( 'Left', 'Transpose', LDDW, 1, NCOL,
     $                      DWORK(INY), LDDW, DWORK(ITAU), DWORK(IRHS),
     $                      LDDW, DWORK(JWORK), LDWORK-JWORK+1, IERR )
               IF ( NCYC ) THEN
C                                                   ij
C                 Save the triangular factor of  [ y   Gamma ]  and the
C                 corresponding right hand side. 
C                 Workspace: need   v.
C
                  CALL DLACPY( 'Upper', NCOL, NCP1, DWORK(INY), LDDW, 
     $                         DWORK(INIR), LDR )
               END IF
            ELSE
C
C              Compress the current (but not the first) data segment.
C              Workspace: need   v + r - 1.
C
               CALL MB04OD( 'Full', NCOL, 1, LNOB, DWORK(INIR), LDR,
     $                      DWORK(INY), LDDW, DWORK(INIH), LDR,
     $                      DWORK(IRHS), LDDW, DWORK(ITAU),
     $                      DWORK(JWORK) )
            END IF
         END IF
C
         IUPNT = IUPNT + NOBS
         IYPNT = IYPNT + NOBS
  190 CONTINUE
C
C     Estimate the reciprocal condition number of the triangular factor
C     of the QR decomposition.
C     Workspace: need  u + 3*r, where
C                      u = t*L*(r + 1), if NCYCLE = 1;
C                      u = w,           if NCYCLE > 1.
C
      CALL DTRCON( '1-norm', 'Upper', 'No Transpose', NCOL, DWORK(INIR),
     $             LDR, RCOND, DWORK(IE), IWORK, IERR )
C
      TOLL = TOL
      IF ( TOLL.LE.ZERO )
     $   TOLL = DLAMCH( 'Precision' )
      IF ( RCOND.LE.TOLL**( TWO/THREE ) ) THEN
         IWARN = 4
C
C        The least squares problem is ill-conditioned.
C        Use SVD to solve it.
C        Workspace: need   u + 6*r;
C                   prefer larger.
C
         IF ( NCOL.GT.1 ) 
     $      CALL DLASET( 'Lower', NCOL-1, NCOL-1, ZERO, ZERO,
     $                   DWORK(INIR+1), LDR )
         ISV   = IE
         JWORK = ISV + NCOL
         CALL DGELSS( NCOL, NCOL, 1, DWORK(INIR), LDR, DWORK(INIH), LDR,
     $                DWORK(ISV), TOLL, RANK, DWORK(JWORK), 
     $                LDWORK-JWORK+1, IERR )
         IF ( IERR.GT.0 ) THEN
C
C           Return if SVD algorithm did not converge.
C
            INFO = 2
            RETURN
         END IF
         MAXWRK = MAX( MAXWRK, INT( DWORK(JWORK) ) - JWORK + 1 )
      ELSE
C
C        Find the least squares solution using QR decomposition only.
C
         CALL DTRSM( 'Left', 'Upper', 'No Transpose', 'Non Unit', NCOL,
     $               1, ONE, DWORK(INIR), LDR, DWORK(INIH), LDR )
      END IF
C
C     Setup the estimated n-by-m input matrix  B,  and the estimated
C     initial state of the system  x0.
C
      CALL DLACPY( 'Full', N, M, DWORK(INIH), N, B, LDB )
C
      IF ( N.GT.0 .AND. WITHX0 ) THEN
         CALL DCOPY( N, DWORK(INIH+NM), 1, X0, 1 )
      ELSE
         CALL DCOPY( N, DUM, 0, X0, 1 )
      END IF
C
      IF ( M.GT.0 .AND. WITHD ) THEN
C
C        Compute the estimated l-by-m input/output matrix  D.
C
         IF ( NCYC ) THEN
            IRHS = INIS + LM*NCOL
            CALL DGEMV( 'No Transpose', LM, NCOL, -ONE, DWORK(INIS),
     $                  LM, DWORK(INIH), 1, ONE, DWORK(IRHS), 1 )
         ELSE
C
            DO 200 K = 1, L
               CALL DGEMV( 'No Transpose', M, NCOL, -ONE,
     $                     DWORK(INIS+(K-1)*NOBS), LDDW, DWORK(INIH), 1,
     $                     ONE, DWORK(IRHS+(K-1)*NOBS), 1 )
  200       CONTINUE
C
            DO 210 K = 2, L
               CALL DCOPY( M, DWORK(IRHS+(K-1)*NOBS), 1, 
     $                     DWORK(IRHS+(K-1)*M), 1 )
  210       CONTINUE
C
         END IF
C
C        Estimate the reciprocal condition number of the triangular
C        factor of the QR decomposition of the matrix U.
C        Workspace: need  u + 3*M.
C
         CALL DTRCON( '1-norm', 'Upper', 'No Transpose', M, U, LDU,
     $                RCONDU, DWORK(IE), IWORK, IERR )
         IF ( RCONDU.LE.TOLL**( TWO/THREE ) ) THEN
            IWARN = 4
C
C           The least squares problem is ill-conditioned.
C           Use SVD to solve it. (QR decomposition of  U  is preserved.)
C           Workspace: need   u + 2*M*M + 6*M;
C                      prefer larger.
C
            IQ    = IE  + M*M
            ISV   = IQ  + M*M
            JWORK = ISV + M
            CALL DLACPY( 'Upper', M, M, U, LDU, DWORK(IE), M )
            CALL MB02UD( 'Not Factored', 'Left', 'No Transpose', 
     $                   'No Pinv', M, L, ONE, TOLL, RANK, DWORK(IE),
     $                   M, DWORK(IQ), M, DWORK(ISV), DWORK(IRHS), M,
     $                   DUM, 1, DWORK(JWORK), LDWORK-JWORK+1, IERR )
            IF ( IERR.GT.0 ) THEN
C
C              Return if SVD algorithm did not converge.
C
               INFO = 2
               RETURN
            END IF
            MAXWRK = MAX( MAXWRK, INT( DWORK(JWORK) ) - JWORK + 1 )
         ELSE
            CALL DTRSM( 'Left', 'Upper', 'No Transpose', 'Non Unit', M,
     $                  L, ONE, U, LDU, DWORK(IRHS), M )
         END IF
         CALL MA02AD( 'Full', M, L, DWORK(IRHS), M, D, LDD )
C
      END IF
C
      DWORK(1) = MAXWRK
      DWORK(2) = RCOND
      IF ( M.GT.0 .AND. WITHD )
     $   DWORK(3) = RCONDU
C
      RETURN
C
C *** End of IB01QD ***
      END
