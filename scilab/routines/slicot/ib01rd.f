      SUBROUTINE IB01RD( JOB, N, M, L, NSMP, A, LDA, B, LDB, C, LDC, D,
     $                   LDD, U, LDU, Y, LDY, X0, TOL, IWORK, DWORK,
     $                   LDWORK, IWARN, INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 2000.
C
C     PURPOSE
C
C     To estimate the initial state of a linear time-invariant (LTI)
C     discrete-time system, given the system matrices  (A,B,C,D)  and
C     the input and output trajectories of the system. The model
C     structure is :
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
C     JOB     CHARACTER*1
C             Specifies whether or not the matrix D is zero, as follows:
C             = 'Z':  the matrix  D  is zero;
C             = 'N':  the matrix  D  is not zero.
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
C             samples used,  t).  NSMP >= N.
C
C     A       (input) DOUBLE PRECISION array, dimension (LDA,N)
C             The leading N-by-N part of this array must contain the
C             system state matrix  A  in a real Schur form. 
C
C     LDA     INTEGER
C             The leading dimension of the array A.  LDA >= MAX(1,N).
C
C     B       (input) DOUBLE PRECISION array, dimension (LDB,M)
C             The leading N-by-M part of this array must contain the
C             system input matrix  B  (corresponding to the real Schur
C             form of  A).
C             If  N = 0  or  M = 0,  this array is not referenced.
C
C     LDB     INTEGER
C             The leading dimension of the array B.
C             LDB >= N,  if  N > 0  and  M > 0;
C             LDB >= 1,  if  N = 0  or   M = 0.
C
C     C       (input) DOUBLE PRECISION array, dimension (LDC,N)
C             The leading L-by-N part of this array must contain the
C             system output matrix  C  (corresponding to the real Schur
C             form of  A).
C
C     LDC     INTEGER
C             The leading dimension of the array C.  LDC >= L.
C
C     D       (input) DOUBLE PRECISION array, dimension (LDD,M)
C             The leading L-by-M part of this array must contain the
C             system input-output matrix.
C             If  M = 0  or  JOB = 'Z',  this array is not referenced.  
C
C     LDD     INTEGER
C             The leading dimension of the array D.
C             LDD >= L,  if  M > 0  and  JOB = 'N';
C             LDD >= 1,  if  M = 0  or   JOB = 'Z'.
C
C     U       (input) DOUBLE PRECISION array, dimension (LDU,M)
C             If  M > 0,  the leading NSMP-by-M part of this array must
C             contain the t-by-m input-data sequence matrix  U,
C             U = [u_1 u_2 ... u_m].  Column  j  of  U  contains the
C             NSMP  values of the j-th input component for consecutive
C             time increments.
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
C             The estimated initial state of the system,  x(0).
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
C     IWORK   INTEGER array, dimension (N)
C
C     DWORK   DOUBLE PRECISION array, dimension (LDWORK)
C             On exit, if  INFO = 0,  DWORK(1) returns the optimal value
C             of LDWORK and  DWORK(2)  contains the reciprocal condition
C             number of the triangular factor of the QR factorization of
C             the matrix  Gamma  (see METHOD).   
C             On exit, if  INFO = -22,  DWORK(1)  returns the minimum
C             value of LDWORK.
C
C     LDWORK  INTEGER
C             The length of the array DWORK.
C             LDWORK >= max( 2, min( LDW1, LDW2 ) ),  where
C             LDW1 = t*L*(N + 1) + 2*N + max( 2*N*N, 4*N ), 
C             LDW2 =   N*(N + 1) + 2*N + 
C                      max( q*(N + 1) + 2*N*N + L*N, 4*N ),
C                q = N*L.
C             For good performance,  LDWORK  should be larger.
C             If  LDWORK >= LDW1,  then standard QR factorization of
C             the matrix  Gamma  (see METHOD) is used. Otherwise, the 
C             QR factorization is computed sequentially by performing
C             NCYCLE  cycles, each cycle (except possibly the last one)
C             processing  s  samples, where  s  is chosen by equating
C             LDWORK  to  LDW2,  for  q  replaced by  s*L.
C             The computational effort may increase and the accuracy may
C             decrease with the decrease of  s.  Recommended value is
C             LDRWRK = LDW1,  assuming a large enough cache size, to
C             also accommodate  A, B, C, D, U,  and  Y.
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
C     An extension and refinement of the method in [1] is used. 
C     Specifically, the output y0(k) of the system for zero initial
C     state is computed for k = 0, 1, ...,  t-1 using the given model.
C     Then the following least squares problem is solved for x(0)
C
C                         (     C     )            (   y(0) - y0(0)   )
C                         (    C*A    )            (   y(1) - y0(1)   )
C        Gamma * x(0)  =  (     :     ) * x(0)  =  (        :         ).
C                         (     :     )            (        :         )
C                         ( C*A^(t-1) )            ( y(t-1) - y0(t-1) )
C
C     The coefficient matrix  Gamma  is evaluated using powers of A with
C     exponents 2^k. The QR decomposition of this matrix is computed.
C     If its triangular factor  R  is too ill conditioned, then singular
C     value decomposition of  R  is used.
C
C     If the coefficient matrix cannot be stored in the workspace (i.e.,
C     LDWORK < LDW1),  the QR decomposition is computed sequentially.
C
C     REFERENCES
C
C     [1] Verhaegen M., and Varga, A.
C         Some Experience with the MOESP Class of Subspace Model
C         Identification Methods in Identifying the BO105 Helicopter.
C         Report TR R165-94, DLR Oberpfaffenhofen, 1994.
C
C     NUMERICAL ASPECTS
C
C     The implemented method is numerically stable.
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
C     IBLOCK is a threshold value for switching to a block algorithm
C     for  U  (to avoid row by row passing through  U).
      INTEGER            IBLOCK
      PARAMETER          ( IBLOCK = 16384 )
C     .. Scalar Arguments ..
      DOUBLE PRECISION   TOL
      INTEGER            INFO, IWARN, L, LDA, LDB, LDC, LDD, LDU,
     $                   LDWORK, LDY, M, N, NSMP
      CHARACTER          JOB
C     .. Array Arguments ..
      DOUBLE PRECISION   A(LDA, *), B(LDB, *), C(LDC, *), D(LDD, *),
     $                   DWORK(*),  U(LDU, *), X0(*), Y(LDY, *)
      INTEGER            IWORK(*)
C     .. Local Scalars ..
      DOUBLE PRECISION   RCOND, TOLL
      INTEGER            I2, IA, IAS, IC, ICYCLE, IE, IERR, IEXPON,
     $                   IG, INIGAM, INIH, INIR, INIT, IQ, IREM, IRHS,
     $                   ISIZE, ISV, ITAU, IU, IUPNT, IUT, IUTRAN, IX,
     $                   IXINIT, IY, IYPNT, J, JWORK, K, LDDW, LDR,
     $                   LDW1, LDW2, MAXWRK, MINSMP, MINWLS, MINWRK, NC,
     $                   NCP1, NCYCLE, NN, NOBS, NRBL, NROW, NSMPL, RANK
      LOGICAL            BLOCK, FIRST, NCYC, POWER2, SWITCH, WITHD
C     .. Local Arrays ..
      DOUBLE PRECISION   DUM(1)
C     .. External Functions ..
      LOGICAL            LSAME
      INTEGER            ILAENV
      DOUBLE PRECISION   DLAMCH
      EXTERNAL           DLAMCH, ILAENV, LSAME
C     .. External Subroutines ..
      EXTERNAL           DAXPY, DCOPY, DGELSS, DGEMV, DGEQRF, DLACPY,
     $                   DLASET, DORMQR, DTRCON, DTRMM, DTRMV, DTRSV, 
     $                   MA02AD, MB01TD, MB04OD, XERBLA
C     .. Intrinsic Functions ..
      INTRINSIC          DBLE, INT, LOG, MAX, MIN, MOD
C     .. Executable Statements ..
C
C     Check the input parameters.
C
      WITHD  = LSAME( JOB, 'N' )
      IWARN  = 0
      INFO   = 0
      NN     = N*N
      MINSMP = N
C
      IF( .NOT.( LSAME( JOB, 'Z' ) .OR. WITHD ) ) THEN
         INFO = -1
      ELSE IF( N.LT.0 ) THEN
         INFO = -2
      ELSE IF( M.LT.0 ) THEN
         INFO = -3
      ELSE IF( L.LE.0 ) THEN
         INFO = -4
      ELSE IF( NSMP.LT.MINSMP ) THEN
         INFO = -5
      ELSE IF( LDA.LT.MAX( 1, N ) ) THEN
         INFO = -7
      ELSE IF( LDB.LT.1 .OR. ( LDB.LT.N .AND. M.GT.0 ) ) THEN
         INFO = -9
      ELSE IF( LDC.LT.L ) THEN
         INFO = -11
      ELSE IF( LDD.LT.1 .OR. ( WITHD .AND. LDD.LT.L .AND. M.GT.0 ) )
     $      THEN
         INFO = -13
      ELSE IF( LDU.LT.1 .OR. ( M.GT.0 .AND. LDU.LT.NSMP ) ) THEN
         INFO = -15
      ELSE IF( LDY.LT.MAX( 1, NSMP ) ) THEN
         INFO = -17
      ELSE IF( TOL.GT.ONE ) THEN
         INFO = -19
      END IF
C
C     Compute workspace.
C      (Note: Comments in the code beginning "Workspace:" describe the
C       minimal amount of workspace needed at that point in the code,
C       as well as the preferred amount for good performance.
C       NB refers to the optimal block size for the immediately
C       following subroutine, as returned by ILAENV.)
C
      NSMPL  = NSMP*L
      IQ     = MINSMP*L
      NCP1   = N + 1
      ISIZE  = NSMPL*NCP1
      IC     = 2*NN
      MINWLS = MINSMP*NCP1
      ITAU   = IC + L*N
      LDW1   = ISIZE  + 2*N + MAX( IC, 4*N )
      LDW2   = MINWLS + 2*N + MAX( IQ*NCP1 + ITAU, 4*N )
      MINWRK = MAX( MIN( LDW1, LDW2 ), 2 )
      IF ( INFO.EQ.0 .AND. LDWORK.GE.MINWRK ) THEN
         MAXWRK = ISIZE + 2*N + MAX( N*ILAENV( 1, 'DGEQRF', ' ', NSMPL,
     $                                         N, -1, -1 ),
     $                               ILAENV( 1, 'DORMQR', 'LT', NSMPL,
     $                                       1, N, -1 ) )
         MAXWRK = MAX( MAXWRK, MINWRK )
      END IF
C
      IF ( INFO.EQ.0 .AND. LDWORK.LT.MINWRK ) THEN
         INFO = -22
         DWORK(1) = MINWRK
      END IF
C
C     Return if there are illegal arguments.
C
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'IB01RD', -INFO )
         RETURN
      END IF
C
C     Quick return if possible.
C
      IF ( N.EQ.0 ) THEN
         DWORK(1) = TWO
         DWORK(2) = ONE
         RETURN
      END IF
C
C     Set up the least squares problem, either directly, if enough
C     workspace, or sequentially, otherwise.
C
      IYPNT = 1
      IUPNT = 1
      INIR  = 1
      IF ( LDWORK.GE.LDW1 ) THEN
C
C        Enough workspace for solving the problem directly.
C
         NCYCLE = 1
         NOBS   = NSMP
         LDDW   = NSMPL
         INIGAM = 1
      ELSE
C
C        NCYCLE > 1  cycles are needed for solving the problem
C        sequentially, taking  NOBS  samples in each cycle (or the
C        remaining samples in the last cycle).
C
         JWORK  = LDWORK - MINWLS - 2*N - ITAU
         LDDW   = JWORK/NCP1
         NOBS   = LDDW/L
         LDDW   = L*NOBS
         NCYCLE = NSMP/NOBS
         IF ( MOD( NSMP, NOBS ).NE.0 ) 
     $      NCYCLE = NCYCLE + 1
         INIH   = INIR + NN
         INIGAM = INIH + N
      END IF
C
      NCYC   = NCYCLE.GT.1
      IRHS   = INIGAM + LDDW*N
      IXINIT = IRHS   + LDDW
      IC     = IXINIT + N
      IF( NCYC ) THEN
         IA   = IC + L*N
         LDR  = N
         IE   = INIGAM
      ELSE
         INIH = IRHS
         IA   = IC
         LDR  = LDDW
         IE   = IXINIT
      END IF
      IUTRAN = IA
      IAS    = IA + NN
      ITAU   = IA
      DUM(1) = ZERO
C
C     Set block parameters for passing through the array  U.
C
      BLOCK = M.GT.1 .AND. NSMP*M.GE.IBLOCK
      IF ( BLOCK ) THEN
         NRBL = ( LDWORK - IUTRAN + 1 )/M
         NC   = NOBS/NRBL
         IF ( MOD( NOBS, NRBL ).NE.0 )
     $      NC = NC + 1
         INIT  = ( NC - 1 )*NRBL
         BLOCK = BLOCK .AND. NRBL.GT.1
      END IF
C
C     Perform direct of sequential compression of the matrix  Gamma.
C
      DO 150 ICYCLE = 1, NCYCLE
         FIRST = ICYCLE.EQ.1
         IF ( .NOT.FIRST ) THEN
            IF ( ICYCLE.EQ.NCYCLE ) THEN
               NOBS = NSMP - ( NCYCLE - 1 )*NOBS
               LDDW = L*NOBS
               IF ( BLOCK ) THEN
                  NC = NOBS/NRBL
                  IF ( MOD( NOBS, NRBL ).NE.0 )
     $               NC = NC + 1
                  INIT = ( NC - 1 )*NRBL
               END IF
            END IF
         END IF
C
C        Compute the extended observability matrix  Gamma.
C        Workspace: need   s*L*(N + 1) + 2*N*N + 2*N + a + w,
C                   where  s = NOBS,
C                          a = 0,   w = 0,          if NCYCLE = 1,
C                          a = L*N, w = N*(N + 1),  if NCYCLE > 1; 
C                   prefer as above, with  s = t,  a = w = 0.
C
         JWORK  = IAS + NN
         IEXPON = INT( LOG( DBLE( NOBS ) )/LOG( TWO ) )
         IREM   = L*( NOBS - 2**IEXPON )
         POWER2 = IREM.EQ.0
         IF ( .NOT.POWER2 ) 
     $      IEXPON = IEXPON + 1
C
         IF ( FIRST ) THEN
            CALL DLACPY( 'Full', L, N, C, LDC, DWORK(INIGAM), LDDW )
         ELSE
            CALL DLACPY( 'Full', L, N, DWORK(IC), L, DWORK(INIGAM), 
     $                   LDDW )
         END IF
C                                       p
C        Use powers of the matrix  A:  A ,  p = 2**(J-1).
C
         CALL DLACPY( 'Upper', N, N, A, LDA, DWORK(IA), N )
         CALL DCOPY( N-1, A(2,1), LDA+1, DWORK(IA+1), N+1 )
         I2   = L
         NROW = 0
C
         DO 20 J = 1, IEXPON
            IG = INIGAM
            IF ( J.LT.IEXPON .OR. POWER2 ) THEN
               NROW = I2
            ELSE 
               NROW = IREM
            END IF
C
            CALL DLACPY( 'Full', NROW, N, DWORK(IG), LDDW, DWORK(IG+I2),
     $                   LDDW )
            CALL DTRMM(  'Right', 'Upper', 'No Transpose', 'Non Unit',
     $                   NROW, N, ONE, DWORK(IA), N, DWORK(IG+I2),
     $                   LDDW )
C                                                            p
C           Compute the contribution of the subdiagonal of  A   to the
C           product.
C
            DO 10 IX = 1, N - 1
               CALL DAXPY( NROW, DWORK(IA+(IX-1)*N+IX), DWORK(IG+LDDW),
     $                     1, DWORK(IG+I2), 1 )
               IG = IG + LDDW
   10       CONTINUE
C  
            IF ( J.LT.IEXPON ) THEN
               CALL DLACPY( 'Upper', N, N, DWORK(IA), N, DWORK(IAS), N )
               CALL DCOPY(  N-1, DWORK(IA+1), N+1, DWORK(IAS+1), N+1 )
               CALL MB01TD( N, DWORK(IAS), N, DWORK(IA), N,
     $                      DWORK(JWORK), IERR )
               I2 = I2*2
            END IF
   20    CONTINUE
C
         IF ( NCYC ) THEN
            IG = INIGAM + I2 + NROW - L
            CALL DLACPY( 'Full', L, N, DWORK(IG), LDDW, DWORK(IC), L )
            CALL DTRMM( 'Right', 'Upper', 'No Transpose', 'Non Unit', L,
     $                  N, ONE, A, LDA, DWORK(IC), L )
C
C           Compute the contribution of the subdiagonal of  A  to the
C           product.
C
            DO 30 IX = 1, N - 1
               CALL DAXPY( L, A(IX+1,IX), DWORK(IG+LDDW), 1,  
     $                     DWORK(IC+(IX-1)*L), 1 )
               IG = IG + LDDW
   30       CONTINUE
C
         END IF
C
C        Setup (part of) the right hand side of the least squares
C        problem starting from  DWORK(IRHS);  use the estimated output
C        trajectory for zero initial state, or for the saved final state
C        value of the previous cycle.  
C        A specialization of SLICOT Library routine TF01ND is used.
C        For large input sets  (NSMP*M >= IBLOCK),  chunks of  U  are 
C        transposed, to reduce the number of row-wise passes.
C        Workspace: need   s*L*(N + 1) + N + w;
C                   prefer as above, with  s = t,  w = 0.
C
         IF ( FIRST )
     $      CALL DCOPY( N, DUM, 0, DWORK(IXINIT), 1 )
         CALL DCOPY( N, DWORK(IXINIT), 1, X0, 1 )
         IY = IRHS
C
         DO 40 J = 1, L
            CALL DCOPY( NOBS, Y(IYPNT,J), 1, DWORK(IY), L )
            IY = IY + 1
   40    CONTINUE
C
         IY = IRHS
         IU = IUPNT
         IF ( M.GT.0 ) THEN
            IF ( WITHD ) THEN
C
               IF ( BLOCK ) THEN
                  SWITCH = .TRUE.
                  NROW   = NRBL
C
                  DO 60 K = 1, NOBS
                     IF ( MOD( K-1, NROW ).EQ.0 .AND. SWITCH ) THEN
                        IUT = IUTRAN
                        IF ( K.GT.INIT ) THEN
                           NROW = NOBS - INIT
                           SWITCH = .FALSE.
                        END IF
                        CALL MA02AD( 'Full', NROW, M, U(IU,1), LDU,
     $                               DWORK(IUT), M )
                        IU = IU + NROW
                     END IF
                     CALL DGEMV( 'No transpose', L, N, -ONE, C, LDC, X0,
     $                           1, ONE, DWORK(IY), 1 )
                     CALL DGEMV( 'No transpose', L, M, -ONE, D, LDD,
     $                           DWORK(IUT), 1, ONE, DWORK(IY), 1 )
                     CALL DTRMV( 'Upper', 'No transpose', 'Non-unit', N,
     $                           A, LDA, X0, 1 )
C
                     DO 50 IX = 2, N
                        X0(IX) = X0(IX) + A(IX,IX-1)*DWORK(IXINIT+IX-2)
   50                CONTINUE
C
                     CALL DGEMV( 'No transpose', N, M, ONE, B, LDB,
     $                           DWORK(IUT), 1, ONE, X0, 1 )
                     CALL DCOPY( N, X0, 1, DWORK(IXINIT), 1 )
                     IY  = IY  + L
                     IUT = IUT + M
   60             CONTINUE
C
               ELSE
C 
                  DO 80 K = 1, NOBS
                     CALL DGEMV( 'No transpose', L, N, -ONE, C, LDC, X0,
     $                           1, ONE, DWORK(IY), 1 )
                     CALL DGEMV( 'No transpose', L, M, -ONE, D, LDD,
     $                           U(IU,1), LDU, ONE, DWORK(IY), 1 )
                     CALL DTRMV( 'Upper', 'No transpose', 'Non-unit', N,
     $                           A, LDA, X0, 1 )
C
                     DO 70 IX = 2, N
                        X0(IX) = X0(IX) + A(IX,IX-1)*DWORK(IXINIT+IX-2)
   70                CONTINUE
C
                     CALL DGEMV( 'No transpose', N, M, ONE, B, LDB,
     $                           U(IU,1), LDU, ONE, X0, 1 )
                     CALL DCOPY( N, X0, 1, DWORK(IXINIT), 1 )
                     IY = IY + L
                     IU = IU + 1
   80             CONTINUE
C
               END IF
C
            ELSE
C
               IF ( BLOCK ) THEN
                  SWITCH = .TRUE.
                  NROW   = NRBL
C
                  DO 100 K = 1, NOBS
                     IF ( MOD( K-1, NROW ).EQ.0 .AND. SWITCH ) THEN
                        IUT = IUTRAN
                        IF ( K.GT.INIT ) THEN
                           NROW = NOBS - INIT
                           SWITCH = .FALSE.
                        END IF
                        CALL MA02AD( 'Full', NROW, M, U(IU,1), LDU,
     $                               DWORK(IUT), M )
                        IU = IU + NROW
                     END IF
                     CALL DGEMV( 'No transpose', L, N, -ONE, C, LDC, X0,
     $                           1, ONE, DWORK(IY), 1 )
                     CALL DTRMV( 'Upper', 'No transpose', 'Non-unit', N,
     $                           A, LDA, X0, 1 )
C
                     DO 90 IX = 2, N
                        X0(IX) = X0(IX) + A(IX,IX-1)*DWORK(IXINIT+IX-2)
   90                CONTINUE
C
                     CALL DGEMV( 'No transpose', N, M, ONE, B, LDB,
     $                           DWORK(IUT), 1, ONE, X0, 1 )
                     CALL DCOPY( N, X0, 1, DWORK(IXINIT), 1 )
                     IY  = IY  + L
                     IUT = IUT + M
  100             CONTINUE
C
               ELSE
C 
                  DO 120 K = 1, NOBS
                     CALL DGEMV( 'No transpose', L, N, -ONE, C, LDC, X0,
     $                           1, ONE, DWORK(IY), 1 )
                     CALL DTRMV( 'Upper', 'No transpose', 'Non-unit', N,
     $                           A, LDA, X0, 1 )
C
                     DO 110 IX = 2, N
                        X0(IX) = X0(IX) + A(IX,IX-1)*DWORK(IXINIT+IX-2)
  110                CONTINUE
C
                     CALL DGEMV( 'No transpose', N, M, ONE, B, LDB,
     $                           U(IU,1), LDU, ONE, X0, 1 )
                     CALL DCOPY( N, X0, 1, DWORK(IXINIT), 1 )
                     IY = IY + L
                     IU = IU + 1
  120             CONTINUE
C
               END IF
C 
            END IF
C
         ELSE
C
            DO 140 K = 1, NOBS
               CALL DGEMV( 'No transpose', L, N, -ONE, C, LDC, X0, 1,
     $                     ONE, DWORK(IY), 1 )
               CALL DTRMV( 'Upper', 'No transpose', 'Non-unit', N, A,
     $                     LDA, X0, 1 )
C
               DO 130 IX = 2, N
                  X0(IX) = X0(IX) + A(IX,IX-1)*DWORK(IXINIT+IX-2)
  130          CONTINUE
C
               CALL DCOPY( N, X0, 1, DWORK(IXINIT), 1 )
               IY = IY + L
  140       CONTINUE
C
         END IF
C
C        Compress the data using (sequential) QR factorization.
C        Workspace: need   v + 2*N;  
C                   where  v = s*L*(N + 1) + N + a + w.
C
         JWORK = ITAU + N
         IF ( FIRST ) THEN
C
C           Compress the first data segment of  Gamma.
C           Workspace: need   v + 2*N, 
C                      prefer v + N + N*NB.
C
            CALL DGEQRF( LDDW, N, DWORK(INIGAM), LDDW, DWORK(ITAU),
     $                   DWORK(JWORK), LDWORK-JWORK+1, IERR )
C
C           Apply the transformation to the right hand side part.
C           Workspace: need   v + N + 1, 
C                      prefer v + N + NB.
C
            CALL DORMQR( 'Left', 'Transpose', LDDW, 1, N, DWORK(INIGAM),
     $                   LDDW, DWORK(ITAU), DWORK(IRHS), LDDW,
     $                   DWORK(JWORK), LDWORK-JWORK+1, IERR )
C
            IF ( NCYC ) THEN
C                                                  
C              Save the triangular factor of  Gamma  and the
C              corresponding right hand side.
C
               CALL DLACPY( 'Upper', N, NCP1, DWORK(INIGAM), LDDW, 
     $                      DWORK(INIR), LDR )
            END IF
         ELSE
C
C           Compress the current (but not the first) data segment of
C           Gamma.
C           Workspace: need   v + N - 1.
C
            CALL MB04OD( 'Full', N, 1,  LDDW, DWORK(INIR), LDR,
     $                   DWORK(INIGAM), LDDW, DWORK(INIH), LDR,
     $                   DWORK(IRHS), LDDW, DWORK(ITAU), DWORK(JWORK) )
         END IF
C
         IUPNT = IUPNT + NOBS
         IYPNT = IYPNT + NOBS
  150 CONTINUE
C
C     Estimate the reciprocal condition number of the triangular factor
C     of the QR decomposition.
C     Workspace: need  u + 3*N, where
C                      u = t*L*(N + 1), if NCYCLE = 1;
C                      u = w,           if NCYCLE > 1.
C
      CALL DTRCON( '1-norm', 'Upper', 'No Transpose', N, DWORK(INIR),
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
C        Workspace: need   u + 6*N;
C                   prefer larger.
C
         CALL DLASET( 'Lower', N-1, N-1, ZERO, ZERO, DWORK(INIR+1),
     $                LDR )
         ISV   = IE
         JWORK = ISV + N
         CALL DGELSS( N, N, 1, DWORK(INIR), LDR, DWORK(INIH), LDR,
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
         CALL DTRSV( 'Upper', 'No Transpose', 'Non Unit', N,
     $               DWORK(INIR), LDR, DWORK(INIH), 1 )
      END IF
C
C     Return the estimated initial state of the system  x0.
C
      CALL DCOPY( N, DWORK(INIH), 1, X0, 1 )
C
      DWORK(1) = MAXWRK
      DWORK(2) = RCOND
C
      RETURN
C
C *** End of IB01RD ***
      END
