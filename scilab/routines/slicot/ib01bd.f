      SUBROUTINE IB01BD( METH, JOB, JOBCK, NOBR, N, M, L, NSMPL, R,
     $                   LDR, A, LDA, C, LDC, B, LDB, D, LDD, Q, LDQ, 
     $                   RY, LDRY, S, LDS, K, LDK, TOL, IWORK, DWORK, 
     $                   LDWORK, BWORK, IWARN, INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 2000.
C
C     PURPOSE
C
C     To estimate the system matrices A, C, B, and D, the noise
C     covariance matrices Q, Ry, and S, and the Kalman gain matrix K
C     of a linear time-invariant state space model, using the 
C     processed triangular factor R of the concatenated block Hankel
C     matrices, provided by SLICOT Library routine IB01AD.
C
C     ARGUMENTS
C
C     Mode Parameters
C
C     METH    CHARACTER*1
C             Specifies the subspace identification method to be used,
C             as follows:
C             = 'M':  MOESP  algorithm with past inputs and outputs;
C             = 'N':  N4SID  algorithm;
C             = 'C':  combined method:  MOESP  algorithm for finding the
C                     matrices A and C, and  N4SID  algorithm for 
C                     finding the matrices B and D.
C
C     JOB     CHARACTER*1
C             Specifies which matrices should be computed, as follows:
C             = 'A':  compute all system matrices, A, B, C, and D;
C             = 'C':  compute the matrices A and C only;
C             = 'B':  compute the matrix B only;
C             = 'D':  compute the matrices B and D only.
C
C     JOBCK   CHARACTER*1
C             Specifies whether or not the covariance matrices and the
C             Kalman gain matrix are to be computed, as follows:
C             = 'C':  the covariance matrices only should be computed;
C             = 'K':  the covariance matrices and the Kalman gain 
C                     matrix should be computed;
C             = 'N':  the covariance matrices and the Kalman gain matrix
C                     should not be computed.
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
C             If  JOBCK = 'C' or 'K',  the total number of samples used
C             for calculating the covariance matrices.
C             NSMPL >= 2*(M+L)*NOBR.
C             This parameter is not meaningful if  JOBCK = 'N'.
C
C     R       (input/workspace) DOUBLE PRECISION array, dimension
C             ( LDR,2*(M+L)*NOBR )
C             On entry, the leading  2*(M+L)*NOBR-by-2*(M+L)*NOBR  part
C             of this array must contain the relevant data for the MOESP
C             or N4SID algorithms, as constructed by SLICOT Library
C             routine IB01AD. Let  R_ij,  i,j = 1:4,  be the
C             ij submatrix of  R  (denoted  S  in IB01AD),  partitioned 
C             by  M*NOBR,  L*NOBR,  M*NOBR,  and  L*NOBR  rows and 
C             columns. The submatrix  R_22  contains the matrix of left
C             singular vectors used. Also needed, for  METH = 'N'  or
C             JOBCK <> 'N',  are the submatrices  R_11,  R_14 : R_44,
C             and, for  METH = 'M' or 'C'  and  JOB <> 'C', the 
C             submatrices  R_31  and  R_12,  containing the processed
C             matrices  R_1c  and  R_2c,  respectively, as returned by
C             SLICOT Library routine IB01AD.
C             Moreover, if  METH = 'N'  and  JOB = 'A' or 'C',  the 
C             block-row  R_41 : R_43  must contain the transpose of the
C             block-column  R_14 : R_34  as returned by SLICOT Library
C             routine IB01AD. 
C             The remaining part of  R  is used as workspace.
C             On exit, part of this array is overwritten. Specifically,
C             if  METH = 'M',  R_22  and  R_31  are overwritten if
C                 JOB = 'B' or 'D',  and  R_12,  R_22,  R_14 : R_34,  
C                 and possibly  R_11  are overwritten if  JOBCK <> 'N';
C             if  METH = 'N',  all needed submatrices are overwritten.
C             The details of the contents of  R  need not be known if
C             this routine is called once just after calling the SLICOT
C             Library routine IB01AD.
C
C     LDR     INTEGER
C             The leading dimension of the array  R.
C             LDR >= 2*(M+L)*NOBR.
C
C     A       (input or output) DOUBLE PRECISION array, dimension 
C             (LDA,N)
C             On entry, if  METH = 'N' or 'C'  and  JOB = 'B' or 'D', 
C             the leading N-by-N part of this array must contain the
C             system state matrix.
C             If  METH = 'M'  or  (METH = 'N' or 'C'  and JOB = 'A' 
C             or 'C'),  this array need not be set on input.
C             On exit, if  JOB = 'A' or 'C'  and  INFO = 0,  the
C             leading N-by-N part of this array contains the system 
C             state matrix.
C
C     LDA     INTEGER
C             The leading dimension of the array A.  
C             LDA >= N,  if  JOB = 'A' or 'C',  or  METH = 'N' or 'C'  
C                            and  JOB = 'B' or 'D';
C             LDA >= 1,  otherwise.
C
C     C       (input or output) DOUBLE PRECISION array, dimension 
C             (LDC,N)
C             On entry, if  METH = 'N' or 'C'  and  JOB = 'B' or 'D',
C             the leading L-by-N part of this array must contain the
C             system output matrix.
C             If  METH = 'M'  or  (METH = 'N' or 'C'  and JOB = 'A'
C             or 'C'),  this array need not be set on input.
C             On exit, if  JOB = 'A' or 'C'  and  INFO = 0,  or 
C             INFO = 3  (or  INFO >= 0,  for  METH = 'M'),  the leading 
C             L-by-N part of this array contains the system output
C             matrix.
C
C     LDC     INTEGER
C             The leading dimension of the array C.
C             LDC >= L,  if  JOB = 'A' or 'C',  or  METH = 'N' or 'C'  
C                            and  JOB = 'B' or 'D';
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
C             If  JOBCK = 'C' or 'K',  the leading N-by-N part of this
C             array contains the positive semidefinite state covariance
C             matrix. If  JOBCK = 'K',  this matrix has been used as
C             state weighting matrix for computing the Kalman gain.
C             This parameter is not referenced if JOBCK = 'N'.
C
C     LDQ     INTEGER
C             The leading dimension of the array Q.
C             LDQ >= N,  if JOBCK = 'C' or 'K';
C             LDQ >= 1,  if JOBCK = 'N'.
C
C     RY      (output) DOUBLE PRECISION array, dimension (LDRY,L)
C             If  JOBCK = 'C' or 'K',  the leading L-by-L part of this
C             array contains the positive (semi)definite output
C             covariance matrix. If  JOBCK = 'K',  this matrix has been
C             used as output weighting matrix for computing the Kalman
C             gain.
C             This parameter is not referenced if JOBCK = 'N'.
C
C     LDRY    INTEGER
C             The leading dimension of the array RY.
C             LDRY >= L,  if JOBCK = 'C' or 'K';
C             LDRY >= 1,  if JOBCK = 'N'.
C
C     S       (output) DOUBLE PRECISION array, dimension (LDS,L)
C             If  JOBCK = 'C' or 'K',  the leading N-by-L part of this
C             array contains the state-output cross-covariance matrix.
C             If  JOBCK = 'K',  this matrix has been used as state-
C             output weighting matrix for computing the Kalman gain.
C             This parameter is not referenced if JOBCK = 'N'.
C
C     LDS     INTEGER
C             The leading dimension of the array S.
C             LDS >= N,  if JOBCK = 'C' or 'K';
C             LDS >= 1,  if JOBCK = 'N'.
C
C     K       (output) DOUBLE PRECISION array, dimension ( LDK,L ) 
C             If  JOBCK = 'K',  the leading  N-by-L  part of this array
C             contains the estimated Kalman gain matrix.
C             If  JOBCK = 'C' or 'N',  this array is not referenced.
C
C     LDK     INTEGER
C             The leading dimension of the array  K.
C             LDK >= N,  if JOBCK = 'K';
C             LDK >= 1,  if JOBCK = 'C' or 'N'.
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
C             LIWORK >= max(LIW1,LIW2), where
C             LIW1 = N,                     if METH <> 'N' and M = 0
C                                        or JOB = 'C' and JOBCK = 'N';
C             LIW1 = M*NOBR+N,              if METH <> 'N', JOB = 'C', 
C                                           and JOBCK <> 'N';
C             LIW1 = max(L*NOBR,M*NOBR),    if METH = 'M', JOB <> 'C', 
C                                           and JOBCK = 'N';
C             LIW1 = max(L*NOBR,M*NOBR+N),  if METH = 'M', JOB <> 'C',
C                                           and JOBCK = 'C' or 'K';
C             LIW1 = max(M*NOBR+N,M*(N+L)), if METH = 'N', or METH = 'C'
C                                           and JOB  <> 'C';
C             LIW2 = 0,                     if JOBCK <> 'K';
C             LIW2 = N*N,                   if JOBCK =  'K'.
C
C     DWORK   DOUBLE PRECISION array, dimension (LDWORK)
C             On exit, if  INFO = 0,  DWORK(1) returns the optimal value
C             of LDWORK,  and  DWORK(2),  DWORK(3),  DWORK(4),  and  
C             DWORK(5)  contain the reciprocal condition numbers of the
C             triangular factors of the following matrices (defined in
C             SLICOT Library routine IB01PD and in the lower level
C             routines):
C                GaL  (GaL = Un(1:(s-1)*L,1:n)),
C                R_1c (if  METH = 'M' or 'C'),
C                M    (if  JOBCK = 'C' or 'K'  or  METH = 'N'),  and  
C                Q or T  (see SLICOT Library routine IB01PY or IB01PX),
C             respectively.
C             If  METH = 'N',  DWORK(3)  is set to one without any
C             calculations. Similarly, if  METH = 'M'  and  JOBCK = 'N',
C             DWORK(4)  is set to one. If  M = 0  or  JOB = 'C',
C             DWORK(3)  and  DWORK(5)  are set to one.
C             If  JOBCK = 'K'  and  INFO = 0,  DWORK(6)  to  DWORK(13)
C             contain information about the accuracy of the results when
C             computing the Kalman gain matrix, as follows:
C                DWORK(6)  - reciprocal condition number of the matrix
C                            U11  of the Nth order system of algebraic
C                            equations from which the solution matrix  X
C                            of the Riccati equation is obtained;
C                DWORK(7)  - reciprocal pivot growth factor for the LU
C                            factorization of the matrix  U11;
C                DWORK(8)  - reciprocal condition number of the matrix
C                            As = A - S*inv(Ry)*C,  which is inverted by
C                            the standard Riccati solver;
C                DWORK(9)  - reciprocal pivot growth factor for the LU
C                            factorization of the matrix  As;
C                DWORK(10) - reciprocal condition number of the matrix
C                            Ry;
C                DWORK(11) - reciprocal condition number of the matrix
C                            Ry + C*X*C';
C                DWORK(12) - reciprocal condition number for the Riccati
C                            equation solution;
C                DWORK(13) - forward error bound for the Riccati
C                            equation solution.
C             On exit, if  INFO = -30,  DWORK(1)  returns the minimum
C             value of LDWORK.
C
C     LDWORK  INTEGER
C             The length of the array DWORK.
C             LDWORK >= max( LDW1,LDW2,LDW3 ), where, if METH = 'M',  
C             LDW1 >= max( 2*(L*NOBR-L)*N+2*N, (L*NOBR-L)*N+N*N+7*N ), 
C                     if JOB = 'C' or JOB = 'A' and M = 0;
C             LDW1 >= max( 2*(L*NOBR-L)*N+N*N+7*N,
C                          (L*NOBR-L)*N+N+6*M*NOBR, (L*NOBR-L)*N+N+ 
C                          max( L+M*NOBR, L*NOBR + max( 3*L*NOBR, M ))),
C                     if M > 0 and JOB = 'A', 'B', or 'D'; 
C             LDW2 >= 0,                          if JOBCK = 'N';
C             LDW2 >= L*NOBR*N+
C                     max( (L*NOBR-L)*N+Aw+2*N+max(5*N,(2*M+L)*NOBR+L),
C                          4*(M*NOBR+N), M*NOBR+2*N+L ),
C                                                 if JOBCK = 'C' or 'K',
C             where Aw = N+N*N, if M = 0 or JOB = 'C';
C                   Aw = 0,     otherwise;
C             if METH = 'N',  
C             LDW1 >= L*NOBR*N+max( (L*NOBR-L)*N+2*N+(2*M+L)*NOBR+L,
C                                   2*(L*NOBR-L)*N+N*N+8*N,
C                                   N+4*(M*NOBR+N), M*NOBR+3*N+L );
C             LDW2 >= 0, if M = 0 or JOB = 'C';
C             LDW2 >= L*NOBR*N+M*NOBR*(N+L)*(M*(N+L)+1)+
C                                max( (N+L)**2, 4*M*(N+L)+1 ),
C                     if M > 0 and JOB = 'A', 'B', or 'D'; 
C             and, if METH = 'C', LDW1 as 
C             max( LDW1 for METH = 'M', JOB = 'C', LDW1 for METH = 'N'),
C             and LDW2 for METH = 'N' are used;
C             LDW3 >= 0,                     if JOBCK <> 'K';
C             LDW3 >= max(  4*N*N+2*N*L+L*L+max( 3*L,N*L ),
C                          14*N*N+12*N+5 ),  if JOBCK =  'K'.
C             For good performance,  LDWORK  should be larger.
C
C     BWORK   LOGICAL array, dimension (LBWORK)
C             LBWORK = 2*N, if JOBCK =  'K';
C             LBWORK = 0,   if JOBCK <> 'K'.
C
C     Warning Indicator
C
C     IWARN   INTEGER
C             = 0:  no warning;
C             = 4:  a least squares problem to be solved has a
C                   rank-deficient coefficient matrix;
C             = 5:  the computed covariance matrices are too small.
C                   The problem seems to be a deterministic one; the
C                   gain matrix is set to zero.
C
C     Error Indicator
C
C     INFO    INTEGER
C             = 0:  successful exit;
C             < 0:  if INFO = -i, the i-th argument had an illegal
C                   value;
C             = 2:  the singular value decomposition (SVD) algorithm did
C                   not converge;
C             = 3:  a singular upper triangular matrix was found;
C             = 3+i:  if  JOBCK = 'K'  and the associated Riccati
C                   equation could not be solved, where i = 1,...,6;
C                   (see the description of the parameter INFO for the
C                   SLICOT Library routine SB02RD for the meaning of
C                   the i values);
C             = 10: the QR algorithm did not converge.
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
C     The Kalman gain matrix is obtained by solving a discrete-time 
C     algebraic Riccati equation.
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
C     [4] Sima, V.
C         Subspace-based Algorithms for Multivariable System 
C         Identification.
C         Studies in Informatics and Control, 5, pp. 335-344, 1996.
C
C     NUMERICAL ASPECTS
C
C     The implemented method consists in numerically stable steps.
C
C     FURTHER COMMENTS
C
C     The covariance matrices are computed using the N4SID approach.
C     Therefore, for efficiency reasons, it is advisable to set 
C     METH = 'N',  if the Kalman gain matrix or covariance matrices 
C     are needed  (JOBCK = 'K', or 'C').  When  JOBCK = 'N',  it could 
C     be more efficient to use the combined method,  METH = 'C'. 
C     Often, this combination will also provide better accuracy than
C     MOESP algorithm.
C     In some applications, it is useful to compute the system matrices
C     using two calls to this routine, the first one with  JOB = 'C', 
C     and the second one with  JOB = 'B' or 'D'.  This is slightly less
C     efficient than using a single call with  JOB = 'A',  because some
C     calculations are repeated. If  METH = 'N',  all the calculations 
C     at the first call are performed again at the second call; 
C     moreover, it is required to save the needed submatrices of  R 
C     before the first call and restore them before the second call.
C     If the covariance matrices and/or the Kalman gain are desired,  
C     JOBCK  should be set to  'C'  or  'K'  at the second call.
C     If  B  and  D  are both needed, they should be computed at once.
C
C     CONTRIBUTOR
C
C     V. Sima, Research Institute for Informatics, Bucharest, Dec. 1999.
C
C     REVISIONS
C
C     March 2000, August 2000.
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
      INTEGER            INFO, IWARN, L, LDA, LDB, LDC, LDD, LDK, LDQ,
     $                   LDR, LDRY, LDS, LDWORK, M, N, NOBR, NSMPL
      CHARACTER          JOB, JOBCK, METH
C     .. Array Arguments ..
      DOUBLE PRECISION   A(LDA, *), B(LDB, *), C(LDC, *), D(LDD, *),
     $                   DWORK(*),  K(LDK, *), Q(LDQ, *), R(LDR, *), 
     $                   RY(LDRY, *), S(LDS, *)
      INTEGER            IWORK( * )
      LOGICAL            BWORK( * )
C     .. Local Scalars ..
      DOUBLE PRECISION   FERR, RCOND, RCONDR, RNORM, SEP
      INTEGER            I, IA, IAW, IC, ID, IERR, IFACT, IG, IK, IO, 
     $                   IQ, IR, IS, IT, IV, IWARNL, IWI, IWR, IX,
     $                   JWORK, LDUNN, LL, LMMNOL, LMNOBR, LNOBR,
     $                   MAXWRK, MINWRK, MNOBR, MNOBRN, N2, NL, NN, NPL,
     $                   NR
      CHARACTER          JOBBD, JOBCOV, JOBCV
      LOGICAL            COMBIN, MOESP, N4SID, WITHAL, WITHB, WITHC,
     $                   WITHCO, WITHD, WITHK
C     .. Local Arrays ..
      DOUBLE PRECISION   RCND(8)
      INTEGER            OUFACT(2)
C     .. External Functions ..
      LOGICAL            LSAME
      EXTERNAL           LSAME
C     .. External Subroutines ..
      EXTERNAL           DLACPY, DLASET, IB01PD, MA02AD, SB02MT, SB02ND,
     $                   SB02RD, XERBLA
C     .. Intrinsic Functions ..
      INTRINSIC          INT, MAX
C     .. Executable Statements ..
C
C     Decode the scalar input parameters.
C
      MOESP  = LSAME( METH,  'M' )
      N4SID  = LSAME( METH,  'N' )
      COMBIN = LSAME( METH,  'C' )
      WITHAL = LSAME( JOB,   'A' )
      WITHC  = LSAME( JOB,   'C' ) .OR. WITHAL
      WITHD  = LSAME( JOB,   'D' ) .OR. WITHAL
      WITHB  = LSAME( JOB,   'B' ) .OR. WITHD
      WITHK  = LSAME( JOBCK, 'K' )
      WITHCO = LSAME( JOBCK, 'C' ) .OR. WITHK
      MNOBR  = M*NOBR
      LNOBR  = L*NOBR
      LMNOBR = LNOBR + MNOBR
      MNOBRN = MNOBR + N
      LDUNN  = ( LNOBR - L )*N
      LMMNOL = LNOBR + 2*MNOBR + L
      NR     = LMNOBR + LMNOBR
      NPL    = N + L
      N2     = N + N
      NN     = N*N
      NL     = N*L
      LL     = L*L
      MINWRK = 1
      IWARN  = 0
      INFO   = 0
C
C     Check the scalar input parameters.
C
      IF( .NOT.( MOESP .OR. N4SID .OR. COMBIN ) ) THEN
         INFO = -1
      ELSE IF( .NOT.( WITHB .OR. WITHC ) ) THEN
         INFO = -2
      ELSE IF( .NOT.( WITHCO .OR. LSAME( JOBCK, 'N' ) ) ) THEN
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
      ELSE IF( LDA.LT.1 .OR. ( ( WITHC .OR. ( WITHB .AND. .NOT.MOESP ) )
     $   .AND. LDA.LT.N ) ) THEN
         INFO = -12
      ELSE IF( LDC.LT.1 .OR. ( ( WITHC .OR. ( WITHB .AND. .NOT.MOESP ) )
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
      ELSE IF( LDK.LT.1  .OR. ( WITHK  .AND. LDK.LT.N ) )  THEN
         INFO = -26
      ELSE IF( LDWORK.GE.1 ) THEN
C
C        Compute workspace.
C        (Note: Comments in the code beginning "Workspace:" describe the
C         minimal amount of workspace needed at that point in the code,
C         as well as the preferred amount for good performance.)
C
         IAW    = 0
         MINWRK = LDUNN + 4*N
         IF( .NOT.N4SID ) THEN 
            ID = 0
            IF( WITHC ) THEN 
               MINWRK = MAX( MINWRK, 2*LDUNN + N2, LDUNN + NN + 7*N )
            END IF
         ELSE
            ID = N
         END IF
C
         IF( ( M.GT.0 .AND. WITHB ) .OR. .NOT.MOESP ) THEN 
            MINWRK = MAX( MINWRK, 2*LDUNN + NN + ID + 7*N )
            IF ( MOESP ) 
     $         MINWRK = MAX( MINWRK, LDUNN + N + 6*MNOBR, LDUNN + N + 
     $                       MAX( L + MNOBR, LNOBR + MAX( 3*LNOBR, M ) )
     $                     )
         ELSE 
            IF( .NOT.N4SID ) 
     $         IAW = N + NN
         END IF
C
         IF( .NOT.MOESP .OR. WITHCO ) THEN 
            MINWRK = MAX( MINWRK, LDUNN + IAW + N2 + MAX( 5*N, LMMNOL ),
     $                    ID + 4*MNOBRN, ID + MNOBRN + NPL ) 
            IF( .NOT.MOESP .AND. M.GT.0 .AND. WITHB ) 
     $         MINWRK = MAX( MINWRK, MNOBR*NPL*( M*NPL + 1 ) + 
     $                       MAX( NPL**2, 4*M*NPL + 1 ) )
            MINWRK = LNOBR*N + MINWRK
         END IF
C
         IF( WITHK ) THEN 
            MINWRK = MAX( MINWRK, 4*NN + 2*NL + LL + MAX( 3*L, NL ),
     $                    14*NN + 12*N + 5 ) 
         END IF
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
         CALL XERBLA( 'IB01BD', -INFO )
         RETURN
      END IF
C
      IF ( .NOT.WITHK ) THEN
         JOBCV = JOBCK
      ELSE
         JOBCV = 'C'
      END IF
C
      IO = 1
      IF ( .NOT.MOESP .OR. WITHCO ) THEN
         JWORK = IO + LNOBR*N
      ELSE
         JWORK = IO
      END IF
      MAXWRK = MINWRK
C
C     Call the computational routine for estimating system matrices.
C
      IF ( .NOT.COMBIN ) THEN
         CALL IB01PD( METH, JOB, JOBCV, NOBR, N, M, L, NSMPL, R, LDR,
     $                A, LDA, C, LDC, B, LDB, D, LDD, Q, LDQ, RY, LDRY,
     $                S, LDS, DWORK(IO), LNOBR, TOL, IWORK,
     $                DWORK(JWORK), LDWORK-JWORK+1, IWARN, INFO )
C
      ELSE
C
         IF ( WITHC ) THEN
            IF ( WITHAL ) THEN
               JOBCOV = 'N'
            ELSE
               JOBCOV = JOBCV
            END IF
            CALL IB01PD( 'MOESP', 'C and A', JOBCOV, NOBR, N, M, L,
     $                   NSMPL, R, LDR, A, LDA, C, LDC, B, LDB, D, LDD, 
     $                   Q, LDQ, RY, LDRY, S, LDS, DWORK(IO), LNOBR,
     $                   TOL, IWORK, DWORK(JWORK), LDWORK-JWORK+1, 
     $                   IWARNL, INFO )
            IF ( INFO.NE.0 ) 
     $         RETURN 
            IWARN  = MAX( IWARN, IWARNL )  
            MAXWRK = MAX( MAXWRK, INT( DWORK(JWORK) ) + JWORK - 1 )
         END IF
C
         IF ( WITHB ) THEN
            IF ( .NOT.WITHAL ) THEN
               JOBBD = JOB
            ELSE
               JOBBD = 'D'
            END IF
            CALL IB01PD( 'N4SID', JOBBD, JOBCV, NOBR, N, M, L, NSMPL, R,
     $                LDR, A, LDA, C, LDC, B, LDB, D, LDD, Q, LDQ, 
     $                RY, LDRY, S, LDS, DWORK(IO), LNOBR, TOL, IWORK,
     $                DWORK(JWORK), LDWORK-JWORK+1, IWARNL, INFO )
            IWARN  = MAX( IWARN, IWARNL )  
         END IF
      END IF
C
      IF ( INFO.NE.0 ) 
     $   RETURN 
      MAXWRK = MAX( MAXWRK, INT( DWORK(JWORK) ) + JWORK - 1 )
C
      DO 10 I = 1, 4
         RCND(I) = DWORK(JWORK+I)
   10 CONTINUE
C
      IF ( WITHK ) THEN
         IF ( IWARN.EQ.5 ) THEN
C
C           The problem seems to be a deterministic one. Set the Kalman
C           gain to zero, set accuracy parameters and return.
C
            CALL DLASET( 'Full', N, L, ZERO, ZERO, K, LDK )
C
            DO 20 I = 6, 12
               DWORK(I) = ONE
   20       CONTINUE
C
            DWORK(13) = ZERO
         ELSE
C
C           Compute the Kalman gain matrix.
C
C           Convert the optimal problem with coupling weighting terms 
C           to a standard problem.
C           Workspace:  need   4*N*N+2*N*L+L*L+max( 3*L,N*L );
C                       prefer larger.
C
            IX    = 1
            IQ    = IX + NN
            IA    = IQ + NN
            IG    = IA + NN
            IC    = IG + NN
            IR    = IC + NL
            IS    = IR + LL
            JWORK = IS + NL
C
            CALL MA02AD( 'Full',  N, N, A,  LDA,  DWORK(IA), N ) 
            CALL MA02AD( 'Full',  L, N, C,  LDC,  DWORK(IC), N ) 
            CALL DLACPY( 'Upper', N, N, Q,  LDQ,  DWORK(IQ), N ) 
            CALL DLACPY( 'Upper', L, L, RY, LDRY, DWORK(IR), L ) 
            CALL DLACPY( 'Full',  N, L, S,  LDS,  DWORK(IS), N ) 
C
            CALL SB02MT( 'G needed', 'Nonzero S', 'Not factored',
     $                   'Upper', N, L, DWORK(IA), N, DWORK(IC), N, 
     $                   DWORK(IQ), N, DWORK(IR), L, DWORK(IS), N,
     $                   IWORK, IFACT, DWORK(IG), N, IWORK(L+1), 
     $                   DWORK(JWORK), LDWORK-JWORK+1, IERR )
            IF ( IERR.NE.0 ) THEN
               INFO = 3 
               RETURN 
            END IF
            MAXWRK = MAX( MAXWRK, INT( DWORK(JWORK) ) + JWORK - 1 )
            RCONDR = DWORK(JWORK+1)
C
C           Solve the Riccati equation. 
C           Workspace:  need   14*N*N+12*N+5;
C                       prefer larger.
C
            IT    = IC
            IV    = IT  + NN
            IWR   = IV  + NN
            IWI   = IWR + N2
            IS    = IWI + N2
            JWORK = IS  + N2*N2
C
            CALL SB02RD( 'All', 'Discrete', 'Direct', 'NoTranspose',
     $                   'Upper', 'General scaling', 'Unstable first', 
     $                   'Not factored', 'Reduced', N, DWORK(IA), N,
     $                   DWORK(IT), N, DWORK(IV), N, DWORK(IG), N,
     $                   DWORK(IQ), N, DWORK(IX), N, SEP, RCOND, FERR, 
     $                   DWORK(IWR), DWORK(IWI), DWORK(IS), N2, IWORK, 
     $                   DWORK(JWORK), LDWORK-JWORK+1, BWORK, IERR )
C
            IF ( IERR.NE.0 .AND. IERR.LT.7 ) THEN
               INFO = IERR + 3 
               RETURN 
            END IF
            MAXWRK = MAX( MAXWRK, INT( DWORK(JWORK) ) + JWORK - 1 )
C
            DO 30 I = 1, 4
               RCND(I+4) = DWORK(JWORK+I)
   30       CONTINUE
C
C           Compute the gain matrix. 
C           Workspace:  need   2*N*N+2*N*L+L*L+3*L;
C                       prefer larger.
C
            IA    = IX + NN
            IC    = IA + NN
            IR    = IC + NL
            IK    = IR + LL
            JWORK = IK + NL
C
            CALL MA02AD( 'Full',  N, N, A,  LDA,  DWORK(IA), N ) 
            CALL MA02AD( 'Full',  L, N, C,  LDC,  DWORK(IC), N ) 
            CALL DLACPY( 'Upper', L, L, RY, LDRY, DWORK(IR), L ) 
C
            CALL SB02ND( 'Discrete', 'NotFactored', 'Upper',
     $                   'Nonzero S', N, L, 0, DWORK(IA), N, DWORK(IC),
     $                   N, DWORK(IR), L, IWORK, S, LDS, DWORK(IX), N,
     $                   RNORM, DWORK(IK), L, OUFACT, IWORK(L+1),
     $                   DWORK(JWORK), LDWORK-JWORK+1, IERR )   
C
            IF ( IERR.NE.0 ) THEN
               IF ( IERR.LE.L+1 ) THEN
                  INFO = 3 
               ELSE IF ( IERR.EQ.L+2 ) THEN
                  INFO = 10 
               END IF
               RETURN 
            END IF
            MAXWRK = MAX( MAXWRK, INT( DWORK(JWORK) ) + JWORK - 1 )
C
            CALL MA02AD( 'Full', L, N, DWORK(IK), L, K, LDK ) 
C
C           Set the accuracy parameters.
C
            DWORK(11) = DWORK(JWORK+1)
C
            DO 40 I = 6, 9
               DWORK(I) = RCND(I-1)
   40       CONTINUE
C
            DWORK(10) = RCONDR
            DWORK(12) = RCOND
            DWORK(13) = FERR
         END IF
      END IF
C
C     Return optimal workspace in  DWORK(1)  and the remaining
C     reciprocal condition numbers in the next locations.
C
      DWORK(1) = MAXWRK
C
      DO 50 I = 2, 5
         DWORK(I) = RCND(I-1)
   50 CONTINUE
C
      RETURN
C
C *** Last line of IB01BD ***
      END
