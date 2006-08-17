      SUBROUTINE IB01AD( METH, ALG, JOBD, BATCH, CONCT, CTRL, NOBR, M,
     $                   L, NSMP, U, LDU, Y, LDY, N, R, LDR, SV, RCOND,
     $                   TOL, IWORK, DWORK, LDWORK, IWARN, INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 2000.
C
C     PURPOSE
C
C     To preprocess the input-output data for estimating the matrices 
C     of a linear time-invariant dynamical system and to find an 
C     estimate of the system order. The input-output data can,
C     optionally, be processed sequentially.
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
C     ALG     CHARACTER*1
C             Specifies the algorithm for computing the triangular
C             factor R, as follows:
C             = 'C':  Cholesky algorithm applied to the correlation
C                     matrix of the input-output data;
C             = 'F':  Fast QR algorithm;
C             = 'Q':  QR algorithm applied to the concatenated block
C                     Hankel matrices.
C
C     JOBD    CHARACTER*1
C             Specifies whether or not the matrices B and D should later
C             be computed using the MOESP approach, as follows:
C             = 'M':  the matrices B and D should later be computed
C                     using the MOESP approach;
C             = 'N':  the matrices B and D should not be computed using
C                     the MOESP approach.
C             This parameter is not relevant for METH = 'N'.
C
C     BATCH   CHARACTER*1
C             Specifies whether or not sequential data processing is to
C             be used, and, for sequential processing, whether or not
C             the current data block is the first block, an intermediate
C             block, or the last block, as follows:
C             = 'F':  the first block in sequential data processing;
C             = 'I':  an intermediate block in sequential data
C                     processing;
C             = 'L':  the last block in sequential data processing;
C             = 'O':  one block only (non-sequential data processing).
C             NOTE that when  100  cycles of sequential data processing
C                  are completed for  BATCH = 'I',  a warning is
C                  issued, to prevent for an infinite loop.
C
C     CONCT   CHARACTER*1
C             Specifies whether or not the successive data blocks in
C             sequential data processing belong to a single experiment,
C             as follows:
C             = 'C':  the current data block is a continuation of the
C                     previous data block and/or it will be continued
C                     by the next data block;
C             = 'N':  there is no connection between the current data
C                     block and the previous and/or the next ones.
C             This parameter is not used if BATCH = 'O'.
C
C     CTRL    CHARACTER*1
C             Specifies whether or not the user's confirmation of the
C             system order estimate is desired, as follows:
C             = 'C':  user's confirmation;
C             = 'N':  no confirmation.
C             If  CTRL = 'C',  a reverse communication routine,  IB01OY,
C             is indirectly called (by SLICOT Library routine IB01OD), 
C             and, after inspecting the singular values and system order
C             estimate,  n,  the user may accept  n  or set a new value.
C             IB01OY  is not called if CTRL = 'N'.
C
C     Input/Output Parameters
C
C     NOBR    (input) INTEGER
C             The number of block rows,  s,  in the input and output
C             block Hankel matrices to be processed.  NOBR > 0.
C             (In the MOESP theory,  NOBR  should be larger than  n,
C             the estimated dimension of state vector.)
C
C     M       (input) INTEGER
C             The number of system inputs.  M >= 0.
C             When M = 0, no system inputs are processed.
C
C     L       (input) INTEGER
C             The number of system outputs.  L > 0.
C
C     NSMP    (input) INTEGER
C             The number of rows of matrices  U  and  Y  (number of
C             samples,  t). (When sequential data processing is used,
C             NSMP  is the number of samples of the current data
C             block.)
C             NSMP >= 2*(M+L+1)*NOBR - 1,  for non-sequential
C                                          processing;
C             NSMP >= 2*NOBR,  for sequential processing.
C             The total number of samples when calling the routine with
C             BATCH = 'L'  should be at least  2*(M+L+1)*NOBR - 1.
C             The  NSMP  argument may vary from a cycle to another in
C             sequential data processing, but  NOBR, M,  and  L  should
C             be kept constant. For efficiency, it is advisable to use
C             NSMP  as large as possible.
C
C     U       (input) DOUBLE PRECISION array, dimension (LDU,M)
C             The leading NSMP-by-M part of this array must contain the
C             t-by-m input-data sequence matrix  U,
C             U = [u_1 u_2 ... u_m].  Column  j  of  U  contains the
C             NSMP  values of the j-th input component for consecutive
C             time increments.
C             If M = 0, this array is not referenced.
C
C     LDU     INTEGER
C             The leading dimension of the array U.
C             LDU >= NSMP, if M > 0;
C             LDU >= 1,    if M = 0.
C
C     Y       (input) DOUBLE PRECISION array, dimension (LDY,L)
C             The leading NSMP-by-L part of this array must contain the
C             t-by-l output-data sequence matrix  Y,
C             Y = [y_1 y_2 ... y_l].  Column  j  of  Y  contains the
C             NSMP  values of the j-th output component for consecutive
C             time increments.
C
C     LDY     INTEGER
C             The leading dimension of the array Y.  LDY >= NSMP.
C
C     N       (output) INTEGER
C             The estimated order of the system.
C             If  CTRL = 'C',  the estimated order has been reset to a
C             value specified by the user.
C
C     R       (output or input/output) DOUBLE PRECISION array, dimension
C             ( LDR,2*(M+L)*NOBR )
C             On exit, if ALG = 'C' and BATCH = 'F' or 'I', the leading
C             2*(M+L)*NOBR-by-2*(M+L)*NOBR upper triangular part of this
C             array contains the current upper triangular part of the
C             correlation matrix in sequential data processing.
C             If ALG = 'F' and BATCH = 'F' or 'I', the array R is not
C             referenced.
C             On exit, if INFO = 0, ALG = 'Q', and BATCH = 'F' or 'I',
C             the leading 2*(M+L)*NOBR-by-2*(M+L)*NOBR upper triangular
C             part of this array contains the current upper triangular
C             factor R from the QR factorization of the concatenated
C             block Hankel matrices. Denote  R_ij, i,j = 1:4,  the 
C             ij submatrix of  R,  partitioned by M*NOBR,  M*NOBR, 
C             L*NOBR,  and  L*NOBR  rows and columns. 
C             On exit, if INFO = 0 and BATCH = 'L' or 'O', the leading
C             2*(M+L)*NOBR-by-2*(M+L)*NOBR upper triangular part of
C             this array contains the matrix S, the processed upper
C             triangular factor R from the QR factorization of the
C             concatenated block Hankel matrices, as required by other
C             subroutines. Specifically, let  S_ij, i,j = 1:4,  be the
C             ij submatrix of  S,  partitioned by M*NOBR,  L*NOBR, 
C             M*NOBR,  and  L*NOBR  rows and columns. The submatrix 
C             S_22  contains the matrix of left singular vectors needed
C             subsequently. Useful information is stored in  S_11  and 
C             in the block-column  S_14 : S_44.  For METH = 'M' and 
C             JOBD = 'M', the upper triangular part of  S_31  contains
C             the upper triangular factor in the QR factorization of the
C             matrix  R_1c = [ R_12'  R_22'  R_11' ]',  and  S_12 
C             contains the corresponding leading part of the transformed
C             matrix  R_2c = [ R_13'  R_23'  R_14' ]'.  For  METH = 'N',
C             the subarray  S_41 : S_43  contains the transpose of the
C             matrix contained in  S_14 : S_34. 
C             The details of the contents of R need not be known if this
C             routine is followed by SLICOT Library routine IB01BD.
C             On entry, if ALG = 'C', or ALG = 'Q', and BATCH = 'I' or 
C             'L', the leading  2*(M+L)*NOBR-by-2*(M+L)*NOBR  upper
C             triangular part of this array must contain the upper
C             triangular matrix R computed at the previous call of this
C             routine in sequential data processing. The array R need
C             not be set on entry if ALG = 'F' or if BATCH = 'F' or 'O'.
C
C     LDR     INTEGER
C             The leading dimension of the array  R.
C             LDR >= MAX( 2*(M+L)*NOBR, 3*M*NOBR ),
C                                  for METH = 'M' and JOBD = 'M';
C             LDR >= 2*(M+L)*NOBR, for METH = 'M' and JOBD = 'N' or
C                                  for METH = 'N'.
C
C     SV      (output) DOUBLE PRECISION array, dimension ( L*NOBR )
C             The singular values used to estimate the system order.
C
C     Tolerances
C
C     RCOND   DOUBLE PRECISION
C             The tolerance to be used for estimating the rank of
C             matrices. If the user sets  RCOND > 0,  the given value
C             of  RCOND  is used as a lower bound for the reciprocal
C             condition number;  an m-by-n matrix whose estimated
C             condition number is less than  1/RCOND  is considered to  
C             be of full rank.  If the user sets  RCOND <= 0,  then an 
C             implicitly computed, default tolerance, defined by 
C             RCONDEF = m*n*EPS,  is used instead, where  EPS  is the 
C             relative machine precision (see LAPACK Library routine 
C             DLAMCH).
C             This parameter is not used for  METH = 'M'.
C
C     TOL     DOUBLE PRECISION
C             Absolute tolerance used for determining an estimate of
C             the system order. If  TOL >= 0,  the estimate is
C             indicated by the index of the last singular value greater
C             than or equal to  TOL.  (Singular values less than  TOL
C             are considered as zero.) When  TOL = 0,  an internally
C             computed default value,  TOL = NOBR*EPS*SV(1),  is used,
C             where  SV(1)  is the maximal singular value, and  EPS  is
C             the relative machine precision (see LAPACK Library routine
C             DLAMCH). When  TOL < 0,  the estimate is indicated by the
C             index of the singular value that has the largest
C             logarithmic gap to its successor.
C
C     Workspace
C
C     IWORK   INTEGER array, dimension (LIWORK)
C             LIWORK >= (M+L)*NOBR, if METH = 'N';
C             LIWORK >= M+L, if METH = 'M' and ALG = 'F';
C             LIWORK >= 0,   if METH = 'M' and ALG = 'C' or 'Q'.
C
C     DWORK   DOUBLE PRECISION array, dimension (LDWORK)
C             On exit, if  INFO = 0,  DWORK(1) returns the optimal value
C             of LDWORK,  and, for  METH = 'N',  and  BATCH = 'L'  or
C             'O',  DWORK(2)  and  DWORK(3)  contain the reciprocal 
C             condition numbers of the triangular factors of the 
C             matrices  U_f  and  r_1  [6].
C             On exit, if  INFO = -23,  DWORK(1)  returns the minimum
C             value of LDWORK.
C             Let
C             k = 0,               if CONCT = 'N' and ALG = 'C' or 'Q';
C             k = 2*NOBR-1,        if CONCT = 'C' and ALG = 'C' or 'Q';
C             k = 2*NOBR*(M+L+1),  if CONCT = 'N' and ALG = 'F';
C             k = 2*NOBR*(M+L+2),  if CONCT = 'C' and ALG = 'F'.
C             The first (M+L)*k elements of  DWORK  should be preserved
C             during successive calls of the routine with  BATCH = 'F'
C             or  'I',  till the final call with  BATCH = 'L'.
C
C     LDWORK  INTEGER
C             The length of the array DWORK.
C             LDWORK >= (4*NOBR-2)*(M+L), if ALG = 'C', BATCH = 'F' or
C                             'I' and CONCT = 'C';
C             LDWORK >= 1, if ALG = 'C', BATCH = 'F' or 'I' and 
C                             CONCT = 'N';
C             LDWORK >= max((4*NOBR-2)*(M+L), 5*L*NOBR), if METH = 'M',
C                             ALG = 'C', BATCH = 'L' and CONCT = 'C';
C             LDWORK >= max((2*M-1)*NOBR, (M+L)*NOBR, 5*L*NOBR), 
C                             if METH = 'M', JOBD = 'M', ALG = 'C',
C                              BATCH = 'O', or 
C                             (BATCH = 'L' and CONCT = 'N');
C             LDWORK >= 5*L*NOBR, if METH = 'M', JOBD = 'N', ALG = 'C',
C                              BATCH = 'O', or 
C                             (BATCH = 'L' and CONCT = 'N');
C             LDWORK >= 5*(M+L)*NOBR, if METH = 'N', ALG = 'C', and
C                             BATCH = 'L' or 'O';
C             LDWORK >= (M+L)*2*NOBR*(M+L+3), if ALG = 'F',
C                             BATCH <> 'O' and CONCT = 'C';
C             LDWORK >= (M+L)*2*NOBR*(M+L+1), if ALG = 'F',
C                             BATCH = 'F', 'I' and CONCT = 'N';
C             LDWORK >= (M+L)*4*NOBR*(M+L+1)+(M+L)*2*NOBR, if ALG = 'F',
C                             BATCH = 'L' and CONCT = 'N', or
C                             BATCH = 'O';
C             LDWORK >= 4*(M+L)*NOBR, if ALG = 'Q', BATCH = 'F', and 
C                             LDR >= NS = NSMP - 2*NOBR + 1;
C             LDWORK >= max(4*(M+L)*NOBR, 5*L*NOBR), if METH = 'M',
C                             ALG = 'Q', BATCH = 'O', and LDR >= NS;
C             LDWORK >= 5*(M+L)*NOBR, if METH = 'N', ALG = 'Q',
C                             BATCH = 'O', and LDR >= NS;
C             LDWORK >= 6*(M+L)*NOBR, if ALG = 'Q', (BATCH = 'F' or 'O',
C                             and LDR < NS), or (BATCH = 'I' or 
C                             'L' and CONCT = 'N');
C             LDWORK >= 4*(NOBR+1)*(M+L)*NOBR, if ALG = 'Q', BATCH = 'I'
C                             or 'L' and CONCT = 'C'.
C             The workspace used for ALG = 'Q' is 
C                       LDRWRK*2*(M+L)*NOBR + 4*(M+L)*NOBR, 
C             where LDRWRK = LDWORK/(2*(M+L)*NOBR) - 2; recommended 
C             value LDRWRK = NS, assuming a large enough cache size.
C             For good performance,  LDWORK  should be larger.
C
C     Warning Indicator
C
C     IWARN   INTEGER
C             = 0:  no warning;
C             = 1:  the number of 100 cycles in sequential data
C                   processing has been exhausted without signaling
C                   that the last block of data was get; the cycle
C                   counter was reinitialized;
C             = 2:  a fast algorithm was requested (ALG = 'C' or 'F'),
C                   but it failed, and the QR algorithm was then used
C                   (non-sequential data processing);
C             = 3:  all singular values were exactly zero, hence  N = 0
C                   (both input and output were identically zero);
C             = 4:  the least squares problems with coefficient matrix
C                   U_f,  used for computing the weighted oblique
C                   projection (for METH = 'N'), have a rank-deficient
C                   coefficient matrix;
C             = 5:  the least squares problem with coefficient matrix
C                   r_1  [6], used for computing the weighted oblique
C                   projection (for METH = 'N'), has a rank-deficient
C                   coefficient matrix.
C             NOTE: the values 4 and 5 of IWARN have no significance
C                   for the identification problem. 
C
C     Error Indicator
C
C     INFO    INTEGER
C             = 0:  successful exit;
C             < 0:  if INFO = -i, the i-th argument had an illegal
C                   value;
C             = 1:  a fast algorithm was requested (ALG = 'C', or 'F')
C                   in sequential data processing, but it failed; the
C                   routine can be repeatedly called again using the
C                   standard QR algorithm;
C             = 2:  the singular value decomposition (SVD) algorithm did
C                   not converge.
C
C     METHOD
C
C     The procedure consists in three main steps, the first step being
C     performed by one of the three algorithms included.
C
C     1.a) For non-sequential data processing using QR algorithm, a
C     t x 2(m+l)s  matrix H is constructed, where  
C
C          H = [ Uf'         Up'      Y'      ],  for METH = 'M',
C                  s+1,2s,t    1,s,t   1,2s,t
C
C          H = [ U'       Y'      ],              for METH = 'N',
C                 1,2s,t   1,2s,t
C
C     and  Up     , Uf        , U      , and  Y        are block Hankel
C            1,s,t    s+1,2s,t   1,2s,t        1,2s,t
C     matrices defined in terms of the input and output data [3].
C     A QR factorization is used to compress the data.
C     The fast QR algorithm uses a QR factorization which exploits 
C     the block-Hankel structure. Actually, the Cholesky factor of H'*H
C     is computed.
C
C     1.b) For sequential data processing using QR algorithm, the QR
C     decomposition is done sequentially, by updating the upper
C     triangular factor  R.  This is also performed internally if the
C     workspace is not large enough to accommodate an entire batch.
C
C     1.c) For non-sequential or sequential data processing using
C     Cholesky algorithm, the correlation matrix of input-output data is
C     computed (sequentially, if requested), taking advantage of the
C     block Hankel structure [7].  Then, the Cholesky factor of the
C     correlation matrix is found, if possible.
C
C     2) A singular value decomposition (SVD) of a certain matrix is
C     then computed, which reveals the order  n  of the system as the
C     number of "non-zero" singular values. For the MOESP approach, this
C     matrix is  [ R_24'  R_34' ]' := R(ms+1:(2m+l)s,(2m+l)s+1:2(m+l)s),
C     where  R  is the upper triangular factor  R  constructed by SLICOT
C     Library routine  IB01MD.  For the N4SID approach, a weighted
C     oblique projection is computed from the upper triangular factor  R
C     and its SVD is then found.
C
C     3) The singular values are compared to the given, or default TOL,
C     and the estimated order  n  is returned, possibly after user's
C     confirmation.
C
C     REFERENCES
C
C     [1] Verhaegen M., and Dewilde, P.
C         Subspace Model Identification. Part 1: The output-error 
C         state-space model identification class of algorithms.
C         Int. J. Control, 56, pp. 1187-1210, 1992.
C
C     [2] Verhaegen M.
C         Subspace Model Identification. Part 3: Analysis of the
C         ordinary output-error state-space model identification
C         algorithm.
C         Int. J. Control, 58, pp. 555-586, 1993.
C
C     [3] Verhaegen M.
C         Identification of the deterministic part of MIMO state space
C         models given in innovations form from input-output data.
C         Automatica, Vol.30, No.1, pp.61-74, 1994.
C
C     [4] Van Overschee, P., and De Moor, B.
C         N4SID: Subspace Algorithms for the Identification of
C         Combined Deterministic-Stochastic Systems.
C         Automatica, Vol.30, No.1, pp. 75-93, 1994.
C
C     [5] Peternell, K., Scherrer, W. and Deistler, M.
C         Statistical Analysis of Novel Subspace Identification Methods.
C         Signal Processing, 52, pp. 161-177, 1996.
C
C     [6] Sima, V.
C         Subspace-based Algorithms for Multivariable System 
C         Identification.
C         Studies in Informatics and Control, 5, pp. 335-344, 1996.
C
C     [7] Sima, V.
C         Cholesky or QR Factorization for Data Compression in 
C         Subspace-based Identification ?
C         Proceedings of the Second NICONET Workshop on ``Numerical 
C         Control Software: SLICOT, a Useful Tool in Industry'', 
C         December 3, 1999, INRIA Rocquencourt, France, pp. 75-80, 1999.
C
C     NUMERICAL ASPECTS
C
C     The implemented method is numerically stable (when QR algorithm is
C     used), reliable and efficient. The fast Cholesky or QR algorithms
C     are more efficient, but the accuracy could diminish by forming the
C     correlation matrix.
C     The most time-consuming computational step is step 1:
C                                        2 
C     The QR algorithm needs 0(t(2(m+l)s) ) floating point operations.
C                                           2              3
C     The Cholesky algorithm needs 0(2t(m+l) s)+0((2(m+l)s) ) floating 
C     point operations.
C                                          2           3 2 
C     The fast QR algorithm needs 0(2t(m+l) s)+0(4(m+l) s ) floating 
C     point operations.
C                                                3
C     Step 2 of the algorithm requires 0(((m+l)s) ) floating point 
C     operations.
C
C     FURTHER COMMENTS
C
C     For ALG = 'Q', BATCH = 'O' and LDR < NS, or BATCH <> 'O', the  
C     calculations could be rather inefficient if only minimal workspace
C     (see argument LDWORK) is provided. It is advisable to provide as
C     much workspace as possible. Almost optimal efficiency can be 
C     obtained for  LDWORK = (NS+2)*(2*(M+L)*NOBR),  assuming that the 
C     cache size is large enough to accommodate R, U, Y, and DWORK.
C
C     CONTRIBUTOR
C
C     V. Sima, Katholieke Universiteit Leuven, Feb. 2000.
C
C     REVISIONS
C
C     August 2000.
C
C     KEYWORDS
C
C     Cholesky decomposition, Hankel matrix, identification methods, 
C     multivariable systems, QR decomposition, singular value 
C     decomposition.
C
C     ******************************************************************
C
C     .. Scalar Arguments ..
      DOUBLE PRECISION   RCOND, TOL
      INTEGER            INFO, IWARN, L, LDR, LDU, LDWORK, LDY, M, N,
     $                   NOBR, NSMP
      CHARACTER          ALG, BATCH, CONCT, CTRL, JOBD, METH
C     .. Array Arguments ..
      INTEGER            IWORK(*)
      DOUBLE PRECISION   DWORK(*), R(LDR, *), SV(*), U(LDU, *), 
     $                   Y(LDY, *)
C     .. Local Scalars ..
      INTEGER            IWARNL, LMNOBR, LNOBR, MAXWRK, MINWRK, MNOBR,
     $                   NOBR21, NR, NS, NSMPSM
      LOGICAL            CHALG, CONNEC, CONTRL, FIRST, FQRALG, INTERM,
     $                   JOBDM, LAST, MOESP, N4SID, ONEBCH, QRALG
C     .. External Functions ..
      LOGICAL            LSAME 
      EXTERNAL           LSAME
C     .. External Subroutines ..
      EXTERNAL           IB01MD, IB01ND, IB01OD, XERBLA
C     .. Intrinsic Functions ..
      INTRINSIC          MAX
C     .. Save Statement ..
C        MAXWRK  is used to store the optimal workspace.
C        NSMPSM  is used to sum up the  NSMP  values for  BATCH <> 'O'.
      SAVE               MAXWRK, NSMPSM
C     ..
C     .. Executable Statements ..
C
C     Decode the scalar input parameters.
C
      MOESP  = LSAME( METH,  'M' )
      N4SID  = LSAME( METH,  'N' )
      FQRALG = LSAME( ALG,   'F' )
      QRALG  = LSAME( ALG,   'Q' )
      CHALG  = LSAME( ALG,   'C' )
      JOBDM  = LSAME( JOBD,  'M' )
      ONEBCH = LSAME( BATCH, 'O' )
      FIRST  = LSAME( BATCH, 'F' ) .OR. ONEBCH
      INTERM = LSAME( BATCH, 'I' )
      LAST   = LSAME( BATCH, 'L' ) .OR. ONEBCH
      CONTRL = LSAME( CTRL,  'C' )
C
      IF( .NOT.ONEBCH ) THEN
         CONNEC = LSAME( CONCT, 'C' )
      ELSE
         CONNEC = .FALSE.
      END IF
C
      MNOBR  = M*NOBR
      LNOBR  = L*NOBR
      LMNOBR = LNOBR  + MNOBR
      NR     = LMNOBR + LMNOBR
      NOBR21 = 2*NOBR - 1
      IWARN  = 0
      INFO   = 0
      IF( FIRST ) THEN
         MAXWRK = 1
         NSMPSM = 0
      END IF
      NSMPSM = NSMPSM + NSMP
C
C     Check the scalar input parameters.
C
      IF( .NOT.( MOESP .OR. N4SID ) ) THEN
         INFO = -1
      ELSE IF( .NOT.( FQRALG .OR. QRALG .OR. CHALG ) ) THEN
         INFO = -2
      ELSE IF( MOESP .AND. .NOT.( JOBDM .OR. LSAME( JOBD, 'N' ) ) ) THEN
         INFO = -3
      ELSE IF( .NOT.( FIRST .OR. INTERM .OR. LAST ) ) THEN
         INFO = -4
      ELSE IF( .NOT. ONEBCH ) THEN
         IF( .NOT.( CONNEC .OR. LSAME( CONCT, 'N' ) ) )
     $      INFO = -5
      END IF
      IF( INFO.EQ.0 ) THEN
         IF( .NOT.( CONTRL .OR. LSAME( CTRL, 'N' ) ) ) THEN
            INFO = -6
         ELSE IF( NOBR.LE.0 ) THEN
            INFO = -7
         ELSE IF( M.LT.0 ) THEN
            INFO = -8
         ELSE IF( L.LE.0 ) THEN
            INFO = -9
         ELSE IF( NSMP.LT.2*NOBR .OR.
     $            ( LAST .AND. NSMPSM.LT.NR+NOBR21 ) ) THEN
            INFO = -10
         ELSE IF( LDU.LT.1 .OR. ( M.GT.0 .AND. LDU.LT.NSMP ) ) THEN
            INFO = -12
         ELSE IF( LDY.LT.NSMP ) THEN
            INFO = -14
         ELSE IF( LDR.LT.NR .OR. ( MOESP .AND. JOBDM .AND. 
     $            LDR.LT.3*MNOBR ) ) THEN
            INFO = -17
         ELSE
C
C           Compute workspace.
C           (Note: Comments in the code beginning "Workspace:" describe
C           the minimal amount of workspace needed at that point in the
C           code, as well as the preferred amount for good performance.)
C
            NS = NSMP - NOBR21
            IF ( CHALG ) THEN
               IF ( .NOT.LAST ) THEN
                  IF ( CONNEC ) THEN
                     MINWRK = 2*( NR - M - L )
                  ELSE
                     MINWRK = 1
                  END IF
               ELSE IF ( MOESP ) THEN
                  IF ( CONNEC .AND. .NOT.ONEBCH ) THEN
                     MINWRK = MAX( 2*( NR - M - L ), 5*LNOBR )
                  ELSE
                     MINWRK = 5*LNOBR
                     IF ( JOBDM ) 
     $                  MINWRK = MAX( 2*MNOBR - NOBR, LMNOBR, MINWRK )
                  END IF
               ELSE
                  MINWRK = 5*LMNOBR
               END IF
            ELSE IF ( FQRALG ) THEN
               IF ( .NOT.ONEBCH .AND. CONNEC ) THEN
                  MINWRK = NR*( M + L + 3 )
               ELSE IF ( FIRST .OR. INTERM ) THEN
                  MINWRK = NR*( M + L + 1 )
               ELSE 
                  MINWRK = 2*NR*( M + L + 1 ) + NR
               END IF
            ELSE 
               MINWRK = 2*NR
               IF ( ONEBCH .AND. LDR.GE.NS ) THEN
                  IF ( MOESP ) THEN
                     MINWRK = MAX( MINWRK, 5*LNOBR )
                  ELSE
                     MINWRK = 5*LMNOBR
                  END IF
               END IF
               IF ( FIRST ) THEN
                  IF ( LDR.LT.NS ) THEN
                     MINWRK = MINWRK + NR
                  END IF
               ELSE
                  IF ( CONNEC ) THEN
                     MINWRK = MINWRK*( NOBR + 1 )
                  ELSE
                     MINWRK = MINWRK + NR
                  END IF
               END IF
            END IF
C
            MAXWRK = MINWRK
C
            IF( LDWORK.LT.MINWRK ) THEN
               INFO = -23
               DWORK( 1 ) = MINWRK
            END IF
         END IF
      END IF       
C
C     Return if there are illegal arguments.
C
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'IB01AD', -INFO )
         RETURN
      END IF
C
C     Compress the input-output data.
C     Workspace: need   c*(M+L)*NOBR, where c is a constant depending
C                       on the algorithm and the options used
C                       (see SLICOT Library routine IB01MD);
C                prefer larger.
C
      CALL IB01MD( METH, ALG, BATCH, CONCT, NOBR, M, L, NSMP, U, LDU, Y,
     $             LDY, R, LDR, IWORK, DWORK, LDWORK, IWARN, INFO )
C
      IF ( INFO.EQ.1 ) THEN
C
C        Error return: A fast algorithm was requested (ALG = 'C', 'F')
C        in sequential data processing, but it failed. 
C
         RETURN
      END IF
C
      MAXWRK = MAX( MAXWRK, INT( DWORK( 1 ) ) )
C
      IF ( .NOT.LAST ) THEN
C
C        Return to get new data.
C
         RETURN
      END IF
C
C     Find the singular value decomposition (SVD) giving the system
C     order, and perform related preliminary calculations needed for
C     computing the system matrices.
C     Workspace: need   max( (2*M-1)*NOBR, (M+L)*NOBR, 5*L*NOBR ),
C                                          if METH = 'M';
C                            5*(M+L)*NOBR, if METH = 'N';
C                prefer larger.
C
      CALL IB01ND( METH, JOBD, NOBR, M, L, R, LDR, SV, RCOND, IWORK,
     $             DWORK, LDWORK, IWARNL, INFO )
      IWARN = MAX( IWARN, IWARNL )
C
      IF ( INFO.EQ.2 ) THEN
C
C        Error return: the singular value decomposition (SVD) algorithm
C        did not converge.
C
         RETURN
      END IF
C
C     Estimate the system order.
C
      CALL IB01OD( CTRL, NOBR, L, SV, N, TOL, IWARNL, INFO )
      IWARN = MAX( IWARN, IWARNL )
C
C     Return optimal workspace in  DWORK(1).
C
      DWORK( 1 ) = MAX( MAXWRK,  INT( DWORK( 1 ) ) )
      RETURN
C
C *** Last line of IB01AD ***
      END
