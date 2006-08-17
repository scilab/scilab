      SUBROUTINE IB01PD( METH, JOB, JOBCV, NOBR, N, M, L, NSMPL, R,
     $                   LDR, A, LDA, C, LDC, B, LDB, D, LDD, Q, LDQ, 
     $                   RY, LDRY, S, LDS, O, LDO, TOL, IWORK, DWORK, 
     $                   LDWORK, IWARN, INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 2000.
C
C     PURPOSE
C
C     To estimate the matrices A, C, B, and D of a linear time-invariant
C     (LTI) state space model, using the singular value decomposition
C     information provided by other routines. Optionally, the system and
C     noise covariance matrices, needed for the Kalman gain, are also
C     determined.
C
C     ARGUMENTS
C
C     Mode Parameters
C
C     METH    CHARACTER*1
C             Specifies the subspace identification method to be used,
C             as follows:
C             = 'M':  MOESP  algorithm with past inputs and outputs;
C             = 'N':  N4SID  algorithm.
C
C     JOB     CHARACTER*1
C             Specifies which matrices should be computed, as follows:
C             = 'A':  compute all system matrices, A, B, C, and D;
C             = 'C':  compute the matrices A and C only;
C             = 'B':  compute the matrix B only;
C             = 'D':  compute the matrices B and D only.
C
C     JOBCV   CHARACTER*1
C             Specifies whether or not the covariance matrices are to
C             be computed, as follows:
C             = 'C':  the covariance matrices should be computed;
C             = 'N':  the covariance matrices should not be computed.
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
C     NSMPL   (input) INTEGER
C             If JOBCV = 'C', the total number of samples used for
C             calculating the covariance matrices.
C             NSMPL >= 2*(M+L)*NOBR.
C             This parameter is not meaningful if  JOBCV = 'N'.
C
C     R       (input/workspace) DOUBLE PRECISION array, dimension
C             ( LDR,2*(M+L)*NOBR )
C             On entry, the leading  2*(M+L)*NOBR-by-2*(M+L)*NOBR  part
C             of this array must contain the relevant data for the MOESP
C             or N4SID algorithms, as constructed by SLICOT Library
C             routines IB01AD or IB01ND. Let  R_ij,  i,j = 1:4,  be the
C             ij submatrix of  R  (denoted  S  in IB01AD and IB01ND),
C             partitioned by  M*NOBR,  L*NOBR,  M*NOBR,  and  L*NOBR
C             rows and columns. The submatrix  R_22  contains the matrix
C             of left singular vectors used. Also needed, for  
C             METH = 'N'  or  JOBCV = 'C',  are the submatrices  R_11, 
C             R_14 : R_44,  and, for  METH = 'M'  and  JOB <> 'C',  the 
C             submatrices  R_31  and  R_12,  containing the processed
C             matrices  R_1c  and  R_2c,  respectively, as returned by
C             SLICOT Library routines IB01AD or IB01ND.
C             Moreover, if  METH = 'N'  and  JOB = 'A' or 'C',  the 
C             block-row  R_41 : R_43  must contain the transpose of the
C             block-column  R_14 : R_34  as returned by SLICOT Library
C             routines IB01AD or IB01ND. 
C             The remaining part of  R  is used as workspace.
C             On exit, part of this array is overwritten. Specifically,
C             if  METH = 'M',  R_22  and  R_31  are overwritten if
C                 JOB = 'B' or 'D',  and  R_12,  R_22,  R_14 : R_34,  
C                 and possibly  R_11  are overwritten if  JOBCV = 'C';
C             if  METH = 'N',  all needed submatrices are overwritten.
C
C     LDR     INTEGER
C             The leading dimension of the array  R.
C             LDR >= 2*(M+L)*NOBR.
C
C     A       (input or output) DOUBLE PRECISION array, dimension 
C             (LDA,N)
C             On entry, if  METH = 'N'  and  JOB = 'B' or 'D',  the
C             leading N-by-N part of this array must contain the system
C             state matrix.
C             If  METH = 'M'  or  (METH = 'N'  and JOB = 'A' or 'C'), 
C             this array need not be set on input.
C             On exit, if  JOB = 'A' or 'C'  and  INFO = 0,  the
C             leading N-by-N part of this array contains the system 
C             state matrix.
C
C     LDA     INTEGER
C             The leading dimension of the array A.  
C             LDA >= N,  if  JOB = 'A' or 'C',  or  METH = 'N'  and  
C                            JOB = 'B' or 'D';
C             LDA >= 1,  otherwise.
C
C     C       (input or output) DOUBLE PRECISION array, dimension 
C             (LDC,N)
C             On entry, if  METH = 'N'  and  JOB = 'B' or 'D',  the
C             leading L-by-N part of this array must contain the system
C             output matrix.
C             If  METH = 'M'  or  (METH = 'N'  and JOB = 'A' or 'C'), 
C             this array need not be set on input.
C             On exit, if  JOB = 'A' or 'C'  and  INFO = 0,  or 
C             INFO = 3  (or  INFO >= 0,  for  METH = 'M'),  the leading 
C             L-by-N part of this array contains the system output
C             matrix.
C
C     LDC     INTEGER
C             The leading dimension of the array C.
C             LDC >= L,  if  JOB = 'A' or 'C',  or  METH = 'N'  and  
C                            JOB = 'B' or 'D';
C             LDC >= 1,  otherwise.
C
C     B       (output) DOUBLE PRECISION array, dimension (LDB,M)
C             If  M > 0,  JOB = 'A', 'B', or 'D'  and  INFO = 0,  the 
C             leading N-by-M part of this array contains the system 
C             input matrix. If  M = 0  or  JOB = 'C',  this array is 
C             not referenced.
C
C     LDB     INTEGER
C             The leading dimension of the array B.
C             LDB >= N,  if M > 0 and JOB = 'A', 'B', or 'D';
C             LDB >= 1,  if M = 0 or  JOB = 'C'.
C
C     D       (output) DOUBLE PRECISION array, dimension (LDD,M)
C             If  M > 0,  JOB = 'A' or 'D'  and  INFO = 0,  the leading
C             L-by-M part of this array contains the system input-output
C             matrix. If  M = 0  or  JOB = 'C' or 'B',  this array is
C             not referenced.  
C
C     LDD     INTEGER
C             The leading dimension of the array D.
C             LDD >= L,  if M > 0 and JOB = 'A' or 'D';
C             LDD >= 1,  if M = 0 or  JOB = 'C' or 'B'.
C
C     Q       (output) DOUBLE PRECISION array, dimension (LDQ,N)
C             If JOBCV = 'C', the leading N-by-N part of this array
C             contains the positive semidefinite state covariance matrix
C             to be used as state weighting matrix when computing the
C             Kalman gain.
C             This parameter is not referenced if JOBCV = 'N'.
C
C     LDQ     INTEGER
C             The leading dimension of the array Q.
C             LDQ >= N,  if JOBCV = 'C';
C             LDQ >= 1,  if JOBCV = 'N'.
C
C     RY      (output) DOUBLE PRECISION array, dimension (LDRY,L)
C             If JOBCV = 'C', the leading L-by-L part of this array
C             contains the positive (semi)definite output covariance
C             matrix to be used as output weighting matrix when
C             computing the Kalman gain.
C             This parameter is not referenced if JOBCV = 'N'.
C
C     LDRY    INTEGER
C             The leading dimension of the array RY.
C             LDRY >= L,  if JOBCV = 'C';
C             LDRY >= 1,  if JOBCV = 'N'.
C
C     S       (output) DOUBLE PRECISION array, dimension (LDS,L)
C             If JOBCV = 'C', the leading N-by-L part of this array
C             contains the state-output cross-covariance matrix to be
C             used as cross-weighting matrix when computing the Kalman
C             gain.
C             This parameter is not referenced if JOBCV = 'N'.
C
C     LDS     INTEGER
C             The leading dimension of the array S.
C             LDS >= N,  if JOBCV = 'C';
C             LDS >= 1,  if JOBCV = 'N'.
C
C     O       (output) DOUBLE PRECISION array, dimension ( LDO,N ) 
C             If  METH = 'M'  and  JOBCV = 'C',  or  METH = 'N',  
C             the leading  L*NOBR-by-N  part of this array contains
C             the estimated extended observability matrix, i.e., the  
C             first  N  columns of the relevant singular vectors.
C             If  METH = 'M'  and  JOBCV = 'N',  this array is not
C             referenced.
C
C     LDO     INTEGER
C             The leading dimension of the array  O.
C             LDO >= L*NOBR,  if  JOBCV = 'C'  or  METH = 'N';
C             LDO >= 1,       otherwise.
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
C     IWORK   INTEGER array, dimension (LIWORK)
C             LIWORK = N,                   if METH = 'M' and M = 0
C                                        or JOB = 'C' and JOBCV = 'N';
C             LIWORK = M*NOBR+N,            if METH = 'M', JOB = 'C', 
C                                           and JOBCV = 'C';
C             LIWORK = max(L*NOBR,M*NOBR),  if METH = 'M', JOB <> 'C', 
C                                           and JOBCV = 'N';
C             LIWORK = max(L*NOBR,M*NOBR+N),  if METH = 'M', JOB <> 'C',
C                                             and JOBCV = 'C';
C             LIWORK = max(M*NOBR+N,M*(N+L)), if METH = 'N'.
C
C     DWORK   DOUBLE PRECISION array, dimension (LDWORK)
C             On exit, if  INFO = 0,  DWORK(1) returns the optimal value
C             of LDWORK,  and  DWORK(2),  DWORK(3),  DWORK(4),  and  
C             DWORK(5)  contain the reciprocal condition numbers of the
C             triangular factors of the matrices, defined in the code,
C             GaL  (GaL = Un(1:(s-1)*L,1:n)),  R_1c  (if  METH = 'M'), 
C             M  (if  JOBCV = 'C'  or  METH = 'N'),  and  Q  or  T  (see
C             SLICOT Library routines IB01PY or IB01PX),  respectively.
C             If  METH = 'N',  DWORK(3)  is set to one without any
C             calculations. Similarly, if  METH = 'M'  and  JOBCV = 'N',
C             DWORK(4)  is set to one. If  M = 0  or  JOB = 'C',
C             DWORK(3)  and  DWORK(5)  are set to one.
C             On exit, if  INFO = -30,  DWORK(1)  returns the minimum
C             value of LDWORK.
C
C     LDWORK  INTEGER
C             The length of the array DWORK.
C             LDWORK >= max( LDW1,LDW2 ), where, if METH = 'M',  
C             LDW1 >= max( 2*(L*NOBR-L)*N+2*N, (L*NOBR-L)*N+N*N+7*N ), 
C                     if JOB = 'C' or JOB = 'A' and M = 0;
C             LDW1 >= max( 2*(L*NOBR-L)*N+N*N+7*N,
C                          (L*NOBR-L)*N+N+6*M*NOBR, (L*NOBR-L)*N+N+ 
C                          max( L+M*NOBR, L*NOBR + max( 3*L*NOBR, M )))
C                     if M > 0 and JOB = 'A', 'B', or 'D'; 
C             LDW2 >= 0,                                 if JOBCV = 'N';
C             LDW2 >= max( (L*NOBR-L)*N+Aw+2*N+max(5*N,(2*M+L)*NOBR+L),
C                          4*(M*NOBR+N), M*NOBR+2*N+L ), if JOBCV = 'C',
C             where Aw = N+N*N, if M = 0 or JOB = 'C';
C                   Aw = 0,     otherwise;
C             and, if METH = 'N',  
C             LDW1 >= max( (L*NOBR-L)*N+2*N+(2*M+L)*NOBR+L,
C                          2*(L*NOBR-L)*N+N*N+8*N, N+4*(M*NOBR+N),
C                          M*NOBR+3*N+L );
C             LDW2 >= 0, if M = 0 or JOB = 'C';
C             LDW2 >= M*NOBR*(N+L)*(M*(N+L)+1)+
C                     max( (N+L)**2, 4*M*(N+L)+1 ),
C                     if M > 0 and JOB = 'A', 'B', or 'D'.
C             For good performance,  LDWORK  should be larger.
C
C     Warning Indicator
C
C     IWARN   INTEGER
C             = 0:  no warning;
C             = 4:  a least squares problem to be solved has a
C                   rank-deficient coefficient matrix;
C             = 5:  the computed covariance matrices are too small.
C                   The problem seems to be a deterministic one.
C
C     Error Indicator
C
C     INFO    INTEGER
C             = 0:  successful exit;
C             < 0:  if INFO = -i, the i-th argument had an illegal
C                   value;
C             = 2:  the singular value decomposition (SVD) algorithm did
C                   not converge;
C             = 3:  a singular upper triangular matrix was found.
C
C     METHOD
C
C     In the MOESP approach, the matrices  A  and  C  are first
C     computed from an estimated extended observability matrix [1],
C     and then, the matrices  B  and  D  are obtained by solving an
C     extended linear system in a least squares sense.
C     In the N4SID approach, besides the estimated extended 
C     observability matrix, the solutions of two least squares problems
C     are used to build another least squares problem, whose solution
C     is needed to compute the system matrices  A,  C,  B,  and  D.  The
C     solutions of the two least squares problems are also optionally
C     used by both approaches to find the covariance matrices.
C
C     REFERENCES
C
C     [1] Verhaegen M., and Dewilde, P.
C         Subspace Model Identification. Part 1: The output-error state-
C         space model identification class of algorithms.
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
C     [4] Sima, V.
C         Subspace-based Algorithms for Multivariable System 
C         Identification.
C         Studies in Informatics and Control, 5, pp. 335-344, 1996.
C
C     NUMERICAL ASPECTS
C
C     The implemented method is numerically stable.
C
C     FURTHER COMMENTS
C
C     In some applications, it is useful to compute the system matrices
C     using two calls to this routine, the first one with  JOB = 'C', 
C     and the second one with  JOB = 'B' or 'D'.  This is slightly less
C     efficient than using a single call with  JOB = 'A',  because some
C     calculations are repeated. If  METH = 'N',  all the calculations 
C     at the first call are performed again at the second call; 
C     moreover, it is required to save the needed submatrices of  R 
C     before the first call and restore them before the second call.
C     If the covariance matrices are desired,  JOBCV  should be set 
C     to  'C'  at the second call. If  B  and  D  are both needed, they
C     should be computed at once.
C     It is possible to compute the matrices A and C using the MOESP  
C     algorithm (METH = 'M'), and the matrices B and D using the N4SID 
C     algorithm (METH = 'N'). This combination could be slightly more
C     efficient than N4SID algorithm alone and it could be more accurate
C     than MOESP algorithm. No saving/restoring is needed in such a
C     combination, provided  JOBCV  is set to  'N'  at the first call.
C     Recommended usage:  either one call with  JOB = 'A',  or
C        first  call with  METH = 'M',  JOB = 'C',  JOBCV = 'N',
C        second call with  METH = 'M',  JOB = 'D',  JOBCV = 'C',  or
C        first  call with  METH = 'M',  JOB = 'C',  JOBCV = 'N',
C        second call with  METH = 'N',  JOB = 'D',  JOBCV = 'C'.
C
C     CONTRIBUTOR
C
C     V. Sima, Research Institute for Informatics, Bucharest, Dec. 1999.
C
C     REVISIONS
C
C     March 2000, Feb. 2001.
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
      INTEGER            INFO, IWARN, L, LDA, LDB, LDC, LDD, LDO, LDQ,
     $                   LDR, LDRY, LDS, LDWORK, M, N, NOBR, NSMPL
      CHARACTER          JOB, JOBCV, METH
C     .. Array Arguments ..
      DOUBLE PRECISION   A(LDA, *), B(LDB, *), C(LDC, *), D(LDD, *),
     $                   DWORK(*),  O(LDO, *), Q(LDQ, *), R(LDR, *), 
     $                   RY(LDRY, *), S(LDS, *)
      INTEGER            IWORK( * )
C     .. Local Scalars ..
      DOUBLE PRECISION   EPS, RCOND1, RCOND2, RCOND3, RCOND4, RNRM, 
     $                   SVLMAX, THRESH, TOLL, TOLL1
      INTEGER            I, IAW, ID, IERR, IGAL, IHOUS, ISV, ITAU,
     $                   ITAU1, ITAU2, IU, IUN2, IWARNL, IX, JWORK,
     $                   LDUN2, LDUNN, LDW, LMMNOB, LMMNOL, LMNOBR,
     $                   LNOBR, LNOBRN, MAXWRK, MINWRK, MNOBR, MNOBRN,
     $                   N2, NCOL, NN, NPL, NR, NR2, NR3, NR4, NR4MN,
     $                   NR4PL, NROW, RANK, RANK11, RANKM
      CHARACTER          FACT, JOBP, JOBPY
      LOGICAL            FULLR, MOESP, N4SID, SHIFT, WITHAL, WITHB,
     $                   WITHC, WITHCO, WITHD
C     .. Local Array ..
      DOUBLE PRECISION   SVAL(3)
C     .. External Functions ..
      LOGICAL            LSAME
      INTEGER            ILAENV
      DOUBLE PRECISION   DLAMCH, DLANGE
      EXTERNAL           DLAMCH, DLANGE, ILAENV, LSAME
C     .. External Subroutines ..
      EXTERNAL           DCOPY, DGEMM, DGEQRF, DLACPY, DLASET, DORMQR,
     $                   DSYRK, DTRCON, DTRSM, DTRTRS, IB01PX, IB01PY,
     $                   MA02AD, MA02ED, MB02QY, MB02UD, MB03OD, XERBLA
C     .. Intrinsic Functions ..
      INTRINSIC          DBLE, MAX
C     .. Executable Statements ..
C
C     Decode the scalar input parameters.
C
      MOESP  = LSAME( METH,  'M' )
      N4SID  = LSAME( METH,  'N' )
      WITHAL = LSAME( JOB,   'A' )
      WITHC  = LSAME( JOB,   'C' ) .OR. WITHAL
      WITHD  = LSAME( JOB,   'D' ) .OR. WITHAL
      WITHB  = LSAME( JOB,   'B' ) .OR. WITHD
      WITHCO = LSAME( JOBCV, 'C' )
      MNOBR  = M*NOBR
      LNOBR  = L*NOBR
      LMNOBR = LNOBR + MNOBR
      LMMNOB = LNOBR + 2*MNOBR
      MNOBRN = MNOBR + N
      LNOBRN = LNOBR - N
      LDUN2  = LNOBR - L
      LDUNN  = LDUN2*N
      LMMNOL = LMMNOB + L
      NR     = LMNOBR + LMNOBR
      NPL    = N + L
      N2     = N + N
      NN     = N*N
      MINWRK = 1
      IWARN  = 0
      INFO   = 0
C
C     Check the scalar input parameters.
C
      IF( .NOT.( MOESP .OR. N4SID ) ) THEN
         INFO = -1
      ELSE IF( .NOT.( WITHB .OR. WITHC ) ) THEN
         INFO = -2
      ELSE IF( .NOT.( WITHCO .OR. LSAME( JOBCV, 'N' ) ) ) THEN
         INFO = -3
      ELSE IF( NOBR.LE.1 ) THEN
         INFO = -4
      ELSE IF( N.LE.0 .OR. N.GE.NOBR ) THEN
         INFO = -5
      ELSE IF( M.LT.0 ) THEN
         INFO = -6
      ELSE IF( L.LE.0 ) THEN
         INFO = -7
      ELSE IF( WITHCO .AND. NSMPL.LT.NR ) THEN
         INFO = -8
      ELSE IF( LDR.LT.NR ) THEN
         INFO = -10
      ELSE IF( LDA.LT.1 .OR. ( ( WITHC .OR. ( WITHB .AND. N4SID ) )
     $   .AND. LDA.LT.N ) ) THEN
         INFO = -12
      ELSE IF( LDC.LT.1 .OR. ( ( WITHC .OR. ( WITHB .AND. N4SID ) )
     $   .AND. LDC.LT.L ) ) THEN
         INFO = -14
      ELSE IF( LDB.LT.1  .OR. ( WITHB  .AND. LDB.LT.N .AND. M.GT.0 ) )
     $      THEN
         INFO = -16
      ELSE IF( LDD.LT.1  .OR. ( WITHD  .AND. LDD.LT.L .AND. M.GT.0 ) )
     $      THEN
         INFO = -18
      ELSE IF( LDQ.LT.1  .OR. ( WITHCO .AND. LDQ.LT.N ) )  THEN
         INFO = -20
      ELSE IF( LDRY.LT.1 .OR. ( WITHCO .AND. LDRY.LT.L ) ) THEN
         INFO = -22
      ELSE IF( LDS.LT.1  .OR. ( WITHCO .AND. LDS.LT.N ) )  THEN
         INFO = -24
      ELSE IF( LDO.LT.1  .OR. ( ( WITHCO .OR. N4SID ) .AND. 
     $         LDO.LT.LNOBR ) )  THEN
         INFO = -26
      ELSE IF( LDWORK.GE.1 ) THEN
C
C        Compute workspace.
C        (Note: Comments in the code beginning "Workspace:" describe the
C         minimal amount of workspace needed at that point in the code,
C         as well as the preferred amount for good performance.
C         NB refers to the optimal block size for the immediately
C         following subroutine, as returned by ILAENV.)
C
         IAW    = 0
         MINWRK = LDUNN + 4*N
         MAXWRK = LDUNN + N + N*ILAENV( 1, 'DGEQRF', ' ', LDUN2, N, -1, 
     $                                  -1 )
         IF( MOESP ) THEN 
            ID = 0
            IF( WITHC ) THEN 
               MINWRK = MAX( MINWRK, 2*LDUNN + N2, LDUNN + NN + 7*N )
               MAXWRK = MAX( MAXWRK, 2*LDUNN + N + N*ILAENV( 1, 
     $                               'DORMQR', 'LT', LDUN2, N, N, -1 ) )
            END IF
         ELSE
            ID = N
         END IF
C
         IF( ( M.GT.0 .AND. WITHB ) .OR. N4SID ) THEN 
            MINWRK = MAX( MINWRK, 2*LDUNN + NN + ID + 7*N )
            IF ( MOESP ) 
     $         MINWRK = MAX( MINWRK, LDUNN + N + 6*MNOBR, LDUNN + N + 
     $                       MAX( L + MNOBR, LNOBR + MAX( 3*LNOBR, M ) )
     $                     )
         ELSE 
            IF( MOESP ) 
     $         IAW = N + NN
         END IF
C
         IF( N4SID .OR. WITHCO ) THEN 
            MINWRK = MAX( MINWRK, LDUNN + IAW + N2 + MAX( 5*N, LMMNOL ),
     $                    ID + 4*MNOBRN, ID + MNOBRN + NPL ) 
            MAXWRK = MAX( MAXWRK, LDUNN + IAW + N2 + 
     $                    MAX( N*ILAENV( 1, 'DGEQRF', ' ', LNOBR, N, -1,
     $                                   -1 ), LMMNOB*
     $                           ILAENV( 1, 'DORMQR', 'LT', LNOBR, 
     $                                   LMMNOB, N, -1 ), LMMNOL*
     $                           ILAENV( 1, 'DORMQR', 'LT', LDUN2, 
     $                                   LMMNOL, N, -1 ) ),
     $                    ID + N + N*ILAENV( 1, 'DGEQRF', ' ', LMNOBR, 
     $                                       N, -1, -1 ), 
     $                    ID + N + NPL*ILAENV( 1, 'DORMQR', 'LT', 
     $                                         LMNOBR, NPL, N, -1 ) )
            IF( N4SID .AND. ( M.GT.0 .AND. WITHB ) ) 
     $         MINWRK = MAX( MINWRK, MNOBR*NPL*( M*NPL + 1 ) + 
     $                       MAX( NPL**2, 4*M*NPL + 1 ) )
         END IF
         MAXWRK = MAX( MINWRK, MAXWRK )
C
         IF ( LDWORK.LT.MINWRK ) THEN
            INFO = -30
            DWORK( 1 ) = MINWRK
         END IF
      END IF
C
C     Return if there are illegal arguments.
C
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'IB01PD', -INFO )
         RETURN
      END IF
C
      NR2 = MNOBR  + 1
      NR3 = LMNOBR + 1
      NR4 = LMMNOB + 1
C
C     Set the precision parameters. A threshold value  EPS**(2/3)  is
C     used for deciding to use pivoting or not, where  EPS  is the 
C     relative machine precision (see LAPACK Library routine DLAMCH).
C
      EPS    = DLAMCH( 'Precision' )
      THRESH = EPS**( TWO/THREE )
      SVLMAX = ZERO
      RCOND4 = ONE
C
C     Let  Un  be the matrix of left singular vectors (stored in  R_22).
C     Copy  un1 = GaL = Un(1:(s-1)*L,1:n)  in the workspace.
C
      IGAL = 1
      CALL DLACPY( 'Full', LDUN2, N, R(NR2,NR2), LDR, DWORK(IGAL), 
     $             LDUN2 )
C
C     Factor un1 = Q1*[r1'  0]' (' means transposition).
C     Workspace: need   L*(NOBR-1)*N+2*N, 
C                prefer L*(NOBR-1)*N+N+N*NB.
C
      ITAU1 = IGAL  + LDUNN
      JWORK = ITAU1 + N
      LDW   = JWORK
      CALL DGEQRF( LDUN2, N, DWORK(IGAL), LDUN2, DWORK(ITAU1),
     $             DWORK(JWORK), LDWORK-JWORK+1, IERR )
C
C     Compute the reciprocal of the condition number of r1.
C     Workspace: need L*(NOBR-1)*N+4*N.
C
      CALL DTRCON( '1-norm', 'Upper', 'NonUnit', N, DWORK(IGAL), LDUN2,
     $             RCOND1, DWORK(JWORK), IWORK, INFO )
C
      TOLL1 = TOL
      IF( TOLL1.LE.ZERO ) 
     $   TOLL1 = NN*EPS
C
      IF ( ( M.GT.0 .AND. WITHB ) .OR. N4SID ) THEN
         JOBP = 'P'
         IF ( WITHAL ) THEN
            JOBPY = 'D'
         ELSE
            JOBPY = JOB
         END IF
      ELSE
         JOBP = 'N'
      END IF
C  
      IF ( MOESP ) THEN
         NCOL = 0
         IUN2 = JWORK
         IF ( WITHC ) THEN
C
C           Set  C = Un(1:L,1:n)  and then compute the system matrix A.
C
C           Set  un2 = Un(L+1:L*s,1:n)  in  DWORK(IUN2).
C           Workspace: need   2*L*(NOBR-1)*N+N.
C
            CALL DLACPY( 'Full', L, N, R(NR2,NR2), LDR, C, LDC )
            CALL DLACPY( 'Full', LDUN2, N, R(NR2+L,NR2), LDR, 
     $                   DWORK(IUN2), LDUN2 )
C
C           Note that un1 has already been factored as
C           un1 = Q1*[r1'  0]'  and usually (generically, assuming
C           observability) has full column rank.
C           Update  un2 <-- Q1'*un2  in  DWORK(IUN2)  and save its
C           first  n  rows in  A.
C           Workspace: need   2*L*(NOBR-1)*N+2*N;
C                      prefer 2*L*(NOBR-1)*N+N+N*NB.
C
            JWORK = IUN2 + LDUNN
            CALL DORMQR( 'Left', 'Transpose', LDUN2, N, N, DWORK(IGAL),
     $                   LDUN2, DWORK(ITAU1), DWORK(IUN2), LDUN2,
     $                   DWORK(JWORK), LDWORK-JWORK+1, IERR )
            CALL DLACPY( 'Full', N, N, DWORK(IUN2), LDUN2, A, LDA )
            NCOL  = N
            JWORK = IUN2
         END IF
C
         IF ( RCOND1.GT.MAX( TOLL1, THRESH ) ) THEN
C
C           The triangular factor r1 is considered to be of full rank.
C           Solve for  A  (if requested),  r1*A = un2(1:n,:)  in  A.
C
            IF ( WITHC ) THEN
               CALL DTRTRS( 'Upper', 'NoTranspose', 'NonUnit', N, N,
     $                      DWORK(IGAL), LDUN2, A, LDA, IERR )
               IF ( IERR.GT.0 ) THEN
                  INFO = 3
                  RETURN
               END IF
            END IF
            RANK = N
         ELSE
C
C           Rank-deficient triangular factor r1.  Use SVD of r1,
C           r1 = U*S*V',  also for computing  A  (if requested) from  
C           r1*A = un2(1:n,:).  Matrix  U  is computed in  DWORK(IU),
C           and  V' overwrites  r1.  If  B  is requested, the
C           pseudoinverse of  r1  and then of  GaL  are also computed
C           in  R(NR3,NR2).
C           Workspace: need   c*L*(NOBR-1)*N+N*N+7*N, 
C                             where  c = 1  if  B and D  are not needed,
C                                    c = 2  if  B and D  are needed;
C                      prefer larger.
C
            IU    = IUN2  
            ISV   = IU  + NN
            JWORK = ISV + N
            IF ( M.GT.0 .AND. WITHB ) THEN
C
C              Save the elementary reflectors used for computing r1,
C              if  B, D  are needed.
C              Workspace: need   2*L*(NOBR-1)*N+2*N+N*N.
C
               IHOUS = JWORK
               JWORK = IHOUS + LDUNN
               CALL DLACPY( 'Lower', LDUN2, N, DWORK(IGAL), LDUN2,
     $                      DWORK(IHOUS), LDUN2 )
            ELSE
               IHOUS = IGAL
            END IF
C
            CALL MB02UD( 'Not factored', 'Left', 'NoTranspose', JOBP, N,
     $                   NCOL, ONE, TOLL1, RANK, DWORK(IGAL), LDUN2, 
     $                   DWORK(IU), N, DWORK(ISV), A, LDA, R(NR3,NR2),
     $                   LDR, DWORK(JWORK), LDWORK-JWORK+1, IERR ) 
            IF ( IERR.NE.0 ) THEN
               INFO = 2 
               RETURN 
            END IF
            MAXWRK = MAX( MAXWRK, INT( DWORK(JWORK) ) + JWORK - 1 )
C
            IF ( RANK.EQ.0 ) THEN
               JOBP = 'N'
            ELSE IF ( M.GT.0 .AND. WITHB ) THEN
C
C              Compute  pinv(GaL)  in  R(NR3,NR2)  if  B, D  are needed.
C              Workspace: need   2*L*(NOBR-1)*N+N*N+3*N;
C                         prefer 2*L*(NOBR-1)*N+N*N+2*N+N*NB.
C
               CALL DLASET( 'Full', N, LDUN2-N, ZERO, ZERO, 
     $                      R(NR3,NR2+N), LDR )
               CALL DORMQR( 'Right', 'Transpose', N, LDUN2, N,
     $                      DWORK(IHOUS), LDUN2, DWORK(ITAU1),
     $                      R(NR3,NR2), LDR, DWORK(JWORK),
     $                      LDWORK-JWORK+1, IERR )
               MAXWRK = MAX( MAXWRK, INT( DWORK(JWORK) ) + JWORK - 1 )
               IF ( WITHCO ) THEN
C
C                 Save  pinv(GaL)  in  DWORK(IGAL).
C
                  CALL DLACPY( 'Full', N, LDUN2, R(NR3,NR2), LDR, 
     $                         DWORK(IGAL), N )
               END IF
               JWORK = IUN2
            END IF
            LDW = JWORK
         END IF
C
         IF ( M.GT.0 .AND. WITHB ) THEN
C
C           Computation of  B  and  D.
C
C           Compute the reciprocal of the condition number of R_1c.
C           Workspace: need L*(NOBR-1)*N+N+3*M*NOBR.
C
            CALL DTRCON( '1-norm', 'Upper', 'NonUnit', MNOBR, R(NR3,1),
     $                   LDR, RCOND2, DWORK(JWORK), IWORK, IERR )
C
            TOLL = TOL
            IF( TOLL.LE.ZERO ) 
     $         TOLL = MNOBR*MNOBR*EPS
C
C           Compute the right hand side and solve for  K  (in  R_23), 
C              K*R_1c' = u2'*R_2c',
C           where  u2 = Un(:,n+1:L*s),  and  K  is  (Ls-n) x ms.
C
            CALL DGEMM( 'Transpose', 'Transpose', LNOBRN, MNOBR, LNOBR,
     $                   ONE, R(NR2,NR2+N), LDR, R(1,NR2), LDR, ZERO,
     $                   R(NR2,NR3), LDR )
C
            IF ( RCOND2.GT.MAX( TOLL, THRESH ) ) THEN
C
C              The triangular factor R_1c is considered to be of full
C              rank. Solve for  K,  K*R_1c' = u2'*R_2c'.
C
               CALL DTRSM( 'Right', 'Upper', 'Transpose', 'Non-unit',
     $                     LNOBRN, MNOBR, ONE, R(NR3,1), LDR, 
     $                     R(NR2,NR3), LDR )
            ELSE
C
C              Rank-deficient triangular factor  R_1c.  Use SVD of  R_1c
C              for computing  K  from  K*R_1c' = u2'*R_2c',  where 
C              R_1c = U1*S1*V1'.  Matrix  U1  is computed in  R_33,
C              and  V1'  overwrites  R_1c.
C              Workspace: need   L*(NOBR-1)*N+N+6*M*NOBR;
C                         prefer larger.
C
               ISV   = LDW
               JWORK = ISV + MNOBR
               CALL MB02UD( 'Not factored', 'Right', 'Transpose', 
     $                      'No pinv', LNOBRN, MNOBR, ONE, TOLL, RANK11,
     $                      R(NR3,1), LDR, R(NR3,NR3), LDR, DWORK(ISV),
     $                      R(NR2,NR3), LDR, DWORK(JWORK), 1,
     $                      DWORK(JWORK), LDWORK-JWORK+1, IERR ) 
               IF ( IERR.NE.0 ) THEN
                  INFO = 2 
                  RETURN 
               END IF
               MAXWRK = MAX( MAXWRK, INT( DWORK(JWORK) ) + JWORK - 1 )
               JWORK  = LDW
            END IF
C
C           Compute the triangular factor of the structured matrix  Q 
C           and apply the transformations to the matrix  Kexpand,  where
C           Q  and  Kexpand  are defined in SLICOT Library routine
C           IB01PY.  Compute also the matrices  B,  D.
C           Workspace: need   L*(NOBR-1)*N+N+max(L+M*NOBR,L*NOBR+
C                                                max(3*L*NOBR,M));
C                      prefer larger.
C
            IF ( WITHCO )
     $         CALL DLACPY( 'Full', LNOBR, N, R(NR2,NR2), LDR, O, LDO )
            CALL IB01PY( METH, JOBPY, NOBR, N, M, L, RANK, R(NR2,NR2),
     $                   LDR, DWORK(IGAL), LDUN2, DWORK(ITAU1), 
     $                   R(NR3,NR2), LDR, R(NR2,NR3), LDR, R(NR4,NR2),
     $                   LDR, R(NR4,NR3), LDR, B, LDB, D, LDD, TOL,
     $                   IWORK, DWORK(JWORK), LDWORK-JWORK+1, IWARN, 
     $                   INFO )
            IF ( INFO.NE.0 )
     $         RETURN 
            MAXWRK = MAX( MAXWRK, INT( DWORK(JWORK) ) + JWORK - 1 )
            RCOND4 = DWORK(JWORK+1)
            IF ( WITHCO )
     $         CALL DLACPY( 'Full', LNOBR, N, O, LDO, R(NR2,1), LDR )
C
         ELSE
            RCOND2 = ONE
         END IF
C
         IF ( .NOT.WITHCO ) THEN
            RCOND3 = ONE
            GO TO 30
         END IF
      ELSE
C
C        For N4SID, set  RCOND2  to one.
C
         RCOND2 = ONE
      END IF
C
C     If needed, save the first  n  columns, representing  Gam,  of the
C     matrix of left singular vectors,  Un,  in  R_21  and in  O.
C
      IF ( N4SID .OR. ( WITHC .AND. .NOT.WITHAL ) ) THEN
         IF ( M.GT.0 )
     $      CALL DLACPY( 'Full', LNOBR, N, R(NR2,NR2), LDR, R(NR2,1), 
     $                   LDR )
         CALL DLACPY( 'Full', LNOBR, N, R(NR2,NR2), LDR, O, LDO )
      END IF
C
C     Computations for covariance matrices, and system matrices (N4SID).
C     Solve the least squares problems  Gam*Y = R4(1:L*s,1:(2*m+L)*s),
C                                       GaL*X = R4(L+1:L*s,:),  where
C     GaL = Gam(1:L*(s-1),:),  Gam  has full column rank, and
C     R4 = [ R_14' R_24' R_34' R_44L' ],  R_44L = R_44(1:L,:), as
C     returned by SLICOT Library routine  IB01ND.
C     First, find the  QR  factorization of  Gam,  Gam = Q*R.
C     Workspace: need   L*(NOBR-1)*N+Aw+3*N;
C                prefer L*(NOBR-1)*N+Aw+2*N+N*NB, where
C                Aw = N+N*N,  if  (M = 0  or  JOB = 'C'),  rank(r1) < N,
C                             and  METH = 'M';
C                Aw = 0,      otherwise.
C
      ITAU2 = LDW
      JWORK = ITAU2 + N
      CALL DGEQRF( LNOBR, N, R(NR2,1), LDR, DWORK(ITAU2),
     $             DWORK(JWORK), LDWORK-JWORK+1, IERR )
C
C     For METH = 'M' or when JOB = 'B' or 'D', transpose  
C     [ R_14' R_24' R_34' ]'  in the last block-row of  R, obtaining  Z,
C     and for METH = 'N' and JOB = 'A' or 'C', use the matrix  Z 
C     already available in the last block-row of  R,  and then apply
C     the transformations, Z <-- Q'*Z.
C     Workspace: need   L*(NOBR-1)*N+Aw+2*N+(2*M+L)*NOBR;
C                prefer L*(NOBR-1)*N+Aw+2*N+(2*M+L)*NOBR*NB.
C
      IF ( MOESP .OR. ( WITHB .AND. .NOT. WITHAL ) )
     $   CALL MA02AD( 'Full', LMMNOB, LNOBR, R(1,NR4), LDR, R(NR4,1), 
     $                LDR )
      CALL DORMQR( 'Left', 'Transpose', LNOBR, LMMNOB, N, R(NR2,1), LDR,
     $             DWORK(ITAU2), R(NR4,1), LDR, DWORK(JWORK),
     $             LDWORK-JWORK+1, IERR )
C
C     Solve for  Y,  RY = Z  in  Z  and save the transpose of the
C     solution  Y  in the second block-column of  R.
C
      CALL DTRTRS( 'Upper', 'NoTranspose', 'NonUnit', N, LMMNOB,
     $             R(NR2,1), LDR, R(NR4,1), LDR, IERR )
      IF ( IERR.GT.0 ) THEN
         INFO = 3
         RETURN
      END IF
      CALL MA02AD( 'Full', N, LMMNOB, R(NR4,1), LDR, R(1,NR2), LDR )
      NR4MN = NR4 - N
      NR4PL = NR4 + L
      NROW  = LMMNOL
C
C     SHIFT is .TRUE. if some columns of  R_14 : R_44L  should be 
C     shifted to the right, to avoid overwriting useful information.
C
      SHIFT = M.EQ.0 .AND. LNOBR.LT.N2
C
      IF ( RCOND1.GT.MAX( TOLL, THRESH ) ) THEN
C
C        The triangular factor  r1  of  GaL  (GaL = Q1*r1)  is
C        considered to be of full rank.
C
C        Transpose  [ R_14' R_24' R_34' R_44L' ]'(:,L+1:L*s)  in the
C        last block-row of  R  (beginning with the  (L+1)-th  row),
C        obtaining  Z1,  and then apply the transformations,
C        Z1 <-- Q1'*Z1.
C        Workspace: need   L*(NOBR-1)*N+Aw+2*N+ (2*M+L)*NOBR + L;
C                   prefer L*(NOBR-1)*N+Aw+2*N+((2*M+L)*NOBR + L)*NB.
C
         CALL MA02AD( 'Full', LMMNOL, LDUN2, R(1,NR4PL), LDR,
     $                R(NR4PL,1), LDR )
         CALL DORMQR( 'Left', 'Transpose', LDUN2, LMMNOL, N,
     $                DWORK(IGAL), LDUN2, DWORK(ITAU1), R(NR4PL,1), LDR,
     $                DWORK(JWORK), LDWORK-JWORK+1, IERR )
C
C        Solve for  X,  r1*X = Z1  in  Z1,  and copy the transpose of  X
C        into the last part of the third block-column of  R.
C
         CALL DTRTRS( 'Upper', 'NoTranspose', 'NonUnit', N, LMMNOL,
     $                DWORK(IGAL), LDUN2, R(NR4PL,1), LDR, IERR )
         IF ( IERR.GT.0 ) THEN
            INFO = 3
            RETURN
         END IF
C
         IF ( SHIFT ) THEN
            NR4MN = NR4
C
            DO 10 I = L - 1, 0, -1
               CALL DCOPY( LMMNOL, R(1,NR4+I), 1, R(1,NR4+N+I), 1 )
   10       CONTINUE
C
         END IF
         CALL MA02AD( 'Full', N, LMMNOL, R(NR4PL,1), LDR, R(1,NR4MN), 
     $                LDR )
         NROW = 0
      END IF
C
      IF ( N4SID .OR. NROW.GT.0 ) THEN
C
C        METH = 'N'  or rank-deficient triangular factor r1.
C        For  METH = 'N',  use SVD of  r1,  r1 = U*S*V', for computing
C        X'  from  X'*GaL' = Z1',  if  rank(r1) < N.  Matrix  U  is
C        computed in  DWORK(IU)  and  V'  overwrites  r1.  Then, the
C        pseudoinverse of  GaL  is determined in  R(NR4+L,NR2).
C        For METH = 'M', the pseudoinverse of  GaL  is already available
C        if  M > 0  and  B  is requested;  otherwise, the SVD of  r1  is
C        available in  DWORK(IU),  DWORK(ISV),  and  DWORK(IGAL).    
C        Workspace for N4SID: need   2*L*(NOBR-1)*N+N*N+8*N;
C                             prefer larger.
C
         IF ( MOESP ) THEN
            FACT = 'F'
            IF ( M.GT.0 .AND. WITHB ) 
     $         CALL DLACPY( 'Full', N, LDUN2, DWORK(IGAL), N, 
     $                      R(NR4PL,NR2), LDR )
         ELSE
C
C           Save the elementary reflectors used for computing r1.
C
            IHOUS = JWORK
            CALL DLACPY( 'Lower', LDUN2, N, DWORK(IGAL), LDUN2,
     $                   DWORK(IHOUS), LDUN2 )
            FACT  = 'N'
            IU    = IHOUS + LDUNN
            ISV   = IU  + NN
            JWORK = ISV + N
         END IF
C  
         CALL MB02UD( FACT, 'Right', 'Transpose', JOBP, NROW, N, ONE,
     $                TOLL1, RANK, DWORK(IGAL), LDUN2, DWORK(IU), N,
     $                DWORK(ISV), R(1,NR4PL), LDR, R(NR4PL,NR2), LDR,
     $                DWORK(JWORK), LDWORK-JWORK+1, IERR ) 
         IF ( NROW.GT.0 ) THEN
            IF ( SHIFT ) THEN
               NR4MN = NR4
               CALL DLACPY( 'Full', LMMNOL, L, R(1,NR4), LDR,
     $                      R(1,NR4-L), LDR )
               CALL DLACPY( 'Full', LMMNOL, N, R(1,NR4PL), LDR,
     $                      R(1,NR4MN), LDR )
               CALL DLACPY( 'Full', LMMNOL, L, R(1,NR4-L), LDR,
     $                      R(1,NR4+N), LDR )
            ELSE
               CALL DLACPY( 'Full', LMMNOL, N, R(1,NR4PL), LDR,
     $                      R(1,NR4MN), LDR )
            END IF
         END IF
C
         IF ( N4SID ) THEN
            IF ( IERR.NE.0 ) THEN
               INFO = 2 
               RETURN 
            END IF
            MAXWRK = MAX( MAXWRK, INT( DWORK(JWORK) ) + JWORK - 1 )
C
C           Compute  pinv(GaL)  in  R(NR4+L,NR2).
C           Workspace: need   2*L*(NOBR-1)*N+3*N;
C                      prefer 2*L*(NOBR-1)*N+2*N+N*NB.
C
            JWORK = IU
            CALL DLASET( 'Full', N, LDUN2-N, ZERO, ZERO, R(NR4PL,NR2+N),
     $                   LDR )
            CALL DORMQR( 'Right', 'Transpose', N, LDUN2, N,
     $                   DWORK(IHOUS), LDUN2, DWORK(ITAU1),
     $                   R(NR4PL,NR2), LDR, DWORK(JWORK), 
     $                   LDWORK-JWORK+1, IERR )
            MAXWRK = MAX( MAXWRK, INT( DWORK(JWORK) ) + JWORK - 1 )
         END IF
      END IF
C
C     For METH = 'N', find part of the solution (corresponding to A 
C     and C) and, optionally, for both  METH = 'M',  or  METH = 'N', 
C     find the residual of the least squares problem that gives the
C     covariances,  M*V = N,  where
C         (     R_11 )
C     M = (  Y'      ),  N = (  X'   R4'(:,1:L) ),  V = V(n+m*s, n+L),
C         (  0   0   )
C     with  M((2*m+L)*s+L, n+m*s),  N((2*m+L)*s+L, n+L),  R4'  being
C     stored in the last block-column of  R.  The last  L  rows of  M
C     are not explicitly considered. Note that, for efficiency, the
C     last  m*s  columns of  M  are in the first positions of arrray  R.
C     This permutation does not affect the residual, only the
C     solution.  (The solution is not needed for METH = 'M'.)
C     Note that R_11 corresponds to the future outputs for both  
C     METH = 'M', or METH = 'N' approaches.  (For  METH = 'N',  the
C     first two block-columns have been interchanged.)  
C     For  METH = 'N',  A and C are obtained as follows:  
C     [ A'  C' ] = V(m*s+1:m*s+n,:).
C
C     First, find the  QR  factorization of  Y'(m*s+1:(2*m+L)*s,:)
C     and apply the transformations to the corresponding part of N.
C     Compress the workspace for N4SID by moving the scalar reflectors
C     corresponding to  Q. 
C     Workspace: need   d*N+2*N;
C                prefer d*N+N+N*NB;
C     where  d = 0,  for  MOESP,  and  d = 1,  for  N4SID.
C
      IF ( MOESP ) THEN
         ITAU = 1
      ELSE
         CALL DCOPY( N, DWORK(ITAU2), 1, DWORK, 1 )
         ITAU = N + 1
      END IF
C
      JWORK = ITAU + N
      CALL DGEQRF( LMNOBR, N, R(NR2,NR2), LDR, DWORK(ITAU),
     $             DWORK(JWORK), LDWORK-JWORK+1, IERR )
C
C     Workspace: need   d*N+N+(N+L);
C                prefer d*N+N+(N+L)*NB.
C
      CALL DORMQR( 'Left', 'Transpose', LMNOBR, NPL, N, R(NR2,NR2), LDR,
     $             DWORK(ITAU), R(NR2,NR4MN), LDR, DWORK(JWORK),
     $             LDWORK-JWORK+1, IERR )
C
      CALL DLASET( 'Lower', L-1, L-1, ZERO, ZERO, R(NR4+1,NR4), LDR )
C
C     Now, matrix  M  with permuted block-columns has been 
C     triangularized.
C     Compute the reciprocal of the condition number of its
C     triangular factor in  R(1:m*s+n,1:m*s+n).
C     Workspace: need d*N+3*(M*NOBR+N).
C
      JWORK = ITAU
      CALL DTRCON( '1-norm', 'Upper', 'NonUnit', MNOBRN, R, LDR, RCOND3,
     $             DWORK(JWORK), IWORK, INFO )
C
      TOLL = TOL
      IF( TOLL.LE.ZERO )  
     $   TOLL = MNOBRN*MNOBRN*EPS
      IF ( RCOND3.GT.MAX( TOLL, THRESH ) ) THEN
C
C        The triangular factor is considered to be of full rank.
C        Solve for  V(m*s+1:m*s+n,:),  giving  [ A'  C' ].
C
         FULLR = .TRUE.
         RANKM = MNOBRN
         IF ( N4SID )
     $      CALL DTRSM( 'Left', 'Upper', 'NoTranspose', 'NonUnit', N,
     $                  NPL, ONE, R(NR2,NR2), LDR, R(NR2,NR4MN), LDR )
      ELSE
         FULLR = .FALSE.
C
C        Use QR factorization (with pivoting). For METH = 'N', save
C        (and then restore) information about the QR factorization of
C        Gam,  for later use. Note that  R_11  could be modified by
C        MB03OD, but the corresponding part of  N  is also modified
C        accordingly.
C        Workspace: need d*N+4*(M*NOBR+N).
C
         DO 20 I = 1, MNOBRN
            IWORK(I) = 0
   20    CONTINUE
C
         IF ( N4SID .AND. ( M.GT.0 .AND. WITHB ) )
     $      CALL DLACPY( 'Full', LNOBR, N, R(NR2,1), LDR, R(NR4,1),
     $                   LDR )
         JWORK = ITAU + MNOBRN
         CALL DLASET( 'Lower', MNOBRN-1, MNOBRN, ZERO, ZERO, R(2,1), 
     $                LDR )
         CALL MB03OD( 'QR', MNOBRN, MNOBRN, R, LDR, IWORK, TOLL, 
     $                SVLMAX, DWORK(ITAU), RANKM, SVAL, DWORK(JWORK),
     $                IERR )
C
C        Workspace: need   d*N+M*NOBR+N+N+L;
C                   prefer d*N+M*NOBR+N+(N+L)*NB.
C
         CALL DORMQR( 'Left', 'Transpose', MNOBRN, NPL, MNOBRN, 
     $                R, LDR, DWORK(ITAU), R(1,NR4MN), LDR, 
     $                DWORK(JWORK), LDWORK-JWORK+1, IERR )
         MAXWRK = MAX( MAXWRK, INT( DWORK(JWORK) ) + JWORK - 1 )
      END IF
C
      IF ( WITHCO ) THEN
C
C        The residual (transposed) of the least squares solution
C        (multiplied by a matrix with orthogonal columns) is stored
C        in the rows  RANKM+1:(2*m+L)*s+L  of V,  and it should be
C        squared-up for getting the covariance matrices. (Generically,  
C        RANKM = m*s+n.)
C
         RNRM = ONE/DBLE( NSMPL )
         IF ( MOESP ) THEN
            CALL DSYRK( 'Upper', 'Transpose', NPL, LMMNOL-RANKM, RNRM,
     $                  R(RANKM+1,NR4MN), LDR, ZERO, R, LDR )
            CALL DLACPY( 'Upper', N, N, R, LDR, Q, LDQ )
            CALL DLACPY( 'Full',  N, L, R(1,N+1), LDR, S, LDS )
            CALL DLACPY( 'Upper', L, L, R(N+1,N+1), LDR, RY, LDRY )
         ELSE
            CALL DSYRK( 'Upper', 'Transpose', NPL, LMMNOL-RANKM, RNRM,
     $                  R(RANKM+1,NR4MN), LDR, ZERO, DWORK(JWORK), NPL )
            CALL DLACPY( 'Upper', N, N, DWORK(JWORK), NPL, Q, LDQ )
            CALL DLACPY( 'Full',  N, L, DWORK(JWORK+N*NPL), NPL, S, 
     $                   LDS )
            CALL DLACPY( 'Upper', L, L, DWORK(JWORK+N*(NPL+1)), NPL, RY,
     $                   LDRY )
         END IF 
         CALL MA02ED( 'Upper', N, Q, LDQ )
         CALL MA02ED( 'Upper', L, RY, LDRY )
C
C        Check the magnitude of the residual.
C
         RNRM = DLANGE( '1-norm', LMMNOL-RANKM, NPL, R(RANKM+1,NR4MN),
     $                  LDR, DWORK(JWORK) )
         IF ( RNRM.LT.THRESH )
     $      IWARN = 5
      END IF
C
      IF ( N4SID ) THEN
         IF ( .NOT.FULLR ) THEN
            IWARN = 4
C
C           Compute part of the solution of the least squares problem,
C           M*V = N,  for the rank-deficient problem.
C           Remark: this computation should not be performed before the
C           symmetric updating operation above.
C           Workspace: need   M*NOBR+3*N+L;
C                      prefer larger.
C
            CALL MB03OD( 'No QR', N, N, R(NR2,NR2), LDR, IWORK, TOLL,
     $                   SVLMAX, DWORK(ITAU), RANKM, SVAL, DWORK(JWORK),
     $                   IERR )
            CALL MB02QY( N, N, NPL, RANKM, R(NR2,NR2), LDR, IWORK,
     $                   R(NR2,NR4MN), LDR, DWORK(ITAU+MNOBR), 
     $                   DWORK(JWORK), LDWORK-JWORK+1, INFO )
            MAXWRK = MAX( MAXWRK, INT( DWORK(JWORK) ) + JWORK - 1 )
            JWORK  = ITAU
            IF ( M.GT.0 .AND. WITHB )
     $         CALL DLACPY( 'Full', LNOBR, N, R(NR4,1), LDR, R(NR2,1),
     $                      LDR )
         END IF
C
         IF ( WITHC ) THEN
C
C           Obtain  A  and  C,  noting that block-permutations have been
C           implicitly used.  
C
            CALL MA02AD( 'Full', N, N, R(NR2,NR4MN), LDR, A, LDA )
            CALL MA02AD( 'Full', N, L, R(NR2,NR4MN+N), LDR, C, LDC )
         ELSE
C
C           Use the given  A  and  C.  
C
            CALL MA02AD( 'Full', N, N, A, LDA, R(NR2,NR4MN), LDR )
            CALL MA02AD( 'Full', L, N, C, LDC, R(NR2,NR4MN+N), LDR )
         END IF
C
         IF ( M.GT.0 .AND. WITHB ) THEN
C
C           Obtain  B  and  D.  
C           First, compute the transpose of the matrix K as
C           N(1:m*s,:) - M(1:m*s,m*s+1:m*s+n)*[A'  C'],  in the first
C           m*s  rows of  R(1,NR4MN).
C
            CALL DGEMM ( 'NoTranspose', 'NoTranspose', MNOBR, NPL, N, 
     $                   -ONE, R(1,NR2), LDR, R(NR2,NR4MN), LDR, ONE, 
     $                   R(1,NR4MN), LDR )
C
C           Denote   M = pinv(GaL)  and construct
C
C                    [ [ A ]   -1   ]                      [ R ]
C           and  L = [ [   ]  R   0 ] Q',  where Gam = Q * [   ].
C                    [ [ C ]        ]                      [ 0 ]
C
C           Then, solve the least squares problem.
C
            CALL DLACPY( 'Full', N, N, A, LDA, R(NR2,NR4), LDR )
            CALL DLACPY( 'Full', L, N, C, LDC, R(NR2+N,NR4), LDR )
            CALL DTRSM(  'Right', 'Upper', 'NoTranspose', 'NonUnit',
     $                   NPL, N, ONE, R(NR2,1), LDR, R(NR2,NR4), LDR )
            CALL DLASET( 'Full', NPL, LNOBRN, ZERO, ZERO, R(NR2,NR4+N),
     $                   LDR )
C
C           Workspace: need 2*N+L; prefer N + (N+L)*NB.
C
            CALL DORMQR( 'Right', 'Transpose', NPL, LNOBR, N, R(NR2,1),
     $                   LDR, DWORK, R(NR2,NR4), LDR, DWORK(JWORK), 
     $                   LDWORK-JWORK+1, IERR )
C
C           Obtain the matrix  K  by transposition, and find  B  and  D.
C           Workspace: need   NOBR*(M*(N+L))**2+M*NOBR*(N+L)+
C                             max((N+L)**2,4*M*(N+L)+1);
C                      prefer larger.
C
            CALL MA02AD( 'Full', MNOBR, NPL, R(1,NR4MN), LDR,
     $                   R(NR2,NR3), LDR )
            IX    = MNOBR*NPL**2*M + 1
            JWORK = IX + MNOBR*NPL
            CALL IB01PX( JOBPY, NOBR, N, M, L, R, LDR, O, LDO,
     $                   R(NR2,NR4), LDR, R(NR4PL,NR2), LDR, R(NR2,NR3),
     $                   LDR, DWORK, MNOBR*NPL, DWORK(IX), B, LDB, D, 
     $                   LDD, TOL, IWORK, DWORK(JWORK), LDWORK-JWORK+1,
     $                   IWARNL, INFO )
            IF ( INFO.NE.0 )
     $         RETURN 
            IWARN  = MAX( IWARN, IWARNL )  
            MAXWRK = MAX( MAXWRK, INT( DWORK(JWORK) ) + JWORK - 1 )
            RCOND4 = DWORK(JWORK+1)
C
         END IF
      END IF
C
   30 CONTINUE  
C
C     Return optimal workspace in  DWORK(1)  and reciprocal condition
C     numbers in the next locations.
C
      DWORK(1) = MAXWRK
      DWORK(2) = RCOND1
      DWORK(3) = RCOND2
      DWORK(4) = RCOND3
      DWORK(5) = RCOND4
      RETURN
C
C *** Last line of IB01PD ***
      END
