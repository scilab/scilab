      SUBROUTINE IB01PY( METH, JOB, NOBR, N, M, L, RANKR1, UL, LDUL,
     $                   R1, LDR1, TAU1, PGAL, LDPGAL, K, LDK, R, LDR, 
     $                   H, LDH, B, LDB, D, LDD, TOL, IWORK, DWORK,
     $                   LDWORK, IWARN, INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 2000.
C
C     PURPOSE
C
C     1. To compute the triangular  (QR)  factor of the  p-by-L*s
C     structured matrix  Q,
C
C         [ Q_1s  Q_1,s-1  Q_1,s-2  ...  Q_12  Q_11 ]
C         [  0      Q_1s   Q_1,s-1  ...  Q_13  Q_12 ]
C     Q = [  0       0       Q_1s   ...  Q_14  Q_13 ],
C         [  :       :        :           :     :   ]
C         [  0       0        0     ...   0    Q_1s ]
C
C     and apply the transformations to the p-by-m matrix  Kexpand,
C
C               [ K_1 ]
C               [ K_2 ]
C     Kexpand = [ K_3 ],
C               [  :  ]
C               [ K_s ]
C
C     where, for MOESP approach (METH = 'M'), p = s*(L*s-n), and
C     Q_1i = u2(L*(i-1)+1:L*i,:)'  is  (Ls-n)-by-L,  for  i = 1:s,
C     u2 = Un(1:L*s,n+1:L*s),  K_i = K(:,(i-1)*m+1:i*m)  (i = 1:s)
C     is  (Ls-n)-by-m, and for N4SID approach (METH = 'N'), p = s*(n+L),
C     and
C
C               [   -L_1|1    ]          [ M_i-1 - L_1|i ]
C        Q_11 = [             ],  Q_1i = [               ],  i = 2:s,
C               [ I_L - L_2|1 ]          [     -L_2|i    ]
C
C     are  (n+L)-by-L  matrices, and   
C     K_i = K(:,(i-1)*m+1:i*m),  i = 1:s,  is  (n+L)-by-m.
C     The given matrices are:  
C     For  METH = 'M',  u2 = Un(1:L*s,n+1:L*s),  
C                       K(1:Ls-n,1:m*s);
C
C                           [ L_1|1  ...  L_1|s ]
C     For  METH = 'N',  L = [                   ],   (n+L)-by-L*s,
C                           [ L_2|1  ...  L_2|s ]
C
C                       M = [ M_1  ...  M_s-1 ],  n-by-L*(s-1),  and
C                       K,                        (n+L)-by-m*s.
C                       Matrix M is the pseudoinverse of the matrix GaL,
C                       built from the first  n  relevant singular 
C                       vectors,  GaL = Un(1:L(s-1),1:n),  and computed
C                       by SLICOT Library routine IB01PD for METH = 'N'.
C
C     Matrix  Q  is triangularized  (in  R),  exploiting its structure,
C     and the transformations are applied from the left to  Kexpand.
C
C     2. To estimate the matrices B and D of a linear time-invariant
C     (LTI) state space model, using the factor  R,  transformed matrix 
C     Kexpand, and the singular value decomposition information provided
C     by other routines. 
C
C     IB01PY  routine is intended for speed and efficient use of the
C     memory space. It is generally not recommended for  METH = 'N',  as
C     IB01PX  routine can produce more accurate results.
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
C             Specifies whether or not the matrices B and D should be
C             computed, as follows:
C             = 'B':  compute the matrix B, but not the matrix D;
C             = 'D':  compute both matrices B and D;
C             = 'N':  do not compute the matrices B and D, but only the
C                     R  factor of  Q  and the transformed Kexpand.
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
C     RANKR1  (input) INTEGER
C             The effective rank of the upper triangular matrix  r1,  
C             i.e., the triangular QR factor of the matrix  GaL,  
C             computed by SLICOT Library routine IB01PD. It is also
C             the effective rank of the matrix  GaL.  0 <= RANKR1 <= N.
C             If  JOB = 'N',  or  M = 0,  or  METH = 'N',  this 
C             parameter is not used. 
C
C     UL      (input/workspace) DOUBLE PRECISION array, dimension 
C             ( LDUL,L*NOBR )
C             On entry, if  METH = 'M',  the leading  L*NOBR-by-L*NOBR
C             part of this array must contain the matrix  Un  of
C             relevant singular vectors. The first  N  columns of  UN
C             need not be specified for this routine.
C             On entry, if  METH = 'N',  the leading  (N+L)-by-L*NOBR
C             part of this array must contain the given matrix  L.
C             On exit, the leading  LDF-by-L*(NOBR-1) part of this array
C             is overwritten by the matrix  F  of the algorithm in [4], 
C             where  LDF = MAX( 1, L*NOBR-N-L ), if  METH = 'M';
C                    LDF = N,                    if  METH = 'N'.
C
C     LDUL    INTEGER
C             The leading dimension of the array  UL.
C             LDUL >= L*NOBR, if  METH = 'M';
C             LDUL >= N+L,    if  METH = 'N'.
C
C     R1      (input) DOUBLE PRECISION array, dimension ( LDR1,N )
C             If  JOB <> 'N',  M > 0,  METH = 'M',  and  RANKR1 = N, 
C             the leading  L*(NOBR-1)-by-N  part of this array must 
C             contain details of the QR factorization of the matrix
C             GaL,  as computed by SLICOT Library routine IB01PD.
C             Specifically, the leading N-by-N upper triangular part 
C             must contain the upper triangular factor  r1  of  GaL, 
C             and the lower  L*(NOBR-1)-by-N  trapezoidal part, together
C             with array TAU1, must contain the factored form of the
C             orthogonal matrix  Q1  in the QR factorization of  GaL.
C             If  JOB = 'N',  or  M = 0,  or  METH = 'N', or  METH = 'M'
C             and  RANKR1 < N,  this array is not referenced.
C
C     LDR1    INTEGER
C             The leading dimension of the array  R1.
C             LDR1 >= L*(NOBR-1), if  JOB <> 'N',  M > 0,  METH = 'M', 
C                                 and  RANKR1 = N;
C             LDR1 >= 1,          otherwise.
C            
C     TAU1    (input) DOUBLE PRECISION array, dimension ( N )
C             If  JOB <> 'N',  M > 0,  METH = 'M',  and  RANKR1 = N, 
C             this array must contain the scalar factors of the
C             elementary reflectors used in the QR factorization of the 
C             matrix  GaL,  computed by SLICOT Library routine IB01PD.
C             If  JOB = 'N',  or  M = 0,  or  METH = 'N', or  METH = 'M'
C             and  RANKR1 < N,  this array is not referenced. 
C
C     PGAL    (input) DOUBLE PRECISION array, dimension 
C             ( LDPGAL,L*(NOBR-1) )
C             If  METH = 'N',  or  JOB <> 'N',  M > 0,  METH = 'M'  and
C             RANKR1 < N,  the leading  N-by-L*(NOBR-1)  part of this
C             array must contain the pseudoinverse of the matrix  GaL,
C             as computed by SLICOT Library routine IB01PD.
C             If  METH = 'M'  and  JOB = 'N',  or  M = 0,  or  
C             RANKR1 = N,  this array is not referenced. 
C
C     LDPGAL  INTEGER
C             The leading dimension of the array  PGAL.  
C             LDPGAL >= N,  if   METH = 'N',  or  JOB <> 'N',  M > 0,
C                           and  METH = 'M'  and RANKR1 < N;
C             LDPGAL >= 1,  otherwise.
C
C     K       (input/output) DOUBLE PRECISION array, dimension 
C             ( LDK,M*NOBR )
C             On entry, the leading  (p/s)-by-M*NOBR  part of this array
C             must contain the given matrix  K  defined above.
C             On exit, the leading  (p/s)-by-M*NOBR  part of this array
C             contains the transformed matrix  K.
C            
C     LDK     INTEGER
C             The leading dimension of the array  K.  LDK >= p/s.
C
C     R       (output) DOUBLE PRECISION array, dimension ( LDR,L*NOBR ) 
C             If  JOB = 'N',  or  M = 0,  or  Q  has full rank, the
C             leading  L*NOBR-by-L*NOBR  upper triangular part of this
C             array contains the  R  factor of the QR factorization of
C             the matrix  Q.
C             If  JOB <> 'N',  M > 0,  and  Q  has not a full rank, the
C             leading  L*NOBR-by-L*NOBR  upper trapezoidal part of this
C             array contains details of the complete orhogonal
C             factorization of the matrix  Q,  as constructed by SLICOT
C             Library routines MB03OD and MB02QY.
C            
C     LDR     INTEGER
C             The leading dimension of the array  R.  LDR >= L*NOBR.
C            
C     H       (output) DOUBLE PRECISION array, dimension ( LDH,M ) 
C             If  JOB = 'N'  or  M = 0,  the leading  L*NOBR-by-M  part
C             of this array contains the updated part of the matrix 
C             Kexpand  corresponding to the upper triangular factor  R
C             in the QR factorization of the matrix  Q.
C             If  JOB <> 'N',  M > 0,  and  METH = 'N'  or  METH = 'M' 
C             and  RANKR1 < N,  the leading  L*NOBR-by-M  part of this 
C             array contains the minimum norm least squares solution of
C             the linear system  Q*X = Kexpand,  from which the matrices
C             B  and  D  are found. The first  NOBR-1  row blocks of  X
C             appear in the reverse order in  H.
C             If  JOB <> 'N',  M > 0,  METH = 'M'  and  RANKR1 = N,  the
C             leading  L*(NOBR-1)-by-M  part of this array contains the
C             matrix product  Q1'*X,  and the subarray
C             L*(NOBR-1)+1:L*NOBR-by-M  contains the  corresponding 
C             submatrix of  X,  with  X  defined in the phrase above.
C            
C     LDH     INTEGER
C             The leading dimension of the array  H.  LDH >= L*NOBR.
C            
C     B       (output) DOUBLE PRECISION array, dimension ( LDB,M )
C             If  M > 0,  JOB = 'B' or 'D'  and  INFO = 0,  the leading
C             N-by-M part of this array contains the system input 
C             matrix.  
C             If  M = 0  or  JOB = 'N',  this array is not referenced.
C
C     LDB     INTEGER
C             The leading dimension of the array B.
C             LDB >= N, if  M > 0 and JOB = 'B' or 'D';
C             LDB >= 1, if  M = 0 or  JOB = 'N'.
C
C     D       (output) DOUBLE PRECISION array, dimension ( LDD,M )
C             If  M > 0,  JOB = 'D'  and  INFO = 0,  the leading
C             L-by-M part of this array contains the system input-output
C             matrix.  
C             If  M = 0  or  JOB = 'B'  or  'N',  this array is not
C             referenced.  
C
C     LDD     INTEGER
C             The leading dimension of the array D.
C             LDD >= L, if  M > 0 and JOB = 'D';
C             LDD >= 1, if  M = 0 or  JOB = 'B' or 'N'.
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
C             This parameter is not used if  M = 0  or  JOB = 'N'.
C
C     Workspace
C
C     IWORK   INTEGER array, dimension ( LIWORK )
C             where  LIWORK >= 0,       if  JOB =  'N',  or   M = 0;
C                    LIWORK >= L*NOBR,  if  JOB <> 'N',  and  M > 0.
C
C     DWORK   DOUBLE PRECISION array, dimension ( LDWORK )
C             On exit, if  INFO = 0,  DWORK(1) returns the optimal value
C             of  LDWORK,  and, if  JOB <> 'N',  and  M > 0,  DWORK(2)  
C             contains the reciprocal condition number of the triangular
C             factor of the matrix  R.
C             On exit, if  INFO = -28,  DWORK(1)  returns the minimum
C             value of LDWORK.
C
C     LDWORK  INTEGER
C             The length of the array DWORK.
C             LDWORK >= MAX( 2*L, L*NOBR, L+M*NOBR ), 
C                                         if  JOB = 'N',  or  M = 0;
C             LDWORK >= MAX( L+M*NOBR, L*NOBR + MAX( 3*L*NOBR, M ) ),  
C                                         if  JOB <> 'N',  and  M > 0.
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
C                   value;
C             = 3:  a singular upper triangular matrix was found.
C
C     METHOD
C
C     The QR factorization is computed exploiting the structure, 
C     as described in [4]. 
C     The matrices  B  and  D  are then obtained by solving certain
C     linear systems in a least squares sense.
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
C     The implemented method for computing the triangular factor and
C     updating Kexpand is numerically stable.
C
C     FURTHER COMMENTS
C
C     The computed matrices B and D are not the least squares solutions
C     delivered by either MOESP or N4SID algorithms, except for the
C     special case n = s - 1, L = 1. However, the computed B and D are
C     frequently good enough estimates, especially for  METH = 'M'. 
C     Better estimates could be obtained by calling SLICOT Library
C     routine IB01PX, but it is less efficient, and requires much more
C     workspace.
C
C     CONTRIBUTOR
C
C     V. Sima, Research Institute for Informatics, Bucharest, Oct. 1999.
C
C     REVISIONS
C
C     Feb. 2000.
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
      INTEGER            INFO, IWARN, L, LDB, LDD, LDH, LDK, LDPGAL, 
     $                   LDR, LDR1, LDUL, LDWORK, M, N, NOBR, RANKR1
      CHARACTER          JOB, METH
C     .. Array Arguments ..
      DOUBLE PRECISION   B(LDB, *), D(LDD, *), DWORK(*), H(LDH, *), 
     $                   K(LDK, *), PGAL(LDPGAL, *), R(LDR, *), 
     $                   R1(LDR1, *), TAU1(*), UL(LDUL, *)
      INTEGER            IWORK( * )
C     .. Local Scalars ..
      DOUBLE PRECISION   EPS, RCOND, SVLMAX, THRESH, TOLL
      INTEGER            I, IERR, ITAU, J, JI, JL, JM, JWORK, LDUN2,
     $                   LNOBR, LP1, MAXWRK, MINWRK, MNOBR, NOBRH,
     $                   NROW, NROWML, RANK
      LOGICAL            MOESP, N4SID, WITHB, WITHD
C     .. Local Array ..
      DOUBLE PRECISION   SVAL(3)
C     .. External Functions ..
      LOGICAL            LSAME
      INTEGER            ILAENV
      DOUBLE PRECISION   DLAMCH
      EXTERNAL           DLAMCH, ILAENV, LSAME
C     .. External Subroutines ..
      EXTERNAL           DGEMM, DGEQRF, DLACPY, DLASET, DORMQR, DSWAP,
     $                   DTRCON, DTRSM, DTRTRS, MA02AD, MB02QY, MB03OD,
     $                   MB04OD, MB04OY, XERBLA
C     .. Intrinsic Functions ..
      INTRINSIC          INT, MAX, MOD
C     .. Executable Statements ..
C
C     Decode the scalar input parameters.
C
      MOESP = LSAME( METH, 'M' )
      N4SID = LSAME( METH, 'N' )
      WITHD = LSAME( JOB,  'D' )
      WITHB = LSAME( JOB,  'B' ) .OR. WITHD
      MNOBR = M*NOBR
      LNOBR = L*NOBR
      LDUN2 = LNOBR - L
      LP1   = L + 1
      IF ( MOESP ) THEN
         NROW = LNOBR - N
      ELSE
         NROW = N + L
      END IF
      NROWML = NROW - L
      IWARN  = 0
      INFO   = 0
C
C     Check the scalar input parameters.
C
      IF( .NOT.( MOESP .OR. N4SID ) ) THEN
         INFO = -1
      ELSE IF( .NOT.( WITHB .OR. LSAME( JOB, 'N' ) ) ) THEN
         INFO = -2
      ELSE IF( NOBR.LE.1 ) THEN
         INFO = -3
      ELSE IF( N.GE.NOBR .OR. N.LE.0 ) THEN
         INFO = -4
      ELSE IF( M.LT.0 ) THEN
         INFO = -5
      ELSE IF( L.LE.0 ) THEN
         INFO = -6
      ELSE IF( ( MOESP .AND. WITHB .AND. M.GT.0 ) .AND. 
     $         ( RANKR1.LT.ZERO .OR. RANKR1.GT.N ) ) THEN
         INFO = -7
      ELSE IF( ( MOESP .AND. LDUL.LT.LNOBR ) .OR. 
     $         ( N4SID .AND. LDUL.LT.NROW ) ) THEN
         INFO = -9
      ELSE IF( LDR1.LT.1 .OR. ( M.GT.0 .AND. WITHB .AND. MOESP .AND. 
     $         LDR1.LT.LDUN2 .AND. RANKR1.EQ.N ) ) THEN
         INFO = -11
      ELSE IF( LDPGAL.LT.1 .OR. 
     $       ( LDPGAL.LT.N .AND. ( N4SID .OR. ( WITHB .AND. M.GT.0 
     $                     .AND. ( MOESP .AND. RANKR1.LT.N ) ) ) ) ) 
     $      THEN
         INFO = -14
      ELSE IF( LDK.LT.NROW ) THEN
         INFO = -16
      ELSE IF( LDR.LT.LNOBR ) THEN
         INFO = -18
      ELSE IF( LDH.LT.LNOBR ) THEN
         INFO = -20
      ELSE IF( LDB.LT.1 .OR. ( M.GT.0 .AND. WITHB .AND. LDB.LT.N ) ) 
     $      THEN
         INFO = -22
      ELSE IF( LDD.LT.1 .OR. ( M.GT.0 .AND. WITHD .AND. LDD.LT.L ) )  
     $      THEN
         INFO = -24
      ELSE IF( LDWORK.GE.1 ) THEN
C
C        Compute workspace.
C        (Note: Comments in the code beginning "Workspace:" describe the
C         minimal amount of workspace needed at that point in the code,
C         as well as the preferred amount for good performance.
C         NB refers to the optimal block size for the immediately
C         following subroutine, as returned by ILAENV.)
C
         MINWRK = MAX( 2*L, LNOBR, L + MNOBR )
         MAXWRK = MINWRK
         MAXWRK = MAX( MAXWRK, L + L*ILAENV( 1, 'DGEQRF', ' ', NROW, L,
     $                                      -1, -1 ) )
         MAXWRK = MAX( MAXWRK, L + LDUN2*ILAENV( 1, 'DORMQR', 'LT',
     $                                           NROW, LDUN2, L, -1 ) )
         MAXWRK = MAX( MAXWRK, L + MNOBR*ILAENV( 1, 'DORMQR', 'LT',
     $                                           NROW, MNOBR, L, -1 ) )
C
         IF( M.GT.0 .AND. WITHB ) THEN  
            MINWRK = MAX( MINWRK, 4*LNOBR, LNOBR + M )
            MAXWRK = MAX( MINWRK, MAXWRK, LNOBR +
     $                    M*ILAENV( 1, 'DORMQR', 'LT', LNOBR, M, LNOBR, 
     $                              -1 ) )
         END IF
C
         IF ( LDWORK.LT.MINWRK ) THEN
            INFO = -28
            DWORK( 1 ) = MINWRK
         END IF
      END IF
C
C     Return if there are illegal arguments.
C
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'IB01PY', -INFO )
         RETURN
      END IF
C
C     Construct in  R  the first block-row of  Q,  i.e., the
C     (p/s)-by-L*s  matrix  [ Q_1s  ...  Q_12  Q_11  ],  where
C     Q_1i,  defined above, is  (p/s)-by-L,  for  i = 1:s.
C
      IF ( MOESP ) THEN
C
         DO 10 I = 1, NOBR
            CALL MA02AD( 'Full', L, NROW, UL(L*(I-1)+1,N+1), LDUL,
     $                    R(1,L*(NOBR-I)+1), LDR )
   10    CONTINUE
C
      ELSE
         JL = LNOBR
         JM = LDUN2
C
         DO 50 JI = 1, LDUN2, L
C
            DO 40 J = JI + L - 1, JI, -1
C
               DO 20 I = 1, N
                  R(I,J) = PGAL(I,JM) - UL(I,JL)
   20          CONTINUE
C
               DO 30 I = N + 1, NROW
                  R(I,J) = -UL(I,JL)
   30          CONTINUE
C
               JL = JL - 1
               JM = JM - 1
   40       CONTINUE
C
   50    CONTINUE
C
         DO 70 J = LNOBR, LDUN2 + 1, -1
C
            DO 60 I = 1, NROW
               R(I,J) = -UL(I,JL)
   60       CONTINUE
C
            JL = JL - 1
            R(N+J-LDUN2,J) = ONE + R(N+J-LDUN2,J)
   70    CONTINUE
      END IF
C
C     Triangularize the submatrix  Q_1s  using an orthogonal matrix  S.
C     Workspace: need 2*L, prefer L+L*NB.
C
      ITAU  = 1
      JWORK = ITAU + L
C
      CALL DGEQRF( NROW, L, R, LDR, DWORK(ITAU), DWORK(JWORK), 
     $             LDWORK-JWORK+1, IERR )
C
C     Apply the transformation  S'  to the matrix
C     [ Q_1,s-1  ...  Q_11 ].  Therefore,
C
C                              [ R  P_s-1  P_s-2  ...  P_2  P_1 ]
C     S'[ Q_1,s  ...  Q_11 ] = [                                ].
C                              [ 0  F_s-1  F_s-2  ...  F_2  F_1 ]
C
C     Workspace: need L*NOBR, prefer L+(L*NOBR-L)*NB.
C
      CALL DORMQR( 'Left', 'Transpose', NROW, LDUN2, L, R, LDR,
     $             DWORK(ITAU), R(1,LP1), LDR, DWORK(JWORK),
     $             LDWORK-JWORK+1, IERR )
C
C     Apply the transformation  S'  to each of the submatrices  K_i  of
C     Kexpand = [ K_1'  K_2'  ...  K_s' ]',  K_i = K(:,(i-1)*m+1:i*m)
C     (i = 1:s)  being  (p/s)-by-m.  Denote  ( H_i'  G_i' )' = S'K_i
C     (i = 1:s),  where  H_i  has  L  rows.
C     Finally,  H_i  is saved in  H(L*(i-1)+1:L*i,1:m), i = 1:s.
C     (G_i  is in  K(L+1:p/s,(i-1)*m+1:i*m),  i = 1:s.)
C     Workspace: need L+M*NOBR, prefer L+M*NOBR*NB.
C
      CALL DORMQR( 'Left', 'Transpose', NROW, MNOBR, L, R, LDR,
     $             DWORK(ITAU), K, LDK, DWORK(JWORK), LDWORK-JWORK+1, 
     $             IERR )
C
C     Put the rows to be annihilated (matrix F) in  UL(1:p/s-L,1:L*s-L).
C
      CALL DLACPY( 'Full', NROWML, LDUN2, R(LP1,LP1), LDR, UL, LDUL )
C
C     Now, the structure of the transformed matrices is:
C
C         [  R   P_s-1  P_s-2  ...  P_2  P_1  ]             [  H_1  ]
C         [  0     R    P_s-1  ...  P_3  P_2  ]             [  H_2  ]
C         [  0     0      R    ...  P_4  P_3  ]             [  H_3  ]
C         [  :     :      :          :    :   ]             [   :   ]
C         [  0     0      0    ...   R  P_s-1 ]             [ H_s-1 ]
C     Q = [  0     0      0     ...  0    R   ],  Kexpand = [  H_s  ],
C         [  0   F_s-1  F_s-2  ...  F_2  F_1  ]             [  G_1  ]
C         [  0     0    F_s-1  ...  F_3  F_2  ]             [  G_2  ]
C         [  :     :      :          :    :   ]             [   :   ]
C         [  0     0      0     ...  0  F_s-1 ]             [ G_s-1 ]
C         [  0     0      0     ...  0    0   ]             [  G_s  ]
C
C     where the block-rows have been permuted, to better exploit the
C     structure. The block-rows having  R  on the diagonal are dealt
C     with successively in the array  R.
C     The  F  submatrices are stored in the array  UL,  as a block-row.
C
C     Copy  H_1  in  H(1:L,1:m).
C
      CALL DLACPY( 'Full', L, M, K, LDK, H, LDH )
C
C     Triangularize the transformed matrix exploiting its structure.
C     Workspace: need L+MAX(L-1,L*NOBR-2*L,M*(NOBR-1)).
C
      DO 90 I = 1, NOBR - 1
C
C        Copy part of the preceding block-row and then annihilate the
C        current submatrix  F_s-i  using an orthogonal matrix modifying
C        the corresponding submatrix  R.  Simultaneously, apply the
C        transformation to the corresponding block-rows of the matrices 
C        R  and  F.
C
         CALL DLACPY( 'Upper', L, LNOBR-L*I, R(L*(I-1)+1,L*(I-1)+1),
     $                LDR, R(L*I+1,L*I+1), LDR )
         CALL MB04OD( 'Full', L, LNOBR-L*(I+1), NROWML, R(L*I+1,L*I+1),
     $                LDR, UL(1,L*(I-1)+1), LDUL, R(L*I+1,L*(I+1)+1),
     $                LDR, UL(1,L*I+1), LDUL, DWORK(ITAU), DWORK(JWORK)
     $              )
C
C        Apply the transformation to the corresponding block-rows of
C        the matrix  G  and copy  H_(i+1)  in  H(L*i+1:L*(i+1),1:m).
C
         DO 80 J = 1, L
            CALL MB04OY( NROWML, M*(NOBR-I), UL(1,L*(I-1)+J), DWORK(J), 
     $                   K(J,M*I+1), LDK, K(LP1,1), LDK, DWORK(JWORK) )
   80    CONTINUE
C
         CALL DLACPY( 'Full', L, M, K(1,M*I+1), LDK, H(L*I+1,1), LDH )
   90 CONTINUE
C
C     Return if only the factorization is needed.
C
      IF( M.EQ.0 .OR. .NOT.WITHB ) THEN
         DWORK(1) = MAXWRK
         RETURN
      END IF
C
C     Set the precision parameters. A threshold value  EPS**(2/3)  is
C     used for deciding to use pivoting or not, where  EPS  is the 
C     relative machine precision (see LAPACK Library routine DLAMCH).
C
      EPS    = DLAMCH( 'Precision' )
      THRESH = EPS**( TWO/THREE )
      TOLL   = TOL
      IF( TOLL.LE.ZERO ) 
     $   TOLL = LNOBR*LNOBR*EPS
      SVLMAX = ZERO
C
C     Compute the reciprocal of the condition number of the triangular 
C     factor  R  of  Q.
C     Workspace: need 3*L*NOBR.
C
      CALL DTRCON( '1-norm', 'Upper', 'NonUnit', LNOBR, R, LDR, RCOND,
     $             DWORK, IWORK, IERR )
C
      IF ( RCOND.GT.MAX( TOLL, THRESH ) ) THEN
C
C        The triangular factor  R  is considered to be of full rank.
C        Solve for  X,  R*X = H.
C
         CALL DTRSM( 'Left', 'Upper', 'NoTranspose', 'Non-unit',
     $               LNOBR, M, ONE, R, LDR, H, LDH )
      ELSE
C
C        Rank-deficient triangular factor  R.  Compute the
C        minimum-norm least squares solution of  R*X = H  using
C        the complete orthogonal factorization of  R.
C
         DO 100 I = 1, LNOBR
            IWORK(I) = 0
  100    CONTINUE
C
C        Workspace: need 4*L*NOBR.
C
         JWORK = ITAU + LNOBR
         CALL DLASET( 'Lower', LNOBR-1, LNOBR, ZERO, ZERO, R(2,1), LDR )
         CALL MB03OD( 'QR', LNOBR, LNOBR, R, LDR, IWORK, TOLL, SVLMAX,
     $                DWORK(ITAU), RANK, SVAL, DWORK(JWORK), IERR )
C
C        Workspace: need L*NOBR+M; prefer L*NOBR+M*NB.
C
         CALL DORMQR( 'Left', 'Transpose', LNOBR, M, LNOBR, R, LDR,
     $                DWORK(ITAU), H, LDH, DWORK(JWORK), LDWORK-JWORK+1,
     $                IERR )
         IF ( RANK.LT.LNOBR ) THEN
C
C           The least squares problem is rank-deficient.
C
            IWARN = 4
         END IF
C
C        Workspace: need L*NOBR+max(L*NOBR,M); prefer larger.
C
         CALL MB02QY( LNOBR, LNOBR, M, RANK, R, LDR, IWORK, H, LDH,
     $                DWORK(ITAU), DWORK(JWORK), LDWORK-JWORK+1, IERR )
         MAXWRK = MAX( MAXWRK, INT( DWORK(JWORK) ) + JWORK - 1 )
      END IF
C
C     Construct the matrix  D,  if needed.
C
      IF ( WITHD )
     $   CALL DLACPY( 'Full', L, M, H(LDUN2+1,1), LDH, D, LDD )
C
C     Compute  B  by solving another linear system (possibly in
C     a least squares sense).
C
C     Make a block-permutation of the rows of the right-hand side,  H,
C     to construct the matrix
C
C        [ H(L*(s-2)+1:L*(s-1),:); ... H(L+1:L*2,:); H(1:L),:) ]
C
C     in  H(1:L*s-L,1:n).
C
      NOBRH = NOBR/2 + MOD( NOBR, 2 ) - 1
C
      DO 120 J = 1, M
C
         DO 110 I = 1, NOBRH
            CALL DSWAP( L, H(L*(I-1)+1,J), 1, H(L*(NOBR-I-1)+1,J), 1 )
  110    CONTINUE
C
  120 CONTINUE
C
C     Solve for  B  the matrix equation  GaL*B = H(1:L*s-L,:),  using
C     the available QR factorization of  GaL,  if  METH = 'M'  and
C     rank(GaL) = n, or the available pseudoinverse of  GaL,  otherwise.
C
      IF ( MOESP .AND. RANKR1.EQ.N ) THEN
C
C        The triangular factor  r1  of  GaL  is considered to be of
C        full rank. Compute  Q1'*H  in  H  and then solve for  B,  
C        r1*B = H(1:n,:)  in  B,  where  Q1  is the orthogonal matrix
C        in the QR factorization of  GaL.
C        Workspace: need M; prefer M*NB.
C
         CALL DORMQR( 'Left', 'Transpose', LDUN2, M, N, R1, LDR1,
     $                TAU1, H, LDH, DWORK, LDWORK, IERR )
         MAXWRK = MAX( MAXWRK, INT( DWORK(1) ) )
C
C        Compute the solution in  B.
C
         CALL DLACPY( 'Full', N, M, H, LDH, B, LDB )
C
         CALL DTRTRS( 'Upper', 'NoTranspose', 'NonUnit', N, M, R1, LDR1,
     $                B, LDB, IERR )
         IF ( IERR.GT.0 ) THEN
            INFO = 3
            RETURN
         END IF
      ELSE
C 
C        Rank-deficient triangular factor  r1.  Use the available
C        pseudoinverse of  GaL  for computing  B  from  GaL*B = H.
C
         CALL DGEMM ( 'NoTranspose', 'NoTranspose', N, M, LDUN2, ONE,
     $                PGAL, LDPGAL, H, LDH, ZERO, B, LDB )
      END IF
C
C     Return optimal workspace in  DWORK(1)  and reciprocal condition
C     number in  DWORK(2).
C
      DWORK(1) = MAXWRK
      DWORK(2) = RCOND
C
      RETURN
C
C *** Last line of IB01PY ***
      END
